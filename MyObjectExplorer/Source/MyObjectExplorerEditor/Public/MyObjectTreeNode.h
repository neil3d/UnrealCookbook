// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyObjectTreeNode.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class MYOBJECTEXPLOREREDITOR_API UMyObjectTreeNode : public UObject
{
	GENERATED_BODY()

public:
	void BindObject(UObject* InObject);

	UFUNCTION(BlueprintCallable, Category = MyObjectExplorer)
		TArray<UMyObjectTreeNode*> GetChildren() const;

public:
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = MyObjectExplorer)
		FString RefType;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = MyObjectExplorer)
		FString ClassName;

	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = MyObjectExplorer)
		FString Name;

private:
	UPROPERTY(Transient)
		UObject* MyObject;
};
