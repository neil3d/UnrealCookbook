// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StdIteratorTest.generated.h"

/*
 * 演示标准C++容器的iterator的使用
*/
UCLASS()
class PLAYITERATOR_API AStdIteratorTest : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	void TestStdVector();
};
