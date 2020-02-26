// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MySubObject.h"
#include "MyActor.generated.h"

UCLASS()
class MYSUBOBJECTTEST_API AMyActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyActor();

	UPROPERTY(VisibleAnywhere, Instanced, BlueprintReadOnly, meta = (ShowOnlyInnerProperties))
		UMySubObject* MySubObject;

};
