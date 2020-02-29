// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstAsyncTask.h"
#include "MyAsyncTasks.h"

using namespace neil3d;

// Sets default values
AFirstAsyncTask::AFirstAsyncTask()
{

}

void AFirstAsyncTask::AsyncLoadTextFile(const FString& FilePath)
{
	FTaskDelegate_FileLoaded TaskDelegate;
	TaskDelegate.BindUFunction(this, "OnFileLoaded");
	
	FGraphEventRef NewTask = TGraphTask<FTask_LoadFileToString>::CreateTask(nullptr, ENamedThreads::GameThread).
		ConstructAndDispatchWhenReady(FilePath, TaskDelegate);
}

