#include "core/FrameLimiter.h"

FrameLimiter::FrameLimiter(double targetFPS)
    : m_targetFrameTime(targetFPS > 0.0 ? 1.0 / targetFPS : 0.0)
    , m_frameStart(glfwGetTime())
{
#ifdef _WIN32
    // Windows' default scheduler tick is ~15.6ms. std::this_thread::sleep_for()
    // is built on that tick, so without this call, "sleep for 3ms" can easily
    // turn into "sleep for 15ms" and blow straight through the frame budget.
    // Asking for 1ms resolution here makes the coarse-sleep phase in EndFrame()
    // land close enough that the spin phase only has to clean up ~1-2ms.
    // This is a process-wide setting, so it's set once for the app's lifetime
    // and undone in the destructor -- don't construct more than one of these.
    timeBeginPeriod(1);
#endif
    // No Linux/Hyprland equivalent needed: std::this_thread::sleep_for() there
    // resolves to clock_nanosleep(CLOCK_MONOTONIC), which typically wakes within
    // tens of microseconds on a stock kernel -- already finer than we need.
    // Hyprland being a Wayland compositor doesn't change this: it's a kernel
    // timer, not something the compositor mediates.
}

FrameLimiter::~FrameLimiter() {
#ifdef _WIN32
    timeEndPeriod(1);
#endif
}

void FrameLimiter::SetTargetFPS(double fps) {
    m_targetFrameTime = fps > 0.0 ? 1.0 / fps : 0.0;
}

float FrameLimiter::BeginFrame() {
    double now = glfwGetTime();
    float deltaTime = (float)(now - m_frameStart);
    m_frameStart = now;
    return deltaTime;
}

void FrameLimiter::EndFrame() {
    if (m_targetFrameTime <= 0.0) {
        return; // uncapped
    }

    double elapsed = glfwGetTime() - m_frameStart;
    double remaining = m_targetFrameTime - elapsed;
    if (remaining <= 0.0) {
        return; // frame already ran over budget; don't make it worse
    }

    // Phase 1: coarse sleep for most of the remaining time. This is where
    // the CPU actually gets to idle/yield to other processes -- on both
    // Windows and Linux this is the power/heat-saving part.
    double spinUntil = remaining - SPIN_THRESHOLD;
    if (spinUntil > 0.0) {
        std::this_thread::sleep_for(std::chrono::duration<double>(spinUntil));
    }

    // Phase 2: spin for the last sliver. yield() still lets the scheduler
    // run other ready threads between checks, so this isn't a hard 100%
    // core pin -- but it does poll the clock tightly enough to land within
    // microseconds of the target on both platforms.
    while (glfwGetTime() - m_frameStart < m_targetFrameTime) {
        std::this_thread::yield();
    }
}
