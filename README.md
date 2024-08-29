# InteractionSDK Sample

This sample illustrates the Meta XR Interaction SDK plugin.

You must have both the MetaXR Integration, and MetaXR InteractionSDK plugins installed for this sample project to load correctly (see below for instructions).

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

### Launch the project in the Unreal Editor using one of the following options.

#### Epic Games Launcher with MetaXR plugins

The easiest way to get started is to use the prebuilt Unreal Engine from the Epic Games Launcher, with MetaXR plugin and MetaXR InteractionSDK plugin.

1. Install the [Epic Games Launcher](https://www.epicgames.com/store/en-US/download)
2. In the launcher, install UE5 (recommended).
3. Download and install the [MetaXR plugin](https://developer.oculus.com/downloads/package/unreal-engine-5-integration).
4. Download and install the [MetaXR Interaction SDK plugin](https://developer.oculus.com/downloads/package/meta-xr-interaction-sdk-unreal).
5. Launch the Unreal Editor
6. From "Recent Projects", click "Browse" and select `MetaIsdkSample.uproject`

#### Meta fork of Epic’s Unreal Engine

The Meta fork of Epic’s Unreal Engine will give you the most up to date integration of Oculus features. However, you must build the editor from its source.

Follow the instructions on [Accessing Unreal Engine source code on GitHub](https://www.unrealengine.com/en-US/ue-on-github) to obtain:
- an Epic account
- a GitHub account
- authorization to access the Unreal Engine source repository
Disregard instructions on downloading Epic’s Unreal Engine source code as you will be building the Meta fork of Epic’s Unreal Engine source.

Make sure you have Visual Studio installed properly:
- Launch the Visual Studio Installer and click Modify for the Visual Studio version you want to use.
- Under the Workloads tab, click Game development with C++ if it isn’t checked and then click Modify.

1. Download the source code from the [Meta fork of Epic’s Unreal Engine on GitHub](https://github.com/Oculus-VR/UnrealEngine).
2. Follow Epic’s instructions on [Building Unreal Engine from Source](https://docs.unrealengine.com/5.2/en-US/building-unreal-engine-from-source/) to complete the process.
3. Download and install the [MetaXR Interaction SDK plugin](https://developer.oculus.com/downloads/package/meta-xr-interaction-sdk-unreal).
4. Launch the Unreal Editor
5. From "Recent Projects", click "Browse" and select `MetaIsdkSample.uproject`

Depending on your machine, the build may take awhile to complete.

# Licenses
The Meta License applies to the SDK and supporting material. The MIT License applies to only certain, clearly marked documents. If an individual file does not indicate which license it is subject to, then the Meta License applies.
