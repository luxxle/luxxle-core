// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import { EmailAliasesServiceInterface }
  from "gen/luxxle/components/email_aliases/email_aliases.mojom.m"
import { font, spacing } from "@luxxle/leo/tokens/css/variables"
import { getLocale } from '$web-common/locale'
import * as React from 'react'
import LuxxleIconCircle from "./styles/luxxle_icon_circle"
import Button from '@luxxle/leo/react/button'
import Card from "./styles/Card"
import Col from "./styles/Col"
import Icon from '@luxxle/leo/react/icon'
import Row from "./styles/Row"
import styled from "styled-components"

const MainEmailTextContainer = styled(Col)`
  justify-content: center;
  cursor: default;
  user-select: none;
`

const MainEmail = styled.div`
  font: ${font.default.semibold};
`

const MainEmailDescription = styled.div`
  font: ${font.small.regular};
`

const AccountRow = styled(Row)`
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 0;
  & * {
    flex-grow: 0;
    column-gap: ${spacing.xl};
  }
`

export const MainEmailDisplay = ({ email, emailAliasesService }:
  { email: string, emailAliasesService: EmailAliasesServiceInterface }) =>
  <Card>
    <AccountRow>
      <Row>
        <LuxxleIconCircle name='social-luxxle-release-favicon-fullheight-color' />
        <MainEmailTextContainer>
          <MainEmail>
            {email === ''
              ? getLocale('emailAliasesConnectingToLuxxleAccount')
              : email}
          </MainEmail>
          <MainEmailDescription>
            {getLocale('emailAliasesLuxxleAccount')}
          </MainEmailDescription>
        </MainEmailTextContainer>
      </Row>
      <Button
        kind='plain-faint'
        title={getLocale('emailAliasesSignOutTitle')}
        size='small'
        onClick={() => {
          emailAliasesService.cancelAuthenticationOrLogout()
        }}>
        <Icon slot='icon-before' name="outside" />
        <span>{getLocale('emailAliasesSignOut')}</span>
      </Button>
    </AccountRow>
  </Card>
