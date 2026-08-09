#include "core/FrameLimit.h"
#include "GLFW/glfw3.h"
#include <thread>

FrameLimit::FrameLimit(float p_targetFPS) {
    c_frameStart = glfwGetTime();
    this->SetTargetFPS(p_targetFPS);
}
FrameLimit::~FrameLimit(){}


void FrameLimit::SetTargetFPS(float p_targetFPS) {
    if(p_targetFPS > 0.0)  c_targetFrameTime = 1.0 / p_targetFPS;
    else    c_targetFrameTime = 0.0;
}


float FrameLimit::StartFrame() {
    double mNow  = glfwGetTime();

    float mDeltaTime = (float)(mNow - c_frameStart);
    c_frameStart = mNow; 
    return mDeltaTime;
}


void FrameLimit::EndFrame() {
    if(c_targetFrameTime <= 0.0)
        return;     // uncapped FPS
    
    double mElapsed = glfwGetTime() - c_frameStart; 
    double mRemaining = c_targetFrameTime - mElapsed; 

    if(mRemaining <= 0.0)
        return; // the frame does not have any budget to sleep


    // instead of proceeding to render and process 
    // just go sleep
    // phase 1 ting: coarse sleep?
    double mSleepUntil = mRemaining - 0.002;
    if (mSleepUntil > 0.0) {
std::this_thread::sleep_for(std::chrono::duration<double>(mSleepUntil));
    }

    // phase 2: ???
    while(glfwGetTime() - c_frameStart < c_targetFrameTime) {
        std::this_thread::yield();
    }
}

