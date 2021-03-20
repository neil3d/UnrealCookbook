// Fill out your copyright notice in the Description page of Project Settings.


#include "MyObjectExplorerWidget.h"

void UMyObjectExplorerWidget::ShowObjects(const TArray<UObject*>& InObjects)
{
	RootNodes.Empty();
	for (const auto& Obj : InObjects) {
		UMyObjectTreeNode* Node = NewObject<UMyObjectTreeNode>();
		Node->BindObject(Obj);
		RootNodes.Add(Node);
	}
	Event_ShowObjects();
}