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
class MYOBJECTEXPLORER_API UMyObjectExplorerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = MyObjectExplorer)
		void ShowObject(const FString& szObjectPath);

	UFUNCTION(BlueprintImplementableEvent, Category = MyObjectExplorer, meta = (DisplayName = "OnShowObject"))
		void Event_ShowObject();

protected:
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = MyObjectExplorer)
		UMyObjectTreeNode* RootNode;
};
