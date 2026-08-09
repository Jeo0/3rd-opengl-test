#pragma once 
// https://stackoverflow.com/questions/14968857/controlling-the-fps-limit-in-an-opengl-application


class FrameLimit {
public:
    double c_targetFrameTime;
    double c_frameStart;


    FrameLimit(float p_targetFPS);
    ~FrameLimit();

    void SetTargetFPS(float p_targetFPS);
    float StartFrame();
    void EndFrame();        // sleep here for whatever's left for the frame time

};
