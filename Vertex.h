#ifndef VERTEX_H
#define VERTEX_H

#include "Core.h"

class Vertex {
public:
	glm::vec3 GetPosition() const;
	glm::vec3 GetNormal() const;

	void SetPostion(glm::vec3& pos);
	void SetNormal(glm::vec3& nor);
private:
	glm::vec3 mPosition;
	glm::vec3 mNormal;
};
#endif // !VERTEX_H

