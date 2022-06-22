# 虚幻网络同步基础

## 【实验1】GameFramework 基础对象的 Local Role

- 对于 Client
  - GameMode: 没有实例，GameMode只存在与服务端
  - GameState、PlayerState: ROLE_AutonomousProxy
  - PlayerController、Pawn: ROLE_SimulatedProxy
- 对于 Server
  - 全都是 ROLE_Authority

## 【实验2】Actor的创建和销毁是自动同步的

- 动态创建的Actor class：需要开启同步
  - C++: void AActor::SetReplicates(bool bInReplicates)
  - 蓝图 ClassDefaults:
    - [x] Replication|Replicates
- 创建和销毁的操作(Spawn/Destroy)需要在Server端执行，才会同步到各个Client
  - 在Client执行 Spawn/Destroy 不会被同步
  - When you spawn or destroy a replicated Actor, ensure that you do it on the server.

## 【实验3】数据类型变量同步|Variable Replication

- RepNotify

## 【实验4】Object 引用同步

- [官方文档：Replicating object references](https://docs.unrealengine.com/5.0/en-US/replicating-object-references-in-unreal-engine/)

## 【实验5】RPCs

- [官方文档：RPCs](https://docs.unrealengine.com/5.0/en-US/rpcs-in-unreal-engine/)

## 【实验6】移动同步|Movement Replication
