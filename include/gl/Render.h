
#pragma once


#include <memory>
#include <vector>

#include "Camera.h"
#include "Model.h"
#include "shaderClass.h"

using SceneObjects = std::vector<std::unique_ptr<Model>>;


class RenderPass {
public:
    virtual ~RenderPass() = default;

    // any derived classes (say object, or an overlay ting) will need to have its own implementation
    // unless u want it to be 0
    virtual void Draw( const SceneObjects& p_objects, Camera& p_camera) = 0;

    bool c_enabled = true;
};


class PointOverlayPass final : public RenderPass {
public:
    explicit PointOverlayPass(Shader& p_shader);

    void Draw(const SceneObjects& p_objects, Camera& p_camera) override;

    float c_size = 6.0f;
    glm::vec3 c_color{1.0f};
    bool c_xray = false;

private:
    Shader& c_shader;   // each scene can now have its own shader
                        // just make sure this render pass do not delete dis
};


class SurfacePass final : public RenderPass {
public:
    explicit SurfacePass(Shader& p_shader);

    void Draw(const SceneObjects& p_objects, Camera& p_camera) override;

    float c_size = 6.0f;
    glm::vec3 c_color{1.0f};
    bool c_xray = false;

    c_

private:
    Shader& c_shader;   // each scene can now have its own shader
                        // just make sure this render pass do not delete dis
};

