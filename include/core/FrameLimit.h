#pragma once 
// https://stackoverflow.com/questions/14968857/controlling-the-fps-limit-in-an-opengl-application


class FrameLimit {
public:
    double c_targetFrameTime;
    double c_frameStart;
    float c_targetFPS = 0.0;


    FrameLimit(float p_targetFPS);
    ~FrameLimit();

    float GetTargetFPS();
    void SetTargetFPS(float p_targetFPS);
    float StartFrame();
    void EndFrame();        // sleep here for whatever's left for the frame time

};
