plugins {
    alias(libs.plugins.android.application)
}

android {
    namespace = "com.game.nativehook"
    compileSdk = 35

    defaultConfig {
        applicationId = "com.game.nativehook"
        minSdk = 24
        targetSdk = 35
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"

        ndk {
            abiFilters += listOf("armeabi-v7a", "arm64-v8a")
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
            signingConfig = signingConfigs.getByName("debug")
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }
    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }
    buildFeatures {
        viewBinding = true
        prefab = true
    }
}

dependencies {
// Xposed API（compileOnly，运行时由框架提供）
    //仅用于编译使用，不会打包到apk ,使用默认xposed 框架里面的api,但是会造成无法代码提示
  //  compileOnly("de.robv.android.xposed:api:82")
    // 可以代码提示，但是编译会带进apk, 造成无法使用xposed框架，编写代码的时候使用此，
    debugImplementation("de.robv.android.xposed:api:82")
    // Release 构建下：只在编译期用，不会进 APK
    releaseCompileOnly("de.robv.android.xposed:api:82")
    compileOnly("de.robv.android.xposed:api:82:sources")
    implementation(libs.appcompat)
    implementation(libs.material)
    implementation(libs.constraintlayout)
    testImplementation(libs.junit)
    androidTestImplementation(libs.ext.junit)
    androidTestImplementation(libs.espresso.core)
    // 给 IDE 用，不会影响运行
    //implementation(files("libs/xposed-api-82.jar"))
    implementation("com.bytedance.android:shadowhook:2.0.0")
}