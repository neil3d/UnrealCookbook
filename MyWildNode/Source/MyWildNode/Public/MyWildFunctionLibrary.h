// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyWildFunctionLibrary.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FDummyStruct {
	GENERATED_USTRUCT_BODY()
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
	UFUNCTION(BlueprintCallable, CustomThunk, Category= MyWildDemo, meta = (CustomStructureParam = "Param1"))
		static void MyGenericInvoke_P1(const FDummyStruct& Param1);
	static void MyGenericInvoke_P1_Impl(const void* StructAddr, const FStructProperty* StructProperty);

	DECLARE_FUNCTION(execMyGenericInvoke_P1) {
		Stack.MostRecentPropertyAddress = nullptr;
		Stack.MostRecentProperty = nullptr;

		Stack.StepCompiledIn<FStructProperty>(NULL);
		void* StructAddr = Stack.MostRecentPropertyAddress;
		FStructProperty* StructProperty = Cast<FStructProperty>(Stack.MostRecentProperty);

		P_FINISH;

		P_NATIVE_BEGIN;
		MyGenericInvoke_P1_Impl(StructAddr, StructProperty);
		P_NATIVE_END;
	}

};
