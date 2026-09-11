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

#include "IsdkResetTransforms.h"

// Sets default values for this component's properties
UIsdkResetTransforms::UIsdkResetTransforms()
{
  PrimaryComponentTick.bCanEverTick = true;
}

void UIsdkResetTransforms::BeginPlay()
{
  Super::BeginPlay();

  InitialTransforms.Reserve(ActorsToReset.Num());
  for (AActor* Actor : ActorsToReset)
  {
    if (IsValid(Actor))
    {
      InitialTransforms.Emplace(Actor->GetActorTransform());
    }
  }
}

void UIsdkResetTransforms::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UIsdkResetTransforms::ResetTransforms()
{
  const int32 NumActors = ActorsToReset.Num();
  for (int32 i = 0; i < NumActors; i++)
  {
    if (IsValid(ActorsToReset[i]))
    {
      AActor* Actor = ActorsToReset[i];
      UPrimitiveComponent* PrimitiveComponent =
          Cast<UPrimitiveComponent>(Actor->GetRootComponent());

      if (PrimitiveComponent && PrimitiveComponent->IsSimulatingPhysics())
      {
        PrimitiveComponent->SetSimulatePhysics(false);
        PrimitiveComponent->PutRigidBodyToSleep();
      }

      if (InitialTransforms[i].IsValid())
      {
        Actor->SetActorTransform(InitialTransforms[i]);
      }

      if (PrimitiveComponent)
      {
        PrimitiveComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
        PrimitiveComponent->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
        PrimitiveComponent->SetSimulatePhysics(true);
        PrimitiveComponent->WakeRigidBody();
      }
    }
  }
}
