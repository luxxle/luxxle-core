# Brave environment setup helpers

The directory contains shell helpers to set Brave-specific environment
variables. These variables allows you to run `gn`, `ninja`, `gclient` and other
tools directly.

These helpers are NOT required to run builds using the usual `npm run ...` flow.

There're 3 versions to cover most popular interpreters:

- `env.cmd` (cmd.exe)
- `env.ps1` (powershell)
- `env.sh` (cygwin/bash/zsh)

Suported parameters:

- `-v` enable verbose output.

If you modify one of the scripts, make sure to do the same changes in other
versions.

Variables are generated by running `npm run gen_env` command.

The script doesn't export `.npmrc`-declared vars:

1. these vars exist in `process.env` at start.
2. `Config.defaultOptions` doesn't add/remove them.
3. `gen_env` sees these vars as "not modified" and doesn't export them.
