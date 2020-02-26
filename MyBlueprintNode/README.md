# 自定义蓝图节点

- 分为 Runtime 和 Editor 两个模块
- 大部分内容在 Editor 模块

## Runtime 模块

- UMyBlueprintFunctionLibrary：实现蓝图模板函数
    * 详见知乎专栏：https://zhuanlan.zhihu.com/p/84956153
    * 或者 GitHub Page: https://neil3d.gitee.io/unreal/blueprint-wildcard.html

## Editor 模块

- BPNode_SayHello: 通过派生class UK2Node创建自定义蓝图节点
    * 详见知乎专栏：https://zhuanlan.zhihu.com/p/84958215
    * 或者 GitHub Page：https://neil3d.gitee.io/unreal/custom_bp_node.html
- BPNode_SaySomething：实现蓝图节点的动态添加/删除Pin
    * 详见知乎专栏：https://zhuanlan.zhihu.com/p/86231216
    * 或者GitHub Page： https://neil3d.gitee.io/unreal/bp_dynamic_pin.html
- BPNode_TriGate：使用Node Handler定制蓝图节点的编译过程
    * 详见知乎专栏：https://zhuanlan.zhihu.com/p/87489168
    * 或者 GitHub Page：https://neil3d.gitee.io/unreal/bp_node_handler.html