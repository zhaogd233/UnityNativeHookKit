#include "util.h"
#include <codecvt>
#include <locale>

namespace Il2CppUtils
{
   std::string Utf16ToUtf8(Il2CppString* str)
    {
        if (!str) return "";

        int32_t length = Il2CppApi::il2cpp_string_length(str);
        const Il2CppChar* chars = Il2CppApi::il2cpp_string_chars(str);

        if (!chars || length <= 0) return "";

        // UTF-16 → UTF-8
        std::u16string u16str(chars, chars + length);
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        return convert.to_bytes(u16str);
    }

   const char* Utf16ToCString(Il2CppString* str)
    {
        static thread_local std::string buffer; // 避免返回悬空指针
        buffer = Utf16ToUtf8(str);
        return buffer.c_str();
    }

    Il2CppString* NewString(const std::string& str)
    {
        return Il2CppApi::il2cpp_string_new(str.c_str());
    }

    Il2CppString* NewString(const char* str)
    {
        return Il2CppApi::il2cpp_string_new(str);
    }
    // 把 Il2CppString 转成 std::string
     std::string ToString(Il2CppString* str)
    {
        if (str == nullptr)
            return std::string();

        const Il2CppChar* chars = Il2CppApi::il2cpp_string_chars(str);
        int32_t len = Il2CppApi::il2cpp_string_length(str);

        std::string result;
        result.reserve(len);

        for (int i = 0; i < len; ++i)
        {
            // 假设都是 ASCII 范围，直接低字节转
            result.push_back(static_cast<char>(chars[i] & 0xFF));
        }
        return result;
    }
}
