// Fill out your copyright notice in the Description page of Project Settings.


#include "MyObjectExplorerWidget.h"

void UMyObjectExplorerWidget::ShowObject(const FString& szObjectPath)
{
	FSoftObjectPath ObjectPath(szObjectPath);
	UObject* Obj = ObjectPath.TryLoad();
	if (Obj)
	{
		RootNode = NewObject<UMyObjectTreeNode>();
		RootNode->BindObject(Obj->GetOutermost());
		Event_ShowObject();
	}
}
