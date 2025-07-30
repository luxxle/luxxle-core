/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/sandbox/win/src/module_file_name_interception.h"

#include <string.h>

#include <algorithm>
#include <optional>
#include <string>
#include <string_view>

#include "base/compiler_specific.h"
#include "base/strings/string_util.h"
#include "base/win/windows_types.h"

namespace {

void ReplaceAt(char* dest, size_t dest_size, std::string_view src) {
  UNSAFE_TODO(::strncpy_s(dest, dest_size, src.data(),
                          std::min(dest_size - 1, src.length())));
}

void ReplaceAt(wchar_t* dest, size_t dest_size, std::wstring_view src) {
  UNSAFE_TODO(::wcsncpy_s(dest, dest_size, src.data(),
                          std::min(dest_size - 1, src.length())));
}

template <typename CharT>
struct LuxxleToChrome;

template <>
struct LuxxleToChrome<char> {
  static constexpr const std::string_view kLuxxle = "luxxle.exe";
  static constexpr const std::string_view kChrome = "chrome.exe";
};

template <>
struct LuxxleToChrome<wchar_t> {
  static constexpr const std::wstring_view kLuxxle = L"luxxle.exe";
  static constexpr const std::wstring_view kChrome = L"chrome.exe";
};

template <typename CharT>
struct TestLuxxleToChrome;

template <>
struct TestLuxxleToChrome<char> {
  static constexpr const std::string_view kLuxxle = "luxxle_browser_tests.exe";
  static constexpr const std::string_view kChrome = "chrome_browser_tests.exe";
};

template <>
struct TestLuxxleToChrome<wchar_t> {
  static constexpr const std::wstring_view kLuxxle = L"luxxle_browser_tests.exe";
  static constexpr const std::wstring_view kChrome =
      L"chrome_browser_tests.exe";
};

template <template <class T> class FromTo, typename CharT>
std::optional<DWORD> PatchFilenameImpl(CharT* filename,
                                       DWORD length,
                                       DWORD size) {
  if (!base::EndsWith(std::basic_string_view<CharT>(filename, length),
                      FromTo<CharT>::kLuxxle,
                      base::CompareCase::INSENSITIVE_ASCII)) {
    return std::nullopt;
  }

  constexpr DWORD kLuxxleLen = FromTo<CharT>::kLuxxle.length();
  constexpr DWORD kChromeLen = FromTo<CharT>::kChrome.length();
  static_assert(kLuxxleLen <= kChromeLen);
  constexpr DWORD kLenDiff = kChromeLen - kLuxxleLen;

  --size;  // space for null-terminator

  const size_t luxxle_pos = length - kLuxxleLen;
  ReplaceAt(UNSAFE_TODO(filename + luxxle_pos), size - luxxle_pos,
            FromTo<CharT>::kChrome);
  if (size < length + kLenDiff) {
    ::SetLastError(ERROR_INSUFFICIENT_BUFFER);
  }
  length = std::min(size, length + kLenDiff);
  UNSAFE_TODO(filename[length]) = 0;
  return length;
}

template <typename CharT>
DWORD PatchFilename(CharT* filename, DWORD length, DWORD size) {
  if (auto r = PatchFilenameImpl<LuxxleToChrome>(filename, length, size)) {
    return *r;
  }
  if (auto r = PatchFilenameImpl<TestLuxxleToChrome>(filename, length, size)) {
    return *r;
  }
  return length;
}

}  // namespace

namespace sandbox {

SANDBOX_INTERCEPT DWORD WINAPI
TargetGetModuleFileNameA(GetModuleFileNameAFunction orig,
                         HMODULE hModule,
                         LPSTR lpFilename,
                         DWORD nSize) {
  const auto result = orig(hModule, lpFilename, nSize);
  if (result != 0) {
    return PatchFilename(lpFilename, result, nSize);
  }
  return result;
}

SANDBOX_INTERCEPT DWORD WINAPI
TargetGetModuleFileNameW(GetModuleFileNameWFunction orig,
                         HMODULE hModule,
                         LPWSTR lpFilename,
                         DWORD nSize) {
  const auto result = orig(hModule, lpFilename, nSize);
  if (result != 0) {
    return PatchFilename(lpFilename, result, nSize);
  }
  return result;
}

SANDBOX_INTERCEPT DWORD WINAPI
TargetGetModuleFileNameExA(GetModuleFileNameExAFunction orig,
                           HANDLE hProcess,
                           HMODULE hModule,
                           LPSTR lpFilename,
                           DWORD nSize) {
  const auto result = orig(hProcess, hModule, lpFilename, nSize);
  if (result != 0) {
    return PatchFilename(lpFilename, result, nSize);
  }
  return result;
}

SANDBOX_INTERCEPT DWORD WINAPI
TargetGetModuleFileNameExW(GetModuleFileNameExWFunction orig,
                           HANDLE hProcess,
                           HMODULE hModule,
                           LPWSTR lpFilename,
                           DWORD nSize) {
  const auto result = orig(hProcess, hModule, lpFilename, nSize);
  if (result != 0) {
    return PatchFilename(lpFilename, result, nSize);
  }
  return result;
}

}  // namespace sandbox
