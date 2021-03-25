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
#include "K2Node_MakeArray.h"
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

	CachedFuncNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, TEXT("FuncName"));

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
	UEdGraphPin* FuncNamePin = FindPinChecked(TEXT("FuncName"), EGPD_Input);
	UEdGraphPin* ParamPin = FindPinChecked(TEXT("Param1"), EGPD_Input);

	if (ExecPin && ThenPin) {
		// create a CallFunction node
		FName MyFunctionName = GET_FUNCTION_NAME_CHECKED(UMyWildFunctionLibrary, MyGenericInvoke);

		UK2Node_CallFunction* CallFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		CallFuncNode->FunctionReference.SetExternalMember(MyFunctionName, UMyWildFunctionLibrary::StaticClass());
		CallFuncNode->AllocateDefaultPins();

		// move pins
		CompilerContext.MovePinLinksToIntermediate(*ExecPin, *(CallFuncNode->GetExecPin()));
		CompilerContext.MovePinLinksToIntermediate(*ThenPin, *(CallFuncNode->GetThenPin()));
		CompilerContext.MovePinLinksToIntermediate(*FuncNamePin, *(CallFuncNode->FindPinChecked(TEXT("FuncName"), EGPD_Input)));
		
		// params pin
		
		
		// Spawn a "Make Struct" node to create the struct needed for formatting the text.
		UK2Node_MakeStruct* MakeMyVarStruct = CompilerContext.SpawnIntermediateNode<UK2Node_MakeStruct>(this, SourceGraph);
		MakeMyVarStruct->StructType = FMyVarParam::StaticStruct();
		MakeMyVarStruct->AllocateDefaultPins();
		MakeMyVarStruct->bMadeAfterOverridePinRemoval = true;
		CompilerContext.MessageLog.NotifyIntermediateObjectCreation(MakeMyVarStruct, this);

		// FMyVarParam 中的 UProperty 会产生 Pin，自动把输入参数连过来
		if (ParamPin->LinkedTo.Num() > 0)
		{
			// 获得当前 Param 连接进来的 Pin
			UEdGraphPin* LinkedParamPin = ParamPin->LinkedTo[0];
			const FName& ParamType = LinkedParamPin->PinType.PinCategory;

			// 设置 FMyVarParam.TypeName
			UEdGraphPin* VarTypePin = MakeMyVarStruct->FindPinChecked(GET_MEMBER_NAME_STRING_CHECKED(FMyVarParam, TypeName));
			MakeMyVarStruct->GetSchema()->TrySetDefaultValue(*VarTypePin, ParamType.ToString());

			// 根据输入 Pin 的类型，来写入 FMyVarParam 的不同成员变量
			if (ParamType == UEdGraphSchema_K2::PC_Float) {
				UEdGraphPin* fValue = MakeMyVarStruct->FindPinChecked(GET_MEMBER_NAME_STRING_CHECKED(FMyVarParam, fValue));
				CompilerContext.MovePinLinksToIntermediate(*ParamPin, *MakeMyVarStruct->FindPinChecked(GET_MEMBER_NAME_STRING_CHECKED(FMyVarParam, fValue)));
			}
			else if (ParamType == UEdGraphSchema_K2::PC_String)
			{
				UEdGraphPin* szValue = MakeMyVarStruct->FindPinChecked(GET_MEMBER_NAME_STRING_CHECKED(FMyVarParam, szValue));
				CompilerContext.MovePinLinksToIntermediate(*ParamPin, *MakeMyVarStruct->FindPinChecked(GET_MEMBER_NAME_STRING_CHECKED(FMyVarParam, szValue)));
			}
			// TODO： 参考 UK2Node_FormatText::ExpandNode() 写各种类型的转换 。。。。
		}

		// Find the output for the pin's "Make Struct" node and link it to the corresponding pin on the "Make Array" node.
		FindOutputStructPinChecked(MakeMyVarStruct)->MakeLinkTo(CallFuncNode->FindPinChecked(TEXT("InArgs"), EGPD_Input));
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