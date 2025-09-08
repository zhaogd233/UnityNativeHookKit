//
// Created by Administrator on 2025/9/8.
//

#include "util.h"
#include <cstring>
#include <cstdlib>

// Unity 提供的 C API (在 libil2cpp.so 内实现)
extern "C"
{
char* il2cpp_string_to_utf8(Il2CppString* str);
void il2cpp_free(void* ptr);
}

std::string Il2CppStringToUtf8(Il2CppString* str)
{
    if (!str) return std::string();

    char* utf8 = il2cpp_string_to_utf8(str);
    if (!utf8) return std::string();

    std::string result(utf8);
    il2cpp_free(utf8);  // 必须释放掉 Unity 内部的内存
    return result;
}

char* Il2CppStringToUtf8Raw(Il2CppString* str)
{
    if (!str) return nullptr;
    return il2cpp_string_to_utf8(str); // 需要手动 FreeIl2CppStringUtf8
}

void FreeIl2CppStringUtf8(char* utf8Str)
{
    if (utf8Str)
    {
        il2cpp_free(utf8Str);
    }
}
