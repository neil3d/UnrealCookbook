// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/Visibility.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Input/Reply.h"
#include "KismetNodes/SGraphNodeK2Base.h"

class SVerticalBox;
class UBPNode_SaySomething;

class SGraphNodeSaySomething : public SGraphNodeK2Base
{
public:
	SLATE_BEGIN_ARGS(SGraphNodeSaySomething){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UBPNode_SaySomething* InNode);
protected:
	virtual void CreateInputSideAddButton(TSharedPtr<SVerticalBox> InputBox) override;
	virtual FReply OnAddPin() override;
	// End of SGraphNode interface
};
