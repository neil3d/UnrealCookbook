// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyObjectTreeNode.h"
#include "MyObjectExplorerWidget.generated.h"

/**
 * see: https://www.unrealengine.com/en-US/tech-blog/umg-best-practices
 */
UCLASS()
class MYOBJECTEXPLOREREDITOR_API UMyObjectExplorerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowObjects(const TArray<UObject*>& InObjects);

	UFUNCTION(BlueprintImplementableEvent, Category = MyObjectExplorer, meta = (DisplayName = "ShowObjects"))
		void Event_ShowObjects();

protected:
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = MyObjectExplorer)
		TArray<UMyObjectTreeNode*> RootNodes;
};
