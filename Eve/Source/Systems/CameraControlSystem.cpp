#include "CameraControlSystem.hpp"

extern eve::Coordinator gCoordinator;

namespace eve
{
    void CameraControlSystem::Init(){

    };

    void CameraControlSystem::Update(float left, float right, float bottom, float top, float near, float far)
    {
        float x_orth = 2 / (right - left);
        float y_orth = 2 / (top - bottom);
        float z_orth = -2 / (far - near);

        float tx = -(right + left) / (right - left);
        float ty = -(top + bottom) / (top - bottom);
        float tz = -(far + near) / (far - near);

        projectionMatrix[0][0] = x_orth;
        projectionMatrix[1][0] = 0;
        projectionMatrix[2][0] = 0;
        projectionMatrix[3][0] = 0;
        projectionMatrix[0][1] = 0;
        projectionMatrix[1][1] = y_orth;
        projectionMatrix[2][1] = 0;
        projectionMatrix[3][1] = 0;
        projectionMatrix[0][2] = 0;
        projectionMatrix[1][2] = 0;
        projectionMatrix[2][2] = z_orth;
        projectionMatrix[3][2] = 0;
        projectionMatrix[0][3] = tx;
        projectionMatrix[1][3] = ty;
        projectionMatrix[2][3] = tz;
        projectionMatrix[3][3] = 1;
    };
}