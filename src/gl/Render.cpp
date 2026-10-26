
#include "gl/Render.h"
PointOverlayPass::PointOverlayPass(Shader& p_shader)
    : c_shader(p_shader) {}

void PointOverlayPass::Draw(const SceneObjects& p_objects, Camera& p_camera) {
    c_shader.Activate();

    glUniform1f(glGetUniformLocation(c_shader.ID, "u_pointSize"), c_size);

    glUniform3f(glGetUniformLocation(c_shader.ID, "u_pointColor"),
            c_color.r, c_color.g, c_color.b);

    const float pixelAngle =
        2.0f * glm::tan(glm::radians(p_camera.c_fovDeg) * 0.5f)
        / static_cast<float>(p_camera.c_height);

    glUniform1f(glGetUniformLocation(c_shader.ID, "u_pixelAngle"), pixelAngle);

    const GLboolean depthWasEnabled = glIsEnabled(GL_DEPTH_TEST);

    if (c_xray) {
        glDisable(GL_DEPTH_TEST);
    }

    for (const auto& object : p_objects) {
        object->DrawPoints(c_shader, p_camera);
    }

    if (c_xray) {
        if (depthWasEnabled) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
    }
}

SurfacePass::SurfacePass(Shader& p_shader)
    : c_shader(p_shader) {}

void SurfacePass::Draw(
    const SceneObjects& p_objects,
    Camera& p_camera
) {
    c_shader.Activate();

    glUniform1f(
        glGetUniformLocation(c_shader.ID, "u_edgeWidth"),
        c_edgeWidth
    );

    glUniform3f(
        glGetUniformLocation(c_shader.ID, "u_edgeColor"),
        c_edgeColor.r,
        c_edgeColor.g,
        c_edgeColor.b
    );

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);

    for (const auto& object : p_objects) {
        if (object) {
            object->Draw(c_shader, p_camera);
        }
    }

    glDisable(GL_POLYGON_OFFSET_FILL);
}
