// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SGraphNodeSaySomething.h"
#include "BPNode_SaySomething.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "GraphEditorSettings.h"
#include "SGraphPin.h"
#include "KismetPins/SGraphPinExec.h"
#include "NodeFactory.h"
#include "ScopedTransaction.h"
#include "Kismet2/BlueprintEditorUtils.h"

void SGraphNodeSaySomething::Construct(const FArguments& InArgs, UBPNode_SaySomething* InNode)
{
	this->GraphNode = InNode;

	this->SetCursor( EMouseCursor::CardinalCross );

	this->UpdateGraphNode();
}

void SGraphNodeSaySomething::CreateInputSideAddButton(TSharedPtr<SVerticalBox> InputBox)
{
	FText Tmp = FText::FromString(TEXT("Add word"));
	TSharedRef<SWidget> AddPinButton = AddPinButtonContent(Tmp, Tmp);

	FMargin AddPinPadding = Settings->GetInputPinPadding();
	AddPinPadding.Top += 6.0f;

	InputBox->AddSlot()
	.AutoHeight()
	.VAlign(VAlign_Center)
	.Padding(AddPinPadding)
	[
		AddPinButton
	];
}

FReply SGraphNodeSaySomething::OnAddPin()
{
	UBPNode_SaySomething* BPNode = CastChecked<UBPNode_SaySomething>(GraphNode);

	const FScopedTransaction Transaction(NSLOCTEXT("Kismet", "AddArgumentPin", "Add Argument Pin"));
	BPNode->Modify();

	BPNode->AddPinToNode();
	FBlueprintEditorUtils::MarkBlueprintAsModified(BPNode->GetBlueprint());

	UpdateGraphNode();
	GraphNode->GetGraph()->NotifyGraphChanged();

	return FReply::Handled();
}
