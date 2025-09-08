#pragma once
#include <dlfcn.h>
#include <shadowhook.h>
#include <stdint.h>
#include <android/log.h>

#define LOG(fmt, ...) __android_log_print(ANDROID_LOG_INFO, "shadowhookKit", fmt, ##__VA_ARGS__)

// ---------------- IL2CPP 类型定义 ----------------
struct Il2CppDomain;
struct Il2CppAssembly;
struct Il2CppClass;
struct Il2CppImage;
typedef uint16_t Il2CppChar;
struct Il2CppString;

// MethodInfo 结构（简化版）
struct MethodInfo {
    const char* name;
    void* methodPointer;
    void* invokerMethod;
    const void* parameters;
};

// ---------------- 函数指针类型 ----------------
typedef Il2CppDomain* (*il2cpp_domain_get_t)();
typedef const Il2CppAssembly* (*il2cpp_domain_assembly_open_t)(Il2CppDomain* domain, const char* name);
typedef const Il2CppAssembly** (*il2cpp_domain_get_assemblies_t)(const Il2CppDomain* domain, size_t* size);
typedef Il2CppImage* (*il2cpp_assembly_get_image_t)(const Il2CppAssembly* assembly);
typedef Il2CppClass* (*il2cpp_class_from_name_t)(Il2CppImage* image, const char* namespaze, const char* name);
typedef const MethodInfo* (*il2cpp_class_get_method_from_name_t)(Il2CppClass* klass, const char* name, int paramCount);
typedef void* (*il2cpp_object_get_class_t)(void* obj);
typedef void* (*il2cpp_class_get_field_from_name_t)(Il2CppClass* klass, const char* name);
typedef void (*il2cpp_field_get_value_t)(void* obj, void* field, void* value);
typedef void (*il2cpp_field_set_value_t)(void* obj, void* field, void* value);

//string
typedef int32_t   (*il2cpp_string_length_t)(Il2CppString* str);
typedef Il2CppChar* (*il2cpp_string_chars_t)(Il2CppString* str);
typedef Il2CppString* (*il2cpp_string_new_t)(const char* str);
typedef Il2CppString* (*il2cpp_string_new_len_t)(const char* str, uint32_t length);
typedef Il2CppString* (*il2cpp_string_new_utf16_t)(const Il2CppChar* text, int32_t len);
typedef Il2CppString* (*il2cpp_string_new_wrapper_t)(const char* str);
typedef Il2CppString* (*il2cpp_string_intern_t)(Il2CppString* str);
typedef Il2CppString* (*il2cpp_string_is_interned_t)(Il2CppString* str);

// ---------------- 延迟加载结构体 ----------------
struct Il2CppApi {
    static il2cpp_domain_get_t il2cpp_domain_get_ptr;
    static il2cpp_domain_assembly_open_t il2cpp_domain_assembly_open_ptr;
    static il2cpp_domain_get_assemblies_t il2cpp_domain_get_assemblies_ptr;
    static il2cpp_assembly_get_image_t il2cpp_assembly_get_image_ptr;
    static il2cpp_class_from_name_t il2cpp_class_from_name_ptr;
    static il2cpp_class_get_method_from_name_t il2cpp_class_get_method_from_name_ptr;
    static il2cpp_object_get_class_t il2cpp_object_get_class_ptr;
    static il2cpp_class_get_field_from_name_t il2cpp_class_get_field_from_name_ptr;
    static il2cpp_field_get_value_t il2cpp_field_get_value_ptr;
    static il2cpp_field_get_value_t il2cpp_field_set_value_ptr;

    // string
    static il2cpp_string_length_t il2cpp_string_length;
    static il2cpp_string_chars_t il2cpp_string_chars;
    static il2cpp_string_new_t il2cpp_string_new;
    static il2cpp_string_new_len_t il2cpp_string_new_len;
    static il2cpp_string_new_utf16_t il2cpp_string_new_utf16;
    static il2cpp_string_new_wrapper_t il2cpp_string_new_wrapper;
    static il2cpp_string_intern_t il2cpp_string_intern;
    static il2cpp_string_is_interned_t il2cpp_string_is_interned;

    static bool Init();
};
