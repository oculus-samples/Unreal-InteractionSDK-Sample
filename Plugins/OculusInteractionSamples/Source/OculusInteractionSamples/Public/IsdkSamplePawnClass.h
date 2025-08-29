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

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tools/IsdkConsoleParser.h"

#include "IsdkSamplePawnClass.generated.h"

class UEnhancedInputComponent;

UCLASS()
class OCULUSINTERACTIONSAMPLES_API AIsdkSamplePawnClass : public APawn, public IIsdkConsoleReceiver
{
  GENERATED_BODY()

 public:
  // Sets default values for this pawn's properties
  AIsdkSamplePawnClass();

  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

 protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
  bool ProcessConsoleCommand(const TArray<FString>& TextArgs, UWorld* World);

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent;
};
