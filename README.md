# Name Those Ashpiles

SKSE/SKSEVR plugin that adds customizable names for ashpiles.

*	[SSE/AE Version](https://www.nexusmods.com/skyrimspecialedition/mods/24701)
*	[VR Version](https://www.nexusmods.com/skyrimspecialedition/mods/59825)

## Requirements
* [CMake](https://cmake.org/)
	* Add this to your `PATH`
* [PowerShell](https://github.com/PowerShell/PowerShell/releases/latest)
* [Vcpkg](https://github.com/microsoft/vcpkg)
	* Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [Visual Studio Community 2019](https://visualstudio.microsoft.com/)
	* Desktop development with C++
* [CommonLibSSE](https://github.com/powerof3/CommonLibSSE/tree/dev)
	* You need to build from the powerof3/dev branch
	* Add this as as an environment variable `CommonLibSSEPath`
* [CommonLibVR](https://github.com/alandtse/CommonLibVR/tree/vr)
	* Add this as as an environment variable `CommonLibVRPath`

## User Requirements
* [SKSE64/VR](https://skse.silverlock.org/)
* [Address Library for SKSE](https://www.nexusmods.com/skyrimspecialedition/mods/32444)
	* Needed for AE
* [VR Address Library for SKSEVR](https://www.nexusmods.com/skyrimspecialedition/mods/58101)
	* Needed for VR

## Register Visual Studio as a Generator
* Open `x64 Native Tools Command Prompt`
* Run `cmake`
* Close the cmd window

## Building

```
git clone https://github.com/powerof3/NameThoseAshPiles.git
cd NameThoseAshPiles
```
### CommonLibSSE/CommonLibVR
```
# pull CommonLibSSE and CommonLibVR
# alternatively, do not pull and set environment variable `CommonLibSSEPath` or `CommonLibVRPath` if you need something different from external

git submodule update --init --recursive

```
### SSE
```
cmake -B build -S . -DVCPKG_TARGET_TRIPLET=x64-windows-static-md
```
Open build/po3_NameThoseAshpiles.sln in Visual Studio to build dll.
### VR
```
cmake -B build2 -S . -DVCPKG_TARGET_TRIPLET=x64-windows-static-md -DBUILD_SKYRIMVR=On
```
Open build2/po3_NameThoseAshpiles.sln in Visual Studio to build dll.

## License
[MIT](LICENSE)
