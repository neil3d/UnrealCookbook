// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWildFunctionCallNode.h"
#include "BlueprintNodeSpawner.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "EdGraphSchema_K2.h"


void UMyWildFunctionCallNode::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FText UMyWildFunctionCallNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("MyWildFunctionCall");;
}

void UMyWildFunctionCallNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

	CachedFuncNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Text, TEXT("FuncName"));
}