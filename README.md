# Unity Native Hook Kit

基于 [ShadowHook](https://github.com/bytedance/android-inline-hook) 的 **Unity Hook 模板项目**。
支持三种运行方式：

1. **Java 测试调用**（独立 App 测试 Hook 逻辑）
2. **Unity C# 调用**（通过 `DllImport` 加载 so 并执行 Hook）
3. **Xposed 模块注入**（无需修改 Unity APK，模块化注入）

---

## 功能

* 使用 [ShadowHook](https://github.com/bytedance/android-inline-hook) 实现 Android Native 层函数 Hook
* 示例代码：Hook Unity 引擎函数 (`GetBufferManagerGLES`) 或 ART Method 调用
* 支持 **Java 层暴露接口**（`doHook()` / `doUnHook()`）
* 支持 **C# 层 DllImport 调用**（`doHookNative()` / `doUnHookNative()`）
* 支持 **Xposed 框架加载 so 并触发 Hook**
* 支持 **LibIL2cpp 函数的注入，修改，执行。

---

## 工程结构

```
UnityNativeHook/
 ├── app/                        # Android 工程
 │   ├── src/main/java/com/game/nativehook/
 │   │     ├── MainActivity.java # Java 层测试入口
 │   │     └── XposedInit.java   # Xposed 模块入口
 │   ├── cpp/
 │   │     └── nativehook.cpp    # Hook 实现
 │   └── build.gradle.kts        # Gradle 配置 (KTS)
 └── unity/                      # Unity 示例工程
       └── HookDemo.cs           # C# 调用示例
```

---

## 使用方式

### 1. Java 测试调用

在 Java 层直接调用暴露的 native 方法：

```java
public class MainActivity extends AppCompatActivity {
    static {
        System.loadLibrary("nativehook");
    }

    public native void doHook();
    public native void doUnHook();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        doHook(); // 测试 Hook
    }
}
```

运行后，Logcat 中会输出：

```
I/ShadowHookDemo: >> Before Invoke: shorty=... args_size=...
I/ShadowHookDemo: zgd hook after
```

---

### 2. Unity C# 调用

在 Unity 脚本中通过 `DllImport` 方式调用：

```csharp
using System.Runtime.InteropServices;
using UnityEngine;

public class HookDemo : MonoBehaviour {
    [DllImport("nativehook")]
    private static extern void doHookNative();

    [DllImport("nativehook")]
    private static extern void doUnHookNative();

    void Start() {
        doHookNative();
        Debug.Log("Unity Hook Initialized!");
    }
}
```

打包 Android APK 后，`libnativehook.so` 会随 Unity 工程一起被加载。

---

### 3. Xposed 模块注入

在 `XposedInit.java` 中注册：

```java
package com.game.nativehook;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

public class XposedInit implements IXposedHookLoadPackage {
    static {
        System.loadLibrary("nativehook");
    }

    @Override
    public void handleLoadPackage(XC_LoadPackage.LoadPackageParam lpparam) {
        if (lpparam.packageName.equals("com.unity3d.player")) {
            NativeHook.doHook();
        }
    }
}
```

* 编译生成 APK
* 安装到设备并在 Xposed/LSPosed 中启用
* 启动 Unity 游戏时会自动加载 `libnativehook.so` 并执行 Hook

---

## 构建说明

### Gradle 配置（KTS）

```kotlin
android {
    defaultConfig {
        ndk {
            abiFilters += listOf("armeabi-v7a", "arm64-v8a")
        }
    }
}

dependencies {
    compileOnly("de.robv.android.xposed:api:82") // Xposed API，仅编译期可见
    implementation("com.bytedance.shadowhook:shadowhook:2.0.0")
}
```

---

## 注意事项

* **避免死循环**：在 Hook 回调中打印 log 时，不要调用被 Hook 的 `Debug.Log`，建议用 `__android_log_print`。
* **符号查找问题**：某些情况下（如 Xposed 环境），符号可能未加载，需要延迟调用 `CheckHookFunc`。
* **多方式共存**：Java/C#/Xposed 可独立使用，互不影响。
* **xposed框架问题**：xposed相关模块必须使用xposed 自身的，项目本身不能编译进去。因此使用的compileOnly，但这样idea 就无法使用相关api，因此采用的是
release 的时候compileOnly，debug的时候impelmentation .因此需要release 打包方式运行到真机测试。可设置

---

## 许可证

MIT License
