pluginManagement {
    repositories {
        google {
            content {
                includeGroupByRegex("com\\.android.*")
                includeGroupByRegex("com\\.google.*")
                includeGroupByRegex("androidx.*")
            }
        }
        mavenCentral()
        gradlePluginPortal()
    }
}
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
        maven { url = uri("https://maven.aliyun.com/nexus/content/groups/public/") }
        maven {  url = uri("https://maven.aliyun.com/nexus/content/repositories/jcenter") }
        maven { url = uri( "https://maven.aliyun.com/nexus/content/repositories/google") }
        maven {  url = uri("https://maven.aliyun.com/nexus/content/repositories/gradle-plugin") }
    }
}

rootProject.name = "NativeHook"
include(":app")
