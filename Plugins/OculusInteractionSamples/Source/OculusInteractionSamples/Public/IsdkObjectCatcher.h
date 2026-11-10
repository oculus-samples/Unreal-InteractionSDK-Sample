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

#include "IsdkObjectCatcher.generated.h"

class UBoxComponent;

UCLASS(ClassGroup = (InteractionSDK), meta = (DisplayName = "ISDK Level Object Catcher"))
class OCULUSINTERACTIONSAMPLES_API AIsdkObjectCatcher : public AActor
{
  GENERATED_BODY()

 public:
  AIsdkObjectCatcher();

  /**
   * Registers an actor so that, when it overlaps this catcher, it is reset to
   * the transform it has at the moment of registration (its "home" pose).
   *
   * Trackable actors (those that simulate physics or carry a grabbable
   * component) present at BeginPlay, and any spawned afterwards, are registered
   * automatically. Call this explicitly from a grabbable's BeginPlay to register
   * objects that are spawned at runtime or that do not simulate physics
   * themselves.
   *
   * @param ActorToRegister Actor whose current transform becomes its home pose.
   *                        Ignored if null.
   */
  UFUNCTION(BlueprintCallable, Category = "InteractionSDK")
  void RegisterActor(AActor* ActorToRegister);

  /**
   * Removes a previously registered actor so that it is no longer reset when it
   * overlaps this catcher. Call from a grabbable's EndPlay to pair with a
   * RegisterActor call in BeginPlay.
   *
   * @param ActorToDeregister Actor to stop tracking. Ignored if null or not
   *                          currently registered.
   */
  UFUNCTION(BlueprintCallable, Category = "InteractionSDK")
  void DeregisterActor(AActor* ActorToDeregister);

 protected:
  virtual void BeginPlay() override;
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  UPROPERTY()
  TMap<TObjectPtr<AActor>, FTransform> StartActorTransforms;

  UPROPERTY()
  TObjectPtr<UBoxComponent> Collider;

  UFUNCTION(BlueprintInternalUseOnly, Category = InteractionSDK)
  void BeginOverlap(
      UPrimitiveComponent* OverlappedComponent,
      AActor* OtherActor,
      UPrimitiveComponent* OtherComp,
      int32 OtherBodyIndex,
      bool bFromSweep,
      const FHitResult& SweepResult);

 private:
  // Auto-registers trackable actors (see ShouldTrackActor) that spawn after
  // BeginPlay, which would otherwise be absent from the start-of-play snapshot.
  void OnActorSpawned(AActor* SpawnedActor);

  // Evicts an actor from StartActorTransforms when it is destroyed, releasing
  // the strong reference the map would otherwise hold.
  UFUNCTION()
  void OnRegisteredActorDestroyed(AActor* DestroyedActor);

  // True if the actor should be tracked by the catcher: it is not a pawn and
  // either simulates physics now or carries a grabbable component (which toggles
  // physics simulation at runtime, so it may not be simulating at BeginPlay).
  static bool ShouldTrackActor(const AActor* Actor);

  // Handle to the world's actor-spawned callback, released in EndPlay.
  FDelegateHandle ActorSpawnedHandle;
};
