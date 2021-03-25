// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/Variant.h"
#include "MyWildFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct FMyVarParam {
	GENERATED_USTRUCT_BODY()

public:
	//TODO: ≥¢ ‘ π”√ FVariant var;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fValue;
};

/**
 *
 */
UCLASS()
class MYWILDNODE_API UMyWildFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//BlueprintInternalUseOnly = "true",
	UFUNCTION(BlueprintCallable, Category = MyWildDemo)
		static void MyGenericInvoke(const FString& FuncName, const FMyVarParam& InArgs);

	//static void MyGenericInvoke(const TArray<FMyVarParam>& InArgs);

};
