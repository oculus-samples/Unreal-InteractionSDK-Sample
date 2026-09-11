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
#include "IsdkInteractableWidget.h"
#include "IsdkHorizontalScrollExampleWidget.generated.h"

/**
 * @brief Example widget actor demonstrating horizontal scroll functionality.
 *
 * This widget provides a sample implementation of a horizontally scrollable UI
 * using the Interaction SDK.
 */
UCLASS(ClassGroup = (InteractionSDK))
class OCULUSINTERACTIONSAMPLES_API AIsdkHorizontalScrollExampleWidget final
    : public AIsdkInteractableWidget
{
  GENERATED_BODY()

 public:
  static constexpr auto HorizontalScrollUIAsset =
      TEXT("/OculusInteractionSamples/Widgets/ScrollUI/HorizontalScrollUI");

  /** @brief Default constructor. */
  AIsdkHorizontalScrollExampleWidget();
#if WITH_EDITOR
  /**
   * @brief Determines if a component property can be edited in the editor.
   * @param Component The component being queried.
   * @param InProperty The property being queried for edit capability.
   * @return True if the property can be edited, false otherwise.
   */
  virtual bool CanEditChangeComponent(const UActorComponent* Component, const FProperty* InProperty)
      const override;
#endif
};
