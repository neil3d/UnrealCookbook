// Fill out your copyright notice in the Description page of Project Settings.

#include "CreateObjectDemoMode.h"
#include "UnrealCookBook.h"
#include "MyActor.h"


ACreateObjectDemoMode::ACreateObjectDemoMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/* <CreateObjectDemo>
	 * 使用NewObject模板函数，来创建UObject派生类对象
	*/
	MyObject = NewObject<UMyObject>();
}

void ACreateObjectDemoMode::BeginPlay()
{
	Super::BeginPlay();

	/* <CreateObjectDemo>
	 * 创建AActor派生类对象不要用NewObject或者new，而要用UWorld::SpawnActor()
	*/
	UWorld* World = GetWorld();
	FVector pos(150, 0, 20);

	AMyActor* MyActor = World->SpawnActor<AMyActor>(pos, FRotator::ZeroRotator);
	if (MyActor != nullptr)
	{
		UE_LOG(MyLog, Log, TEXT("Sapwn MyActor SUCCEED!"));
	}
	else
	{
		UE_LOG(MyLog, Log, TEXT("Sapwn MyActor FAILED!"));
	}
}