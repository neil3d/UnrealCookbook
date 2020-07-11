# Unreal Cookbook

- UE4 一些常用功能的写法例子
- 每个目录是一个单独的 Unreal Project，里面还有详细的 Readme

## Project Setup

``` shell
git clone git@github.com:neil3d/UnrealCookbook.git
cd UnrealCookbook

git submodule init
git submodule update
```

## 目录说明

| 目录 | 简单说明 | 详细文档 |
|-|-|-|
|   **AnimatedGIFDemo** |   GIF动画贴图Demo，基于 [Animated Texture 插件](https://github.com/neil3d/UnrealAnimatedTexturePlugin)    |   https://neil3d.gitee.io/coding/anim-texture.html  |
|   **BlueprintAsync** |   将异步操作封装为蓝图节点    |   https://zhuanlan.zhihu.com/p/107021667  |
|   **CppUnitTest** |   使用Automation系统进行单元测试    |   https://neil3d.gitee.io/unreal/ue4_cpp_unit_test.html  |
|   **MakingUseOfTaskGraph** | TaskGraph实用指南 |  https://zhuanlan.zhihu.com/ue4-modern-cpp  |
|   **MyBlueprintNode** |   深入Unreal蓝图开发  |   https://zhuanlan.zhihu.com/blueprints-in-depth  |
|   **MySubObjectTest** |   使用 UObject 派生类对象作为属性 | TODO  |
|   **Project2015** |   2015年的一些例子集合，每个 Map 是一个单独的例子 |   https://neil3d.gitee.io/unreal/ue-cookbook.html |

## TODO

- UObject与GC同步对象
    * 使用 FGCCSyncObject Singleton 对象进行 GC 多线程同步控制
    * 使用 FGCObjectScopeGuard 防止 UObject 对象在加载过程中被意外回收
- 异步资源加载
    - 官方文档：https://docs.unrealengine.com/en-US/Programming/Assets/AsyncLoading/index.html
    - FStreamableManager
        * UAssetManager::GetStreamableManager().RequestAsyncLoad()
    - UAsyncActionLoadPrimaryAssetList
