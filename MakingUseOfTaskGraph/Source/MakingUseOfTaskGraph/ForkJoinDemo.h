// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dom/JsonObject.h"	// Json
#include "Dom/JsonValue.h"	// Json
#include "ForkJoinDemo.generated.h"

DECLARE_DELEGATE_OneParam(FTaskDelegate_StockAnalyzeComplete, FVector);

struct FStockAnalyzeContext
{
	bool bRunning = false;
	FString DataFilePath;
	FTaskDelegate_StockAnalyzeComplete CompletionDelegate;

	TArray<TSharedPtr<FJsonValue>> StockData;
	FVector Result;	// {X:max, Y:min, Z:average}

	int32 GetStockDataCount() const;
	float GetStockData(int32 Index) const;
};

UCLASS()
class MAKINGUSEOFTASKGRAPH_API AForkJoinDemo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AForkJoinDemo();

	UFUNCTION(BlueprintCallable)
		void AsyncAnalyzeStockData(const FString& FilePath);

	UFUNCTION(BlueprintImplementableEvent)
		void OnAnalyzeComplete(FVector Result);

protected:
	FStockAnalyzeContext TaskContext;

};
