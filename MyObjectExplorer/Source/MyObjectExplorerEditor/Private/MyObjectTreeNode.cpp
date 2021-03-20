// Fill out your copyright notice in the Description page of Project Settings.


#include "MyObjectTreeNode.h"

void UMyObjectTreeNode::BindObject(UObject* InObject)
{
	MyObject = InObject;
	MyObject->GetName(this->Name);
	this->ClassName = MyObject->GetClass()->GetFullName();
}

TArray<UMyObjectTreeNode*> UMyObjectTreeNode::GetChildren() const
{
	TArray<UMyObjectTreeNode*> Children;


	return Children;
}