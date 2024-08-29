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

#include "IsdkSamplesFunctionLibrary.h"

void UIsdkSamplesFunctionLibrary::SampleGraphicSettingCommands(APlayerController* PlayerController)
{
  FString Command = "r.TonemapperGamma 0";
  PlayerController->ConsoleCommand(*Command);

  Command = "r.TonemapperFilm 0";
  PlayerController->ConsoleCommand(*Command);

  Command = "r.Tonemapper.Sharpen 0";
  PlayerController->ConsoleCommand(*Command);

  Command = "r.Tonemapper.GrainQuantization 0";
  PlayerController->ConsoleCommand(*Command);

  Command = "r.Tonemapper.Quality 0";
  PlayerController->ConsoleCommand(*Command);

  Command = "r.EyeAdaptation.ExponentialTransitionDistance 0";
  PlayerController->ConsoleCommand(*Command);

  Command = "r.EyeAdaptationQuality 0";
  PlayerController->ConsoleCommand(*Command);

  Command = "r.MSAACount 4";
  PlayerController->ConsoleCommand(*Command);
}
