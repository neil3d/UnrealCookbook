// Fill out your copyright notice in the Description page of Project Settings.


#include "SMyWildNodeWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMyWildNodeWidget::Construct(const FArguments& InArgs, UMyWildFunctionCallNode* InNode)
{
	this->GraphNode = InNode;
	this->SetCursor(EMouseCursor::Default);
	this->UpdateGraphNode();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
