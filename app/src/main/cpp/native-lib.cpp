#include <jni.h>
#include <string>
#include <shadowhook.h>
#include <android/log.h>
#include "il2cpp-api.h"
#include "util.h"

#define LOG_TAG "shadowhookKit"
#define LOG(fmt, ...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

///功能实现
void *orig = NULL;
void *stub = NULL;

// 被 hook 函数的类型定义
typedef void (*artmethod_invoke_func_type_t)(void *, void *, uint32_t *, uint32_t, void *,
                                             const char *);

// 代理函数
void
artmethod_invoke_proxy(void *thiz, void *thread, uint32_t *args, uint32_t args_size, void *result,
                       const char *shorty) {
    // do something
    LOG(">> Before Invoke: shorty=%s args_size=%u", shorty, args_size);
    ((artmethod_invoke_func_type_t) orig)(thiz, thread, args, args_size, result, shorty);
    // do something
    LOG("zgd hook after");
}

void init() {
    shadowhook_init(SHADOWHOOK_MODE_UNIQUE, true);
}

void do_hook_impl() {

    stub = shadowhook_hook_sym_name(
            "libart.so",
            "_ZN3art9ArtMethod6InvokeEPNS_6ThreadEPjjPNS_6JValueEPKc",
            (void *) artmethod_invoke_proxy,
            (void **) &orig);

    if (stub == NULL) {
        int err_num = shadowhook_get_errno();
        const char *err_msg = shadowhook_to_errmsg(err_num);
        LOG("hook error %d - %s", err_num, err_msg);
    }
}

void do_unHook_impl() {
    // TODO: implement doUnHook()
    int result = shadowhook_unhook(stub);

    if (result != 0) {
        int err_num = shadowhook_get_errno();
        const char *err_msg = shadowhook_to_errmsg(err_num);
        LOG("unhook error %d - %s", err_num, err_msg);
    }
}


// 被 hook 函数的类型定义
typedef int64_t (*unitymethod_GetBufferMgr_func_type_t)();

// 代理函数
int64_t unitymethod_getBufferMgr_proxy() {
    // do something
    LOG("Before Invoke: success");
    int64_t result =  ((unitymethod_GetBufferMgr_func_type_t) orig)();
    // do something
    LOG("zgd hook after");
    return result;
}


void do_Hook_GetBufferMgrGLES()
{
    stub = shadowhook_hook_sym_name(
            "libunity.so",
            "_Z20GetBufferManagerGLESv",
            (void *) unitymethod_getBufferMgr_proxy,
            (void **) &orig);
}

bool CheckHookFunc(const char* soName,const char* symName)
{
    void *handle = shadowhook_dlopen(soName);
    void *sym_addr = shadowhook_dlsym(handle, symName);
    shadowhook_dlclose(handle);
    if (sym_addr == NULL) {
        LOGE("symbol check: not found");
        return false;
    }
    LOGW("symbol check: found");
    return true;
}


/*
 * 测试il2cpp 的hook
 */

// ShadowHook 全局变量
void* orig_DoAddUI = nullptr;
void* stub_DoAddUI = nullptr;

// 代理函数签名
typedef void* (*DoAddUI_func_t)(void* thiz, void* uiData, void* curAssetRef);

// 代理函数
void* DoAddUI_proxy(void* thiz, void* uiData, void* curAssetRef) {
    LOG(">> DoAddUI called!");

/*    // 1. 获取 class
    Il2CppClass* klass = (Il2CppClass*)Il2CppApi::il2cpp_object_get_class_ptr(uiData);

    // 2. 获取 field
    void* field = Il2CppApi::il2cpp_class_get_field_from_name_ptr(klass, "path");

    // 3. 取出字段值（它是 System.String*）
    void* strObj = nullptr;
    Il2CppApi::il2cpp_field_get_value_ptr(uiData, field, &strObj);

    // 4. 转换成 char*
    if (strObj) {
        std::string utf8 = Il2CppStringToUtf8(strObj); // 需要写个 helper
        LOG("UIPathData.path = %s", utf8);
        FreeIl2CppStringUtf8((char*)utf8);
    }*/

    // 调用原始函数
    void* result = ((DoAddUI_func_t)orig_DoAddUI)(thiz, uiData, curAssetRef);


    LOG("<< DoAddUI finished");
    return result;
}

// 查找 DoAddUI 地址
bool find_DoAddUI(void** out_addr) {
    if (!Il2CppApi::Init()) return false;

    Il2CppDomain* domain = Il2CppApi::il2cpp_domain_get_ptr();
    if (!domain) {
        LOGE("il2cpp_domain_get failed!");
        return false;
    }

    // 获取 assemblies
    size_t assembly_count = 0;
    const Il2CppAssembly** assemblies = Il2CppApi::il2cpp_domain_get_assemblies_ptr(domain, &assembly_count);
    if (assembly_count == 0) {
        LOGE("No assemblies found!");
        return false;
    }

   const MethodInfo* targetMethod = nullptr;

    // 遍历所有 assembly
    for (size_t i = 0; i < assembly_count; i++) {
        const Il2CppAssembly* assembly = assemblies[i];
        Il2CppImage* image = Il2CppApi::il2cpp_assembly_get_image_ptr(assembly);
        if (!image) continue;

        // 修改成你的目标命名空间和类名
      // Il2CppClass* klass = Il2CppApi::il2cpp_class_from_name_ptr(image, "", "testhook");
        Il2CppClass* klass = Il2CppApi::il2cpp_class_from_name_ptr(image, "", "UIManager");
        if (!klass) continue;

        targetMethod = Il2CppApi::il2cpp_class_get_method_from_name_ptr(klass, "DoAddUI", 2);
        if (targetMethod) break;
    }

    if (!targetMethod) {
        LOGE("DoAddUI method not found!");
        return false;
    }

    *out_addr = targetMethod->methodPointer;

    return true;
}

// Hook DoAddUI
extern "C" void doHookAddUI() {
    if (!find_DoAddUI(&orig_DoAddUI)) {
        LOG("DoAddUI not found!");
        return;
    }

    stub_DoAddUI = shadowhook_hook_func_addr((void*)orig_DoAddUI, (void*)DoAddUI_proxy, &orig_DoAddUI);
    if (!stub_DoAddUI) {
        int err_num = shadowhook_get_errno();
        LOG("shadowhook failed: %d - %s", err_num, shadowhook_to_errmsg(err_num));
    } else {
        LOG("DoAddUI hook success!");
    }
}


extern "C" {
    // 暴漏给c#
    /*
     * c# 里面的定义
     *  [DllImport("nativehook")]
    private static extern void doHookNative();
     */
    void doHookNative() {
        LOGE("form C#");
        init();
        do_hook_impl();
    }

    void doUnHookNative() {
        do_unHook_impl();
    }

    void doHookUnityGetBufferManagerGles()
    {
        LOGE("form C#");
        init();
        if(CheckHookFunc("libunity.so","_Z20GetBufferManagerGLESv"))
         do_Hook_GetBufferMgrGLES();
    }

    /// 暴漏给Java 测试
    JNIEXPORT void JNICALL
    Java_com_game_nativehook_MainActivity_doUnHook(JNIEnv *env, jobject thiz) {
        LOGE("form java");
        do_unHook_impl();
    }

    JNIEXPORT void JNICALL
    Java_com_game_nativehook_MainActivity_doHook(JNIEnv *env, jobject thiz) {
        // TODO: implement doHook()
        do_hook_impl();
    }

    JNIEXPORT jstring JNICALL
    Java_com_game_nativehook_MainActivity_stringFromJNI(
            JNIEnv *env,
            jobject /* this */) {
        std::string hello = "Hello from C++";
        return env->NewStringUTF(hello.c_str());
    }

    JNIEXPORT void JNICALL
    Java_com_game_nativehook_XposedInit_doHookbyXposed(
            JNIEnv *env,
            jobject /* this */) {
        LOGE("form xposed");
        doHookUnityGetBufferManagerGles();
    }

    JNIEXPORT void JNICALL
    Java_com_game_nativehook_XposedInit_doHookDOAddUI(JNIEnv *env, jobject thiz) {
        // TODO: implement doHookDOAddUI()
        init();
        doHookAddUI();
    }
}
