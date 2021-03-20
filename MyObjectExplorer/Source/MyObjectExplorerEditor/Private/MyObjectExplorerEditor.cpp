// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyObjectExplorerEditor.h"
#include "MyContentBrowserExtensions.h"

#define LOCTEXT_NAMESPACE "FMyObjectExplorerEditorModule"

void FMyObjectExplorerEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FMyContentBrowserExtensions::InstallHooks();
}

void FMyObjectExplorerEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FMyContentBrowserExtensions::RemoveHooks();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMyObjectExplorerEditorModule, MyObjectExplorerEditor)