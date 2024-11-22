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
#include "Components/ActorComponent.h"
#include "IsdkRepositionOnDrop.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FIsdkRepositionOnDropEventDelegate,
    UIsdkRepositionOnDrop*,
    RepositionOnDropPtr);

/**
 * @brief Helper object that resets the position and physics state of the target transform, when it
 * @brief falls bellow the "FloorHeight". Position is reset to the state when the target was added.
 */
UCLASS(
    Blueprintable,
    ClassGroup = (InteractionSDK),
    meta = (DisplayName = "ISDK Reposition On Drop"))
class OCULUSINTERACTIONSAMPLES_API UIsdkRepositionOnDrop : public UObject
{
  GENERATED_BODY()

 public:
  UIsdkRepositionOnDrop();
  UFUNCTION(BlueprintSetter, Category = InteractionSDK)
  void SetTransformTarget(USceneComponent* Target);
  UFUNCTION(BlueprintGetter, Category = InteractionSDK)
  USceneComponent* GetTransformTarget()
  {
    return TransformTarget;
  }

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractionSDK")
  float FloorHeight = 0.0;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractionSDK")
  bool bActive = true;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractionSDK")
  FTransform ResetWorldTransform;
  UPROPERTY(BlueprintAssignable, Category = InteractionSDK)
  FIsdkRepositionOnDropEventDelegate RepositionDelegate;

 private:
  UPROPERTY(BlueprintSetter = SetTransformTarget, Category = "InteractionSDK")
  TObjectPtr<USceneComponent> TransformTarget;
  void HandleTransformUpdated(
      USceneComponent* UpdatedComponent,
      EUpdateTransformFlags UpdateTransformFlags,
      ETeleportType TeleportType);

  FDelegateHandle TransformUpdatedHandle;
};
