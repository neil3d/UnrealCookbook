// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "MyFunctionalTestActor.generated.h"

/**
 *
 */
UCLASS()
class CPPUNITTEST_API AMyFunctionalTestActor : public AFunctionalTest {
  GENERATED_BODY()

 public:
  virtual void StartTest() override {
    AFunctionalTest::StartTest();

    AssertIsValid(this, TEXT("My first functional test"));
    AssertValue_Int(1 + 1, EComparisonMethod::Equal_To, 2,
                    TEXT("My 2nd functional test"));
    AssertValue_Int(1 + 1, EComparisonMethod::Not_Equal_To, 2,
                    TEXT("My 3rd functional test"));
  }
};
