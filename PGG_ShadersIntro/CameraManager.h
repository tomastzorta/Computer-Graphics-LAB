#pragma once
#include <GLM/glm.hpp>

class CameraManager {
public:
    CameraManager();
    ~CameraManager();

    // Getters and setters
    glm::mat4 GetViewMatrix() const { return m_viewMatrix; }
    glm::mat4 GetProjMatrix() const { return m_projMatrix; }
    float GetCameraAngleX() const { return m_cameraAngleX; }
    float GetCameraAngleY() const { return m_cameraAngleY; }
    void SetCameraAngleX(float value) { m_cameraAngleX = value; }
    void SetCameraAngleY(float value) { m_cameraAngleY = value; }
    
private:
    glm::mat4 m_viewMatrix, m_projMatrix;
    float m_cameraAngleX, m_cameraAngleY;
};
