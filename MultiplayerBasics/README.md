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