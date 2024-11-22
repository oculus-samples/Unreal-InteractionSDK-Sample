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
#include "GameFramework/GameModeBase.h"
#include "IsdkSampleGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class OCULUSINTERACTIONSAMPLES_API AIsdkSampleGameModeBase : public AGameModeBase
{
  GENERATED_BODY()

  /*
   * 0 = Controller + Hands (Procedural)
   * 1 = Controller + Hands (Animated)
   * 2 = Controller Only
   * 3 = Hands Only (Procedural)
   * 4 = Hands Only (Animated)
   */
  UFUNCTION(Exec)
  void ISDK_SetControllerHandBehavior(int ControllerHandBehavior);
};
