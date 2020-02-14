// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintAsyncHttpRequest.h"

UBlueprintAsyncHttpRequest* UBlueprintAsyncHttpRequest::HttpRequest(const FString& URL)
{
	UBlueprintAsyncHttpRequest* NewRequest = NewObject<UBlueprintAsyncHttpRequest>();
	NewRequest->SendRequest(URL);
	return NewRequest;
}

void UBlueprintAsyncHttpRequest::SendRequest(const FString& URL)
{
	AddToRoot();

	FHttpModule& HttpModule = FHttpModule::Get();

	TSharedRef<IHttpRequest> Request = HttpModule.CreateRequest();
	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindUObject(this, &UBlueprintAsyncHttpRequest::OnHttpResponse);
	Request->ProcessRequest();
}

void UBlueprintAsyncHttpRequest::OnHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		OnSuccess.Broadcast(
			Response->GetResponseCode(),
			Response->GetContentAsString()
		);
	}
	else
	{
		OnFail.Broadcast(-1, TEXT(""));
	}

	RemoveFromRoot();
}
