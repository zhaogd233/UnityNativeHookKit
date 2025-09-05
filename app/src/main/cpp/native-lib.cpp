#include <jni.h>
#include <string>
#include <shadowhook.h>
#include <android/log.h>

#define LOG_TAG "ShadowHookDemo"
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
}