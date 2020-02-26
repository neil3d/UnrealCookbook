// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MySubObject.generated.h"

UCLASS()
class MYSUBOBJECTTEST_API UMySubObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString MyString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool MyBool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> MyArray;

};
