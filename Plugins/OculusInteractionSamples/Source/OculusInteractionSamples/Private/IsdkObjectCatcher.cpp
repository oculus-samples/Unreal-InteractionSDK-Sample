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

#include "IsdkObjectCatcher.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AIsdkObjectCatcher::AIsdkObjectCatcher()
{
  PrimaryActorTick.bCanEverTick = true;
  Collider = CreateDefaultSubobject<UBoxComponent>("Collider");
  Collider->SetGenerateOverlapEvents(true);
}

void AIsdkObjectCatcher::BeginPlay()
{
  Super::BeginPlay();

  Collider->OnComponentBeginOverlap.AddDynamic(this, &AIsdkObjectCatcher::BeginOverlap);

  TArray<AActor*> Actors;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
  for (auto Actor : Actors)
  {
    auto Transform = Actor->GetActorTransform();
    StartActorTransforms.Add(Actor, Transform);
  }
}

void AIsdkObjectCatcher::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);

  if (Collider != nullptr)
  {
    Collider->OnComponentBeginOverlap.RemoveDynamic(this, &AIsdkObjectCatcher::BeginOverlap);
    Collider = nullptr;
  }
}

void AIsdkObjectCatcher::BeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
  // don't want to reset the location of the pawn!
  if (!OtherActor || Cast<APawn>(OtherActor))
  {
    return;
  }

  OtherActor->GetRootComponent()->ComponentVelocity.Set(0, 0, 0);
  const auto Transform = StartActorTransforms.Find(OtherActor);
  OtherActor->SetActorTransform(*Transform);
  if (const auto PhysicsComponent = Cast<UPrimitiveComponent>(
          OtherActor->GetComponentByClass(UPrimitiveComponent::StaticClass())))
  {
    PhysicsComponent->SetPhysicsLinearVelocity(FVector::Zero());
    PhysicsComponent->SetAllPhysicsAngularVelocityInDegrees(FVector::Zero());
  }
}
