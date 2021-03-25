// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWildFunctionLibrary.h"


void UMyWildFunctionLibrary::MyGenericInvoke(const FString& FuncName, const TArray<FMyVarParam>& InArgs)
{
	FString Param;
	for (const auto& Arg : InArgs)
	{
		if (Arg.TypeName == FName("float"))
			Param += FString::Printf(TEXT("float:%f,"), Arg.fValue);
		else if (Arg.TypeName == FName("string"))
			Param += FString::Printf(TEXT("string:%s,"), *(Arg.szValue));
		else
			Param += TEXT("unknown,");
	}

	UE_LOG(LogTemp, Error, TEXT("MyGenericInvoke: %s(%s)"), *FuncName, *Param);
}
