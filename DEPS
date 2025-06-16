use_relative_paths = True

vars = {
  'download_prebuilt_sparkle': False,  # Disable Sparkle for updates
  'checkout_dmg_tool': False,
}

deps = {
  "vendor/python-patch": "https://github.com/luxxle/python-patch@d8880110be6554686bc08261766538c2926d4e82",
  "vendor/gn-project-generators": "https://github.com/luxxle/gn-project-generators.git@b76e14b162aa0ce40f11920ec94bfc12da29e5d0",
  "third_party/reclient_configs/src": "https://github.com/EngFlow/reclient-configs.git@21c8fe69ff771956c179847b8c1d9fd216181967",
  'third_party/android_deps/libs/com_google_android_play_core': {
      'packages': [
          {
              'package': 'chromium/third_party/android_deps/libs/com_google_android_play_core',
              'version': 'version:2@1.10.3.cr1',
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },
}

recursedeps = []

hooks = [
  {
    'name': 'bootstrap',
    'pattern': '.',
    'action': ['vpython3', 'script/bootstrap.py'],
  },
  {
    'name': 'bootstrap_ios',
    'pattern': '.',
    'condition': 'checkout_ios and host_os == "mac"',
    'action': ['python3', 'script/ios_bootstrap.py']
  },
  {
    # Download hermetic xcode for goma
    'name': 'download_hermetic_xcode',
    'pattern': '.',
    'condition': 'checkout_mac or checkout_ios',
    'action': ['vpython3', 'build/mac/download_hermetic_xcode.py'],
  },
  {
    'name': 'configure_reclient',
    'pattern': '.',
    'action': ['python3', 'third_party/reclient_configs/src/configure_reclient.py',
               '--src_dir=..',
               '--exec_root=../..',
               '--custom_py=third_party/reclient_configs/brave_custom/brave_custom.py'],
  },
  {
    'name': 'update_pip',
    'pattern': '.',
    'action': ['python3', '-m', 'pip', '-q', '--disable-pip-version-check', 'install', '-U', '--no-warn-script-location', 'pip'],
  },
  {
    'name': 'download_cryptography',
    'pattern': '.',
    'action': ['python3', '-m', 'pip', '-q', '--disable-pip-version-check', 'install', '-U', '-t', 'third_party/cryptography', '--only-binary', 'cryptography', 'cryptography==37.0.4'],
  },
  {
    'name': 'generate_licenses',
    'pattern': '.',
    'action': ['vpython3', 'script/generate_licenses.py'],
  },
  {
    # Overwrite Chromium's LASTCHANGE using the latest Luxxle version commit.
    'name': 'luxxle_lastchange',
    'pattern': '.',
    'action': ['python3', '../build/util/lastchange.py',
               '--output', '../build/util/LASTCHANGE',
               '--source-dir', '.',
               '--filter', '^[0-9]\{{1,\}}\.[0-9]\{{1,\}}\.[0-9]\{{1,\}}$'],
  },
  {
    # Downloads & overwrites Chromium's swift-format dep on macOS only
    'name': 'download_swift_format',
    'pattern': '.',
    'condition': 'host_os == "mac"',
    'action': ['python3', 'build/apple/download_swift_format.py', '510.1.0', '0ddbb486640cde862fa311dc0f7387e6c5171bdcc0ee0c89bc9a1f8a75e8bfaf']
  },
  {
    # Generate .clang-format.
    'name': 'generate_clang_format',
    'pattern': '.',
    'action': ['vpython3', 'build/util/generate_clang_format.py', '../.clang-format', '.clang-format']
  },
  {
    'name': 'download_rust_toolchain_aux',
    'pattern': '.',
    'action': ['python3', 'build/rust/download_rust_toolchain_aux.py']
  },
]

include_rules = [
  "-chrome",
  "-brave",
  "-third_party/rust",

  # Everybody can use some things.
  "+luxxle/base",
]
