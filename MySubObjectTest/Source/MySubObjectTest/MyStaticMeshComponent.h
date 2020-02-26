// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MySubObject.h"
#include "Components/StaticMeshComponent.h"
#include "MyStaticMeshComponent.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class MYSUBOBJECTTEST_API UMyStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_UCLASS_BODY()
	
public:
	UMyStaticMeshComponent();

	virtual TStructOnScope<FActorComponentInstanceData> GetComponentInstanceData() const override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

		UPROPERTY(VisibleAnywhere, Instanced, BlueprintReadOnly,meta = (ShowOnlyInnerProperties))
		UMySubObject* MySubObject;
};


/** 关卡编辑时：
 1. 蓝图对象属性改变会触发蓝图自动重新编译、蓝图对象重建
 2. 在重建时，需要对子对象属性做特殊处理
 3. 所谓的特殊处理，主要是把老的子对象属性，复制到新的子对象中
*/
USTRUCT()
struct FMyComponentInstanceData : public FActorComponentInstanceData
{
	GENERATED_BODY()
public:
	FMyComponentInstanceData()
		: MySubObject(nullptr)
	{}
	FMyComponentInstanceData(const UActorComponent* InSourceComponent);
	virtual ~FMyComponentInstanceData() = default;

	virtual bool ContainsData() const override
	{
		return (MySubObject != nullptr) || Super::ContainsData();
	}

	virtual void ApplyToComponent(UActorComponent* Component, const ECacheApplyPhase CacheApplyPhase) override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	UPROPERTY()
		UMySubObject* MySubObject;
};