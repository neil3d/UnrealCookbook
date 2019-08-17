// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MYBLUEPRINTNODE_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
		static void SayHello_Internal();

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
		static void SaySomething_Internal(const TArray<FString>& InWords);
};
