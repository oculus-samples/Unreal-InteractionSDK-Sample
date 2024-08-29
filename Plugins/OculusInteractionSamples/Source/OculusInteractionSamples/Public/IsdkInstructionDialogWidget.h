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

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CoreMinimal.h"
#include "IsdkInstructionDialogWidget.generated.h"

UCLASS(Blueprintable, Abstract)
class OCULUSINTERACTIONSAMPLES_API UIsdkInstructionDialogWidget : public UUserWidget
{
  GENERATED_BODY()

 public:
  virtual void SynchronizeProperties() override;
  UPROPERTY(EditAnywhere, Category = "InteractionSDK")
  FText TitleValue;
  UPROPERTY(EditAnywhere, Category = "InteractionSDK")
  FText InstructionsValue;
  UPROPERTY(BlueprintReadOnly, Category = "InteractionSDK", meta = (BindWidget))
  UTextBlock* Title = nullptr;
  UPROPERTY(BlueprintReadOnly, Category = "InteractionSDK", meta = (BindWidget))
  UTextBlock* Instructions = nullptr;
};
