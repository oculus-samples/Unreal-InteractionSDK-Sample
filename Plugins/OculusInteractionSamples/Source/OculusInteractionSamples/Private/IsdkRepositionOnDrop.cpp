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

#include "IsdkRepositionOnDrop.h"

UIsdkRepositionOnDrop::UIsdkRepositionOnDrop() {}

void UIsdkRepositionOnDrop::SetTransformTarget(USceneComponent* Target)
{
  if (IsValid(TransformTarget))
  {
    TransformTarget->TransformUpdated.Remove(TransformUpdatedHandle);
    TransformUpdatedHandle = FDelegateHandle();
  }
  TransformTarget = Target;
  if (IsValid(TransformTarget))
  {
    ResetWorldTransform = TransformTarget->GetComponentTransform();
    TransformUpdatedHandle = TransformTarget->TransformUpdated.AddUObject(
        this, &UIsdkRepositionOnDrop::HandleTransformUpdated);
  }
}

void UIsdkRepositionOnDrop::HandleTransformUpdated(
    USceneComponent* UpdatedComponent,
    EUpdateTransformFlags UpdateTransformFlags,
    ETeleportType TeleportType)
{
  if (!IsValid(TransformTarget) || !bActive)
  {
    return;
  }
  if (TransformTarget->GetComponentLocation().Z < FloorHeight)
  {
    if (ResetWorldTransform.GetLocation().Z < FloorHeight)
    {
      FVector ResetLocation = ResetWorldTransform.GetLocation();
      ResetLocation.Z = FloorHeight + 0.0001;
      ResetWorldTransform.SetLocation(ResetLocation);
    }

    FTransform Reset = ResetWorldTransform;
    TObjectPtr<USceneComponent> Target = TransformTarget;
    FTimerHandle TimerHandler;
    // The transform update is delayed because it does not work well with a transform driven by a
    // grabbable otherwise, for some reason the scale is not updated
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandler,
        [Target, Reset]()
        {
          if (!IsValid(Target))
            return;
          Target->SetWorldTransform(Reset);
          auto* Primitive = Cast<UPrimitiveComponent>(Target);
          if (!IsValid(Primitive))
            return;
          if (!Primitive->IsSimulatingPhysics())
            return;
          Primitive->SetPhysicsLinearVelocity(FVector::Zero());
          Primitive->SetAllPhysicsAngularVelocityInDegrees(FVector::Zero());
        },
        0.001f,
        false);

    RepositionDelegate.Broadcast(this);
  }
}
