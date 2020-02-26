// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStaticMeshComponent.h"
#include "Serialization/ObjectWriter.h"
#include "Serialization/ObjectReader.h"


UMyStaticMeshComponent::UMyStaticMeshComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MySubObject = ObjectInitializer.CreateDefaultSubobject<UMySubObject>(this, "MyProperty");
}

#if WITH_EDITOR

void UMyStaticMeshComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UProperty* ChangedProperty = PropertyChangedEvent.Property;
	if (ChangedProperty
		&& ChangedProperty->GetName() == GET_MEMBER_NAME_STRING_CHECKED(UMyStaticMeshComponent, MySubObject))
	{
		// do something here
		UE_LOG(LogTemp, Log, TEXT("UMyStaticMeshComponent.MySubObject changed"));
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);

}

#endif // WITH_EDITOR

TStructOnScope<FActorComponentInstanceData> UMyStaticMeshComponent::GetComponentInstanceData() const
{
	return MakeStructOnScope<FActorComponentInstanceData, FMyComponentInstanceData>(this);
}

FMyComponentInstanceData::FMyComponentInstanceData(const UActorComponent* InSourceComponent)
	: FActorComponentInstanceData(InSourceComponent)
{
	const UMyStaticMeshComponent* OldComponent = CastChecked<UMyStaticMeshComponent>(InSourceComponent); ;
	MySubObject = OldComponent->MySubObject;
}

void FMyComponentInstanceData::ApplyToComponent(UActorComponent* Component, const ECacheApplyPhase CacheApplyPhase)
{
	FActorComponentInstanceData::ApplyToComponent(Component, CacheApplyPhase);

	UMyStaticMeshComponent* NewComponent = CastChecked<UMyStaticMeshComponent>(Component);
	if (NewComponent && NewComponent->MySubObject)
	{
		TArray<uint8> SavedPropertyBuffer;
		FObjectWriter Writer(MySubObject, SavedPropertyBuffer);
		FObjectReader Reader(NewComponent->MySubObject, SavedPropertyBuffer);
	}
}

void FMyComponentInstanceData::AddReferencedObjects(FReferenceCollector& Collector)
{
	FActorComponentInstanceData::AddReferencedObjects(Collector);
	if (MySubObject)
	{
		Collector.AddReferencedObject(MySubObject);
	}
}


