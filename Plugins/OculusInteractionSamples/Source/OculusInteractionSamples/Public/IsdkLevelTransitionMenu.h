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
  DistanceGrabExamples
};

USTRUCT()
struct FIsdkLevelButton
{
  GENERATED_BODY()

  size_t Index;
  FName LevelName{};
  FString ButtonLabel{};
};

UCLASS(ClassGroup = (InteractionSDK), meta = (DisplayName = "ISDK Level Transition Menu"))
class OCULUSINTERACTIONSAMPLES_API AIsdkLevelTransitionMenu : public AActor
{
  GENERATED_BODY()
 public:
  AIsdkLevelTransitionMenu();

 private:
  void CreateButtons();
  void UpdateButtonsLayout();
  void UpdateBorderMesh();
  void UpdateVersionLabel();

  UFUNCTION()
  void HandlePointerEvent(const FIsdkInteractionPointerEvent& PointerEvent);

 protected:
  virtual void BeginPlay() override;

  UPROPERTY()
  TMap<EIsdkSampleLevel, UIsdkRoundedButtonComponent*> TransitionButtons;

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

  UPROPERTY(Category = "InteractionSDK|Label", EditAnywhere)
  float LabelOffset;
  UPROPERTY(Category = "InteractionSDK|Label", EditAnywhere)
  float LabelSize;

  UPROPERTY(Category = "InteractionSDK|Border", EditAnywhere)
  float BorderWidth;
  UPROPERTY(Category = "InteractionSDK|Border", EditAnywhere)
  FLinearColor BorderColor;
  UPROPERTY(Category = "InteractionSDK|Border", EditAnywhere)
  FVector2D BorderPadding;
  UPROPERTY(Category = "InteractionSDK|Border", EditAnywhere)
  float CornerRadius;

  UPROPERTY(Category = "InteractionSDK|Layout", EditAnywhere)
  float GridGap;
  UPROPERTY(Category = "InteractionSDK|Layout", EditAnywhere)
  int GridColumns;
  UPROPERTY(Category = "InteractionSDK|Button Properties", EditAnywhere)
  FVector2D ButtonSize;
  UPROPERTY(Category = "InteractionSDK|Button Properties", EditAnywhere)
  float ButtonLabelSize;
};
