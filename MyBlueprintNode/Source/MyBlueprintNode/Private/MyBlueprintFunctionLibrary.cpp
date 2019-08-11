// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"
#include "Engine.h"

void UMyBlueprintFunctionLibrary::SayHello_Internal() {
	if (GEngine) {
		const int32 AlwaysAddKey = -1;

		GEngine->AddOnScreenDebugMessage(AlwaysAddKey,
			5.0f,
			FColor::Purple,
			TEXT("Hello World!")
		);
	}
}
