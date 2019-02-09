////////////////////////////////////////
// Tester.h
////////////////////////////////////////

#pragma once

#include "Shader.h"
#include "Camera.h"
#include "SpinningCube.h"
#include "Skeleton.h"
#include "Skin.h"
#include "Joint.h"
#include "Player.h"
#include "AnimationClip.h"

////////////////////////////////////////////////////////////////////////////////

// The 'Tester' is a simple top level application class. It creates and manages a
// window with the GLUT extension to OpenGL and it maintains a simple 3D scene
// including a camera and some other components.

class Tester {
public:
	Tester(const char *windowTitle,int argc,char **argv);
	~Tester();

	void Update();
	void Reset();
	void Draw();

	void Quit();

	// Event handlers
	void Resize(int x,int y);
	void Keyboard(int key,int x,int y);
	void MouseButton(int btn,int state,int x,int y);
	void MouseMotion(int x,int y);

private:
	// Window management
	int WindowHandle;
	int WinX,WinY;

	// Input
	bool LeftDown,MiddleDown,RightDown;
	int MouseX,MouseY;

	//  Interaction 
	int JointId=0;
	float Change = 0.1f;
	// Components
	ShaderProgram *Program;
	ShaderProgram *ProgramSkin;
	SpinningCube *Cube;
	Skeleton *mSkel;
	Skin *mSkin;
	Camera *Cam;
	Player *mPlayer;
	AnimationClip *mAnimationClip;
};

////////////////////////////////////////////////////////////////////////////////
