// Copyright (c) 2023 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import { assert, assertNotReached } from 'chrome://resources/js/assert.js'
import { eachLimit } from 'async'

// constants
import {
  BraveWallet,
  CoinTypes,
  BitcoinBalances,
  WalletStatus,
} from '../../../constants/types'
import { coinTypesMapping } from '../constants'
import {
  NATIVE_EVM_ASSET_CONTRACT_ADDRESS,
  POLYGON_NATIVE_ASSET_CONTRACT_ADDRESS,
} from '../../../common/constants/magics'

// types
import { WalletApiEndpointBuilderParams } from '../api-base.slice'

// utils
import getAPIProxy from '../../async/bridge'
import Amount from '../../../utils/amount'
import {
  GetBlockchainTokenIdArg,
  getAssetIdKey,
  isNativeAsset,
} from '../../../utils/asset-utils'
import { handleEndpointError } from '../../../utils/api-utils'
import {
  BalanceResult,
  createEmptyTokenBalancesRegistry,
  getAccountBalancesKey,
  setBalance,
} from '../../../utils/balance-utils'
import { getEntitiesListFromEntityState } from '../../../utils/entities.utils'
import { networkSupportsAccount } from '../../../utils/network-utils'
import { cacher } from '../../../utils/query-cache-utils'
import { getNetworkId } from '../entities/network.entity'
import { TokenBalancesRegistry } from '../entities/token-balance.entity'
import { type BaseQueryCache } from '../../async/base-query-cache'
import {
  getPersistedTokenBalances,
  setPersistedPortfolioTokenBalances,
} from '../../../utils/local-storage-utils'
import { getIsRewardsNetwork } from '../../../utils/rewards_utils'
import {
  blockchainTokenEntityAdaptorInitialState, //
} from '../entities/blockchain-token.entity'
import { reportActiveWalletsToP3A } from '../../../utils/p3a_utils'

type BalanceNetwork = Pick<
  BraveWallet.NetworkInfo,
  'chainId' | 'coin' | 'supportedKeyrings'
>

type GetHardwareAccountDiscoveryBalanceArg = {
  coin: BraveWallet.CoinType
  chainId: string
  address: string
}

type GetAccountTokenCurrentBalanceArg = {
  accountId: BraveWallet.AccountId
  token: GetBlockchainTokenIdArg
}

type GetSPLTokenBalancesForAddressAndChainIdArg = {
  accountId: BraveWallet.AccountId
  chainId: string

  /**
   * optional, if not provided, will fetch all tokens using
   * getTokenAccountsByOwner.
   */
  tokens: GetBlockchainTokenIdArg[]
  coin: typeof CoinTypes.SOL
}

type GetTokenBalancesForAddressAndChainIdArg = {
  accountId: BraveWallet.AccountId
  tokens: GetBlockchainTokenIdArg[]
  chainId: string
  coin:
    | typeof CoinTypes.ETH
    | typeof CoinTypes.FIL
    | typeof CoinTypes.BTC
    | typeof CoinTypes.ZEC
    | typeof CoinTypes.ADA
}

type GetTokenBalancesForChainIdArg =
  | GetSPLTokenBalancesForAddressAndChainIdArg
  | GetTokenBalancesForAddressAndChainIdArg

export type GetTokenBalancesRegistryArg = {
  accountIds: BraveWallet.AccountId[]
  networks: BalanceNetwork[]
  useAnkrBalancesFeature: boolean
  /** if true, only spam NFT balances will be fetched, if falsey, only user
   * token balances will be fetched */
  isSpamRegistry?: boolean
}

const MAX_NFT_BALANCE_CHECK_BATCH_SIZE = 50

function mergeTokenBalancesRegistry(
  a: TokenBalancesRegistry | { accounts?: undefined },
  b: TokenBalancesRegistry,
): TokenBalancesRegistry {
  const result: TokenBalancesRegistry = a?.accounts
    ? { ...a }
    : { accounts: {} }

  for (const accountId of Object.keys(b.accounts)) {
    const accountBalances = b.accounts[accountId]

    for (const chainId of Object.keys(accountBalances.chains)) {
      const chainBalances = accountBalances.chains[chainId]

      result.accounts[accountId] = result.accounts[accountId] || {
        chains: {},
      }

      result.accounts[accountId].chains[chainId] = chainBalances
    }
  }

  return result
}

export const tokenBalancesEndpoints = ({
  mutation,
  query,
}: WalletApiEndpointBuilderParams) => {
  return {
    getAccountTokenCurrentBalance: query<
      string,
      GetAccountTokenCurrentBalanceArg
    >({
      queryFn: async (arg, { endpoint }, extraOptions, baseQuery) => {
        const { data: api } = baseQuery(undefined)
        const {
          jsonRpcService,
          bitcoinWalletService,
          zcashWalletService,
          cardanoWalletService,
        } = api
        try {
          return {
            data: await fetchAccountTokenCurrentBalance({
              arg,
              bitcoinWalletService,
              jsonRpcService,
              zcashWalletService,
              cardanoWalletService,
            }),
          }
        } catch (error) {
          return handleEndpointError(
            endpoint,
            'Failed to get account token current balance',
            error,
          )
        }
      },
      providesTags: (result, error, { token }) =>
        cacher.cacheByBlockchainTokenArg('AccountTokenCurrentBalance')(
          result,
          error,
          token,
        ),
    }),

    getIsTokenOwnedByUser: query<boolean, GetBlockchainTokenIdArg>({
      queryFn: async (arg, { endpoint }, extraOptions, baseQuery) => {
        const { data: api, cache } = baseQuery(undefined)
        const {
          jsonRpcService,
          bitcoinWalletService,
          zcashWalletService,
          cardanoWalletService,
        } = api
        try {
          const { accounts } = await cache.getAllAccounts()
          const accountForCoinType = accounts.filter((account) => {
            return account.accountId.coin === arg.coin
          })
          let isOwned = false
          for (const { accountId } of accountForCoinType) {
            const balance = await fetchAccountTokenCurrentBalance({
              arg: {
                accountId,
                token: arg,
              },
              bitcoinWalletService,
              jsonRpcService,
              zcashWalletService,
              cardanoWalletService,
            })
            if (new Amount(balance).gt(0)) {
              isOwned = true
              break
            }
          }

          return {
            data: isOwned,
          }
        } catch (error) {
          return handleEndpointError(
            endpoint,
            `Failed to check if token (${getAssetIdKey(arg)}) is owned`,
            error,
          )
        }
      },
      providesTags: (result, error, token) =>
        cacher.cacheByBlockchainTokenArg('AccountTokenCurrentBalance')(
          result,
          error,
          token,
        ),
    }),

    getTokenBalancesForChainId: query<
      TokenBalancesRegistry,
      GetTokenBalancesForChainIdArg[]
    >({
      queryFn: async (args, { endpoint }, extraOptions, baseQuery) => {
        const { data: api, cache } = baseQuery(undefined)
        const {
          jsonRpcService,
          bitcoinWalletService,
          zcashWalletService,
          cardanoWalletService,
        } = api

        try {
          const registry: TokenBalancesRegistry =
            await fetchTokenBalanceRegistryForAccountsAndChainIds({
              args,
              cache,
              bitcoinWalletService,
              jsonRpcService,
              zcashWalletService,
              cardanoWalletService,
            })

          // NFTs
          for (const arg of args) {
            const { accountId } = arg
            const nfts = arg.tokens.filter((token) => token.isNft)
            await fetchNftBalancesForAccount({
              accountId,
              nfts,
              onBalance: (balanceResult) =>
                setBalance({
                  accountId: accountId,
                  balance: balanceResult.balance,
                  chainId: balanceResult.chainId,
                  coinType: balanceResult.coinType,
                  contractAddress: balanceResult.contractAddress,
                  tokenId: balanceResult.tokenId,
                  tokenBalancesRegistry: registry,
                  isShielded: balanceResult.isShielded,
                }),
            })
          }

          return {
            data: registry,
          }
        } catch (error) {
          return handleEndpointError(
            endpoint,
            `Unable to fetch getTokenBalancesForChainId(
                ${JSON.stringify(args, undefined, 2)}
              )`,
            error,
          )
        }
      },
      providesTags: (result, err, args) =>
        err
          ? ['TokenBalancesForChainId', 'UNKNOWN_ERROR']
          : args.flatMap(({ tokens, accountId, coin, chainId }) =>
              tokens
                ? tokens.map((token) => ({
                    type: 'TokenBalancesForChainId',
                    id: [
                      accountId.uniqueKey,
                      coin,
                      chainId,
                      token.contractAddress,
                    ].join('-'),
                  }))
                : [
                    {
                      type: 'TokenBalancesForChainId',
                      id: `${accountId.uniqueKey}-${coin}-${chainId}`,
                    },
                  ],
            ) || ['TokenBalancesForChainId'],
    }),

    getTokenBalancesRegistry: query<
      TokenBalancesRegistry | null,
      GetTokenBalancesRegistryArg
    >({
      queryFn: async (arg, { endpoint }, extraOptions, baseQuery) => {
        try {
          const { data: api, cache } = baseQuery(undefined)

          const {
            braveWalletService,
            jsonRpcService,
            bitcoinWalletService,
            zcashWalletService,
            cardanoWalletService,
          } = api

          const tokenBalancesRegistry = createEmptyTokenBalancesRegistry()

          const includeRewardsBalance = arg.networks.some(getIsRewardsNetwork)

          if (includeRewardsBalance) {
            const rewardsData =
              cache.rewardsInfo || (await cache.getBraveRewardsInfo())

            const {
              balance: rewardsBalance,
              provider: externalRewardsProvider,
              rewardsToken,
              status: rewardsStatus,
            } = rewardsData

            if (
              rewardsStatus === WalletStatus.kConnected
              && rewardsToken
              && externalRewardsProvider
              && rewardsBalance
            ) {
              // add rewards info to balance registry
              setBalance(
                // TODO(apaymyshev): we need a better way to handle such fake
                // account
                {
                  accountId: { uniqueKey: externalRewardsProvider },
                  chainId: BraveWallet.MAINNET_CHAIN_ID,
                  contractAddress: rewardsToken.contractAddress,
                  balance: new Amount(rewardsBalance)
                    .multiplyByDecimals(rewardsToken.decimals)
                    .format(),
                  tokenBalancesRegistry,
                  coinType: BraveWallet.CoinType.ETH,
                  tokenId: '',
                  isShielded: false,
                },
              )
            }
          }

          function onBalance(result: BalanceResult) {
            setBalance({
              ...result,
              tokenBalancesRegistry,
            })
          }

          function onAnkrBalances(
            accountId: BraveWallet.AccountId,
            ankrAssetBalances: BraveWallet.AnkrAssetBalance[],
          ) {
            for (const { asset, balance } of ankrAssetBalances) {
              setBalance({
                accountId,
                chainId: asset.chainId,
                contractAddress: asset.contractAddress,
                balance,
                tokenBalancesRegistry,
                coinType: asset.coin,
                tokenId: asset.tokenId,
                isShielded: asset.isShielded,
              })
            }
          }

          const ankrSupportedNetworks: BalanceNetwork[] = []
          const nonAnkrSupportedNetworks: BalanceNetwork[] =
            arg.useAnkrBalancesFeature ? [] : arg.networks

          if (arg.useAnkrBalancesFeature) {
            const { chainIds: ankrSupportedChainIds } =
              await braveWalletService.getAnkrSupportedChainIds()

            for (const network of arg.networks) {
              if (ankrSupportedChainIds.includes(network.chainId)) {
                ankrSupportedNetworks.push(network)
              } else {
                nonAnkrSupportedNetworks.push(network)
              }
            }
          }

          await eachLimit(
            arg.accountIds,
            arg.accountIds.length,
            async (accountId: BraveWallet.AccountId) => {
              const ankrSupportedAccountChainIds = ankrSupportedNetworks.filter(
                (network) => networkSupportsAccount(network, accountId),
              )

              const ankrAssetBalances: BraveWallet.AnkrAssetBalance[] =
                ankrSupportedAccountChainIds.length
                  ? await fetchAnkrAccountBalances(
                      jsonRpcService,
                      accountId,
                      ankrSupportedAccountChainIds,
                    )
                  : []

              onAnkrBalances(accountId, ankrAssetBalances)

              const nonAnkrSupportedAccountNetworks =
                nonAnkrSupportedNetworks.filter((network) =>
                  networkSupportsAccount(network, accountId),
                )

              const userTokensRegistry = arg.isSpamRegistry
                ? blockchainTokenEntityAdaptorInitialState
                : await cache.getUserTokensRegistry()

              const spamTokens = arg.isSpamRegistry
                ? await cache.getSpamNftsForAccountId(accountId)
                : []

              if (nonAnkrSupportedAccountNetworks.length) {
                await eachLimit(
                  nonAnkrSupportedAccountNetworks,
                  3,
                  async (network: BraveWallet.NetworkInfo) => {
                    assert(coinTypesMapping[network.coin] !== undefined)
                    try {
                      const tokens = arg.isSpamRegistry
                        ? spamTokens.filter(
                            (token) =>
                              token.coin === network.coin
                              && token.chainId === network.chainId,
                          )
                        : getEntitiesListFromEntityState(
                            userTokensRegistry,
                            userTokensRegistry.idsByChainId[
                              getNetworkId({
                                coin: network.coin,
                                chainId: network.chainId,
                              })
                            ],
                          )

                      await fetchTokenBalanceRegistryForAccountsAndChainIds({
                        args: [
                          {
                            accountId,
                            coin: coinTypesMapping[network.coin],
                            chainId: network.chainId,
                            tokens,
                          },
                        ],
                        cache,
                        bitcoinWalletService,
                        jsonRpcService,
                        zcashWalletService,
                        cardanoWalletService,
                        onBalance,
                      })
                    } catch (error) {
                      console.error(error)
                    }
                  },
                )
              }

              // NFTs
              const nfts = arg.isSpamRegistry
                ? spamTokens.filter(
                    (token) => token.isNft && token.coin === accountId.coin,
                  )
                : getEntitiesListFromEntityState(
                    userTokensRegistry,
                    userTokensRegistry.nonFungibleIdsByCoinType[accountId.coin],
                  )
              await fetchNftBalancesForAccount({
                accountId,
                nfts,
                onBalance,
              })
            },
          )

          const mergedRegistry = mergeTokenBalancesRegistry(
            getPersistedTokenBalances(arg.isSpamRegistry),
            tokenBalancesRegistry,
          )

          // update combined registry in local storage
          setPersistedPortfolioTokenBalances(mergedRegistry, arg.isSpamRegistry)

          // report to P3A
          reportActiveWalletsToP3A(arg.accountIds, mergedRegistry)

          return {
            data: tokenBalancesRegistry,
          }
        } catch (error) {
          return handleEndpointError(
            endpoint,
            `Failed to get token balances: ${error}`,
            error,
          )
        }
      },
      providesTags: (result, err, args) =>
        err
          ? ['TokenBalances', 'UNKNOWN_ERROR']
          : args.accountIds.flatMap((accountId) => {
              const networkKeys = args.networks
                .filter((network) => accountId.coin === network.coin)
                .map((network) => network.chainId)
              const accountKey = getAccountBalancesKey(accountId)
              return networkKeys.map((networkKey) => ({
                type: 'TokenBalances',
                id: `${accountKey}-${networkKey}`,
              }))
            }),
    }),

    getHardwareAccountDiscoveryBalance: query<
      string,
      GetHardwareAccountDiscoveryBalanceArg
    >({
      queryFn: async (
        { coin, chainId, address },
        { endpoint },
        extraOptions,
        baseQuery,
      ) => {
        try {
          // apiProxy
          const { jsonRpcService, bitcoinWalletService } =
            baseQuery(undefined).data

          switch (coin) {
            case BraveWallet.CoinType.SOL: {
              const { balance, errorMessage } =
                await jsonRpcService.getSolanaBalance(address, chainId)

              if (errorMessage) {
                console.log(`getBalance error: ${errorMessage}`)
                return {
                  error: errorMessage,
                }
              }

              return {
                data: Amount.normalize(balance.toString()),
              }
            }

            case BraveWallet.CoinType.FIL:
            case BraveWallet.CoinType.ETH: {
              const { balance, errorMessage } = await jsonRpcService.getBalance(
                address,
                coin,
                chainId,
              )

              if (errorMessage) {
                console.log(`getBalance error: ${errorMessage}`)
                return {
                  error: errorMessage,
                }
              }

              return {
                data: Amount.normalize(balance),
              }
            }

            case BraveWallet.CoinType.BTC: {
              const { balance, errorMessage } =
                await bitcoinWalletService.getExtendedKeyAccountBalance(
                  chainId,
                  address,
                )
              if (errorMessage) {
                console.log(`getBalance error: ${errorMessage}`)
                return {
                  error: errorMessage,
                }
              }
              return {
                data: Amount.normalize(balance?.totalBalance.toString() || '0'),
              }
            }

            default: {
              assertNotReached(`Unknown coin ${coin}`)
            }
          }
        } catch (error) {
          return handleEndpointError(
            endpoint,
            `Unable to fetch balance(
                ${JSON.stringify({ coin, chainId, address }, undefined, 2)}
              )`,
            error,
          )
        }
      },
      providesTags: (result, err, arg) => [
        {
          type: 'HardwareAccountDiscoveryBalance',
          id: [arg.coin, arg.chainId, arg.address].join('-'),
        },
      ],
    }),
    getBitcoinBalances: query<BitcoinBalances, BraveWallet.AccountId>({
      queryFn: async (arg, { endpoint }, extraOptions, baseQuery) => {
        try {
          const { data: api } = baseQuery(undefined)
          const { balance, errorMessage } =
            await api.bitcoinWalletService.getBalance(arg)

          if (errorMessage || balance === null) {
            throw new Error(errorMessage || 'Unknown error')
          }
          return {
            data: {
              availableBalance: Amount.normalize(
                balance.availableBalance.toString(),
              ),
              pendingBalance: Amount.normalize(
                balance.pendingBalance.toString(),
              ),
              totalBalance: Amount.normalize(balance.totalBalance.toString()),
            },
          }
        } catch (error) {
          return handleEndpointError(
            endpoint,
            `Failed to fetch Bitcoin balances: ${JSON.stringify(
              arg,
              undefined,
              2,
            )}`,
            error,
          )
        }
      },
    }),
  }
}

// Internals
async function fetchAnkrAccountBalances(
  jsonRpcService: BraveWallet.JsonRpcServiceRemote,
  accountId: BraveWallet.AccountId,
  chainIds: BraveWallet.ChainId[],
) {
  const { balances, error, errorMessage } =
    await jsonRpcService.ankrGetAccountBalances(accountId.address, chainIds)

  if (error !== BraveWallet.ProviderError.kSuccess || errorMessage) {
    console.log(
      `ankrGetAccountBalance error: ${error}
                      msg: ${errorMessage}`,
    )
  }

  return balances || []
}

async function fetchAccountCurrentNativeBalance({
  arg: { accountId, token },
  jsonRpcService,
  bitcoinWalletService,
  zcashWalletService,
  cardanoWalletService,
}: {
  arg: GetAccountTokenCurrentBalanceArg
  jsonRpcService: BraveWallet.JsonRpcServiceRemote
  bitcoinWalletService: BraveWallet.BitcoinWalletServiceRemote
  zcashWalletService: BraveWallet.ZCashWalletServiceRemote
  cardanoWalletService: BraveWallet.CardanoWalletServiceRemote
}): Promise<string> {
  // LOCALHOST
  if (
    token.chainId === BraveWallet.LOCALHOST_CHAIN_ID
    && accountId.coin !== BraveWallet.CoinType.SOL
  ) {
    const { balance, error, errorMessage } = await jsonRpcService.getBalance(
      accountId.address,
      accountId.coin,
      token.chainId,
    )

    // LOCALHOST will error until a local instance is detected
    // return a '0' balance until it's detected.
    if (error !== 0) {
      console.log(
        `getBalance (LOCALHOST - ${accountId.coin}) error: ${errorMessage}`,
      )
      return Amount.zero().format()
    }

    return Amount.normalize(balance)
  }

  // NON-LOCALHOST
  switch (accountId.coin) {
    case BraveWallet.CoinType.SOL: {
      const { balance, error } = await jsonRpcService.getSolanaBalance(
        accountId.address,
        token.chainId,
      )

      if (token.chainId === BraveWallet.LOCALHOST_CHAIN_ID && error !== 0) {
        return Amount.zero().format()
      }

      return Amount.normalize(balance.toString())
    }

    case BraveWallet.CoinType.FIL:
    case BraveWallet.CoinType.ETH: {
      const { balance, error, errorMessage } = await jsonRpcService.getBalance(
        accountId.address,
        accountId.coin,
        token.chainId,
      )

      if (error && errorMessage) {
        throw new Error(
          `getBalance (${
            accountId.coin === BraveWallet.CoinType.FIL ? 'FIL' : 'ETH'
          } - ${token.chainId}) error: ${errorMessage}`,
        )
      }

      return Amount.normalize(balance)
    }

    case BraveWallet.CoinType.BTC: {
      const { balance, errorMessage } =
        await bitcoinWalletService.getBalance(accountId)

      if (errorMessage || balance === null) {
        throw new Error(
          `getBalance (BTC) error: ${errorMessage || 'Unknown error'}`,
        )
      }

      return Amount.normalize(balance.totalBalance.toString())
    }

    case BraveWallet.CoinType.ZEC: {
      const { balance, errorMessage } = await zcashWalletService.getBalance(
        token.chainId,
        accountId,
      )

      if (errorMessage || balance === null) {
        throw new Error(
          `getBalance (ZEC) error: ${errorMessage || 'Unknown error'}`,
        )
      }

      return token.isShielded
        ? Amount.normalize(balance.shieldedBalance.toString())
        : Amount.normalize(balance.transparentBalance.toString())
    }

    case BraveWallet.CoinType.ADA: {
      const { balance, errorMessage } =
        await cardanoWalletService.getBalance(accountId)

      if (errorMessage || balance === null) {
        throw new Error(
          `getBalance (ADA) error: ${errorMessage || 'Unknown error'}`,
        )
      }

      return Amount.normalize(balance.totalBalance.toString())
    }

    default: {
      assertNotReached(`Unknown coin ${accountId.coin}`)
    }
  }
}

async function fetchAccountTokenCurrentBalance({
  arg: { accountId, token },
  jsonRpcService,
  bitcoinWalletService,
  zcashWalletService,
  cardanoWalletService,
}: {
  arg: GetAccountTokenCurrentBalanceArg
  jsonRpcService: BraveWallet.JsonRpcServiceRemote
  bitcoinWalletService: BraveWallet.BitcoinWalletServiceRemote
  zcashWalletService: BraveWallet.ZCashWalletServiceRemote
  cardanoWalletService: BraveWallet.CardanoWalletServiceRemote
}): Promise<string> {
  // Native asset balances
  if (isNativeAsset(token)) {
    return fetchAccountCurrentNativeBalance({
      arg: { accountId, token },
      jsonRpcService,
      bitcoinWalletService,
      zcashWalletService,
      cardanoWalletService,
    })
  }

  // Token Balances
  switch (accountId.coin) {
    // Ethereum Network tokens
    case BraveWallet.CoinType.ETH: {
      const { balance, error, errorMessage } = token.isErc721
        ? await jsonRpcService.getERC721TokenBalance(
            token.contractAddress,
            token.tokenId ?? '',
            accountId.address,
            token.chainId,
          )
        : await jsonRpcService.getERC20TokenBalance(
            token.contractAddress,
            accountId.address,
            token.chainId,
          )

      if (error && errorMessage) {
        throw new Error(
          errorMessage
            + `-- ERC${token.isErc721 ? '721' : '20'} -- ${token.chainId} -- ${
              token.contractAddress
            }`
            || `Unknown ERC${
              token.isErc721 ? '721' : '20'
            } Balance error for chain: ${token.chainId} and contract: ${
              token.contractAddress
            }`,
        )
      }

      return Amount.normalize(balance)
    }
    // Solana Network Tokens
    case BraveWallet.CoinType.SOL: {
      if (token.isNft) {
        const { balances, errorMessage } = await jsonRpcService.getNftBalances(
          accountId.address,
          [
            {
              chainId: { coin: token.coin, chainId: token.chainId },
              contractAddress: token.contractAddress,
              tokenId: token.tokenId,
            },
          ],
        )

        if (errorMessage) {
          throw new Error(
            `Unable to fetch Compressed NFT (${
              token.contractAddress //
            }) balance -- error: ${errorMessage}`,
          )
        }

        return balances[0].toString()
      }

      const { amount, uiAmountString, error, errorMessage } =
        await jsonRpcService.getSPLTokenAccountBalance(
          accountId.address,
          token.contractAddress,
          token.chainId,
        )

      if (error && errorMessage) {
        throw new Error(
          errorMessage + `-- SPL (${token.chainId})`
            || `Unknown SPL balance error on chain: ${
              token.chainId //
            } and contract: ${token.contractAddress}`,
        )
      }

      return token.isNft ? uiAmountString : amount
    }

    // Other network type tokens
    default: {
      return Amount.zero().format()
    }
  }
}

async function fetchAccountTokenBalanceRegistryForChainId({
  arg,
  bitcoinWalletService,
  jsonRpcService,
  zcashWalletService,
  cardanoWalletService,
  onBalance,
  cache,
}: {
  arg: GetTokenBalancesForChainIdArg
  jsonRpcService: BraveWallet.JsonRpcServiceRemote
  bitcoinWalletService: BraveWallet.BitcoinWalletServiceRemote
  zcashWalletService: BraveWallet.ZCashWalletServiceRemote
  cardanoWalletService: BraveWallet.CardanoWalletServiceRemote
  onBalance: (arg: BalanceResult) => void | Promise<void>
  cache: BaseQueryCache
}): Promise<void> {
  const nativeTokenArgs = arg.tokens.filter((token) => isNativeAsset(token))

  if (arg.coin === CoinTypes.ETH) {
    const supportedChainIds =
      cache.balanceScannerSupportedChains
      || (await cache.getBalanceScannerSupportedChains())

    if (supportedChainIds.includes(arg.chainId)) {
      // Filter for ERC20 tokens, including MATIC token on Polygon
      //
      // POL balance is fetched using the native asset contract, and not the
      // proxy ERC20 contract (0x0000000000000000000000000000000000001010) to
      // avoid double counting the balance.
      const erc20Tokens = arg.tokens.filter(
        (token) =>
          !token.isNft
          && !isNativeAsset(token)
          && !(
            token.chainId === BraveWallet.POLYGON_MAINNET_CHAIN_ID
            && token.contractAddress === POLYGON_NATIVE_ASSET_CONTRACT_ADDRESS
          ),
      )

      const tokens = [
        ...nativeTokenArgs.map((token) => ({
          ...token,
          contractAddress: NATIVE_EVM_ASSET_CONTRACT_ADDRESS,
        })),
        ...erc20Tokens,
      ]

      // nothing to fetch
      if (!tokens.length) {
        return
      }

      const result = await jsonRpcService.getERC20TokenBalances(
        tokens.map((token) => token.contractAddress),
        arg.accountId.address,
        arg.chainId,
      )

      if (result.error !== BraveWallet.ProviderError.kSuccess) {
        throw new Error(
          'Error calling '
            + 'jsonRpcService.getERC20TokenBalances: '
            + `error=${result.errorMessage} arg=`
            + JSON.stringify(
              {
                tokens: erc20Tokens,
                address: arg.accountId.address,
                chainId: arg.chainId,
              },
              undefined,
              2,
            ),
        )
      }

      for (const { balance, contractAddress } of result.balances) {
        if (balance) {
          onBalance({
            accountId: arg.accountId,
            chainId: arg.chainId,
            contractAddress:
              contractAddress === NATIVE_EVM_ASSET_CONTRACT_ADDRESS
                ? ''
                : contractAddress,
            balance: new Amount(balance).format(),
            coinType: arg.coin,
            tokenId: '', // these are ERC20 tokens,
            isShielded: false,
          })
        }
      }

      return
    }
  }

  await eachLimit(
    nativeTokenArgs,
    2,
    async (token: BraveWallet.BlockchainToken) => {
      const balance = await fetchAccountTokenCurrentBalance({
        arg: {
          accountId: arg.accountId,
          token: token,
        },
        bitcoinWalletService,
        jsonRpcService,
        zcashWalletService,
        cardanoWalletService,
      })

      if (balance) {
        onBalance({
          accountId: arg.accountId,
          chainId: arg.chainId,
          contractAddress: token.contractAddress,
          balance,
          coinType: token.coin,
          tokenId: '',
          isShielded: token.isShielded,
        })
      }
    },
  )

  if (arg.coin === CoinTypes.SOL) {
    const result = await jsonRpcService.getSPLTokenBalances(
      arg.accountId.address,
      arg.chainId,
    )

    if (result.error !== BraveWallet.ProviderError.kSuccess) {
      throw new Error(
        `Error calling jsonRpcService.getSPLTokenBalances:
              error=${result.errorMessage}
              arg=` + JSON.stringify(arg, undefined, 2),
      )
    }

    for (const { mint, amount } of result.balances) {
      if (amount && new Amount(amount).gt(0)) {
        onBalance({
          accountId: arg.accountId,
          chainId: arg.chainId,
          contractAddress: mint,
          balance: Amount.normalize(amount),
          coinType: arg.coin,
          tokenId: '',
          isShielded: false,
        })
      }
    }

    return
  }

  // Fallback to fetching individual balances
  const nonNativeTokens = arg.tokens.filter((token) => !isNativeAsset(token))
  await eachLimit(
    nonNativeTokens,
    10,
    async (token: BraveWallet.BlockchainToken) => {
      const result = await fetchAccountTokenCurrentBalance({
        arg: {
          accountId: arg.accountId,
          token,
        },
        bitcoinWalletService,
        jsonRpcService,
        zcashWalletService,
        cardanoWalletService,
      })

      if (result && new Amount(result).gt(0)) {
        onBalance({
          accountId: arg.accountId,
          chainId: arg.chainId,
          contractAddress: token.contractAddress,
          balance: result,
          coinType: token.coin,
          tokenId: token.tokenId,
          isShielded: token.isShielded,
        })
      }
    },
  )
}

async function fetchTokenBalanceRegistryForAccountsAndChainIds({
  args,
  jsonRpcService,
  bitcoinWalletService,
  zcashWalletService,
  cardanoWalletService,
  onBalance,
  cache,
}: {
  args: GetTokenBalancesForChainIdArg[]
  jsonRpcService: BraveWallet.JsonRpcServiceRemote
  bitcoinWalletService: BraveWallet.BitcoinWalletServiceRemote
  zcashWalletService: BraveWallet.ZCashWalletServiceRemote
  cardanoWalletService: BraveWallet.CardanoWalletServiceRemote
  onBalance?: (arg: BalanceResult) => void | Promise<void>
  cache: BaseQueryCache
}): Promise<TokenBalancesRegistry> {
  const tokenBalancesRegistry = createEmptyTokenBalancesRegistry()

  await eachLimit(args, 1, async (arg: GetTokenBalancesForChainIdArg) => {
    await fetchAccountTokenBalanceRegistryForChainId({
      arg,
      bitcoinWalletService,
      jsonRpcService,
      zcashWalletService,
      cardanoWalletService,
      cache,
      onBalance:
        onBalance
        || function (result: BalanceResult) {
          setBalance({
            ...result,
            tokenBalancesRegistry,
          })
        },
    })
  })

  return tokenBalancesRegistry
}

async function fetchNftBalancesForAccount({
  nfts,
  accountId,
  onBalance,
}: {
  nfts: GetBlockchainTokenIdArg[]
  accountId: BraveWallet.AccountId
  onBalance: (arg: BalanceResult) => void | Promise<void>
}) {
  if (nfts.length) {
    const { jsonRpcService } = getAPIProxy()

    // batch nfts into batches of 50 or less,
    // then fetch balances for each batch
    // this is to avoid hitting the max request size
    // Can be removed once core implements batching
    // https://github.com/brave/brave-browser/issues/39954
    const nftBatches = []
    for (let i = 0; i < nfts.length; i += MAX_NFT_BALANCE_CHECK_BATCH_SIZE) {
      nftBatches.push(nfts.slice(i, i + MAX_NFT_BALANCE_CHECK_BATCH_SIZE))
    }

    // get balances for groups
    for (const nftsBatch of nftBatches) {
      if (!nftsBatch.length) {
        continue
      }
      const {
        balances: nftBatchBalances,
        errorMessage: nftBatchBalancesErrorMessage,
      } = await jsonRpcService.getNftBalances(
        accountId.address,
        nftsBatch.map((token) => {
          return {
            chainId: { coin: token.coin, chainId: token.chainId },
            contractAddress: token.contractAddress,
            tokenId: token.tokenId,
          }
        }),
      )

      if (nftBatchBalancesErrorMessage) {
        console.warn(
          'An error occurred while fetching NFT balances: '
            + nftBatchBalancesErrorMessage,
        )
        console.warn({
          failedNftBalanceCheckBatchArgs: {
            address: accountId.address,
            nfts: nftsBatch,
            coin: accountId.coin,
          },
        })
      }

      nftBatchBalances.forEach((nftBalance, index) => {
        const token = nftsBatch[index]
        if (!token) {
          return
        }
        onBalance({
          accountId: accountId,
          chainId: token.chainId,
          contractAddress: token.contractAddress,
          balance: nftBalance.toString(),
          coinType: token.coin,
          tokenId: token.tokenId,
          isShielded: false,
        })
      })
    }
  }
}
