// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 开启 Replication
	bReplicates = true;
}

void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*
	* 参考 ACharacter：CurrentHP 这个属性只同步给自己的 Simulated Actor
	* see: https://www.unrealengine.com/en-US/blog/network-tips-and-tricks
	* only replicate to clients that have a simulating copy of this actor.
	*/
	//DOREPLIFETIME(AMyCharacter, CurrentHP);
	DOREPLIFETIME_CONDITION(AMyCharacter, CurrentHP, COND_SimulatedOnly);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 服务端逻辑：设置 Timer 修改 CurrentHP
	if (GetLocalRole() == ROLE_Authority)
	{
		UWorld* WorldContext = GetWorld();
		WorldContext->GetTimerManager().SetTimer(TestTimer, this, &AMyCharacter::OnTimer, 2.0, true);
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyCharacter::OnRep_CurrentHP()
{
	ReciveCurrentHPChanged();
}

void AMyCharacter::OnTimer()
{
	check(GetLocalRole() == ROLE_Authority);
	CurrentHP += 5;
}
