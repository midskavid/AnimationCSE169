#include "Milieu.h"

glm::vec3 Milieu::mGravity = { 0.0f, -9.8f, 0.0f };
glm::vec3 Milieu::mAirSpeed = { 0.0f, 0.0f, 100.0f };
float Milieu::mC = 0.0010;
float Milieu::mRho = 1.14;