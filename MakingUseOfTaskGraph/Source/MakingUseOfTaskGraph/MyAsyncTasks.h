// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Async/TaskGraphInterfaces.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

namespace neil3d
{
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

		static ENamedThreads::Type GetDesiredThread() { return ENamedThreads::AnyThread; }

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
			FGraphEventRef ChildTask = TGraphTask<FTaskCompletion_LoadFileToString>::CreateTask(nullptr, ENamedThreads::GameThread).
				ConstructAndDispatchWhenReady(TaskDelegate, FileContent);
			MyCompletionGraphEvent->DontCompleteUntil(ChildTask);
		}
	};

}// end of namespace