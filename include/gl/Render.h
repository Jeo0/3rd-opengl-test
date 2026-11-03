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

    virtual const char* GetName() const = 0;
    virtual void DrawUI() {}            // optional knobs, default none

    bool c_enabled = true;
};


class PointOverlayPass final : public RenderPass {
public:
    explicit PointOverlayPass(Shader& p_shader);

    const char* GetName() const override {
        return "PointOverlayPass";
    }

    void Draw(const SceneObjects& p_objects, Camera& p_camera) override;

    // defaults match the shader-debug-view
    float c_size = 10.0f;                       // px, - and =
    glm::vec3 c_color{1.0f, 0.85f, 0.1f};
    bool c_xray = false;                        // X, dots visible through the surface
    // float c_size = 6.0f;
    // glm::vec3 c_color{1.0f};
    // bool c_xray = false;

private:
    Shader& c_shader;   // each scene can now have its own shader
                        // just make sure this render pass do not delete dis
};


class SurfacePass final : public RenderPass {
public:
    explicit SurfacePass(Shader& p_shader);

    const char* GetName() const override {
        return "SurfacePass";
    }

    void Draw(const SceneObjects& p_objects, Camera& p_camera) override;
    void DrawUI() override;

    // wireframe settings are owned by the pass that uses them.
    float c_edgeWidth = 2.0f;  // px, [ and ]; 0 disables the wireframe
    glm::vec3 c_edgeColor{0.05f, 0.05f, 0.05f};

    // 
    static constexpr float c_edgeWidthMin = 0.0f;
    static constexpr float c_edgeWidthMax = 20.0f;

private:
    Shader& c_shader;   // each scene can now have its own shader
                        // just make sure this render pass do not delete dis
};

