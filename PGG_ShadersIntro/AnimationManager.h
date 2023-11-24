#pragma once
#include <iostream>
#include <GLM/glm.hpp>

class AnimationManager
{
public:
    AnimationManager();
    ~AnimationManager();
    
    void Update(float a_deltaTime);

    // Getters and Setters for various scene object variables
    glm::mat4 GetModelMatrixCube1() const { return m_modelMatrixCube1; }
    glm::mat4 GetModelMatrixCube2() const { return m_modelMatrixCube2; }
    glm::mat4 GetModelMatrixCube3() const { return m_modelMatrixCube3; }

    void SetAnimateLight(bool value) { m_animateLight = value; }
    bool GetAnimateLight() const { return m_animateLight; }

    void SetLightAngle(float value) { m_cube2Angle = value; }
    float GetLightAngle() const { return m_cube2Angle; }

    void SetAnimateCube(bool value) { m_animateCentreCube = value; }
    bool GetAnimateCube() const { return m_animateCentreCube; }

    void SetCube1Angle(float value) { m_cube1Angle = value; }
    float GetCube1Angle() const { return m_cube1Angle; }
    void SetCube2Angle(float value) { m_cube2Angle = value; }
    float GetCube2Angle() const { return m_cube2Angle; }
    
    
    

private:
    glm::mat4 m_modelMatrixCube1, m_modelMatrixCube2, m_modelMatrixCube3;
    float m_cube1Angle, m_cube2Angle;
    bool m_animateCentreCube, m_animateLight;
};
