// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

/**
 * create asset context menu for my object explorer
 */
class FMyContentBrowserExtensions
{
public:
	static void InstallHooks();
	static void RemoveHooks();
};