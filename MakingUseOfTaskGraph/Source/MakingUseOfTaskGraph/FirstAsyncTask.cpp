// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstAsyncTask.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
DECLARE_DELEGATE_OneParam(FTaskDelegate_FileLoaded, FString);

class FTaskCompletion_LoadFileToString
{
	FTaskDelegate_FileLoaded TaskDelegate;
	FString FileContent;
public:
	FTaskCompletion_LoadFileToString(FTaskDelegate_FileLoaded InTaskDelegate, FString InFileContent) :
		TaskDelegate(InTaskDelegate), FileContent(InFileContent)
	{}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTaskCompletion_LoadFileToString, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread() { return ENamedThreads::GameThread; }

	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		check(IsInGameThread());

		TaskDelegate.ExecuteIfBound(MoveTemp(FileContent));
	}
};

FAutoConsoleTaskPriority CPrio_LoadFileToString(
	TEXT("TaskGraph.TaskPriorities.LoadFileToString"),
	TEXT("Task and thread priority for file loading."),
	ENamedThreads::HighThreadPriority,
	ENamedThreads::NormalTaskPriority,
	ENamedThreads::HighTaskPriority 
);

class FTask_LoadFileToString
{
	FTaskDelegate_FileLoaded TaskDelegate;
	FString FilePath;

	FString FileContent;
public:
	FTask_LoadFileToString(FString InFilePath, FTaskDelegate_FileLoaded InTaskDelegate) :
		TaskDelegate(InTaskDelegate), FilePath(MoveTemp(InFilePath))
	{}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTask_LoadFileToString, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread() { return CPrio_LoadFileToString.Get(); }

	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }


	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		// load file from Content folder
		FString FullPath = FPaths::Combine(FPaths::ProjectContentDir(), FilePath);
		if (FPaths::FileExists(FullPath))
		{
			FFileHelper::LoadFileToString(FileContent, *FullPath);
		}

		// create completion task
		FGraphEventRef ChildTask = TGraphTask<FTaskCompletion_LoadFileToString>::CreateTask(nullptr, CurrentThread).
			ConstructAndDispatchWhenReady(TaskDelegate, FileContent);
		MyCompletionGraphEvent->SetGatherThreadForDontCompleteUntil(ENamedThreads::GameThread);
		MyCompletionGraphEvent->DontCompleteUntil(ChildTask);
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Sets default values
AFirstAsyncTask::AFirstAsyncTask()
{

}

void AFirstAsyncTask::AsyncLoadTextFile(const FString& FilePath)
{
	FTaskDelegate_FileLoaded TaskDelegate;
	TaskDelegate.BindUFunction(this, "OnFileLoaded");

	TGraphTask<FTask_LoadFileToString>::CreateTask().ConstructAndDispatchWhenReady(FilePath, TaskDelegate);
}

