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

#include "IsdkInstructionDialog.h"
#include "Components/SynthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MediaPlayer.h"
#include "RoundedBoxVisuals/IsdkRoundedBoxFunctionLibrary.h"
#include "UObject/ConstructorHelpers.h"

AIsdkInstructionDialog::AIsdkInstructionDialog()
{
  PrimaryActorTick.bCanEverTick = true;
  Root = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));
  Widget = CreateDefaultSubobject<UWidgetComponent>(FName("Instruction Widget"));
  Widget->SetupAttachment(Root);
  Sound = CreateDefaultSubobject<UMediaSoundComponent>(FName("Sound Component"));
  Sound->SetupAttachment(Root);

  ConstructorHelpers::FClassFinder<UIsdkInstructionDialogWidget> InstructionWidget(
      DialogWidgetAsset);
  if (ensureMsgf(InstructionWidget.Succeeded(), TEXT("Instruction widget was not found")))
  {
    InstructionWidgetClass = InstructionWidget.Class;
  }
  ConstructorHelpers::FObjectFinder<UMaterialInterface> WidgetMaterialFinder(DialogWidgetMaterial);
  if (ensureMsgf(
          WidgetMaterialFinder.Succeeded(), TEXT("Material for instruction widget was not found")))
  {
    WidgetMaterial = WidgetMaterialFinder.Object;
  }
  ConstructorHelpers::FObjectFinder<UMediaPlayer> MediaPlayerFinder(DialogMediaPlayer);
  if (ensureMsgf(
          MediaPlayerFinder.Succeeded(), TEXT("Media player for instruction widget was not found")))
  {
    DialogVideoPlayer = MediaPlayerFinder.Object;
  }
}

void AIsdkInstructionDialog::BeginPlay()
{
  Super::BeginPlay();
  if (ensureMsgf(
          IsValid(DialogVideoPlayer) && IsValid(InstructionVideoSource),
          TEXT("Current Dialog Video Player or source is invalid")))
  {
    Widget->SetVisibility(false);
    auto Handler = FTimerHandle();
    GetWorldTimerManager().SetTimer(
        Handler, this, &AIsdkInstructionDialog::OpenVideo, 0.2f, false, 1.0f);
  }
}

void AIsdkInstructionDialog::OpenVideo()
{
  auto PlayerOptions = FMediaPlayerOptions();
  PlayerOptions.PlayOnOpen = EMediaPlayerOptionBooleanOverride::Enabled;
  PlayerOptions.Loop = EMediaPlayerOptionBooleanOverride::Enabled;
  DialogVideoPlayer->OnMediaOpened.AddUniqueDynamic(this, &AIsdkInstructionDialog::MediaOpened);
  auto WillOpenVideo =
      DialogVideoPlayer->OpenSourceWithOptions(InstructionVideoSource, PlayerOptions);
  ensureMsgf(WillOpenVideo, TEXT("Video is not going to be opened"));
}

void AIsdkInstructionDialog::Initialize()
{
  if (IsValid(InstructionWidgetClass))
  {
    auto InstructionWidget = Cast<UIsdkInstructionDialogWidget>(
        CreateWidget(GetWorld(), InstructionWidgetClass, FName("InstructionWidget")));
    InstructionWidget->TitleValue = Title;
    InstructionWidget->InstructionsValue = Instructions;
    Widget->SetWidget(InstructionWidget);
    Widget->SetDrawSize(WidgetSize);
    Widget->SetMaterial(0, WidgetMaterial);
    auto MaterialInstance = Widget->GetMaterialInstance();
    UIsdkRoundedBoxFunctionLibrary::SetRoundedBoxDimensions(MaterialInstance, WidgetSize, 0.0, 0.0);
    UIsdkRoundedBoxFunctionLibrary::SetRoundedBoxMaterialCornerRadius(
        MaterialInstance, FVector4(CornerRadius, CornerRadius, CornerRadius, CornerRadius));
  }
}

void AIsdkInstructionDialog::OnConstruction(const FTransform& Transform)
{
  Super::OnConstruction(Transform);
  Initialize();
}

void AIsdkInstructionDialog::PostInitializeComponents()
{
  Super::PostInitializeComponents();
  Initialize();
}

void AIsdkInstructionDialog::MediaOpened(FString MediaOpened)
{
  Widget->SetVisibility(true);
  DialogVideoPlayer->SetNativeVolume(0.0);
  Sound->SetVolumeMultiplier(0.0);
}

void AIsdkInstructionDialog::Destroyed()
{
  DialogVideoPlayer->OnMediaOpened.RemoveDynamic(this, &AIsdkInstructionDialog::MediaOpened);
  Super::Destroyed();
}
