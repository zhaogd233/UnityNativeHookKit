#include "il2cpp-api.h"

// 初始化静态成员
il2cpp_domain_get_t Il2CppApi::il2cpp_domain_get_ptr = nullptr;
il2cpp_domain_assembly_open_t Il2CppApi::il2cpp_domain_assembly_open_ptr = nullptr;
il2cpp_domain_get_assemblies_t Il2CppApi::il2cpp_domain_get_assemblies_ptr = nullptr;
il2cpp_assembly_get_image_t Il2CppApi::il2cpp_assembly_get_image_ptr = nullptr;
il2cpp_class_from_name_t Il2CppApi::il2cpp_class_from_name_ptr = nullptr;
il2cpp_class_get_method_from_name_t Il2CppApi::il2cpp_class_get_method_from_name_ptr = nullptr;
il2cpp_object_get_class_t Il2CppApi::il2cpp_object_get_class_ptr = nullptr;
il2cpp_class_get_field_from_name_t Il2CppApi::il2cpp_class_get_field_from_name_ptr = nullptr;
il2cpp_field_get_value_t Il2CppApi::il2cpp_field_get_value_ptr = nullptr;
il2cpp_field_get_value_t Il2CppApi::il2cpp_field_set_value_ptr = nullptr;

il2cpp_string_length_t Il2CppApi::il2cpp_string_length = nullptr;
il2cpp_string_chars_t Il2CppApi::il2cpp_string_chars = nullptr;
il2cpp_string_new_t Il2CppApi::il2cpp_string_new = nullptr;
il2cpp_string_new_len_t Il2CppApi::il2cpp_string_new_len = nullptr;
il2cpp_string_new_utf16_t Il2CppApi::il2cpp_string_new_utf16 = nullptr;
il2cpp_string_new_wrapper_t Il2CppApi::il2cpp_string_new_wrapper = nullptr;
il2cpp_string_intern_t Il2CppApi::il2cpp_string_intern = nullptr;
il2cpp_string_is_interned_t Il2CppApi::il2cpp_string_is_interned = nullptr;

bool Il2CppApi::Init() {
    static bool inited = false;
    if (inited) return true;

    void* handle = shadowhook_dlopen("libil2cpp.so");
    if (!handle) {
        LOG("dlopen libil2cpp.so failed");
        return false;
    }

    il2cpp_domain_get_ptr = (il2cpp_domain_get_t)shadowhook_dlsym(handle, "il2cpp_domain_get");
    il2cpp_domain_assembly_open_ptr = (il2cpp_domain_assembly_open_t)shadowhook_dlsym(handle, "il2cpp_domain_assembly_open");
    il2cpp_domain_get_assemblies_ptr = (il2cpp_domain_get_assemblies_t)shadowhook_dlsym(handle, "il2cpp_domain_get_assemblies");
    il2cpp_assembly_get_image_ptr = (il2cpp_assembly_get_image_t)shadowhook_dlsym(handle, "il2cpp_assembly_get_image");
    il2cpp_class_from_name_ptr = (il2cpp_class_from_name_t)shadowhook_dlsym(handle, "il2cpp_class_from_name");
    il2cpp_class_get_method_from_name_ptr = (il2cpp_class_get_method_from_name_t)shadowhook_dlsym(handle, "il2cpp_class_get_method_from_name");
    il2cpp_object_get_class_ptr = (il2cpp_object_get_class_t)shadowhook_dlsym(handle, "il2cpp_object_get_class");
    il2cpp_class_get_field_from_name_ptr = (il2cpp_class_get_field_from_name_t)shadowhook_dlsym(handle, "il2cpp_class_get_field_from_name");
    il2cpp_field_get_value_ptr = (il2cpp_field_get_value_t)shadowhook_dlsym(handle, "il2cpp_field_get_value");
    il2cpp_field_set_value_ptr = (il2cpp_field_set_value_t)shadowhook_dlsym(handle, "il2cpp_field_set_value");

    // string
    il2cpp_string_length = (il2cpp_string_length_t)shadowhook_dlsym(handle, "il2cpp_string_length");
    il2cpp_string_chars = (il2cpp_string_chars_t)shadowhook_dlsym(handle, "il2cpp_string_chars");
    il2cpp_string_new = (il2cpp_string_new_t)shadowhook_dlsym(handle, "il2cpp_string_new");
    il2cpp_string_new_len = (il2cpp_string_new_len_t)shadowhook_dlsym(handle, "il2cpp_string_new_len");
    il2cpp_string_new_utf16 = (il2cpp_string_new_utf16_t)shadowhook_dlsym(handle, "il2cpp_string_new_utf16");
    il2cpp_string_new_wrapper = (il2cpp_string_new_wrapper_t)shadowhook_dlsym(handle, "il2cpp_string_new_wrapper");
    il2cpp_string_intern = (il2cpp_string_intern_t)shadowhook_dlsym(handle, "il2cpp_string_intern");
    il2cpp_string_is_interned = (il2cpp_string_is_interned_t)shadowhook_dlsym(handle, "il2cpp_string_is_interned");


    if (!il2cpp_domain_get_ptr || !il2cpp_domain_assembly_open_ptr || !il2cpp_domain_get_assemblies_ptr
        || !il2cpp_assembly_get_image_ptr || !il2cpp_class_from_name_ptr || !il2cpp_class_get_method_from_name_ptr
        || !il2cpp_class_get_field_from_name_ptr ||!il2cpp_object_get_class_ptr ||!il2cpp_field_get_value_ptr
        ||!il2cpp_field_set_value_ptr) {
        LOG("Failed to load one or more il2cpp symbols:il2cpp");
        return false;
    }

    if (!il2cpp_domain_get_ptr || !il2cpp_class_from_name_ptr || !il2cpp_string_length) {
        LOG("Failed to load one or more il2cpp symbols:string");
        return false;
    }

    inited = true;
    LOG("Il2CppApi loaded successfully");
    return true;
}