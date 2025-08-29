/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 * All rights reserved.
 *
 * Licensed under the Oculus SDK License Agreement (the "License");
 * you may not use the Oculus SDK except in compliance with the License,
 * which is provided at the time of installation or download, or which
 * otherwise accompanies this software in either electronic or hard copy form.
 *
 * You may obtain a copy of the License at
 *
 * https://developer.oculus.com/licenses/oculussdk/
 *
 * Unless required by applicable law or agreed to in writing, the Oculus SDK
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "IsdkSamplePawnClass.h"

#include "EnhancedInputComponent.h"

// Sets default values
AIsdkSamplePawnClass::AIsdkSamplePawnClass()
{
  // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you
  // don't need it.
  PrimaryActorTick.bCanEverTick = true;

  EnhancedInputComponent =
      CreateDefaultSubobject<UEnhancedInputComponent>("EnhancedInputComponent");
  InputComponent = EnhancedInputComponent;
}

// Called when the game starts or when spawned
void AIsdkSamplePawnClass::BeginPlay()
{
  Super::BeginPlay();
  UIsdkConsoleParser::Init();
  UIsdkConsoleParser::RegisterListener(this);
}

void AIsdkSamplePawnClass::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  UIsdkConsoleParser::UnregisterListener(this);
  Super::EndPlay(EndPlayReason);
}

// Called every frame
void AIsdkSamplePawnClass::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AIsdkSamplePawnClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool AIsdkSamplePawnClass::ProcessConsoleCommand(const TArray<FString>& TextArgs, UWorld* World)
{
  if (TextArgs.Num() > 0 && TextArgs[0].ToLower() == TEXT("soundoff"))
  {
    UE_LOG(LogTemp, Log, TEXT("%s"), *GetName());
    return true;
  }
  return false;
}
