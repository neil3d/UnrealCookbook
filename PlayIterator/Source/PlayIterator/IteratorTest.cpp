// Fill out your copyright notice in the Description page of Project Settings.


#include "IteratorTest.h"
#include "EngineUtils.h"	// Engine
#include "Engine/StaticMeshActor.h"	// Engine

void AIteratorTest::BeginPlay() {
	Super::BeginPlay();

	ArrayTest();
	MapTest();
}

void AIteratorTest::ArrayTest() {

	TArray<int> MyArray;
	for (int i = 0; i < 16; i++) {
		MyArray.Add(i);
	}

	MyArray.RemoveAll([](int item) {
		return item % 2;
	});

}

void AIteratorTest::MapTest() {
	TMap<FName, AActor*>	MyActors;

	// 填充测试数据
	for (TActorIterator<AActor> Iter(GetWorld()); Iter; ++Iter) {
		AActor* Actor = *Iter;
		MyActors.Add(Actor->GetFName(), Actor);
	}

	UE_LOG(LogTemp, Warning, _T("AIteratorTest: actor map size = %d."), MyActors.Num());

	// 删除指定对象
	for (TMap<FName, AActor*>::TIterator Iter(MyActors); Iter; ++Iter) {
		FName ActorName = Iter.Key();
		AActor* Actor = Iter.Value();

		if (Cast<AStaticMeshActor>(Actor)) {
			UE_LOG(LogTemp, Warning, _T("AIteratorTest: remove [%s]."), *GetDebugName(Actor));
			Actor->Destroy();
			Iter.RemoveCurrent();	// 调用 TIterator 的方法
		}
	}// end of for

	UE_LOG(LogTemp, Warning, _T("AIteratorTest: actor map size after remove = %d."), MyActors.Num());
}
