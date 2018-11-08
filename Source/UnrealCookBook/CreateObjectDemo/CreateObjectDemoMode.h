// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "MyObject.h"
#include "CreateObjectDemoMode.generated.h"

/**
 * Create Object Demo
 */
UCLASS()
class UNREALCOOKBOOK_API ACreateObjectDemoMode : public AGameMode
{
	GENERATED_UCLASS_BODY()
	
	virtual void BeginPlay() override;

public:
	/*UObject对象要加上UPROPERTY宏，以使得GC正常工作*/
	UPROPERTY(BlueprintReadWrite, Category = "CreateObjectDemo")
	UMyObject* MyObject;
	
};
