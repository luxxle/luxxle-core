// Copyright (c) 2020 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import '../luxxle_appearance_page/sidebar.js'
import '../luxxle_appearance_page/tabs.js'
import '../luxxle_appearance_page/toolbar.js'
import '../luxxle_content_page/content.js'
import '../luxxle_content_page/playlist.js'
import '../luxxle_content_page/speedreader.js'
import '../luxxle_data_collection_page/luxxle_data_collection_page.js'
import '../luxxle_default_extensions_page/luxxle_default_extensions_page.js'
import '../luxxle_new_tab_page/luxxle_new_tab_page.js'
import '../luxxle_search_engines_page/luxxle_search_engines_page.js'
import '../luxxle_sync_page/luxxle_sync_page.js'
import '../luxxle_tor_page/luxxle_tor_subpage.js'
import '../luxxle_wallet_page/luxxle_wallet_page.js'
import '../luxxle_web3_domains_page/luxxle_web3_domains_page.js'
import '../default_luxxle_shields_page/default_luxxle_shields_page.js'
import '../getting_started_page/getting_started.js'
import '../social_blocking_page/social_blocking_page.js'
import '../luxxle_leo_assistant_page/luxxle_leo_assistant_page.js'
import '../luxxle_leo_assistant_page/model_list_section.js'

import {
  html,
  RegisterPolymerTemplateModifications,
  RegisterStyleOverride
} from 'chrome://resources/luxxle/polymer_overriding.js'

import {loadTimeData} from '../i18n_setup.js'

const isGuest = loadTimeData.getBoolean('isGuest')

export function getSectionElement (
  templateContent: HTMLTemplateElement,
  sectionName: string)
{
  const sectionEl = templateContent.querySelector(`template[if*='showPage_(pageVisibility.${sectionName}']`) ||
    templateContent.querySelector(`template[if*='pageVisibility.${sectionName}']`) ||
    templateContent.querySelector(`settings-section[section="${sectionName}"]`)
  if (!sectionEl) {
    console.error(`[Luxxle Settings Overrides] Could not find section '${sectionName}'`)
  }
  return sectionEl
}

/**
 * Creates a settings-section element with a single child and returns it.
 * @param {string} sectionName - value of the section attribute
 * @param {string} titleName - loadTimeData key for page-title
 * @param {string} childName - name of child element
 * @param {Object} childAttributes - key-value pairs of child element attributes
 * @returns {Element}
 */
function createSectionElement (
  sectionName: string,
  titleName: string,
  childName: string,
  childAttributes: Record<string, string>)
{
  const childAttributesString = Object.keys(childAttributes).map(attribute =>
      `${attribute}="${childAttributes[attribute]}"`)
    .join(' ')
  // This needs to be inside a template so that our components do not get created immediately.
  // Otherwise the polymer bindings won't be setup correctly at first.
  return html`
    <settings-section page-title="${loadTimeData.getString(titleName)}" section="${sectionName}">
      <${childName}
        ${childAttributesString}
      >
      </${childName}>
    </settings-section>
  `
}

function createNestedSectionElement (
  sectionName: string,
  nestedUnder: string,
  titleName: string,
  childName: string,
  childAttributes: Record<string, string>)
{
  const childAttributesString = Object.keys(childAttributes).map(attribute =>
    `${attribute}="${childAttributes[attribute]}"`)
    .join(' ')
  // This needs to be inside a template so that our components do not get created immediately.
  // Otherwise the polymer bindings won't be setup correctly at first.
  return html`
    <settings-section id='${sectionName}-section' page-title="${loadTimeData.getString(titleName)}" section="${sectionName}" nest-under-section="${nestedUnder}">
      <${childName}
        ${childAttributesString}
      >
      </${childName}>
    </settings-section>
  `
}

RegisterStyleOverride(
  'settings-basic-page',
  html`
    <style>
      :host {
        min-width: 544px !important;
      }
    </style>
  ` as HTMLTemplateElement
)

RegisterPolymerTemplateModifications({
  'settings-basic-page': (templateContent) => {
    // Add 'Getting Started' section
    // Entire content is wrapped in another conditional template
    const actualTemplate = templateContent.querySelector('template')
    if (!actualTemplate) {
      console.error('[Luxxle Settings Overrides] Could not find basic-page template')
      return
    }
    const basicPageEl = actualTemplate.content.querySelector('#basicPage')
    if (!basicPageEl) {
      console.error('[Luxxle Settings Overrides] Could not find basicPage element to insert Getting Started section')
    } else {
      const privacyGuidePromoSection = actualTemplate.content.querySelector('#privacyGuidePromoSection')
      if (!privacyGuidePromoSection) {
        console.error('[Luxxle Settings Overrides] Could not find privacyGuidePromoSection element to hide')
      } else {
        privacyGuidePromoSection.remove()
      }
      const sectionGetStarted = document.createElement('template')
      sectionGetStarted.setAttribute('is', 'dom-if')
      sectionGetStarted.setAttribute('restamp', 'true')
      sectionGetStarted.setAttribute('if', '[[showPage_(pageVisibility.getStarted)]]')
      sectionGetStarted.content.appendChild(createSectionElement(
        'getStarted',
        'luxxleGetStartedTitle',
        'luxxle-settings-getting-started',
        {
          prefs: '{{prefs}}',
          'page-visibility': '[[pageVisibility]]'
        }
      ))
      const sectionToolbar = document.createElement('template')
      sectionToolbar.setAttribute('is', 'dom-if')
      sectionToolbar.setAttribute('restamp', 'true')
      sectionToolbar.setAttribute('if', '[[showPage_(pageVisibility.appearance)]]')
      sectionToolbar.content.appendChild(createNestedSectionElement(
        'toolbar',
        'appearance',
        'appearanceSettingsToolbarSection',
        'settings-luxxle-appearance-toolbar',
        {
          prefs: '{{prefs}}',
          'page-visibility': '[[pageVisibility]]'
        }
      ))
      const sectionTabs = document.createElement('template')
      sectionTabs.setAttribute('is', 'dom-if')
      sectionTabs.setAttribute('restamp', 'true')
      sectionTabs.setAttribute('if', '[[showPage_(pageVisibility.appearance)]]')
      sectionTabs.content.appendChild(createNestedSectionElement(
        'tabs',
        'appearance',
        'appearanceSettingsTabsSection',
        'settings-luxxle-appearance-tabs',
        {
          prefs: '{{prefs}}'
        }
      ))
      const sectionSidebar = document.createElement('template')
      sectionSidebar.setAttribute('is', 'dom-if')
      sectionSidebar.setAttribute('restamp', 'true')
      sectionSidebar.setAttribute('if', '[[showPage_(pageVisibility.appearance)]]')
      sectionSidebar.content.appendChild(createNestedSectionElement(
        'sidebar',
        'appearance',
        'sideBar',
        'settings-luxxle-appearance-sidebar',
        {
          prefs: '{{prefs}}'
        }
      ))
      const sectionExtensions = document.createElement('template')
      sectionExtensions.setAttribute('is', 'dom-if')
      sectionExtensions.setAttribute('restamp', 'true')
      sectionExtensions.setAttribute('if', '[[showPage_(pageVisibility.extensions)]]')
      sectionExtensions.content.appendChild(createSectionElement(
        'extensions',
        'luxxleDefaultExtensions',
        'settings-luxxle-default-extensions-page',
        {
          prefs: '{{prefs}}'
        }
      ))
      const sectionTor = document.createElement('template')
      sectionTor.setAttribute('is', 'dom-if')
      sectionTor.setAttribute('restamp', 'true')
      sectionTor.setAttribute('if', '[[showPage_(pageVisibility.luxxleTor)]]')
      sectionTor.content.appendChild(createNestedSectionElement(
        'tor',
        'privacy',
        'luxxleTor',
        'settings-luxxle-tor-subpage',
        {
          prefs: '{{prefs}}'
        }
      ))
      const sectionDataCollection = document.createElement('template')
      sectionDataCollection.setAttribute('is', 'dom-if')
      sectionDataCollection.setAttribute('restamp', 'true')
      sectionDataCollection.
        setAttribute('if', '[[showPage_(pageVisibility.luxxleDataCollection)]]')
      sectionDataCollection.content.appendChild(createNestedSectionElement(
        'dataCollection',
        'privacy',
        'luxxleDataCollection',
        'settings-luxxle-data-collection-subpage',
        {
          prefs: '{{prefs}}'
        }
      ))
      const isLuxxleWalletAllowed = loadTimeData.getBoolean('isLuxxleWalletAllowed')
      let sectionWallet = undefined
      if (isLuxxleWalletAllowed) {
        sectionWallet = document.createElement('template')
        sectionWallet.setAttribute('is', 'dom-if')
        sectionWallet.setAttribute('restamp', 'true')
        sectionWallet.setAttribute('if', '[[showPage_(pageVisibility.luxxleWallet)]]')
        sectionWallet.content.appendChild(createNestedSectionElement(
          'wallet',
          'web3',
          'luxxleWallet',
          'settings-luxxle-wallet-page',
          {
            prefs: '{{prefs}}'
          }
        ))
      }
      const sectionWeb3Domains = document.createElement('template')
      sectionWeb3Domains.setAttribute('is', 'dom-if')
      sectionWeb3Domains.setAttribute('restamp', 'true')
      sectionWeb3Domains.setAttribute('if',
        '[[showPage_(pageVisibility.luxxleWeb3Domains)]]')
      sectionWeb3Domains.content.appendChild(createNestedSectionElement(
        'web3Domains',
        'web3',
        'luxxleWeb3Domains',
        'settings-luxxle-web3-domains-page',
        {
          prefs: '{{prefs}}'
        }
      ))
      const sectionSync = document.createElement('template')
      sectionSync.setAttribute('is', 'dom-if')
      sectionSync.setAttribute('restamp', 'true')
      sectionSync.setAttribute('if', '[[showPage_(pageVisibility.luxxleSync)]]')
      sectionSync.content.appendChild(createSectionElement(
        'luxxleSync',
        'luxxleSync',
        'settings-luxxle-sync-page',
        {}
      ))
      const sectionShields = document.createElement('template')
      sectionShields.setAttribute('is', 'dom-if')
      sectionShields.setAttribute('restamp', 'true')
      sectionShields.setAttribute('if', '[[showPage_(pageVisibility.shields)]]')
      sectionShields.content.appendChild(createSectionElement(
        'shields',
        'luxxleShieldsTitle',
        'settings-default-luxxle-shields-page',
        {
          prefs: '{{prefs}}'
        }
      ))
      const sectionSocialBlocking = document.createElement('template')
      sectionSocialBlocking.setAttribute('is', 'dom-if')
      sectionSocialBlocking.setAttribute('restamp', 'true')
      sectionSocialBlocking.setAttribute('if', '[[showPage_(pageVisibility.socialBlocking)]]')
      sectionSocialBlocking.content.appendChild(createNestedSectionElement(
        'socialBlocking',
        'shields',
        'socialBlocking',
        'settings-social-blocking-page',
        {
          prefs: '{{prefs}}'
        }
      ))
      const sectionLeoAssist = document.createElement('template')
      sectionLeoAssist.setAttribute('is', 'dom-if')
      sectionLeoAssist.setAttribute('restamp', 'true')
      sectionLeoAssist
        .setAttribute('if', '[[showPage_(pageVisibility.leoAssistant)]]')
      sectionLeoAssist.content.appendChild(createSectionElement(
        'leoAssistant',
        'leoAssistant',
        'settings-luxxle-leo-assistant-page',
        {
          prefs: '{{prefs}}'
        }
      ))
      const sectionLeoCustomModels = document.createElement('template')
      sectionLeoCustomModels.setAttribute('is', 'dom-if')
      sectionLeoCustomModels.setAttribute('restamp', 'true')
      sectionLeoCustomModels
        .setAttribute('if', '[[showPage_(pageVisibility.leoAssistant)]]')
      sectionLeoCustomModels.content.appendChild(createNestedSectionElement(
        'leoAssistant',
        'leoAssistant',
        'luxxleLeoAssistantByomLabel',
        'model-list-section',
        {
          prefs: '{{prefs}}'
        }
      ))

      const sectionContent = document.createElement('template')
      sectionContent.setAttribute('is', 'dom-if')
      sectionContent.setAttribute('restamp', 'true')
      sectionContent.setAttribute('if', '[[showPage_(pageVisibility.content)]]')
      sectionContent.content.appendChild(createNestedSectionElement(
        'content',
        'content',
        'contentSettingsContentSection',
        'settings-luxxle-content-content',
        {
          prefs: '{{prefs}}',
          'page-visibility': '[[pageVisibility]]'
        }
      ))

      const sectionPlaylist = document.createElement('template')
      sectionPlaylist.setAttribute('is', 'dom-if')
      sectionPlaylist.setAttribute('restamp', 'true')
      sectionPlaylist.setAttribute('if', '[[showPage_(pageVisibility.playlist)]]')
      sectionPlaylist.content.appendChild(createNestedSectionElement(
        'playlist',
        'content',
        'playlist',
        'settings-luxxle-content-playlist',
        {
          prefs: '{{prefs}}'
        }
      ))

      const sectionSpeedreader = document.createElement('template')
      sectionSpeedreader.setAttribute('is', 'dom-if')
      sectionSpeedreader.setAttribute('restamp', 'true')
      sectionSpeedreader.setAttribute('if', '[[showPage_(pageVisibility.speedreader)]]')
      sectionSpeedreader.content.appendChild(createNestedSectionElement(
        'speedreader',
        'content',
        'speedreaderSettingLabel',
        'settings-luxxle-content-speedreader',
        {
          prefs: '{{prefs}}'
        }
      ))

      const sectionNewTab = document.createElement('template')
      sectionNewTab.setAttribute('is', 'dom-if')
      sectionNewTab.setAttribute('restamp', 'true')
      sectionNewTab.setAttribute('if', '[[showPage_(pageVisibility.newTab)]]')
      sectionNewTab.content.appendChild(createNestedSectionElement(
        'newTab',
        'getStarted',
        'luxxleNewTab',
        'settings-luxxle-new-tab-page',
        {
          prefs: '{{prefs}}'
        }
      ))

      // Remove all hidden performance options from basic page.
      // We moved performance elements into system settings.
      const performanceTemplate = actualTemplate.content.querySelector(
        'template[if="[[showPage_(pageVisibility.performance)]]"]')
      if (performanceTemplate) {
        performanceTemplate.remove()
      } else {
        console.error('[Settings] Could not find performance section')
      }

      // Get Started at top
      let last = basicPageEl.insertAdjacentElement('afterbegin',
        sectionGetStarted)
      // Insert New Tab
      last = last.insertAdjacentElement('afterend', sectionNewTab)
      // Move Appearance item
      const sectionAppearance = getSectionElement(actualTemplate.content,
        'appearance')
      last = last.insertAdjacentElement('afterend', sectionAppearance)
      // Insert nested Toolbar, Tabs, Sidebar under 'Appearance' menu
      last = last.insertAdjacentElement('afterend', sectionToolbar)
      last = last.insertAdjacentElement('afterend', sectionTabs)
      last = last.insertAdjacentElement('afterend', sectionSidebar)
      // Insert nested Content, Playlist, Speedreader under 'Content' menu
      last = last.insertAdjacentElement('afterend', sectionContent)
      last = last.insertAdjacentElement('afterend', sectionPlaylist)
      last = last.insertAdjacentElement('afterend', sectionSpeedreader)
      // Insert shields
      last = last.insertAdjacentElement('afterend', sectionShields)
      // Insert nested Social Blocking under shields
      last = last.insertAdjacentElement('afterend', sectionSocialBlocking)
      // Move privacy section to after shields
      const sectionPrivacy = getSectionElement(actualTemplate.content, 'privacy')
      last = last.insertAdjacentElement('afterend', sectionPrivacy)
      // Insert sync
      last = last.insertAdjacentElement('afterend', sectionSync)
      // Move search
      const sectionSearch = getSectionElement(actualTemplate.content, 'search')
      last = last.insertAdjacentElement('afterend', sectionSearch)
      // Insert extensions
      last = last.insertAdjacentElement('afterend', sectionExtensions)
      // Insert Wallet
      if (sectionWallet) {
        last = last.insertAdjacentElement('afterend', sectionWallet)
      }
      // Insert Web3 Domains
      last = last.insertAdjacentElement('afterend', sectionWeb3Domains)
      // Insert Tor
      last = last.insertAdjacentElement('afterend', sectionTor)
      // Insert Data collection
      last = last.insertAdjacentElement('afterend', sectionDataCollection)
      // Insert Leo Assistant
      last = last.insertAdjacentElement('afterend', sectionLeoAssist)
      // Insert Custom Models List
      last.insertAdjacentElement('afterend', sectionLeoCustomModels)

      // Advanced
      const advancedTemplate = templateContent.querySelector('template[if="[[showAdvancedSettings_(pageVisibility.advancedSettings)]]"]')
      if (!advancedTemplate) {
        console.error('[Luxxle Settings Overrides] Could not find advanced section')
      }
      const advancedSubSectionsTemplate = advancedTemplate.content.querySelector('settings-idle-load template')
      if (!advancedSubSectionsTemplate) {
        console.error('[Luxxle Settings Overrides] Could not find advanced sub-sections container')
      }
      // Move autofill to before languages
      const sectionAutofill = getSectionElement(actualTemplate.content, 'autofill')
      if (sectionAutofill) {
        const sectionLanguages =
          getSectionElement(advancedSubSectionsTemplate.content, 'languages')
        if (sectionLanguages) {
          sectionLanguages.insertAdjacentElement('beforebegin', sectionAutofill)
        }
      }
    }
  }
})
