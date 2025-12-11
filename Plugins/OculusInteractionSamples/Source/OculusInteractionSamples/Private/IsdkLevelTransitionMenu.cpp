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

static inline constexpr auto IsdkSamplePawnPath =
    TEXT("/OculusInteractionSamples/Blueprints/IsdkSamplePawn");

AIsdkLevelTransitionMenu::AIsdkLevelTransitionMenu()
{
  PrimaryActorTick.bCanEverTick = true;
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

  LevelButtons.Add(
      EIsdkSampleLevel::PokeExamples,
      FIsdkLevelButton{0, FName("/OculusInteractionSamples/Levels/PokeExamples"), "Poke"});

  LevelButtons.Add(
      EIsdkSampleLevel::RayExamples,
      FIsdkLevelButton{1, FName("/OculusInteractionSamples/Levels/RayExamples"), "Ray"});

  LevelButtons.Add(
      EIsdkSampleLevel::TransformerExamples,
      FIsdkLevelButton{
          2, FName("/OculusInteractionSamples/Levels/TransformerExamples"), "Transformers"});

  LevelButtons.Add(
      EIsdkSampleLevel::DistanceGrabExamples,
      FIsdkLevelButton{
          3, FName("/OculusInteractionSamples/Levels/DistanceGrabExamples"), "Distance Grab"});

  CreateButtons();
}

void AIsdkLevelTransitionMenu::CreateButtons()
{
  for (auto LevelButton : LevelButtons)
  {
    auto ButtonInstance =
        CreateDefaultSubobject<UIsdkRoundedButtonComponent>(FName(LevelButton.Value.ButtonLabel));
    ButtonInstance->SetupAttachment(this->GetRootComponent());
    TransitionButtons.Add(LevelButton.Key, ButtonInstance);
  }
}

void AIsdkLevelTransitionMenu::UpdateButtonsLayout()
{
  for (auto Button : TransitionButtons)
  {
    auto LevelButton = LevelButtons.Find(Button.Key);
    size_t i = LevelButton->Index;
    float x = float(i % GridColumns);
    float y = float(i / GridColumns);

    FVector2D Center = FVector2D::Zero();
    Center += ButtonSize * 0.5;
    Center += (ButtonSize + FVector2D(GridGap, GridGap)) * FVector2D(x, y);

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

  auto VersionString = FOculusInteractionModule::GetChecked().GetVersionString();
  VersionLabel->SetText(FText::FromString(FString("Version #") + VersionString));
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
  for (auto Button : TransitionButtons)
  {
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
  for (auto Button : TransitionButtons)
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
  FLatentActionInfo LatentInfo;
  LatentInfo.UUID = 1;
  LatentInfo.Linkage = 1;
  LatentInfo.CallbackTarget = this;
  LatentInfo.ExecutionFunction = GET_FUNCTION_NAME_CHECKED(AIsdkLevelTransitionMenu, OnLevelLoad);
  UGameplayStatics::LoadStreamLevel(
      GetWorld(), LevelButtons.Find(NextLevel)->LevelName, true, true, LatentInfo);
  TemporarilyDisableButtons();
}

void AIsdkLevelTransitionMenu::UpdateButtonState(EIsdkSampleLevel Level)
{
  for (auto Button : TransitionButtons)
  {
    auto bShouldBeActivate = Button.Key != Level;
    auto ButtonComponent = Button.Value;
    ButtonComponent->SetActive(bShouldBeActivate);
    ButtonComponent->GetPokeInteractable()->SetActive(bShouldBeActivate);
  }
}

void AIsdkLevelTransitionMenu::TemporarilyDisableButtons()
{
  for (auto Button : TransitionButtons)
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
