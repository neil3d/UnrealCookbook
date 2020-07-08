#include "CoreTypes.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMySimpleTest, "UnrealCookbook.MySimpleTest",
                                 EAutomationTestFlags::ApplicationContextMask |
                                     EAutomationTestFlags::SmokeFilter)

bool FMySimpleTest::RunTest(const FString& Parameters) {
  TestEqual(TEXT("My first test case"), 2, 1 + 1);

  TestEqual(TEXT("My second test case"), 2, 2 + 1);

  UE_LOG(LogTemp, Error, TEXT("log error test"));
  return true;
}

#endif  // WITH_DEV_AUTOMATION_TESTS
