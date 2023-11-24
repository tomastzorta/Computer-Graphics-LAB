#include "AnimationManager.h"
#include <GLM/gtc/matrix_transform.hpp>

AnimationManager::AnimationManager()
{
    m_cube1Angle = 0.0f;
    m_cube2Angle = 0.0f;
    m_animateCentreCube = true;
    m_animateLight = true;
    m_modelMatrixCube2 = glm::scale(glm::translate(glm::mat4(1.0f),glm::vec3(1.0f,0.0f,0.0f)),glm::vec3(0.1f,0.1f,0.1f));
    m_modelMatrixCube3 = glm::scale(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-1.0f,0.0f)),glm::vec3(2.0f,0.1f,2.0f));

}

AnimationManager::~AnimationManager()
{
    
}

void AnimationManager::Update(float a_deltaTs)
{
    if (m_animateCentreCube)
    {
        m_cube1Angle += a_deltaTs * 0.5f;
        // Limit the loop to 360 degrees
        // This will prevent numerical inaccuracies
        while( m_cube1Angle > (3.14159265358979323846 * 2.0) )
        {
            m_cube1Angle -= (3.14159265358979323846 * 2.0);
        }
    }

    if (m_animateLight)
    {
        m_cube2Angle += a_deltaTs * 2.0f;
        while (m_cube2Angle > (3.14159265358979323846 * 2.0))
        {
            m_cube2Angle -= (3.14159265358979323846 * 2.0);
        }
    }

    // Update model matrices
    
    m_modelMatrixCube1 = glm::rotate( glm::mat4(1.0f), m_cube1Angle, glm::vec3(0,1,0) );
    m_modelMatrixCube2 = glm::rotate(glm::mat4(1.0f), m_cube2Angle, glm::vec3(0, 1, 0));
    m_modelMatrixCube2 = glm::translate(m_modelMatrixCube2, glm::vec3(1, 0, 0));
    m_modelMatrixCube2 = glm::scale(m_modelMatrixCube2, glm::vec3(0.1f, 0.1f, 0.1f));
}

