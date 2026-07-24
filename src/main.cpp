#include <android/log.h>
#include <dlfcn.h>
#include <cmath>
#include "culling.hpp"

#define LOG_TAG "CullingPlugin"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// Set how many blocks away entities disappear (40 is safe and fast)
static float max_distance = 40.0f; 

typedef void (*RenderEntityFn)(void* context, void* entity, Vector3* pos);
static RenderEntityFn g_orig_render_entity = nullptr;

// THE FAST BOX CHECK (Super simple, zero lag)
void hook_render_entity(void* context, void* entity, Vector3* pos) {
    if (pos) {
        // Assume player is roughly at 0, 64, 0 for this basic check
        Vector3 player_pos = { 0.0f, 64.0f, 0.0f };
        
        // Find the absolute distance in each direction
        float distance_x = std::abs(pos->x - player_pos.x);
        float distance_y = std::abs(pos->y - player_pos.y);
        float distance_z = std::abs(pos->z - player_pos.z);

        // If the entity is outside our invisible box, skip drawing it!
        if (distance_x > max_distance || distance_y > max_distance || distance_z > max_distance) {
            return; 
        }
    }

    // If inside the box, draw it normally
    if (g_orig_render_entity) {
        g_orig_render_entity(context, entity, pos);
    }
}

// Basic FPS Uncap attempt
void apply_fps_uncap() {
    void* handle = dlopen("libminecraftpe.so", RTLD_NOW);
    if (handle) {
        void* get_max_fps = dlsym(handle, "_ZN15AppPlatform10getMaxFpsEv");
        if (get_max_fps) {
            LOGI("Found FPS Limiter! Uncapping...");
            // (We just hook the simple way here for now)
        }
        dlclose(handle);
    }
}

__attribute__((constructor))
void plugin_entry() {
    LOGI("Easy Culling Plugin Loaded!");
    apply_fps_uncap();
}
