// Fill out your copyright notice in the Description page of Project Settings.

#include "MyActor.h"
#include "UnrealCookBook.h"


// Sets default values
AMyActor::AMyActor()
{
	/* <CreateObjectDemo>
	* 创建Component对象，要使用CreateDefaultSubobject模板函数
	*/
	MyComponent = CreateDefaultSubobject<UMyActorComponent>(TEXT("MyComponent"));

	/* <CreateObjectDemo>
	* 加载模型、贴图等对象，使用StaticLoadObject函数
	*/
	UStaticMesh* SM_Vase = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(),
		NULL,
		TEXT("/Game/Assets/StaticMeshes/SM_Vase"))
		);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetStaticMesh(SM_Vase);
}
