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

#include "IsdkPokeExampleWidget.h"
#include "IsdkContentAssetPaths.h"
#include "Sound/SoundBase.h"

AIsdkPokeExampleWidget::AIsdkPokeExampleWidget()
{
  ConstructorHelpers::FClassFinder<UUserWidget> PokeWidgetClassFinder(PlaneScrollUIAsset);
  if (ensureMsgf(PokeWidgetClassFinder.Succeeded(), TEXT("Could not find Poke Example UI")))
  {
    InteractableWidgetComponent->WidgetClass = PokeWidgetClassFinder.Class;
    InteractableWidgetComponent->GetWidget()->SetWidgetClass(PokeWidgetClassFinder.Class);
  }

  InteractableWidgetComponent->bCreatePokeInteractable = true;

  InteractableWidgetComponent->bUseRoundedBoxMaterial = true;
  InteractableWidgetComponent->DrawSize = FVector2D(800.0, 1200.0);
  InteractableWidgetComponent->WidgetScale = 0.0175;
  InteractableWidgetComponent->BackgroundColor =
      FLinearColor(FVector(0.011612, 0.024158, 0.033105));
  InteractableWidgetComponent->CornerRadius = FVector4(30.0, 30.0, 30.0, 30.0);
  InteractableWidgetComponent->bTwoSided = true;
  InteractableWidgetComponent->BlendMode = EIsdkWidgetBlendMode::Transparent;

  InteractableWidgetComponent->GetPointableWidget()->MinMoveTravelDistance = 2.0;

  using SoundFinder = ConstructorHelpers::FObjectFinder<USoundBase>;
  using Audio = IsdkContentAssetPaths::Audio;

  static SoundFinder SelectedHovered(Audio::Interaction_BasicRay_Press);
  if (ensureMsgf(SelectedHovered.Succeeded(), TEXT("Sound not found for: SelectedHovered")))
  {
    InteractableWidgetComponent->GetSelectedHoveredAudio()->SetSound(SelectedHovered.Object);
  }

  static SoundFinder SelectedEmpty(Audio::Interaction_BasicPoke_ButtonPress);
  if (ensureMsgf(SelectedEmpty.Succeeded(), TEXT("Sound not found for: SelectedEmpty")))
  {
    InteractableWidgetComponent->GetSelectedEmptyAudio()->SetSound(SelectedEmpty.Object);
  }
}

#if WITH_EDITOR
bool AIsdkPokeExampleWidget::CanEditChangeComponent(
    const UActorComponent* Component,
    const FProperty* InProperty) const
{
  if (InProperty)
  {
    FString PropertyName = InProperty->GetName();
    if (PropertyName ==
            GET_MEMBER_NAME_STRING_CHECKED(
                UIsdkInteractableWidgetComponent, bUseRoundedBoxMaterial) ||
        PropertyName ==
            GET_MEMBER_NAME_STRING_CHECKED(UIsdkInteractableWidgetComponent, DrawSize) ||
        PropertyName ==
            GET_MEMBER_NAME_STRING_CHECKED(UIsdkInteractableWidgetComponent, WidgetScale) ||
        PropertyName ==
            GET_MEMBER_NAME_STRING_CHECKED(UIsdkInteractableWidgetComponent, WidgetClass) ||
        PropertyName ==
            GET_MEMBER_NAME_STRING_CHECKED(
                UIsdkInteractableWidgetComponent, bCreatePokeInteractable) ||
        PropertyName ==
            GET_MEMBER_NAME_STRING_CHECKED(
                UIsdkInteractableWidgetComponent, bCreateRayInteractable))
    {
      return false;
    }
  }
  return Super::CanEditChangeComponent(Component, InProperty);
}
#endif
