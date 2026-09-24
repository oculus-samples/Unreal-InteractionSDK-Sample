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

#include "IsdkLevelTransitionMenu.h"

#include "OculusInteraction.h"
#include "IsdkContentAssetPaths.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "RoundedBoxVisuals/IsdkRoundedBoxFunctionLibrary.h"
#include "Subsystem/IsdkWorldSubsystem.h"
#include "UObject/ConstructorHelpers.h"

#define LOCTEXT_NAMESPACE "IsdkLevelTransitionMenu"

AIsdkLevelTransitionMenu::AIsdkLevelTransitionMenu()
{
  PrimaryActorTick.bCanEverTick = false;
  auto Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
  SetRootComponent(Root);

  VersionLabel = CreateDefaultSubobject<UTextRenderComponent>(FName("Version Label"));
  VersionLabel->SetupAttachment(Root);

  ConstructorHelpers::FObjectFinder<UMaterial> LabelMaterialFinder(
      IsdkContentAssetPaths::Materials::TextUnlit);
  if (ensureMsgf(LabelMaterialFinder.Succeeded(), TEXT("Could not find Label Material")))
  {
    VersionLabel->SetMaterial(0, LabelMaterialFinder.Object);
  }

  BorderMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Border Mesh"));
  BorderMesh->SetupAttachment(Root);

  LevelButtons.Reserve(5);

  LevelButtons.Emplace(
      EIsdkSampleLevel::PokeExamples,
      FIsdkLevelButton{
          0,
          FName("/OculusInteractionSamples/Levels/PokeExamples"),
          LOCTEXT("PokeButtonLabel", "Poke")});

  LevelButtons.Emplace(
      EIsdkSampleLevel::RayExamples,
      FIsdkLevelButton{
          1,
          FName("/OculusInteractionSamples/Levels/RayExamples"),
          LOCTEXT("RayButtonLabel", "Ray")});

  LevelButtons.Emplace(
      EIsdkSampleLevel::TransformerExamples,
      FIsdkLevelButton{
          2,
          FName("/OculusInteractionSamples/Levels/TransformerExamples"),
          LOCTEXT("TransformerButtonLabel", "Transformers")});
  LevelButtons.Emplace(
      EIsdkSampleLevel::DistanceGrabExamples,
      FIsdkLevelButton{
          3,
          FName("/OculusInteractionSamples/Levels/DistanceGrabExamples"),
          LOCTEXT("DistanceGrabButtonLabel", "Distance Grab")});
  LevelButtons.Emplace(
      EIsdkSampleLevel::GazeExamples,
      FIsdkLevelButton{
          4,
          FName("/OculusInteractionSamples/Levels/GazeExamples"),
          LOCTEXT("GazeButtonLabel", "Gaze")});

  CreateButtons();
}

void AIsdkLevelTransitionMenu::CreateButtons()
{
  for (const auto& LevelButton : LevelButtons)
  {
    const FName ButtonName(
        *FString::Printf(TEXT("LevelButton_%d"), static_cast<int32>(LevelButton.Value.Index)));
    auto ButtonInstance = CreateDefaultSubobject<UIsdkRoundedButtonComponent>(ButtonName);
    ButtonInstance->SetupAttachment(this->GetRootComponent());
    TransitionButtons.Add(LevelButton.Key, ButtonInstance);
  }
}

void AIsdkLevelTransitionMenu::RebuildTransitionButtons()
{
  // Fully reset before repopulating so stale or null entries (e.g. left behind by
  // editor delete+undo) are discarded rather than accumulated.
  TransitionButtons.Reset();

  TInlineComponentArray<UIsdkRoundedButtonComponent*> ButtonComponents(this);
  for (const auto& LevelButton : LevelButtons)
  {
    const FName ButtonName(
        *FString::Printf(TEXT("LevelButton_%d"), static_cast<int32>(LevelButton.Value.Index)));
    for (UIsdkRoundedButtonComponent* ButtonComponent : ButtonComponents)
    {
      if (IsValid(ButtonComponent) && ButtonComponent->GetFName() == ButtonName)
      {
        TransitionButtons.Add(LevelButton.Key, ButtonComponent);
        break;
      }
    }
  }
}

void AIsdkLevelTransitionMenu::UpdateButtonsLayout()
{
  // The cache is Transient and may be empty/stale after undo/reinstancing, so
  // rebuild it from the live components before iterating.
  RebuildTransitionButtons();

  const FVector2D GridGapVec(GridGap, GridGap);
  for (const auto& Button : TransitionButtons)
  {
    if (!IsValid(Button.Value))
    {
      continue;
    }
    auto LevelButton = LevelButtons.Find(Button.Key);
    size_t i = LevelButton->Index;
    float x = float(i % GridColumns);
    float y = float(i / GridColumns);

    FVector2D Center = FVector2D::Zero();
    Center += ButtonSize * 0.5;
    Center += (ButtonSize + GridGapVec) * FVector2D(x, y);

    Button.Value->ButtonSize = ButtonSize;
    Button.Value->SetLabelText(LevelButton->ButtonLabel);
    Button.Value->SetLabelSize(ButtonLabelSize);

    auto Center3d = FVector(0.0, -Center.X, -Center.Y);
    Button.Value->SetRelativeLocation(Center3d);
  }
}

void AIsdkLevelTransitionMenu::UpdateBorderMesh()
{
  int Row = ((LevelButtons.Num() - 1) / GridColumns) + 1;
  FVector2D BorderSize = (FVector2D(GridColumns, Row) * ButtonSize) +
      (FVector2D(GridColumns - 1, Row - 1) * FVector2D(GridGap, GridGap));
  FVector2D BorderCenter = BorderSize * 0.5;

  auto Properties = FIsdkRoundedBoxProperties();
  Properties.OuterBorderRadius = BorderWidth;
  Properties.OutlineColor = BorderColor;
  Properties.FillColor = BorderColor;
  Properties.FillColor.A = 0.0;
  Properties.Size = BorderSize + BorderPadding;
  Properties.CornerRadius = FVector4d(CornerRadius, CornerRadius, CornerRadius, CornerRadius);

  UMaterialInstanceDynamic* BorderMaterial;
  UIsdkRoundedBoxFunctionLibrary::SetupRoundedBox(
      this, Properties, GetRootComponent(), BorderMesh, BorderMaterial, false);
  auto BorderCenter3d = FVector(0.0, -BorderCenter.X, -BorderCenter.Y);
  BorderMesh->SetRelativeLocation(BorderCenter3d);
  BorderMesh->SetRelativeRotation(FQuat::MakeFromEuler(FVector(-90.0, 0.0, 90.0)));
}

void AIsdkLevelTransitionMenu::UpdateVersionLabel()
{
  int Row = ((LevelButtons.Num() - 1) / GridColumns) + 1;
  FVector2D BorderSize = (FVector2D(GridColumns, Row) * ButtonSize) +
      (FVector2D(GridColumns - 1, Row - 1) * FVector2D(GridGap, GridGap));
  FVector2D BorderCenter = BorderSize * 0.5;

  VersionLabel->SetText(LOCTEXT("VersionLabelPlaceholder", "Version #X.X.X"));
  auto BorderCenter3d = FVector(0.0, -BorderCenter.X, -BorderCenter.Y);
  auto LabelPosition = BorderCenter3d;
  LabelPosition.Z -= LabelOffset + (BorderSize + BorderPadding).Y * 0.5;
  VersionLabel->SetRelativeLocation(LabelPosition);
  VersionLabel->SetWorldSize(LabelSize);
  VersionLabel->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
  VersionLabel->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
}

void AIsdkLevelTransitionMenu::BeginPlay()
{
  Super::BeginPlay();

  // Set the dynamic build-version text at runtime only. Setting it during editor
  // construction (OnConstruction) would serialize a transient FString-initialized
  // FText into the saved level package, which triggers cook-time warnings.
  const auto VersionString = FOculusInteractionModule::GetChecked().GetVersionString();
  VersionLabel->SetText(
      FText::Format(
          LOCTEXT("VersionLabelFormat", "Version #{0}"), FText::AsCultureInvariant(VersionString)));

  RebuildTransitionButtons();
  for (const auto& Button : TransitionButtons)
  {
    if (!IsValid(Button.Value))
    {
      continue;
    }
    Button.Value->SetActive(false);
    Button.Value->GetPokeInteractable()->SetVisibility(false);

    auto ColorVisual = Button.Value->GetInteractableColorVisual();
    auto DisabledColor = ColorVisual->GetColorState(EIsdkInteractableState::Disabled);
    DisabledColor.Color = FLinearColor(0.3f, 0.3f, 0.3f, 0.3f);
    ColorVisual->SetColorState(EIsdkInteractableState::Disabled, DisabledColor);

    Button.Value->GetPokeInteractable()->GetInteractionPointerEventDelegate().AddUniqueDynamic(
        this, &AIsdkLevelTransitionMenu::HandlePointerEvent);
  }
  bInitialTransition = true;

  // Defer the initial level load by a short delay to allow the menu and render
  // thread to fully initialize before streaming begins. This prevents a race
  // condition where the render thread is still setting up StartLevel's scene
  // views while the game thread is already tearing them down to stream the
  // initial sub-level. The 0.1s delay is enough to break the race without being
  // noticeable to the user.
  FTimerHandle InitialLoadHandle;
  GetWorld()->GetTimerManager().SetTimer(
      InitialLoadHandle, this, &AIsdkLevelTransitionMenu::HandleInitialLevelLoad, 0.1f, false);
}

void AIsdkLevelTransitionMenu::HandleInitialLevelLoad()
{
  TransitionToLevel(InitialLevel);
}

void AIsdkLevelTransitionMenu::HandlePointerEvent(const FIsdkInteractionPointerEvent& PointerEvent)
{
  if (PointerEvent.Type != EIsdkPointerEventType::Select)
  {
    return;
  }
  bool bRequiredLevelLoad = false;
  EIsdkSampleLevel NextLevel{};
  for (const auto& Button : TransitionButtons)
  {
    if (Button.Value->GetPokeInteractable() == PointerEvent.Interactable.GetObject() &&
        Button.Key != CurrentLevel)
    {
      NextLevel = Button.Key;
      bRequiredLevelLoad = true;
      break;
    }
  }

  if (bRequiredLevelLoad)
  {
    TransitionToLevel(NextLevel);
  }
}

void AIsdkLevelTransitionMenu::OnConstruction(const FTransform& Transform)
{
  Super::OnConstruction(Transform);
  UpdateButtonsLayout();
  UpdateBorderMesh();
  UpdateVersionLabel();
}

void AIsdkLevelTransitionMenu::PostInitializeComponents()
{
  Super::PostInitializeComponents();
  UpdateButtonsLayout();
  UpdateBorderMesh();
  UpdateVersionLabel();
}

void AIsdkLevelTransitionMenu::TransitionToLevel(EIsdkSampleLevel NextLevel)
{
  PreviousLevel = CurrentLevel;
  CurrentLevel = NextLevel;

  const FName LevelName = LevelButtons.Find(NextLevel)->LevelName;

  FLatentActionInfo LatentInfo;
  LatentInfo.UUID = 1;
  LatentInfo.Linkage = 1;
  LatentInfo.CallbackTarget = this;
  LatentInfo.ExecutionFunction = GET_FUNCTION_NAME_CHECKED(AIsdkLevelTransitionMenu, OnLevelLoad);
  UGameplayStatics::LoadStreamLevel(GetWorld(), LevelName, true, true, LatentInfo);
  TemporarilyDisableButtons();
}

void AIsdkLevelTransitionMenu::UpdateButtonState(EIsdkSampleLevel Level)
{
  for (const auto& Button : TransitionButtons)
  {
    auto bShouldBeActivate = Button.Key != Level;
    auto ButtonComponent = Button.Value;
    ButtonComponent->SetActive(bShouldBeActivate);
    ButtonComponent->GetPokeInteractable()->SetActive(bShouldBeActivate);
  }
}

void AIsdkLevelTransitionMenu::TemporarilyDisableButtons()
{
  for (const auto& Button : TransitionButtons)
  {
    auto ButtonComponent = Button.Value;
    ButtonComponent->SetActive(false);
    ButtonComponent->GetPokeInteractable()->SetActive(false);
  }

  // Disable all buttons and re-enable them after a short delay to help prevent crashes
  // that may happen if the player rapidly changes levels.
  GetWorld()->GetTimerManager().SetTimer(
      DisableButtonsTimerHandle,
      this,
      &AIsdkLevelTransitionMenu::HandleEnableButtonsTimer,
      0.3f,
      false);
}

void AIsdkLevelTransitionMenu::HandleEnableButtonsTimer()
{
  // Re-enable buttons
  UpdateButtonState(CurrentLevel);
}

void AIsdkLevelTransitionMenu::OnLevelLoad()
{
  if (!bInitialTransition)
  {
    UGameplayStatics::UnloadStreamLevel(
        GetWorld(), LevelButtons.Find(PreviousLevel)->LevelName, FLatentActionInfo(), false);
  }
  bInitialTransition = false;
}

#undef LOCTEXT_NAMESPACE
