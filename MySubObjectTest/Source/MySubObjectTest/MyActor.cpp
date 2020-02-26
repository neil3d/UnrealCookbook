// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

// Sets default values
AMyActor::AMyActor()
{
	MySubObject = CreateDefaultSubobject<UMySubObject>("MySubObject");
}

