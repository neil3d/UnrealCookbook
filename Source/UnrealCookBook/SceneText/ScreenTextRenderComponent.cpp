// Fill out your copyright notice in the Description page of Project Settings.

#include "ScreenTextRenderComponent.h"
#include "UnrealCookBook.h"

UScreenTextRenderComponent::UScreenTextRenderComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Mobility = EComponentMobility::Movable;
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;
}

void UScreenTextRenderComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	APlayerCameraManager *PCM = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (PCM != nullptr)
	{
		FVector CameraLoc = PCM->GetCameraLocation();
		FRotator LookAtRot = FRotationMatrix::MakeFromX(GetComponentLocation() - CameraLoc).Rotator();
		LookAtRot.Roll = LookAtRot.Pitch = 0;

		SetRelativeRotation(LookAtRot);
	}
}