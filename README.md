<div align="center">

# ⚡ Culling Plugin for LeviLauncher

**A high-performance, multithreaded native C++ engine optimization module for Minecraft Bedrock on Android.**

[![Build Status](https://img.shields.io/github/actions/workflow/status/Ink-exp/Culling-plugin/build.yml?branch=main&style=for-the-badge&logo=github)](https://github.com/Ink-exp/Culling-plugin/actions)
[![Platform](https://img.shields.io/badge/Platform-Android%20(ARM64)--v8a-brightgreen?style=for-the-badge&logo=android)](https://developer.android.com)
[![NDK Version](https://img.shields.io/badge/NDK-r25c+-orange?style=for-the-badge&logo=cmake)](https://developer.android.com/ndk)

[Features](#-key-features) • [Architecture](#-architecture--how-it-works) • [Quick Start](#-quick-start) • [Building from Source](#-building-from-source) • [Contributing](#-contributing)

</div>

---

## 🚀 Overview

**Culling Plugin** is a low-level native module (`.so`) engineered specifically for **LeviLauncher** on Android. Designed to tackle GPU and CPU bottlenecks on mobile hardware, it dynamically modifies render pipeline calls at runtime—uncapping frame rates and bypassing draw calls for off-screen entities, particles, and geometry.

---

## ✨ Key Features

### 🔓 Native Frame Rate Uncapper
* **Bypass VSync Restrictions:** Overrides hardcoded display refresh rate locks in `libminecraftpe.so`.
* **Zero Input Lag:** Frees the rendering thread from engine-enforced frame delays.

### 🎯 Spatial Entity & Particle Culling
* **Bounding Sphere Frustum Checks:** Calculates spatial distance between camera viewports and target entities in C++ before draw call submission.
* **Aggressive Particle Suppression:** Eliminates off-screen particle emitter processing to conserve GPU fill rate.
* **Dynamic Distance Scaling:** Automatically adjusts render radii based on current system load.

---

## 🏗 Architecture & How It Works

```text
┌────────────────────────────────────────────────────────┐
│                   LeviLauncher Core                    │
└──────────────────────────┬─────────────────────────────┘
                           │ Mod Injection (dlopen)
                           ▼
┌────────────────────────────────────────────────────────┐
│                     Culling Plugin                     │
│  ┌───────────────────────┐   ┌──────────────────────┐  │
│  │   FPS Uncap Module    │   │ Spatial Culling Engine│  │
│  └───────────┬───────────┘   └──────────┬───────────┘  │
└──────────────┼──────────────────────────┼──────────────┘
               │ Symbol Patch             │ Hooks
               ▼                          ▼
┌────────────────────────────────────────────────────────┐
│                   libminecraftpe.so                    │
│             (Minecraft Bedrock Render Loop)            │
└────────────────────────────────────────────────────────┘

```
## 📥 Quick Start
### Installation via LeviLauncher
 1. Navigate to the **Actions** tab in this repository.
 2. Select the latest successful workflow run and download CullingPlugin-Release.
 3. Open **LeviLauncher** on your Android device.
 4. Navigate to **Modules / Native Mods** ➔ **Import**.
 5. Select culling-plugin.zip and restart the launcher.
## ⚙ Configurable Parameters
The plugin behavior can be configured inside manifest.json or dynamically at startup:
| Parameter | Type | Default | Description |
|---|---|---|---|
| uncap_fps | boolean | true | Bypasses native VSync and frame cap locks |
| cull_entities | boolean | true | Enables spatial distance culling for entities |
| cull_distance_blocks | float | 40.0 | Radius (in blocks) beyond which entities are culled |
## 🛠 Building from Source
### Prerequisites
 * **Android NDK:** r25c or newer
 * **CMake:** 3.18.1+
 * **Ninja / Make** build system
### Local Compilation
```bash
# Clone repository
git clone [https://github.com/Ink-exp/Culling-plugin.git](https://github.com/Ink-exp/Culling-plugin.git)
cd Culling-plugin

# Configure CMake targeting ARM64
cmake -B build \
      -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake \
      -DANDROID_ABI=arm64-v8a \
      -DANDROID_PLATFORM=android-28

# Compile shared library
cmake --build build --parallel

```
The output binary libculling_plugin.so will be created inside the build/ directory.
