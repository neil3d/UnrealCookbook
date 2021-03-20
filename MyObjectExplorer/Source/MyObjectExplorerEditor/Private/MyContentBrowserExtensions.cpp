// Fill out your copyright notice in the Description page of Project Settings.


#include "MyContentBrowserExtensions.h"
#include "ContentBrowserModule.h"	// ContentBrowser
#include "ContentBrowserDelegates.h"	// ContentBrowser
#include "EditorStyleSet.h"		// EditorStyle
#include "Styling/ISlateStyle.h"	// 	SlateCore

static FContentBrowserMenuExtender_SelectedAssets ContentBrowserExtenderDelegate;
static FDelegateHandle ContentBrowserExtenderDelegateHandle;

struct FContentBrowserSelectedAssetExtensionBase
{
public:
	TArray<struct FAssetData> SelectedAssets;

public:
	virtual void Execute() {}
	virtual ~FContentBrowserSelectedAssetExtensionBase() {}
};


struct FMyObjectExplorerExtension : public FContentBrowserSelectedAssetExtensionBase
{
	virtual void Execute() override
	{
		UE_LOG(LogTemp, Log, TEXT("Begin of UObject Explorer"));
		for(const auto& asset:SelectedAssets)
			UE_LOG(LogTemp, Log, TEXT("\t %s"), *(asset.AssetName.ToString()));
		UE_LOG(LogTemp, Log, TEXT("End of  UObject Explorer"));
	}
};

class FMyContentBrowserExtensions_Impl
{
public:
	static TSharedRef<FExtender> OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& SelectedAssets)
	{
		TSharedRef<FExtender> Extender(new FExtender());

		Extender->AddMenuExtension(
			"GetAssetActions",
			EExtensionHook::After,
			nullptr,
			FMenuExtensionDelegate::CreateStatic(&FMyContentBrowserExtensions_Impl::CreateMyActionsSubMenu, SelectedAssets));

		return Extender;
	}

	static void CreateMyActionsSubMenu(FMenuBuilder& MenuBuilder, TArray<FAssetData> SelectedAssets)
	{
		TSharedPtr<FMyObjectExplorerExtension> MyExplorerFunctor = MakeShareable(new FMyObjectExplorerExtension());
		MyExplorerFunctor->SelectedAssets = SelectedAssets;

		FUIAction Action_MyExplorer(
			FExecuteAction::CreateStatic(&FMyContentBrowserExtensions_Impl::ExecuteSelectedContentFunctor, StaticCastSharedPtr<FContentBrowserSelectedAssetExtensionBase>(MyExplorerFunctor)));

		const FName StyleSetName = FEditorStyle::Get().GetStyleSetName();

		MenuBuilder.AddMenuEntry(
			FText::FromString("My Object Explorer"),
			FText::FromString("Explorer Unreal Objects"),
			FSlateIcon(StyleSetName, "ClassIcon.Blueprint"),
			Action_MyExplorer,
			NAME_None,
			EUserInterfaceActionType::Button);
	}

	static void ExecuteSelectedContentFunctor(TSharedPtr<FContentBrowserSelectedAssetExtensionBase> SelectedAssetFunctor)
	{
		SelectedAssetFunctor->Execute();
	}

	static TArray<FContentBrowserMenuExtender_SelectedAssets>& GetExtenderDelegates()
	{
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
		return ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
	}
};

void FMyContentBrowserExtensions::InstallHooks()
{
	ContentBrowserExtenderDelegate = FContentBrowserMenuExtender_SelectedAssets::CreateStatic(&FMyContentBrowserExtensions_Impl::OnExtendContentBrowserAssetSelectionMenu);

	TArray<FContentBrowserMenuExtender_SelectedAssets>& CBMenuExtenderDelegates = FMyContentBrowserExtensions_Impl::GetExtenderDelegates();
	CBMenuExtenderDelegates.Add(ContentBrowserExtenderDelegate);
	ContentBrowserExtenderDelegateHandle = CBMenuExtenderDelegates.Last().GetHandle();
}

void FMyContentBrowserExtensions::RemoveHooks()
{
	TArray<FContentBrowserMenuExtender_SelectedAssets>& CBMenuExtenderDelegates = FMyContentBrowserExtensions_Impl::GetExtenderDelegates();
	CBMenuExtenderDelegates.RemoveAll([](const FContentBrowserMenuExtender_SelectedAssets& Delegate) { return Delegate.GetHandle() == ContentBrowserExtenderDelegateHandle; });
}