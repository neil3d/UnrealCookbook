# 使用 UObject 派生类对象作为属性

- 目标：尝试创建一个 UObject 的派生类，并将其实例作为 AActor 或者 UActorComponent 派生类的 UPROPERTY 
- 需要正确完成序列化/反序列化
- 需要在蓝图编辑器中正常编辑 Class Defaults
- 需要在关卡编辑器中正常编辑实例对象属性

## 聚合进 AActor 

这个情况比较比较简单直接：
- 使用 `UPROPERTY(VisibleAnywhere, Instanced)` 来标记 `UMySubObject* MySubObject`；
- 在 Actor 的构造函数中创建子对象： `MySubObject = CreateDefaultSubobject<UMySubObject>("MySubObject")`

## 聚合进入 UActorComponent 

按照上面的 Actor 的情况做了之后，会发现在关卡编辑的时候，属性无法改变。调试了一些引擎源代码，得出的结论如下：
1. 如果一个对象是 Blueprint Generated Class 的实例，那么属性变更时，会触发蓝图的重新编译
2. 蓝图重新编译之后，会实现新的 Class 来创建关卡中的 Actor 实例，并替换原来老的对象
3. 对于 Actor Component 需要在这个过程中做一点特殊处理：
    * 实现 `UActorComponent::GetComponentInstanceData()` 函数
    * 在这个函数的调用过程中，需要把老的 Component 中子对象存一下，然后读取到新的 Component 中

``` cpp
TArray<uint8> SavedPropertyBuffer;
FObjectWriter Writer(OldComponentSubObject, SavedPropertyBuffer);
FObjectReader Reader(NewComponent->MySubObject, SavedPropertyBuffer);
```