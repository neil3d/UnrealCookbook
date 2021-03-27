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
		FName TypeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString szValue;
};

/**
 *
 */
UCLASS()
class MYWILDNODE_API UMyWildFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/*
	* 
	* ≤Œøº: class ENGINE_API UKismetTextLibrary : public UBlueprintFunctionLibrary
	* 
		//Used for formatting text using the FText::Format function and utilized by the UK2Node_FormatText 
		UFUNCTION(BlueprintPure, meta = (BlueprintInternalUseOnly = "true"))
		static FText Format(FText InPattern, TArray<FFormatArgumentData> InArgs);
	*/
	UFUNCTION(BlueprintCallable, Category = MyWildDemo, meta = (BlueprintInternalUseOnly = "true"))
		static void MyGenericInvoke(UObject* InSelf, const FString& InFuncName, const TArray<FMyVarParam>& InArgs);

};
