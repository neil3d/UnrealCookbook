// Fill out your copyright notice in the Description page of Project Settings.


#include "MyObjectTreeNode.h"
#include "UObject/UObjectHash.h"

void UMyObjectTreeNode::BindObject(UObject* InObject)
{
	MyObject = InObject;
	MyObject->GetName(this->Name);
	this->ClassName = MyObject->GetClass()->GetFullName();

	TArray<UObject*> MyChildObjects = GetChildObjects();
	this->NumChildren = MyChildObjects.Num();
}

TArray<UObject*> UMyObjectTreeNode::GetChildObjects() const
{
	TArray<UObject*> MyObjectsWithOuter;
	GetObjectsWithOuter(MyObject, MyObjectsWithOuter);

	TArray<UObject*> MyChildObjects;
	for (UObject* Obj : MyObjectsWithOuter)
	{
		if (MyObject == Obj->GetOuter())
			MyChildObjects.AddUnique(Obj);
	}
	return MyChildObjects;
}

TArray<UMyObjectTreeNode*> UMyObjectTreeNode::GetChildren() const
{
	// get all sub objects
	TArray<UObject*> MyChildObjects = GetChildObjects();

	// create nodes for sub objects
	TArray<UMyObjectTreeNode*> Children;
	for (UObject* Obj : MyChildObjects)
	{
		UMyObjectTreeNode* ChildNode = NewObject<UMyObjectTreeNode>();
		ChildNode->BindObject(Obj);
		ChildNode->Depth = this->Depth + 1;
		Children.Add(ChildNode);
	}

	return Children;
}