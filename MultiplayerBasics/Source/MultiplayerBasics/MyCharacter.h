// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class MULTIPLAYERBASICS_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	// Replication 属性同步必要的重载函数
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// 定义一个具备 Rep Notify 的同步属性变量
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_CurrentHP)
	float CurrentHP;

	// 用来接收 Rep Notify 的回调函数
	UFUNCTION()
	void OnRep_CurrentHP();

	UFUNCTION(BlueprintImplementableEvent)
	void ReciveCurrentHPChanged();

	FTimerHandle TestTimer;
	void OnTimer();
};
