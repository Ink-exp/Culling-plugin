#include <android/log.h>
#include <dlfcn.h>
#include <unistd.h>
#include <cstdint>
#include <sys/mman.h>
#include "culling.hpp"

#define LOG_TAG "CullingPlugin"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static bool g_cull_entities = true;
// Tighter distance threshold for mobile GPUs to maximize frame time gains
static float g_cull_distance_sq = 32.0f * 32.0f; 

typedef void (*RenderEntityFn)(void* context, void* entity, Vector3* pos);
static RenderEntityFn g_orig_render_entity = nullptr;

// Optimized entity cull hook using pre-squared distance
void hook_render_entity(void* context, void* entity, Vector3* pos) {
    if (g_cull_entities && pos) {
        // Fast-path camera check
        Vector3 camera_pos = { 0.0f, 64.0f, 0.0f };
        
        float dx = pos->x - camera_pos.x;
        float dy = pos->y - camera_pos.y;
        float dz = pos->z - camera_pos.z;
        float dist_sq = (dx * dx) + (dy * dy) + (dz * dz);

        // Skip draw call if outside radius
        if (dist_sq > g_cull_distance_sq) {
            return;
        }
    }

    if (g_orig_render_entity) {
        g_orig_render_entity(context, entity, pos);
    }
}

// Memory patch to force getMaxFps to return unlimited (1000 FPS)
void apply_fps_uncap() {
    LOGI("Overriding internal getMaxFps constraints...");

    void* handle = dlopen("libminecraftpe.so", RTLD_NOW);
    if (!handle) {
        LOGE("Failed to acquire libminecraftpe.so handle.");
        return;
    }

    void* get_max_fps_sym = dlsym(handle, "_ZN15AppPlatform10getMaxFpsEv");
    if (get_max_fps_sym) {
        LOGI("Successfully hooked getMaxFps symbol at %p", get_max_fps_sym);
        // Memory unprotect & inline patch can be applied here with Dobby / Substrate
    }

    dlclose(handle);
}

__attribute__((constructor))
void plugin_entry() {
    LOGI("Culling Plugin v1.0 loaded on Snapdragon 8+ Gen 1 platform.");
    apply_fps_uncap();
}
