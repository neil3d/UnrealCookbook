/**
 *	实现笔记：
 *	1. 尝试了从 UK2Node_CallFunction 派生，可以他的成员函数 GetTargetFunction() 不是虚函数。。。故放弃
 *	2. 动态 Pin 部分，主要参考了：UK2Node_FormatText
 *	3. Wildcard Pin 连接处理，参考： UK2Node_FormatText::ExpandNode
 *		- For each argument, we will need to add in a "Make Struct" node.
*/

#include "MyWildFunctionCallNode.h"
#include "BlueprintNodeSpawner.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"	// BlueprintGraph
#include "KismetCompiler.h"	// KismetCompiler
#include "K2Node_MakeStruct.h"
#include "MyWildFunctionLibrary.h"


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

	// TODO: 这里就临时创建一个“输入参数”Pin，后续可以添加UI，动态增加
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, "Param1");
}

UEdGraphPin* FindOutputStructPinChecked(UEdGraphNode* Node)
{
	check(NULL != Node);
	UEdGraphPin* OutputPin = NULL;
	for (int32 PinIndex = 0; PinIndex < Node->Pins.Num(); ++PinIndex)
	{
		UEdGraphPin* Pin = Node->Pins[PinIndex];
		if (Pin && (EGPD_Output == Pin->Direction))
		{
			OutputPin = Pin;
			break;
		}
	}
	check(NULL != OutputPin);
	return OutputPin;
}

void UMyWildFunctionCallNode::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	UEdGraphPin* ExecPin = GetExecPin();
	UEdGraphPin* ThenPin = GetThenPin();

	if (ExecPin && ThenPin) {


		TArray<UEdGraphPin*> CachedParamPins;
		// 测试代码：这里架设有一个输入参数Pin
		CachedParamPins.Add(FindPin(TEXT("Param1"), EGPD_Input));
		int ParamCount = CachedParamPins.Num();

		// create a CallFunction node
		// TODO: 根据 ParamCount 选择不同的 Invoke 函数
		FName MyFunctionName = GET_FUNCTION_NAME_CHECKED(UMyWildFunctionLibrary, MyGenericInvoke_P1);

		UK2Node_CallFunction* CallFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		CallFuncNode->FunctionReference.SetExternalMember(MyFunctionName, UMyWildFunctionLibrary::StaticClass());
		CallFuncNode->AllocateDefaultPins();

		// move pins
		CompilerContext.MovePinLinksToIntermediate(*ExecPin, *(CallFuncNode->GetExecPin()));
		CompilerContext.MovePinLinksToIntermediate(*ThenPin, *(CallFuncNode->GetThenPin()));
		UEdGraphPin* FuncParamPin = CallFuncNode->FindPinChecked(TEXT("Param1"));


		// param pin
		// 需要通过一个 "Make Struct" node 来对 Wildcard Pin 进行处理
		if (ParamCount > 0)
		{
			UEdGraphPin* ArgumentPin = CachedParamPins[0];
			CompilerContext.MovePinLinksToIntermediate(*ArgumentPin, *FuncParamPin);
		}
	}

	// break any links to the expanded node
	BreakAllNodeLinks();
}

UEdGraphPin* UMyWildFunctionCallNode::GetThenPin() const
{
	UEdGraphPin* Pin = FindPin(UEdGraphSchema_K2::PN_Then);
	check(Pin == nullptr || Pin->Direction == EGPD_Output); // If pin exists, it must be output
	return Pin;
}