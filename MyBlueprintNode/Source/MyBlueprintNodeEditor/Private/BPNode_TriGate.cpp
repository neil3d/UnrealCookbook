// Fill out your copyright notice in the Description page of Project Settings.


#include "BPNode_TriGate.h"
#include "EdGraphSchema_K2.h"	// BlueprintGraph
#include "BlueprintNodeSpawner.h"	// BlueprintGraph
#include "BlueprintActionDatabaseRegistrar.h"	// BlueprintGraph
#include "BPTerminal.h"
#include "KismetCompilerMisc.h"
#include "KismetCompiler.h"
#include "Kismet/KismetMathLibrary.h"
#include "EdGraphUtilities.h"

namespace TriGatePN
{
	FName Input = TEXT("Integer");
	FName Positive = TEXT("Positive");
	FName Zero = TEXT("Zero");
	FName Negative = TEXT("Negative");
}

class FKCHandler_TriGate : public FNodeHandlingFunctor
{
protected:
	// 为每个Node开辟一个临时bool变量，用来存储输入参数的比较结果
	TMap<UEdGraphNode*, FBPTerminal*> BoolTermMap;
	FBPTerminal* LiteralZeroTerm;

public:
	FKCHandler_TriGate(FKismetCompilerContext& InCompilerContext)
		: FNodeHandlingFunctor(InCompilerContext)
	{
	}

	virtual void RegisterNets(FKismetFunctionContext& Context, UEdGraphNode* Node) override
	{
		FNodeHandlingFunctor::RegisterNets(Context, Node);

		FBPTerminal* BoolTerm = Context.CreateLocalTerminal();
		BoolTerm->Type.PinCategory = UEdGraphSchema_K2::PC_Boolean;
		BoolTerm->Source = Node;
		BoolTerm->Name = Context.NetNameMap->MakeValidName(Node) + TEXT("_CmpResult");
		BoolTermMap.Add(Node, BoolTerm);

		LiteralZeroTerm = Context.CreateLocalTerminal(ETerminalSpecification::TS_Literal);
		LiteralZeroTerm->bIsLiteral = true;
		LiteralZeroTerm->Type.PinCategory = UEdGraphSchema_K2::PC_Int;
		LiteralZeroTerm->Name = TEXT("0");

	}

	virtual void RegisterNet(FKismetFunctionContext& Context, UEdGraphPin* Pin)
	{
	}

	virtual void Compile(FKismetFunctionContext& Context, UEdGraphNode* Node) override
	{
		UBPNode_TriGate* MyNode = CastChecked<UBPNode_TriGate>(Node);

		// 查找输入的那个整数的Pin对应的Terminal
		UEdGraphPin* InputPin = Context.FindRequiredPinByName(Node, TriGatePN::Input, EGPD_Input);

		UEdGraphPin* PinToTry = FEdGraphUtilities::GetNetFromPin(InputPin);
		FBPTerminal** pInputTerm = Context.NetMap.Find(PinToTry);
		if (pInputTerm == nullptr)
		{
			CompilerContext.MessageLog.Error(TEXT("FKCHandler_TriGate: Failed to resolve term passed into"), InputPin);
			return;
		}

		FBPTerminal* InputTerm = *pInputTerm;

		// 查找三个输出Pin
		UEdGraphPin* PositivePin = MyNode->FindPin(TriGatePN::Positive, EGPD_Output);
		UEdGraphPin* ZeroPin = MyNode->FindPin(TriGatePN::Zero, EGPD_Output);
		UEdGraphPin* NegativePin = MyNode->FindPin(TriGatePN::Negative, EGPD_Output);

		// 临时bool变量的Terminal
		FBPTerminal* BoolTerm = BoolTermMap.FindRef(MyNode);

		UClass* MathLibClass = UKismetMathLibrary::StaticClass();
		UFunction* CreaterFuncPtr = FindField<UFunction>(MathLibClass, "Greater_IntInt");
		UFunction* EqualFuncPtr = FindField<UFunction>(MathLibClass, "EqualEqual_IntInt");

		// Statement 1: 计算表达式 BoolTerm = Interger > 0
		FBlueprintCompiledStatement& CallCreaterZero = Context.AppendStatementForNode(MyNode);
		CallCreaterZero.Type = KCST_CallFunction;
		CallCreaterZero.FunctionToCall = CreaterFuncPtr;
		CallCreaterZero.LHS = BoolTerm;
		CallCreaterZero.RHS.Add(InputTerm);
		CallCreaterZero.RHS.Add(LiteralZeroTerm);

		// Statement 2: if(BoolTerm)
		FBlueprintCompiledStatement& IfPositive = Context.AppendStatementForNode(Node);
		IfPositive.Type = KCST_GotoIfNot;
		IfPositive.LHS = BoolTerm;

		// Statement 3: 执行 Positive Pin
		FBlueprintCompiledStatement& ExecPositive = Context.AppendStatementForNode(Node);
		ExecPositive.Type = KCST_UnconditionalGoto;
		Context.GotoFixupRequestMap.Add(&ExecPositive, PositivePin);

		// Statement 4: 计算表达式 BoolTerm = Interger == 0
		FBlueprintCompiledStatement& CallEqualZero = Context.AppendStatementForNode(MyNode);
		CallEqualZero.Type = KCST_CallFunction;
		CallEqualZero.FunctionToCall = EqualFuncPtr;
		CallEqualZero.LHS = BoolTerm;
		CallEqualZero.bIsJumpTarget = true;
		CallEqualZero.RHS.Add(InputTerm);
		CallEqualZero.RHS.Add(LiteralZeroTerm);

		IfPositive.TargetLabel = &CallEqualZero;

		// Statement 5: GotoIfNot(BoolTerm)
		FBlueprintCompiledStatement& IfZero = Context.AppendStatementForNode(Node);
		IfZero.Type = KCST_GotoIfNot;
		IfZero.LHS = BoolTerm;
		Context.GotoFixupRequestMap.Add(&IfZero, NegativePin);

		// Statement 6: 执行 Zero Pin
		FBlueprintCompiledStatement& ExecZero = Context.AppendStatementForNode(Node);
		ExecZero.Type = KCST_UnconditionalGoto;
		Context.GotoFixupRequestMap.Add(&ExecZero, ZeroPin);

	}
};

void UBPNode_TriGate::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Int, TriGatePN::Input);

	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, TriGatePN::Positive);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, TriGatePN::Zero);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, TriGatePN::Negative);

}

void UBPNode_TriGate::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	UClass* ActionKey = GetClass();

	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FNodeHandlingFunctor * UBPNode_TriGate::CreateNodeHandler(FKismetCompilerContext & CompilerContext) const
{
	return new FKCHandler_TriGate(CompilerContext);
}
