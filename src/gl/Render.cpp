
#include "gl/Render.h"
#include <GLAD/glad.h>
#include "glm/trigonometric.hpp"

PointOverlayPass::PointOverlayPass(Shader& p_shader)
    : c_shader(p_shader) {}

void PointOverlayPass::Draw(const SceneObjects& p_objects, Camera& p_camera) {
    c_shader.Activate();
    glUniform1f(glGetUniformLocation(c_shader.ID, "u_pointSize"), c_size);
    glUniform3f(glGetUniformLocation(c_shader.ID, "u_pointColor"),
            c_color.r, 
            c_color.g, 
            c_color.b);

    // world size of one pixel at distance 1 from the camera
    const float m_pixelAngle =
        2.0 * glm::tan(glm::radians(p_camera.c_fovDeg) * 0.5)
        / static_cast<float>(p_camera.c_height);
    glUniform1f(glGetUniformLocation(c_shader.ID, "u_pixelAngle"), m_pixelAngle);

    // remember both states before touching them, so they can be put back afterwards
    const GLboolean m_programPointSizeWasEnabled = glIsEnabled(GL_PROGRAM_POINT_SIZE);

    const GLboolean m_depthTestWasEnabled = glIsEnabled(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);    // lets point.vert set gl_PointSize
    if (c_xray) 
        glDisable(GL_DEPTH_TEST);

    for (const auto& ii_object : p_objects) {
        if (ii_object) {
            ii_object->DrawPoints(c_shader, p_camera);
        }
    }

    // restore the prior GL state instead of assuming it was enabled.
    if (m_depthTestWasEnabled == GL_TRUE) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    if (m_programPointSizeWasEnabled == GL_FALSE) {
        glDisable(GL_PROGRAM_POINT_SIZE);
    }
}


SurfacePass::SurfacePass(Shader& p_shader)
    : c_shader(p_shader) {}

void SurfacePass::Draw( const SceneObjects& p_objects, Camera& p_camera) {
    c_shader.Activate();

    glUniform1f(glGetUniformLocation(c_shader.ID, "u_edgeWidth"), c_edgeWidth);
    glUniform3f(
        glGetUniformLocation(c_shader.ID, "u_edgeColor"),
        c_edgeColor.r,
        c_edgeColor.g,
        c_edgeColor.b
    );


    // push the filled triangles very slightly away from the camera in the depth buffer,
    // so dots sitting exactly on the surface win the depth test instead of flickering.
    // preserve whether this state was enabled
    //
    // Offset filled triangles slightly in depth so the vertex overlay can be
    // drawn on top without z-fighting. Preserve whether this state was enabled.
    const GLboolean m_polygonOffsetWasEnabled = glIsEnabled(GL_POLYGON_OFFSET_FILL);
    GLfloat m_previousPolygonOffsetFactor = 0.0;
    GLfloat m_previousPolygonOffsetUnits = 0.0;
    glGetFloatv(GL_POLYGON_OFFSET_FACTOR, &m_previousPolygonOffsetFactor);
    glGetFloatv(GL_POLYGON_OFFSET_UNITS, &m_previousPolygonOffsetUnits);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);

    for (const auto& ii_object : p_objects) {
        if (ii_object) {
            ii_object->Draw(c_shader, p_camera);
        }
    }

    glPolygonOffset(m_previousPolygonOffsetFactor, m_previousPolygonOffsetUnits);
    if (m_polygonOffsetWasEnabled == GL_FALSE) {
        glDisable(GL_POLYGON_OFFSET_FILL);
    }
}
