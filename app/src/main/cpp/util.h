#pragma once
#include <string>
#include "il2cpp-api.h"   // 你之前定义的 DO_API 等宏

// 工具函数：处理 Il2CppString 和 C++ string 互转
namespace Il2CppUtils
{
    // Il2CppString* → std::string (UTF-8)
    std::string Utf16ToUtf8(Il2CppString* str);

   // Il2CppString* → const char* (UTF-8)，注意生命周期
    const char* Utf16ToCString(Il2CppString* str);

    // std::string (UTF-8) → Il2CppString*
    Il2CppString* NewString(const std::string& str);

    // const char* (UTF-8) → Il2CppString*
    Il2CppString* NewString(const char* str);

    std::string ToString(Il2CppString* str);
}
