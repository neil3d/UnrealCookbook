// Fill out your copyright notice in the Description page of Project Settings.


#include "SMyWildNodeWidget.h"
#include "SlateOptMacros.h"
#include "EditorStyleSet.h"	// EditorStyle
#include "MyWildFunctionCallNode.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMyWildNodeWidget::Construct(const FArguments& InArgs, UMyWildFunctionCallNode* InNode)
{
	this->GraphNode = InNode;
	this->SetCursor(EMouseCursor::Default);
	this->UpdateGraphNode();
}

void SMyWildNodeWidget::CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox)
{
	MainBox->AddSlot()
		.AutoHeight()
		.Padding(5.0f)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("NoBorder"))
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SAssignNew(SubClassCombo, SComboBox<UClass*>)
				.OptionsSource(&SubClassList)
				.OnComboBoxOpening(this, &SMyWildNodeWidget::OnClassComboList_Update)
				.OnGenerateWidget(this, &SMyWildNodeWidget::OnClassComboList_GenerateWidget)
				.OnSelectionChanged(this, &SMyWildNodeWidget::OnClassComboList_SelectionChanged)
				[
					SNew(STextBlock)
					.Text(this, &SMyWildNodeWidget::OnClassComboList_GetFilterText)
				]
			]
		];

	MainBox->AddSlot()
		.AutoHeight()
		.Padding(5.0f)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("NoBorder"))
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SAssignNew(MemberFuncCombo, SComboBox<UFunction*>)
				.OptionsSource(&MemberFuncList)
				.OnComboBoxOpening(this, &SMyWildNodeWidget::OnFuncComboList_Update)
				.OnGenerateWidget(this, &SMyWildNodeWidget::OnFuncComboList_GenerateWidget)
				.OnSelectionChanged(this, &SMyWildNodeWidget::OnFuncComboList_SelectionChanged)
				[
					SNew(STextBlock)
					.Text(this, &SMyWildNodeWidget::OnFuncComboList_GetFilterText)
				]
			]
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMyWildNodeWidget::OnClassComboList_Update()
{
	UMyWildFunctionCallNode* Node = Cast<UMyWildFunctionCallNode>(this->GraphNode);
	UClass* ParentClass = Node->GetSelectedClass();

	SubClassList.Empty();
	for (TObjectIterator<UClass> Iter; Iter; ++Iter)
	{
		UClass* Class = *Iter;
		// 可能需要过滤掉：虚基类(CLASS_Abstract)，废弃的类(CLASS_Deprecated), 参加：enum EClassFlags
		if (Class->HasAnyClassFlags(CLASS_Deprecated))
			continue;

		if (Class->IsChildOf(ParentClass))
			SubClassList.Add(Class);
	}// end of for
}

TSharedRef<SWidget> SMyWildNodeWidget::OnClassComboList_GenerateWidget(UClass* InItem)
{
	return SNew(SBox)
		.WidthOverride(300)
		[
			SNew(STextBlock)
			.Text(InItem->GetDisplayNameText())
		];
}
void SMyWildNodeWidget::OnClassComboList_SelectionChanged(UClass* Item, ESelectInfo::Type SelectInfo)
{
}

FText SMyWildNodeWidget::OnClassComboList_GetFilterText() const
{
	auto SelectedItem = SubClassCombo->GetSelectedItem();
	if (SelectedItem)
		return SelectedItem->GetDisplayNameText();
	else
		return FText::FromString("None");
}

void SMyWildNodeWidget::OnFuncComboList_Update()
{
	UMyWildFunctionCallNode* Node = Cast<UMyWildFunctionCallNode>(this->GraphNode);
	UClass* ParentClass = Node->GetSelectedClass();

	MemberFuncList.Empty();
	for (TFieldIterator<UFunction> Iter(ParentClass, EFieldIteratorFlags::ExcludeSuper); Iter; ++Iter)
	{
		MemberFuncList.Add(*Iter);
	}// end of for
}

TSharedRef<SWidget> SMyWildNodeWidget::OnFuncComboList_GenerateWidget(UFunction* InItem)
{
	return SNew(SBox)
		.WidthOverride(300)
		[
			SNew(STextBlock)
			.Text(InItem->GetDisplayNameText())
		];
}

void SMyWildNodeWidget::OnFuncComboList_SelectionChanged(UFunction* Item, ESelectInfo::Type SelectInfo)
{
}

FText SMyWildNodeWidget::OnFuncComboList_GetFilterText() const
{
	auto SelectedItem = MemberFuncCombo->GetSelectedItem();
	if (SelectedItem)
		return SelectedItem->GetDisplayNameText();
	else
		return FText::FromString("None");
}