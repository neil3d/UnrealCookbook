// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWildFunctionLibrary.h"


void UMyWildFunctionLibrary::MyGenericInvoke(const FString& FuncName, const FMyVarParam& InArgs)
{
	UE_LOG(LogTemp, Error, TEXT("MyGenericInvoke: %s(%f)"), *FuncName, InArgs.fValue);
}
