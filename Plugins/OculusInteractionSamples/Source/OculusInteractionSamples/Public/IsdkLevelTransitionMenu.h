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
#include "GameFramework/Actor.h"
#include "IsdkRoundedButton.h"

#include "IsdkLevelTransitionMenu.generated.h"

UENUM(BlueprintType)
enum class EIsdkSampleLevel : uint8
{
  PokeExamples,
  RayExamples,
  TransformerExamples,
  DistanceGrabExamples,
  GazeExamples
};

USTRUCT()
struct FIsdkLevelButton
{
  GENERATED_BODY()

  size_t Index;
  FName LevelName{};
  FText ButtonLabel{};
};

UCLASS(ClassGroup = (InteractionSDK), meta = (DisplayName = "ISDK Level Transition Menu"))
class OCULUSINTERACTIONSAMPLES_API AIsdkLevelTransitionMenu : public AActor
{
  GENERATED_BODY()
 public:
  AIsdkLevelTransitionMenu();

 private:
  void CreateButtons();

  /**
   * Rebuilds the TransitionButtons cache by scanning this actor's button
   * components and matching each one to its EIsdkSampleLevel via the deterministic
   * "LevelButton_<Index>" subobject name. Safe to call repeatedly; it fully resets
   * the map first.
   */
  void RebuildTransitionButtons();

  void UpdateButtonsLayout();
  void UpdateBorderMesh();
  void UpdateVersionLabel();

  UFUNCTION()
  void HandlePointerEvent(const FIsdkInteractionPointerEvent& PointerEvent);

  UFUNCTION()
  void HandleInitialLevelLoad();

 protected:
  virtual void BeginPlay() override;

  // Cache mapping each sample level to its button component. Marked Transient and
  // rebuilt via RebuildTransitionButtons(): instanced component references stored as
  // TMap *values* are not reliably restored by the editor's undo/reinstancing system
  // (delete + undo would otherwise leave the keys present with null values, crashing
  // UpdateButtonsLayout).
  UPROPERTY(Transient)
  TMap<EIsdkSampleLevel, TObjectPtr<UIsdkRoundedButtonComponent>> TransitionButtons;

  UPROPERTY()
  TObjectPtr<UTextRenderComponent> VersionLabel;

  UPROPERTY()
  TObjectPtr<UStaticMeshComponent> BorderMesh;

  virtual void OnConstruction(const FTransform& Transform) override;
  virtual void PostInitializeComponents() override;

  bool bInitialTransition;
  void TransitionToLevel(EIsdkSampleLevel NextLevel);
  void UpdateButtonState(EIsdkSampleLevel Level);

  UFUNCTION()
  void OnLevelLoad();
  UPROPERTY(EditAnywhere, Category = "InteractionSDK")
  EIsdkSampleLevel InitialLevel;
  UPROPERTY(VisibleAnywhere, Category = "InteractionSDK")
  EIsdkSampleLevel CurrentLevel;
  UPROPERTY(VisibleAnywhere, Category = "InteractionSDK")
  EIsdkSampleLevel PreviousLevel;

 public:
  UPROPERTY()
  TMap<EIsdkSampleLevel, FIsdkLevelButton> LevelButtons;

  // Default values

  UPROPERTY(Category = "InteractionSDK|Label", EditAnywhere)
  float LabelOffset = 2.5f;

  UPROPERTY(Category = "InteractionSDK|Label", EditAnywhere)
  float LabelSize = 3.0f;

  UPROPERTY(Category = "InteractionSDK|Border", EditAnywhere)
  float BorderWidth = 0.3f;

  UPROPERTY(Category = "InteractionSDK|Border", EditAnywhere)
  FLinearColor BorderColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.25f);

  UPROPERTY(Category = "InteractionSDK|Border", EditAnywhere)
  FVector2D BorderPadding = FVector2D(2.5, 2.0);

  UPROPERTY(Category = "InteractionSDK|Border", EditAnywhere)
  float CornerRadius = 1.0f;

  UPROPERTY(Category = "InteractionSDK|Layout", EditAnywhere)
  float GridGap = 1.0f;

  UPROPERTY(Category = "InteractionSDK|Layout", EditAnywhere)
  int GridColumns = 2;

  UPROPERTY(Category = "InteractionSDK|Button Properties", EditAnywhere)
  FVector2D ButtonSize = FVector2D(17.0, 4.0);

  UPROPERTY(Category = "InteractionSDK|Button Properties", EditAnywhere)
  float ButtonLabelSize = 2.1f;

 private:
  void TemporarilyDisableButtons();
  void HandleEnableButtonsTimer();

  FTimerHandle DisableButtonsTimerHandle;
};
