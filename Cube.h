# ifndef CUBE_H
# define CUBE_H

#include "Model.h"

////////////////////////////////////////////////////////////////////////////////

class Cube {
public:
	Cube(glm::vec3& boxMin, glm::vec3& boxMax, uint shader);

	void Draw(const glm::mat4 &viewProjMtx, uint shader);

private:
	// Constants
	Model CubeModel;
	glm::vec3 Position;
	glm::vec3 Axis;

	// Variables
	float Angle;
};

#endif // !CUBE_H

