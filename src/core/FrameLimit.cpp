#include "core/FrameLimit.h"
#include "GLFW/glfw3.h"
#include <thread>

FrameLimit::FrameLimit(float pTargetFPS) {
    cFrameStart = glfwGetTime();
    this->SetTargetFPS(pTargetFPS);
}
FrameLimit::~FrameLimit(){}


void FrameLimit::SetTargetFPS(float pTargetFPS) {
    if(pTargetFPS > 0.0)  cTargetFrameTime = 1.0 / pTargetFPS;
    else    cTargetFrameTime = 0.0;
}


float FrameLimit::StartFrame() {
    double mNow  = glfwGetTime();

    float mDeltaTime = (float)(mNow - cFrameStart);
    cFrameStart = mNow; 
    return mDeltaTime;
}


void FrameLimit::EndFrame() {
    if(cTargetFrameTime <= 0.0)
        return;     // uncapped FPS
    
    double mElapsed = glfwGetTime() - cFrameStart; 
    double mRemaining = cTargetFrameTime - mElapsed; 

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
    while(glfwGetTime() - cFrameStart < cTargetFrameTime) {
        std::this_thread::yield();
    }
}

