// Fill out your copyright notice in the Description page of Project Settings.


#include "ForkJoinDemo.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Math/NumericLimits.h"
#include "Async/TaskGraphInterfaces.h"	// Core
#include "Serialization/JsonReader.h"	// Json
#include "Serialization/JsonSerializer.h" // Json

int32 FStockAnalyzeContext::GetStockDataCount() const
{
	return StockData.Num();
}

float FStockAnalyzeContext::GetStockData(int32 Index) const
{
	const TSharedPtr<FJsonValue>& Element = StockData[Index];
	const TSharedPtr<FJsonObject>& Stock = Element->AsObject();
	const TSharedPtr<FJsonValue>* FieldPtr = Stock->Values.Find(TEXT("close"));

	if (!FieldPtr)
		return 0.0f;

	const TSharedPtr<FJsonValue>& Field = *FieldPtr;

	check(Field && !Field->IsNull());
	return FCString::Atof(*(Field->AsString()));
}

FAutoConsoleTaskPriority CPrio_StockTasks(
	TEXT("TaskGraph.TaskPriorities.StockTasks"),
	TEXT("Task and thread priority for stock analyzation."),
	ENamedThreads::HighThreadPriority,
	ENamedThreads::NormalTaskPriority,
	ENamedThreads::HighTaskPriority
);

class FTaskCompletion_StockAnalyze
{
	FStockAnalyzeContext* Context;

public:
	FTaskCompletion_StockAnalyze(FStockAnalyzeContext* InContext) : Context(InContext)
	{}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTaskCompletion_StockAnalyze, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread() { return ENamedThreads::GameThread; }

	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		check(IsInGameThread());

		Context->CompletionDelegate.ExecuteIfBound(Context->Result);
		Context->bRunning = false;
	}
};

class FTask_StockMax
{
	FStockAnalyzeContext* Context;

public:
	FTask_StockMax(FStockAnalyzeContext* InContext) : Context(InContext)
	{}

	TStatId GetStatId() const {
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTask_StockMax, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread() { return CPrio_StockTasks.Get(); }
	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		// process data 
		float Result = TNumericLimits<float>::Min();
		int32 Count = Context->GetStockDataCount();
		for (int32 i = 0; i < Count; i++)
		{
			float Value = Context->GetStockData(i);
			if (Value > Result)
				Result = Value;
		}

		// write resut to context
		Context->Result.X = Result;
	}
};


class FTask_StockMin
{
	FStockAnalyzeContext* Context;

public:
	FTask_StockMin(FStockAnalyzeContext* InContext) : Context(InContext)
	{}

	TStatId GetStatId() const {
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTask_StockMin, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread() { return CPrio_StockTasks.Get(); }
	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		// process data 
		float Result = TNumericLimits<float>::Max();
		int32 Count = Context->GetStockDataCount();
		for (int32 i = 0; i < Count; i++)
		{
			float Value = Context->GetStockData(i);
			if (Value < Result)
				Result = Value;
		}

		// write resut to context
		Context->Result.Y = Result;
	}
};


class FTask_StockAverage
{
	FStockAnalyzeContext* Context;

public:
	FTask_StockAverage(FStockAnalyzeContext* InContext) : Context(InContext)
	{}

	TStatId GetStatId() const {
		RETURN_QUICK_DECLARE_CYCLE_STAT(FTask_StockAverage, STATGROUP_TaskGraphTasks);
	}

	static ENamedThreads::Type GetDesiredThread() { return CPrio_StockTasks.Get(); }
	static ESubsequentsMode::Type GetSubsequentsMode() { return ESubsequentsMode::TrackSubsequents; }

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
	{
		// process data 
		float Result = 0;
		int32 Count = Context->GetStockDataCount();
		for (int32 i = 0; i < Count; i++)
		{
			float Value = Context->GetStockData(i);
			Result += Value;
		}

		// write resut to context
		Context->Result.Z = Result / Count;
	}
};

class FTask_LoadFileToJson
{
	FStockAnalyzeContext* Context;

public:
	FTask_LoadFileToJson(FStockAnalyzeContext* InContext) : Context(InContext)
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
		FString FilePath = Context->DataFilePath;
		FString FileContent;
		FString FullPath = FPaths::Combine(FPaths::ProjectContentDir(), FilePath);
		if (FPaths::FileExists(FullPath))
		{
			if (FFileHelper::LoadFileToString(FileContent, *FullPath))
			{
				TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(FileContent);
				FJsonSerializer::Deserialize(Reader, JsonObject);
			}
		}

		// write resut to context
		if (JsonObject)
			Context->StockData = JsonObject->GetArrayField(TEXT("stock"));
	}
};

// Sets default values
AForkJoinDemo::AForkJoinDemo()
{

}

void AForkJoinDemo::AsyncAnalyzeStockData(const FString& FilePath)
{
	if (TaskContext.bRunning)
		return;

	FTaskDelegate_StockAnalyzeComplete CompletionDelegate;
	CompletionDelegate.BindUFunction(this, "OnAnalyzeComplete");

	TaskContext = {};
	TaskContext.bRunning = true;
	TaskContext.CompletionDelegate = CompletionDelegate;
	TaskContext.DataFilePath = FilePath;

	FGraphEventRef LoadJson = TGraphTask<FTask_LoadFileToJson>::CreateTask().
		ConstructAndDispatchWhenReady(&TaskContext);

	// data process tasks
	FGraphEventArray RootTasks = { LoadJson };
	FGraphEventRef CalMax = TGraphTask<FTask_StockMax>::CreateTask(&RootTasks, ENamedThreads::AnyThread).
		ConstructAndDispatchWhenReady(&TaskContext);

	FGraphEventRef CalMin = TGraphTask<FTask_StockMin>::CreateTask(&RootTasks, ENamedThreads::AnyThread).
		ConstructAndDispatchWhenReady(&TaskContext);

	FGraphEventRef CalAverage = TGraphTask<FTask_StockAverage>::CreateTask(&RootTasks, ENamedThreads::AnyThread).
		ConstructAndDispatchWhenReady(&TaskContext);

	// compeletion
	FGraphEventArray CalTasks = { CalMax, CalMin, CalAverage };
	TGraphTask<FTaskCompletion_StockAnalyze>::CreateTask(&CalTasks, ENamedThreads::AnyThread).
		ConstructAndDispatchWhenReady(&TaskContext);

}