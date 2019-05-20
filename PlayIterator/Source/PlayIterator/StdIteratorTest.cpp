// Fill out your copyright notice in the Description page of Project Settings.

#include "StdIteratorTest.h"
#include <vector>

void AStdIteratorTest::BeginPlay()
{
	Super::BeginPlay();

	TestStdVector();
}

void AStdIteratorTest::TestStdVector() {

	std::vector<int> MyVector;

	// 填充测试数据
	for (int i = 0; i < 16; i++) {
		MyVector.push_back(i);
	}

	UE_LOG(LogTemp, Warning, _T("AStdIteratorTest: vector size = %d."), MyVector.size());

	// 在循环中删除vector中的元素
	// std标准容器的操作方法都是类似的
	for (auto iter = MyVector.begin(); iter != MyVector.end();) {
		int num = *iter;
		if (num % 2)
			iter = MyVector.erase(iter);	// std::vector::erase() 的返回值就是干这个用的
		else
			++iter;
	}

	UE_LOG(LogTemp, Warning, _T("AStdIteratorTest: vector size after delete = %d."), MyVector.size());

}