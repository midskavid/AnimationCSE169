////////////////////////////////////////
// SpinningCube.cpp
////////////////////////////////////////

#include "SpinningCube.h"

////////////////////////////////////////////////////////////////////////////////

SpinningCube::SpinningCube() {
	// Initialize constant data
	Position=glm::vec3(0,0,0);
	Axis=glm::vec3(0,1,0);		// Y-Axis
	CubeModel.MakeBox(glm::vec3(-1),glm::vec3(1));
	SpinDelta=0.001f;

	// Resets variable data
	Reset();
}

////////////////////////////////////////////////////////////////////////////////

void SpinningCube::Update() {
	// Update (animate) any variable properties
#if 1
	Angle+=SpinDelta;
	WorldMtx = glm::rotate(Angle, Axis);
	WorldMtx[3]=glm::vec4(Position,1);
#else
	Angle += SpinDelta;
	WorldMtx = glm::translate(-Position);
	WorldMtx = glm::rotate(Angle, Axis)*WorldMtx;
	WorldMtx = glm::translate(Position)*WorldMtx;

#endif
}

////////////////////////////////////////////////////////////////////////////////

void SpinningCube::Reset() {
	// Reset dynamic variables to a default state
	Angle=0;
	WorldMtx=glm::mat4(1.0);		// Reset to identity matrix
}

////////////////////////////////////////////////////////////////////////////////

void SpinningCube::Draw(const glm::mat4 &viewProjMtx,uint shader) {
	CubeModel.Draw(WorldMtx,viewProjMtx,shader);
}

////////////////////////////////////////////////////////////////////////////////
