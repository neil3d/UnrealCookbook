// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Http.h"	// HTTP
#include "BlueprintAsyncHttpRequest.generated.h"

// UMulticastDelegateProperty
// 只能支持一个 Delegate Signature
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHttpResponseDelegatge, int32, Code, FString, Data);

/**
 * 
 */
UCLASS()
class BLUEPRINTASYNC_API UBlueprintAsyncHttpRequest : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	// Factory Method
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
		static UBlueprintAsyncHttpRequest* HttpRequest(const FString& URL);

	UPROPERTY(BlueprintAssignable)
		FHttpResponseDelegatge OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FHttpResponseDelegatge OnFail;

private:
	void OnHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void SendRequest(const FString& URL);
};
