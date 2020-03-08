// Fill out your copyright notice in the Description page of Project Settings.


#include "ForkJoinDemo.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Async/TaskGraphInterfaces.h"	// Core
#include "Serialization/JsonReader.h"	// Json
#include "Serialization/JsonSerializer.h" // Json

FAutoConsoleTaskPriority CPrio_StockTasks(
	TEXT("TaskGraph.TaskPriorities.StockTasks"),
	TEXT("Task and thread priority for stock analyzation."),
	ENamedThreads::HighThreadPriority,
	ENamedThreads::NormalTaskPriority,
	ENamedThreads::HighTaskPriority
);

class FTask_LoadFileToJson
{
	FStockAnalyzeContext* Context;
	FString FilePath;

public:
	FTask_LoadFileToJson(FString InFilePath, FStockAnalyzeContext* InContext) :
		Context(InContext), FilePath(MoveTemp(InFilePath))
	{}

	TStatId GetStatId() const {
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTask_LoadFileToJson, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread() { return CPrio_StockTasks.Get(); }
	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		TSharedPtr<FJsonObject> JsonObject;

		// load file from Content folder
		FString FileContent;
		FString FullPath = FPaths::Combine(FPaths::ProjectContentDir(), FilePath);
		if (FPaths::FileExists(FullPath))
		{
			if (FFileHelper::LoadFileToString(FileContent, *FullPath))
			{
				TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(FileContent);
				if (!FJsonSerializer::Deserialize(Reader, JsonObject))
				{
					UE_LOG(LogTemp, Error, TEXT("Json Parse FAILED!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Json File Load FAILED!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Json File NOT Exists!"));
		}

		// write resut to context
		if (JsonObject)
			Context->StockData = JsonObject;
		else
			Context->StockData = MakeShared<FJsonObject>();	// make a empty json object
	}
};

// Sets default values
AForkJoinDemo::AForkJoinDemo()
{

}

void AForkJoinDemo::AsyncAnalyzeStockData(const FString& FilePath)
{
	TGraphTask<FTask_LoadFileToJson>::CreateTask().
		ConstructAndDispatchWhenReady(FilePath, &TaskContext);

}