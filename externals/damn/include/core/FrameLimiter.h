#pragma once
#include "GLFW/glfw3.h"
#include <thread>
#include <chrono>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

// Owns frame pacing for the main loop: measures real deltaTime and,
// if a target FPS is set, sleeps off whatever time is left in a frame's
// budget. This is the only class in the project that touches a sleep
// primitive or an OS-specific timer call -- everything else (Window,
// Camera, Core's render code) stays platform-agnostic.
//
// Usage in Core::Run():
//   while (!glfwWindowShouldClose(windowObj.ID)) {
//       float dt = limiter.BeginFrame();
//       Update(dt);
//       Render();
//       glfwPollEvents();
//       limiter.EndFrame();
//   }
class FrameLimiter {
public:
    // targetFPS <= 0 means uncapped (sleep is skipped entirely).
    explicit FrameLimiter(double targetFPS = 0.0);
    ~FrameLimiter();

    FrameLimiter(const FrameLimiter&) = delete;
    FrameLimiter& operator=(const FrameLimiter&) = delete;

    void SetTargetFPS(double fps);

    // Call once at the very top of the loop. Returns the wall-clock
    // seconds since the previous BeginFrame() call -- i.e. the actual,
    // already-capped duration of the previous frame. Feed this straight
    // into Update()/Camera so movement stays speed-correct regardless
    // of what the cap or vsync did to frame time.
    float BeginFrame();

    // Call once at the bottom of the loop, after Render() and
    // glfwPollEvents(). Sleeps off the remainder of this frame's
    // budget, if any is left.
    void EndFrame();

private:
    double m_targetFrameTime; // seconds; 0 = uncapped
    double m_frameStart;      // time BeginFrame() was last called

    // How close to the deadline we stop sleeping and start spinning.
    // No OS sleep call -- Sleep() on Windows, nanosleep() on Linux --
    // is guaranteed to wake up exactly on time, only "at or after."
    // Spinning the last couple of milliseconds trades a small amount
    // of CPU for landing on the target instead of overshooting it.
    static constexpr double SPIN_THRESHOLD = 0.002; // 2ms
};
