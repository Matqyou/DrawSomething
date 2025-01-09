//
// Created by Matq on 19/12/2024.
//

#pragma once

#include <iostream>

namespace Strings {
    std::wstring FStringColorsW(const wchar_t* format, ...);
    std::string FStringColors(const char* format, ...);
    std::wstring FStringW(const wchar_t* format, ...);
    std::string FString(const char* format, ...);
    std::string ErasePrefix(std::string string, const std::string& prefix);
    std::string EraseSuffix(std::string string, const std::string& suffix);
    std::string RepeatString(const std::string& str, int times);
    std::wstring RepeatStringW(const std::wstring& str, int times);
}