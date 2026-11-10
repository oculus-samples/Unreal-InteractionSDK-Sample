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
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Interaction/IsdkGrabbableComponent.h"
#include "Kismet/GameplayStatics.h"

AIsdkObjectCatcher::AIsdkObjectCatcher()
{
  PrimaryActorTick.bCanEverTick = true;
  Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
  Collider->SetGenerateOverlapEvents(true);
}

void AIsdkObjectCatcher::BeginPlay()
{
  Super::BeginPlay();

  if (IsValid(Collider))
  {
    Collider->OnComponentBeginOverlap.AddDynamic(this, &AIsdkObjectCatcher::BeginOverlap);
  }

  // Snapshot the home pose of every trackable actor present at play start.
  // Actors that neither simulate physics nor carry a grabbable component (level
  // geometry, lights, pawns, etc.) never need to be caught, so they are skipped.
  TArray<AActor*> Actors;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
  for (AActor* Actor : Actors)
  {
    if (ShouldTrackActor(Actor))
    {
      RegisterActor(Actor);
    }
  }

  // Trackable actors spawned after BeginPlay are missing from the snapshot
  // above; record their home pose as they appear so they can still be caught.
  if (UWorld* World = GetWorld())
  {
    ActorSpawnedHandle = World->AddOnActorSpawnedHandler(
        FOnActorSpawned::FDelegate::CreateUObject(this, &AIsdkObjectCatcher::OnActorSpawned));
  }
}

void AIsdkObjectCatcher::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  if (UWorld* World = GetWorld())
  {
    World->RemoveOnActorSpawnedHandler(ActorSpawnedHandle);
  }

  if (Collider != nullptr)
  {
    Collider->OnComponentBeginOverlap.RemoveDynamic(this, &AIsdkObjectCatcher::BeginOverlap);
    Collider = nullptr;
  }

  Super::EndPlay(EndPlayReason);
}

void AIsdkObjectCatcher::RegisterActor(AActor* ActorToRegister)
{
  if (ActorToRegister == nullptr)
  {
    return;
  }

  // Evict the actor from the map when it is destroyed; the strong reference held
  // by StartActorTransforms would otherwise keep it alive, and a reused actor
  // address could later match a stale entry. Guarded so re-registering an actor
  // does not bind the handler twice.
  if (!StartActorTransforms.Contains(ActorToRegister))
  {
    ActorToRegister->OnDestroyed.AddDynamic(this, &AIsdkObjectCatcher::OnRegisteredActorDestroyed);
  }

  // TMap::Add overwrites an existing entry, so re-registering an actor simply
  // resets its home pose to the current transform (idempotent).
  StartActorTransforms.Add(ActorToRegister, ActorToRegister->GetActorTransform());
}

void AIsdkObjectCatcher::DeregisterActor(AActor* ActorToDeregister)
{
  if (ActorToDeregister == nullptr)
  {
    return;
  }

  ActorToDeregister->OnDestroyed.RemoveDynamic(
      this, &AIsdkObjectCatcher::OnRegisteredActorDestroyed);
  StartActorTransforms.Remove(ActorToDeregister);
}

void AIsdkObjectCatcher::OnRegisteredActorDestroyed(AActor* DestroyedActor)
{
  StartActorTransforms.Remove(DestroyedActor);
}

void AIsdkObjectCatcher::OnActorSpawned(AActor* SpawnedActor)
{
  if (ShouldTrackActor(SpawnedActor))
  {
    RegisterActor(SpawnedActor);
  }
}

bool AIsdkObjectCatcher::ShouldTrackActor(const AActor* Actor)
{
  if (Actor == nullptr)
  {
    return false;
  }

  // Pawns are repositioned by their controllers and are explicitly ignored by
  // BeginOverlap, so there is never a reason to track them.
  if (Actor->IsA<APawn>())
  {
    return false;
  }

  // A grabbable toggles physics simulation on/off at runtime, so it may not be
  // simulating at BeginPlay; track it regardless of its current physics state.
  if (Actor->FindComponentByClass<UIsdkGrabbableComponent>() != nullptr)
  {
    return true;
  }

  TArray<UPrimitiveComponent*> PrimitiveComponents;
  Actor->GetComponents<UPrimitiveComponent>(PrimitiveComponents);
  for (const UPrimitiveComponent* Component : PrimitiveComponents)
  {
    if (Component != nullptr && Component->IsSimulatingPhysics())
    {
      return true;
    }
  }

  return false;
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
  if (!IsValid(OtherActor) || Cast<APawn>(OtherActor))
  {
    return;
  }

  const FTransform* Transform = StartActorTransforms.Find(OtherActor);
  if (Transform == nullptr)
  {
    // Actor is not tracked (it neither simulates physics nor carries a grabbable
    // component, and was never registered manually), so we have no home
    // transform to reset it to.
    return;
  }

  OtherActor->GetRootComponent()->ComponentVelocity.Set(0, 0, 0);
  OtherActor->SetActorTransform(*Transform);
  if (const auto PhysicsComponent = Cast<UPrimitiveComponent>(
          OtherActor->GetComponentByClass(UPrimitiveComponent::StaticClass())))
  {
    PhysicsComponent->SetPhysicsLinearVelocity(FVector::Zero());
    PhysicsComponent->SetAllPhysicsAngularVelocityInDegrees(FVector::Zero());
  }
}
