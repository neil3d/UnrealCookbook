// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include "MyWildFunctionCallNode.h"

/**
 * 
 */
class MYWILDNODEEDITOR_API SMyWildNodeWidget : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SMyWildNodeWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, UMyWildFunctionCallNode* InNode);

	virtual void CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox) override;

private:
	void OnClassComboList_Update();
	TSharedRef<SWidget> OnClassComboList_GenerateWidget(UClass* InItem);
	void OnClassComboList_SelectionChanged(UClass* Item, ESelectInfo::Type SelectInfo);
	FText OnClassComboList_GetFilterText() const;

	void OnFuncComboList_Update();
	TSharedRef<SWidget> OnFuncComboList_GenerateWidget(UFunction* InItem);
	void OnFuncComboList_SelectionChanged(UFunction* Item, ESelectInfo::Type SelectInfo);
	FText OnFuncComboList_GetFilterText() const;

private:
	TSharedPtr<SComboBox<UClass*>> SubClassCombo;
	TArray<UClass*> SubClassList;

	TSharedPtr<SComboBox<UFunction*>> MemberFuncCombo;
	TArray<UFunction*> MemberFuncList;
};
