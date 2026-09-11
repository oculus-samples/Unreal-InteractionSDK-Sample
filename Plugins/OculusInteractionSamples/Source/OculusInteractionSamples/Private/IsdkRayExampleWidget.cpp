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

#include "IsdkRayExampleWidget.h"
#include "IsdkContentAssetPaths.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h"
#include "Widget/IsdkPointableWidget.h"
#include "Audio/IsdkWidgetEventAudioPlayer.h"

AIsdkRayExampleWidget::AIsdkRayExampleWidget()
{
  ConstructorHelpers::FClassFinder<UUserWidget> RayWidgetClassFinder(RayExampleWidgetAsset);
  if (ensureMsgf(RayWidgetClassFinder.Succeeded(), TEXT("Could not find Ray Example Widget")))
  {
    InteractableWidgetComponent->WidgetClass = RayWidgetClassFinder.Class;
    InteractableWidgetComponent->GetWidget()->SetWidgetClass(RayWidgetClassFinder.Class);
  }

  InteractableWidgetComponent->bCreateRayInteractable = true;

  InteractableWidgetComponent->bUseRoundedBoxMaterial = true;
  InteractableWidgetComponent->DrawSize = FVector2D(1560.0, 950.0);
  InteractableWidgetComponent->WidgetScale = 0.1;
  InteractableWidgetComponent->BackgroundColor = FLinearColor(0.008f, 0.036f, 0.309f);
  InteractableWidgetComponent->CornerRadius = FVector4(40.0, 40.0, 40.0, 40.0);
  InteractableWidgetComponent->bTwoSided = true;
  InteractableWidgetComponent->BlendMode = EIsdkWidgetBlendMode::Transparent;

  InteractableWidgetComponent->GetPointableWidget()->MinMoveTravelDistance = 5.0;

  static ConstructorHelpers::FObjectFinder<USoundBase> RayAudio(
      IsdkContentAssetPaths::Audio::Interaction_BasicRay_Press);
  if (ensureMsgf(RayAudio.Succeeded(), TEXT("Sound not found for: RayAudio Selected")))
  {
    InteractableWidgetComponent->GetSelectedEmptyAudio()->SetSound(RayAudio.Object);
    InteractableWidgetComponent->GetSelectedHoveredAudio()->SetSound(RayAudio.Object);
  }
  UnselectedHoveredAudio =
      CreateDefaultSubobject<UAudioComponent>(FName(TEXT("Unselected Hovered Audio")));
  UnselectedHoveredAudio->SetupAttachment(InteractableWidgetComponent);
  UnselectedHoveredAudio->SetAutoActivate(false);
  static ConstructorHelpers::FObjectFinder<USoundWave> RayUnselect(RayUnselectHovererdAudio);
  if (ensureMsgf(RayUnselect.Succeeded(), TEXT("Sound not found for: RayAudio Unselected")))
  {
    UnselectedHoveredAudio->SetSound(RayUnselect.Object);
  }
  InteractableWidgetComponent->GetWidgetEventAudioPlayer()->SetUnselectedHoveredAudio(
      UnselectedHoveredAudio);
}

#if WITH_EDITOR
bool AIsdkRayExampleWidget::CanEditChangeComponent(
    const UActorComponent* Component,
    const FProperty* InProperty) const
{
  if (InProperty)
  {
    const FName PropertyName = InProperty->GetFName();
    if (PropertyName ==
            GET_MEMBER_NAME_CHECKED(UIsdkInteractableWidgetComponent, bUseRoundedBoxMaterial) ||
        PropertyName == GET_MEMBER_NAME_CHECKED(UIsdkInteractableWidgetComponent, DrawSize) ||
        PropertyName == GET_MEMBER_NAME_CHECKED(UIsdkInteractableWidgetComponent, WidgetScale) ||
        PropertyName == GET_MEMBER_NAME_CHECKED(UIsdkInteractableWidgetComponent, WidgetClass) ||
        PropertyName ==
            GET_MEMBER_NAME_CHECKED(UIsdkInteractableWidgetComponent, bCreatePokeInteractable) ||
        PropertyName ==
            GET_MEMBER_NAME_CHECKED(UIsdkInteractableWidgetComponent, bCreateRayInteractable))
    {
      return false;
    }
  }
  return Super::CanEditChangeComponent(Component, InProperty);
}
#endif
