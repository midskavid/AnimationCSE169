#include "Vertex.h"

glm::vec3 Vertex::GetPosition() const
{
	return mPosition;
}

void Vertex::SetPostion(glm::vec3& pos)
{
	mPosition = pos;
}

glm::vec3 Vertex::GetNormal() const
{
	return mNormal;
}

void Vertex::SetNormal(glm::vec3 & nor)
{
	mNormal = nor;
}
