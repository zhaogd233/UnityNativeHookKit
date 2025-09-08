#pragma once
#include "il2cpp-api.h"   // il2cpp 提供的头文件，确保能访问 Il2CppString
#include <string>

struct Il2CppString {
    void* klass;
    void* monitor;
    int32_t length;
    uint16_t chars[0]; // UTF-16
};

/// <summary>
/// 把 Il2CppString* 转换成 std::string (UTF8)
/// 内部调用 Unity 提供的 il2cpp_string_to_utf8
/// </summary>
std::string Il2CppStringToUtf8(Il2CppString* str);

/// <summary>
/// 释放由 Il2CppStringToUtf8Raw 分配的内存
/// </summary>
void FreeIl2CppStringUtf8(char* utf8Str);

/// <summary>
/// 获取 UTF8 原始指针（malloc 分配，需要手动调用 FreeIl2CppStringUtf8 释放）
/// </summary>
char* Il2CppStringToUtf8Raw(Il2CppString* str);
