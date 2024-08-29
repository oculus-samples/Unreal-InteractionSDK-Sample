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

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "IsdkResetTransforms.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OCULUSINTERACTIONSAMPLES_API UIsdkResetTransforms : public UActorComponent
{
  GENERATED_BODY()

 public:
  UIsdkResetTransforms();

 protected:
  virtual void BeginPlay() override;

 public:
  virtual void TickComponent(
      float DeltaTime,
      ELevelTick TickType,
      FActorComponentTickFunction* ThisTickFunction) override;
  UFUNCTION(BlueprintCallable, Category = "ISDK Reset Transforms")
  void ResetTransforms();

  UPROPERTY(EditAnywhere, Category = "ISDK Reset Transforms")
  TArray<AActor*> ActorsToReset;

 private:
  TArray<FTransform> InitialTransforms;
};
