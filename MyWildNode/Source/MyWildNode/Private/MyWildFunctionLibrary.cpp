// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWildFunctionLibrary.h"


void UMyWildFunctionLibrary::MyGenericInvoke_P1_Impl(const void* StructAddr, const FStructProperty* StructProperty)
{
	UE_LOG(LogTemp, Error, TEXT("MyGenericInvoke_P1_Impl"));
}
