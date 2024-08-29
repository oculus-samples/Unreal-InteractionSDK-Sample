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

#include "Components/ShapeComponent.h"
#include "Kismet/GameplayStatics.h"

AIsdkObjectCatcher::AIsdkObjectCatcher()
{
  PrimaryActorTick.bCanEverTick = true;
  USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("CatcherRoot"));
}

void AIsdkObjectCatcher::BeginPlay()
{
  Super::BeginPlay();

  Collider = Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()));
  checkf(Collider != nullptr, TEXT("IsdkObjectCatcher requires collision shape to work properly"));

  Collider->SetGenerateOverlapEvents(true);
  Collider->OnComponentBeginOverlap.AddDynamic(this, &AIsdkObjectCatcher::BeginOverlap);

  TArray<AActor*> actors;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), actors);
  for (auto Actor : actors)
  {
    auto t = Actor->GetActorTransform();
    StartActorTransforms.Add(Actor, t);
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
  if (Cast<APawn>(OtherActor) == nullptr)
  {
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
}
