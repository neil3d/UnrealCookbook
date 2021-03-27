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

void SMyWildNodeWidget::CreatePinWidgets()
{
	// 1. 先创建 Exec-Then Pin & Self Pin
	auto ExecPin = GraphNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute);
	auto ThenPin = GraphNode->FindPinChecked(UEdGraphSchema_K2::PN_Then);
	auto SelfPin = GraphNode->FindPinChecked(UEdGraphSchema_K2::PN_Self, EGPD_Input);
	CreateStandardPinWidget(ExecPin);
	CreateStandardPinWidget(ThenPin);
	CreateStandardPinWidget(SelfPin);

	// 2. 两个 Combo box 放中间
	CreateComboLists();

	// 3. 下面是自动生成的 Param Pins
	for (int32 PinIndex = 0; PinIndex < GraphNode->Pins.Num(); ++PinIndex)
	{
		UEdGraphPin* CurPin = GraphNode->Pins[PinIndex];
		if (CurPin == SelfPin || CurPin==ExecPin || CurPin==ThenPin)
			continue;

		if (!ensureMsgf(CurPin->GetOuter() == GraphNode
			, TEXT("Graph node ('%s' - %s) has an invalid %s pin: '%s'; (with a bad %s outer: '%s'); skiping creation of a widget for this pin.")
			, *GraphNode->GetNodeTitle(ENodeTitleType::ListView).ToString()
			, *GraphNode->GetPathName()
			, (CurPin->Direction == EEdGraphPinDirection::EGPD_Input) ? TEXT("input") : TEXT("output")
			, CurPin->PinFriendlyName.IsEmpty() ? *CurPin->PinName.ToString() : *CurPin->PinFriendlyName.ToString()
			, CurPin->GetOuter() ? *CurPin->GetOuter()->GetClass()->GetName() : TEXT("UNKNOWN")
			, CurPin->GetOuter() ? *CurPin->GetOuter()->GetPathName() : TEXT("NULL")))
		{
			continue;
		}

		CreateStandardPinWidget(CurPin);
	}

}

void SMyWildNodeWidget::CreateComboLists()
{
	TSharedPtr<SVerticalBox> MainBox = this->LeftNodeBox;
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
	SubClassList.Empty();

	UMyWildFunctionCallNode* Node = Cast<UMyWildFunctionCallNode>(this->GraphNode);
	UClass* ParentClass = Node->GetSelectedClass();
	if (!ParentClass)
		return;

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
	SelectedClass = Item;
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
	MemberFuncList.Empty();

	UMyWildFunctionCallNode* Node = Cast<UMyWildFunctionCallNode>(this->GraphNode);
	UClass* ParentClass = Node->GetSelectedClass();
	if (!ParentClass)
		return;

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
	SelectedFunc = Item;
	UMyWildFunctionCallNode* Node = Cast<UMyWildFunctionCallNode>(this->GraphNode);
	Node->OnFunctionSelected(Item);
}

FText SMyWildNodeWidget::OnFuncComboList_GetFilterText() const
{
	auto SelectedItem = MemberFuncCombo->GetSelectedItem();
	if (SelectedItem)
		return SelectedItem->GetDisplayNameText();
	else
		return FText::FromString("None");
}