// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IteratorTest.generated.h"

/*
 * Unreal内置的容器的Iterator的使用
 * https://www.unrealengine.com/en-US/blog/ue4-libraries-you-should-know-about?sessionInvalidated=true&lang=en-US
 * 
*/
UCLASS()
class PLAYITERATOR_API AIteratorTest : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	// https ://docs.unrealengine.com/en-us/Programming/UnrealArchitecture/TArrays
	void ArrayTest();

	// https://docs.unrealengine.com/en-us/Programming/UnrealArchitecture/TMap
	void MapTest();

};
