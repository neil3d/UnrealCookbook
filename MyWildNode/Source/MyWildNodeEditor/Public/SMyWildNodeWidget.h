// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include "MyWildFunctionCallNode.h"

/**
 * 
 */
class MYWILDNODEEDITOR_API SMyWildNodeWidget : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SMyWildNodeWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, UMyWildFunctionCallNode* InNode);
};
