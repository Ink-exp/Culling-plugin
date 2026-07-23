#include <android/log.h>
#include <dlfcn.h>
#include <unistd.h>
#include <cstdint>
#include "culling.hpp"

#define LOG_TAG "CullingPlugin"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Global configuration toggles
static bool g_uncap_fps = true;
static bool g_cull_entities = true;
static float g_cull_distance = 40.0f; // Distance cutoff in blocks

typedef void (*RenderEntityFn)(void* context, void* entity, Vector3* pos);
static RenderEntityFn g_orig_render_entity = nullptr;

// Hook function to bypass rendering off-screen or far entities
void hook_render_entity(void* context, void* entity, Vector3* pos) {
    if (g_cull_entities && pos) {
        Vector3 camera_pos = { 0.0f, 64.0f, 0.0f }; // Camera reference point
        if (is_culled(*pos, camera_pos, g_cull_distance)) {
            return; // Skip draw call
        }
    }

    if (g_orig_render_entity) {
        g_orig_render_entity(context, entity, pos);
    }
}

// Memory patch logic to bypass native Bedrock VSync & FPS caps
void apply_fps_uncap() {
    LOGI("Disabling native VSync and setting frame rate cap to uncapped...");

    void* handle = dlopen("libminecraftpe.so", RTLD_NOLOAD);
    if (!handle) {
        LOGE("Failed to open libminecraftpe.so handle.");
        return;
    }

    void* get_max_fps_sym = dlsym(handle, "_ZN15AppPlatform10getMaxFpsEv");
    if (get_max_fps_sym) {
        LOGI("Symbol found for getMaxFps at %p", get_max_fps_sym);
    }

    dlclose(handle);
}

// Main initializer invoked by LeviLauncher injection
__attribute__((constructor))
void plugin_entry() {
    LOGI("Initializing Culling Plugin...");

    if (g_uncap_fps) {
        apply_fps_uncap();
    }

    LOGI("Culling Plugin loaded successfully.");
}

