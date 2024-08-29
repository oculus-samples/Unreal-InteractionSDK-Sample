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

#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IsdkInstructionDialogWidget.h"
#include "MediaSoundComponent.h"
#include "MediaSource.h"

#include "IsdkInstructionDialog.generated.h"

UCLASS()
class OCULUSINTERACTIONSAMPLES_API AIsdkInstructionDialog : public AActor
{
  GENERATED_BODY()

  static inline constexpr auto DialogWidgetAsset =
      TEXT("/OculusInteractionSamples/Objects/Environment/Tooltip/DialogWidget");
  static inline constexpr auto DialogWidgetMaterial =
      TEXT("/OculusInteractionSamples/Objects/Environment/Tooltip/Materials/DialogRoundedBox");
  static inline constexpr auto DialogMediaPlayer =
      TEXT("/OculusInteractionSamples/Objects/Environment/Tooltip/DialogVideos/DialogVideoPlayer");

 public:
  AIsdkInstructionDialog();
  UPROPERTY()
  USceneComponent* Root;
  UPROPERTY()
  UWidgetComponent* Widget;
  UPROPERTY()
  UMediaSoundComponent* Sound;

  UPROPERTY(EditAnywhere, Category = "InteractionSDK", meta = (MultiLine = true))
  FText Title;
  UPROPERTY(EditAnywhere, Category = "InteractionSDK", meta = (MultiLine = true))
  FText Instructions;

  UPROPERTY()
  UMaterialInterface* WidgetMaterial;

  UPROPERTY(EditAnywhere, Category = "InteractionSDK")
  FVector2D WidgetSize = FVector2D(400.0, 400.0);
  UPROPERTY(EditAnywhere, Category = "InteractionSDK")
  UMediaPlayer* DialogVideoPlayer;
  UPROPERTY(EditAnywhere, Category = "InteractionSDK")
  UMediaSource* InstructionVideoSource;
  UPROPERTY(EditAnywhere, Category = "InteractionSDK")
  float CornerRadius;
  UPROPERTY()
  TSubclassOf<UIsdkInstructionDialogWidget> InstructionWidgetClass;

 protected:
  virtual void BeginPlay() override;
  void OpenVideo();

 public:
  void Initialize();
  virtual void OnConstruction(const FTransform& Transform) override;
  virtual void PostInitializeComponents() override;
  UFUNCTION()
  void MediaOpened(FString MediaOpened);
  virtual void Destroyed() override;
};
