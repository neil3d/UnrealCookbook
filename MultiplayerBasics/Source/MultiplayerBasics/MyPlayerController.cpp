// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

bool AMyPlayerController::IsLocalController_BP() const
{
    return Super::IsLocalController();
}
