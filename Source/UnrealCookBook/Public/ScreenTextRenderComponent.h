// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextRenderComponent.h"
#include "ScreenTextRenderComponent.generated.h"

/**
 * Screen Align Text Render
 */
UCLASS(Blueprintable, ClassGroup=Rendering, meta=(BlueprintSpawnableComponent))
class UNREALCOOKBOOK_API UScreenTextRenderComponent : public UTextRenderComponent
{
	GENERATED_UCLASS_BODY()
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);
};
