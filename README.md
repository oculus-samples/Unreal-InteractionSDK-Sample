# InteractionSDK Sample

This sample illustrates the Meta XR Interaction SDK plugin.

You must have both the MetaXR Integration, and MetaXR InteractionSDK plugins installed for this sample project to load correctly (see below for instructions).

As this sample is a C++ project, you will also need the [appropriate version](https://dev.epicgames.com/documentation/en-us/unreal-engine/setting-up-visual-studio-development-environment-for-cplusplus-projects-in-unreal-engine) of Visual Studio installed and configured for your engine version.

Make sure you have Visual Studio installed properly:

- Launch the Visual Studio Installer and click Modify for the Visual Studio version you want to use.
- Under the Workloads tab, click Game development with C++ if it isn’t checked and then click Modify.

## How to Use

### Load the project

First, ensure you have Git LFS installed by running this command:

```sh
git lfs install
```

Then, clone this repo using the "Code" button above, or this command:

```sh
git clone https://github.com/oculus-samples/Unreal-InteractionSDK-Sample
```

### Launch the project in the Unreal Editor using one of the following options

#### Option 1: Epic Games Launcher with MetaXR plugins

The easiest way to get started is to use the prebuilt Unreal Engine from the Epic Games Launcher, with MetaXR plugin and MetaXR InteractionSDK plugin.

1. Install the [Epic Games Launcher](https://www.epicgames.com/store/en-US/download)
2. In the launcher, install UE5.
3. Download and install the [MetaXR plugin](https://developer.oculus.com/downloads/package/unreal-engine-5-integration).
4. Download and install the [MetaXR Interaction SDK plugin](https://developer.oculus.com/downloads/package/meta-xr-interaction-sdk-unreal).
5. Clone or download this github samples repository.
6. Right click on `MetaIsdkSample.uproject`, select 'Generate Visual Studio project files'
7. Open the `MetaIsdkSample.sln` file in Visual studio
8. Within Visual Studio, select 'Start without debugging' from the 'Debug' menu.

#### Option 2: Meta fork of Epic’s Unreal Engine

The Meta fork of Epic’s Unreal Engine will give you the most up to date integration of Oculus features. However, you must build the editor from its source.

Follow the instructions on [Accessing Unreal Engine source code on GitHub](https://www.unrealengine.com/en-US/ue-on-github) to obtain:

- an Epic account
- a GitHub account
- authorization to access the Unreal Engine source repository

Disregard instructions on downloading Epic’s Unreal Engine source code as you will be building the Meta fork of Epic’s Unreal Engine source.

1. Download the source code from the [Meta fork of Epic’s Unreal Engine on GitHub](https://github.com/Oculus-VR/UnrealEngine).
2. Follow Epic’s instructions on [Building Unreal Engine from Source](https://docs.unrealengine.com/5.2/en-US/building-unreal-engine-from-source/) to complete the process.
3. Download and install the [MetaXR Interaction SDK plugin](https://developer.oculus.com/downloads/package/meta-xr-interaction-sdk-unreal).5. Clone or download this github samples repository.
4. Right click on `MetaIsdkSample.uproject`, select 'Switch Unreal Engine Version' and select your source build
5. Open the `MetaIsdkSample.sln` file in Visual studio
6. Within Visual Studio, select 'Start without debugging' from the 'Debug' menu.

Depending on your machine, the build may take awhile to complete.

## Licenses

The Meta License applies to the SDK and supporting material. The MIT License applies to only certain, clearly marked documents. If an individual file does not indicate which license it is subject to, then the Meta License applies.
