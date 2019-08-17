// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "BPNode_SaySomething.generated.h"

/**
 *
 */
UCLASS()
class MYBLUEPRINTNODEEDITOR_API UBPNode_SaySomething : public UK2Node
{
	GENERATED_BODY()

public:
	// UEdGraphNode interface
	virtual void AllocateDefaultPins() override;

	virtual FText GetTooltipText() const override { return FText::FromString(TEXT("a dynamic pin demo node")); }
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString(TEXT("Say Something")); }
	// End of UEdGraphNode interface

	// UK2Node interface
	virtual FText GetMenuCategory() const { return FText::FromString(TEXT("MyBlueprintNodes")); }
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;

	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	// End of UK2Node interface

	UEdGraphPin* GetThenPin() const;

	// Begin of dynamic pin
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;

	void AddPinToNode();
	// End

private:
	UPROPERTY()
	TArray<FName> ArgPinNames;
};
