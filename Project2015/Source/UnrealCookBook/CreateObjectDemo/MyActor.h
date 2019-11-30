// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MyActorComponent.h"
#include "MyActor.generated.h"

UCLASS()
class UNREALCOOKBOOK_API AMyActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="CreateObjectDemo")
	UMyActorComponent* MyComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="CreateObjectDemo")
	UStaticMeshComponent* StaticMeshComponent;

public:	
	// Sets default values for this actor's properties
	AMyActor();
	
};
