// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dom/JsonObject.h"	// Json
#include "ForkJoinDemo.generated.h"

struct FStockAnalyzeContext
{
	FString DataFilePath;
	TSharedPtr<FJsonObject> StockData;
	FVector Result;	// {X:max, Y:min, Z:average}
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
