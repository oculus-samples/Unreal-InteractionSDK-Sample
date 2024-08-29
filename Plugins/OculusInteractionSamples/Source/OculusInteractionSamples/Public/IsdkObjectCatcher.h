﻿/*
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

#include "IsdkObjectCatcher.generated.h"

UCLASS(ClassGroup = (InteractionSDK), meta = (DisplayName = "ISDK Level Object Catcher"))
class OCULUSINTERACTIONSAMPLES_API AIsdkObjectCatcher : public AActor
{
  GENERATED_BODY()
 public:
  AIsdkObjectCatcher();

 protected:
  virtual void BeginPlay() override;
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  TMap<TObjectPtr<AActor>, FTransform> StartActorTransforms;

  UPROPERTY()
  TObjectPtr<UPrimitiveComponent> Collider{nullptr};

  UFUNCTION(BlueprintInternalUseOnly, Category = InteractionSDK)
  void BeginOverlap(
      UPrimitiveComponent* OverlappedComponent,
      AActor* OtherActor,
      UPrimitiveComponent* OtherComp,
      int32 OtherBodyIndex,
      bool bFromSweep,
      const FHitResult& SweepResult);
};
