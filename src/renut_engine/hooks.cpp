#include <atomic>
#include <chrono>
#include <thread>
#include <cstdint> // For uintptr_t
#include <rex/cvar.h>
#include <rex/ppc/types.h>
#include <rex/system/kernel_state.h>
#include "rex_macros.h"
#include "globals.h"
#include <rex/logging.h>



namespace renut::log {
  inline const rex::LogCategoryId Input = rex::RegisterLogCategory("renut_hooks");
}

#define RNUT_TRACE(...) REXLOG_CAT_TRACE(::renut::log::Input, __VA_ARGS__)
#define RNUT_DEBUG(...) REXLOG_CAT_DEBUG(::renut::log::Input, __VA_ARGS__)
#define RNUT_INFO(...)  REXLOG_CAT_INFO(::renut::log::Input, __VA_ARGS__)
#define RNUT_WARN(...)  REXLOG_CAT_WARN(::renut::log::Input, __VA_ARGS__)
#define RNUT_ERROR(...) REXLOG_CAT_ERROR(::renut::log::Input, __VA_ARGS__)

// Name = "Showdown Town Vehicles"
REXCVAR_DEFINE_BOOL(overworld_vehicles, false, "Nuts&Bolts", "Enables Overworld Vehicles");
// Name = "No Notes Spent"
REXCVAR_DEFINE_BOOL(no_notes_spent, false, "Nuts&Bolts", "hook created by serenity");
// Name = "VSync Mode"
REXCVAR_DEFINE_INT32(vsync, 0, "Nuts&Bolts", "Immediate (0) 60Hz (1) 30Hz (2) VSync = 30Hz, Threshold 20 (7)")
.range(0, 7)
.lifecycle(rex::cvar::Lifecycle::kRequiresRestart);
// Name = "Disable LOD"
REXCVAR_DEFINE_BOOL(disable_lod, false, "Nuts&Bolts", "Disables LOD (Level of Detail) scaling");
// Name = "Infinite Fuel and Ammo"
REXCVAR_DEFINE_BOOL(infinite_fuel_and_ammo, false, "Nuts&Bolts", "fuel never decreases");
// Name = "Infinite Health"
REXCVAR_DEFINE_BOOL(infinite_health, false, "Nuts&Bolts", "health never decreases");





inline int bWidth = 640;
inline int bHeight = 480;
auto frameTime = std::chrono::system_clock::now();
int frame = 0;

bool overworld_vehicles_hook() {
    if (REXCVAR_GET(overworld_vehicles)) {
        return true;
    }
    return false;
}

bool no_notes_spent() {
    if (REXCVAR_GET(no_notes_spent)) {
        return true;
    }
    return false;
}

void fps_hook(PPCRegister& r3) {

    r3.u64 = REXCVAR_GET(vsync);
}

void fpsCount_hook() {
    frame++;
    auto Time = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> delta = Time - frameTime;
    frameTime = Time;
    double fpsfromMS = 1000 / delta.count();
    if (frame >= 60) {
        frame = 0;
        fpsCount = fpsfromMS;
    }
}


bool meGetResolutionParams_hook(PPCRegister& r5, PPCRegister& r6) {
    // r5.u32 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&bWidth));
     //r6.u32 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&bHeight));
    //if (REXCVAR_GET(lowres)) {
    //
    //    return true;
    //}
    return false;
}

void Optimization_Hook() {
    std::this_thread::yield();
}

bool disable_lod() {
    return REXCVAR_GET(disable_lod);
}

void Infinite_fuel_and_ammo() {
    if (REXCVAR_GET(infinite_fuel_and_ammo)) {
        return;
    }
}

REX_PPC_EXTERN_IMPORT(gameFlagsSetFlag_82363ED0);

inline bool hasStopNSwap_hook_Ran = false;

void StopNSwap_hook(PPCRegister& r22)
{
    int a1 = r22.u32 - 76;  // profile pointer
    
    if(hasStopNSwap_hook_Ran) {
        return;
    }
    hasStopNSwap_hook_Ran = true;


    // Force all Stop-N-Swap flags
    uint8_t* base = reinterpret_cast<uint8_t*>(0x100000000);

    // Read the guest pointer at *(a1 + 92) — big-endian byte swap
    uint32_t flags_ptr = _byteswap_ulong(*reinterpret_cast<uint32_t*>(base + a1 + 92));

    RNUT_ERROR("Profile pointer: 0x{:X}", a1);
    RNUT_ERROR("Flags pointer: 0x{:X}", flags_ptr);

    // Set byte flags and call gameFlagsSetFlag with the dereferenced pointer
    *(base + flags_ptr + 236) = 1;
    REX_PPC_INVOKE2(void, gameFlagsSetFlag_82363ED0, flags_ptr, 1, 0x10E, 1);
    REX_PPC_INVOKE2(void, gameFlagsSetFlag_82363ED0, flags_ptr, 0, 0x703, 1);

    *(base + flags_ptr + 237) = 1;
    REX_PPC_INVOKE2(void, gameFlagsSetFlag_82363ED0, flags_ptr, 1, 0x110, 1);
    REX_PPC_INVOKE2(void, gameFlagsSetFlag_82363ED0, flags_ptr, 0, 0x705, 1);

    *(base + flags_ptr + 238) = 1;
    REX_PPC_INVOKE2(void, gameFlagsSetFlag_82363ED0, flags_ptr, 1, 0x10D, 1);
    REX_PPC_INVOKE2(void, gameFlagsSetFlag_82363ED0, flags_ptr, 0, 0x702, 1);

    *(base + flags_ptr + 239) = 1;
    REX_PPC_INVOKE2(void, gameFlagsSetFlag_82363ED0, flags_ptr, 1, 0x10C, 1);
    REX_PPC_INVOKE2(void, gameFlagsSetFlag_82363ED0, flags_ptr, 0, 0x701, 1);

    *(base + flags_ptr + 240) = 1;
    REX_PPC_INVOKE2(void, gameFlagsSetFlag_82363ED0, flags_ptr, 1, 0x10B, 1);
    REX_PPC_INVOKE2(void, gameFlagsSetFlag_82363ED0, flags_ptr, 0, 0x700, 1);

    *(base + flags_ptr + 241) = 1;
    REX_PPC_INVOKE2(void, gameFlagsSetFlag_82363ED0, flags_ptr, 1, 0x10F, 1);
    REX_PPC_INVOKE2(void, gameFlagsSetFlag_82363ED0, flags_ptr, 0, 0x704, 1);

    *(base + flags_ptr + 242) = 1;
    REX_PPC_INVOKE2(void, gameFlagsSetFlag_82363ED0, flags_ptr, 1, 0x111, 1);
    REX_PPC_INVOKE2(void, gameFlagsSetFlag_82363ED0, flags_ptr, 0, 0x706, 1);

}