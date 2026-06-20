#pragma once 

class FrameLimit {
public:
    double cTargetFrameTime;
    double cFrameStart;


    FrameLimit(float pTargetFPS);
    ~FrameLimit();

    void SetTargetFPS(float pTargetFPS);
    float StartFrame();
    void EndFrame();        // sleep here for whatever's left for the frame time

};
