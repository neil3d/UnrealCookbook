// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWildFunctionLibrary.h"


void UMyWildFunctionLibrary::MyGenericInvoke(const FString& FuncName, const FMyVarParam& InArgs)
{
	FString Param;
	if (InArgs.TypeName == FName("float"))
		Param = FString::Printf(TEXT("float:%f"), InArgs.fValue);
	else if (InArgs.TypeName == FName("string"))
		Param = FString::Printf(TEXT("string:%s"), *(InArgs.szValue));
	else
		Param = TEXT("unknown");

	UE_LOG(LogTemp, Error, TEXT("MyGenericInvoke: %s(%s)"), *FuncName, *Param);
}
