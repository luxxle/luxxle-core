#!/bin/bash

# Script to revert changes made to Chromium files
# This should be run after applying the build configuration fix

echo "Reverting changes to Chromium files..."

# Revert the changes we made
git checkout -- chrome/browser/download/download_ui_safe_browsing_util.cc
git checkout -- chrome/browser/enterprise/connectors/connectors_service.cc
git checkout -- chrome/browser/enterprise/data_protection/data_protection_navigation_observer.cc
git checkout -- chrome/browser/enterprise/signals/context_info_fetcher.cc
git checkout -- chrome/browser/enterprise/signals/profile_signals_collector.cc

echo "Changes reverted. The build configuration in luxxle/BUILD.gn will now handle unused function warnings."
echo "To apply this configuration, you may need to modify the chrome/browser/BUILD.gn to include our config." 