////////////////////////////////////////
// Tester.cpp
////////////////////////////////////////

#include "Tester.h"
#include "Milieu.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
#define SPINNING_CUBE 0
#define CLOTHSIMULATION 0
#define CHAIN 1
#define TIMEPERIOD 100

static Tester *TESTER=0;

int main(int argc, char **argv) {
	glutInit(&argc, argv);
#if SPINNING_CUBE
	TESTER=new Tester("Spinning Cube",argc,argv);
#else
	TESTER = new Tester("Skeleton", argc, argv);
#endif
	glutMainLoop();
	delete TESTER;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(const char *windowTitle,int argc,char **argv) {
	WinX=800;
	WinY=600;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;

	// Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 100, 100 );
	WindowHandle = glutCreateWindow( windowTitle );
	glutSetWindowTitle( windowTitle );
	glutSetWindow( WindowHandle );

	// Background color
	glClearColor( 0., 0., 0., 1. );

	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );

	// Initialize GLEW
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Initialize components
	Program=new ShaderProgram("Model.glsl",ShaderProgram::eRender);
	ProgramSkin = new ShaderProgram("Model_Skin.glsl", ShaderProgram::eRender);
	ProgramCloth = new ShaderProgram("Model_Cloth.glsl", ShaderProgram::eRender);
#if SPINNING_CUBE
	Cube=new SpinningCube;
#else
#if CLOTHSIMULATION
	mCloth = std::make_unique<Cloth>();
	mCloth->InitCloth(20, 20);
	mCloth->Update();
#elif CHAIN
	mChain = std::make_unique<Chain>();
	mGoal = std::make_unique<Goal>();
	mChain->Init(5);
	mChain->Update();
#else
	mSkel = std::make_unique<Skeleton>();
	std::string filename = (argc > 3) ? argv[1] : "waspA.skel";
	mSkel->Load(filename);
	mSkin = std::make_unique<Skin>();
	filename = (argc > 3) ? argv[2] : "waspA.skin";
	mSkin->Load(filename);
	mAnimationClip = std::make_unique<AnimationClip>();
	filename = (argc > 3) ? argv[3] : "waspA.anim";
	mAnimationClip->Load(filename);
	mPlayer = std::make_unique<Player>(mSkel.get(), 1.0f/ TIMEPERIOD, mAnimationClip.get());
#pragma message("TODO : clean this.. shouldn't be here")
	mSkel->Update();
	mSkin->Update();
#endif

#endif
	Cam=new Camera;
	Cam->SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	delete Program;
	delete ProgramSkin;
	delete ProgramCloth;
#if SPINNING_CUBE
	delete Cube;
#endif
	delete Cam;

	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
#if SPINNING_CUBE
	Cube->Update();
#else
#if CLOTHSIMULATION
	mCloth->Update();
#elif CHAIN
	mChain->Update();
#else
	mPlayer->Animate();
	mSkel->Update();
	mSkin->Update();
#endif
#endif
	Cam->Update();

	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam->Reset();
	Cam->SetAspect(float(WinX)/float(WinY));
#if SPINNING_CUBE
	Cube->Reset();
#else
#pragma message("TODO : Complete Reset functionality")
#endif
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {
	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw components
#if SPINNING_CUBE
	Cube->Draw(Cam->GetViewProjectMtx(),Program->GetProgramID());
#else 
#if CLOTHSIMULATION
	mCloth->Draw(Cam->GetViewProjectMtx(), ProgramCloth->GetProgramID());
#elif CHAIN
	mChain->Draw(Cam->GetViewProjectMtx(), Program->GetProgramID());
	mGoal->Draw(Cam->GetViewProjectMtx(), Program->GetProgramID());
#else
	//mSkel->Draw(Cam->GetViewProjectMtx(), Program->GetProgramID());
	mSkin->Draw(Cam->GetViewProjectMtx(), ProgramSkin->GetProgramID());
#endif
#endif
	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam->SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;
#if CLOTHSIMULATION
		case 'v' :
			// Increase speed of wind here..
			Milieu::mAirSpeed.z += 1.0f;
			std::cout << "Wind Velocity : " << Milieu::mAirSpeed.x << " " << Milieu::mAirSpeed.y << " " << Milieu::mAirSpeed.z << std::endl;
			break;
		case 'b' : 
			// Decrease speed of wind here...
			Milieu::mAirSpeed.z -= 1.0f;
			std::cout << "Wind Velocity : " << Milieu::mAirSpeed.x << " " << Milieu::mAirSpeed.y << " " << Milieu::mAirSpeed.z << std::endl;
			break;
		case 'c' :
			Milieu::mAirSpeed.z = 0.0f;
			std::cout << "No wind!"<<std::endl;
			break;
		case 'n' :
			Milieu::mAirSpeed.z = 100.0f;
			std::cout << "Let there be wind!" << std::endl;
			break;
		case 'w' :
			// Rotate about y anticlock..
			for (const auto& pt : mCloth->mParticles) {
				if (pt->mIsFixed) {
					auto r = glm::length(pt->mPosition);
					pt->mPosition = glm::vec3(glm::cos(0.05f)*pt->mPosition.x - glm::sin(0.05f)*pt->mPosition.z, 0.0f, glm::sin(0.05f)*pt->mPosition.x + glm::cos(0.05f)*pt->mPosition.z);
				}
			}
			break;
		case 's':
			// Rotate about y anticlock..
			for (const auto& pt : mCloth->mParticles) {
				if (pt->mIsFixed) {
					auto r = glm::length(pt->mPosition);
					pt->mPosition = glm::vec3(glm::cos(-0.05f)*pt->mPosition.x - glm::sin(-0.05f)*pt->mPosition.z, 0.0f, glm::sin(-0.05f)*pt->mPosition.x + glm::cos(-0.05f)*pt->mPosition.z);
				}
			}
#elif CHAIN
		case 'q': {
			// Increase X
			mGoal->Update(glm::vec3{ Change, 0.0f,0.0f });
			mChain->mConverged = false;
			break;
		}
		case 'w': {
			// Increase Y
			mGoal->Update(glm::vec3{ 0.0f, Change, 0.0f });
			mChain->mConverged = false;
			break;
		}
		case 'e': {
			// Increase Z
			mGoal->Update(glm::vec3{ 0.0f,0.0f, Change });
			mChain->mConverged = false;
			break;
		}
		case 'a': {
			// Decrease X
			mGoal->Update(glm::vec3{ -Change, 0.0f,0.0f });
			mChain->mConverged = false;
			break;
		}
		case 's': {
			// Decrease Y
			mGoal->Update(glm::vec3{ 0.0f, -Change,0.0f });
			mChain->mConverged = false;
			break;
		}
		case 'd': {
			// Decrease Z
			mGoal->Update(glm::vec3{ 0.0f,0.0f, -Change });
			mChain->mConverged = false;
			break;
		}
#else
		case 'n' :
			// Next Joint..
			JointId = (JointId+1) % Skeleton::sOrderedJoints.size();
			std::cout << "Joint " << Skeleton::sOrderedJointName[JointId] << " selected.\n";
			Update();
			break;
		case 'b':
			// Next Joint..
			JointId = (JointId-1) % Skeleton::sOrderedJoints.size();
			std::cout << "Joint " << Skeleton::sOrderedJointName[JointId] << " selected.\n";
			Update();
			break;
		case 'q' : {
			// Increase X
			auto jt = Skeleton::sOrderedJoints[JointId];
			jt->mPose.x += Change;
			std::cout << "Current Pose : " << jt->mPose.x << " " << jt->mPose.y << " " << jt->mPose.z << std::endl;
			Update();
			break;
		}
		case 'w': {
			// Increase Y
			auto jt = Skeleton::sOrderedJoints[JointId];
			jt->mPose.y += Change;
			std::cout << "Current Pose : " << jt->mPose.x << " " << jt->mPose.y << " " << jt->mPose.z << std::endl;
			Update();
			break;
		}
		case 'e': {
			// Increase Z
			auto jt = Skeleton::sOrderedJoints[JointId];
			jt->mPose.z += Change;
			std::cout << "Current Pose : " << jt->mPose.x << " " << jt->mPose.y << " " << jt->mPose.z << std::endl;
			Update();
			break;
		}
		case 'a': {
			// Decrease X
			auto jt = Skeleton::sOrderedJoints[JointId];
			jt->mPose.x -= Change;
			std::cout << "Current Pose : " << jt->mPose.x << " " << jt->mPose.y << " " << jt->mPose.z << std::endl;
			Update();
			break;
		}
		case 's': {
			// Decrease Y
			auto jt = Skeleton::sOrderedJoints[JointId];
			jt->mPose.y -= Change;
			std::cout << "Current Pose : " << jt->mPose.x << " " << jt->mPose.y << " " << jt->mPose.z << std::endl;
			Update();
			break;
		}
		case 'd': {
			// Decrease Z
			auto jt = Skeleton::sOrderedJoints[JointId];
			jt->mPose.z -= Change;
			std::cout << "Current Pose : " << jt->mPose.x << " " << jt->mPose.y << " " << jt->mPose.z << std::endl;
			Update();
			break;
		}
#endif
		case 'r':
			Reset();
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int maxDelta=100;
	int dx = glm::clamp(nx - MouseX,-maxDelta,maxDelta);
	int dy = glm::clamp(-(ny - MouseY),-maxDelta,maxDelta);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam->SetAzimuth(Cam->GetAzimuth()+dx*rate);
		Cam->SetIncline(glm::clamp(Cam->GetIncline()-dy*rate,-90.0f,90.0f));
	}
	if(RightDown) {
		const float rate=0.005f;
		float dist=glm::clamp(Cam->GetDistance()*(1.0f-dx*rate),0.01f,1000.0f);
		Cam->SetDistance(dist);
	}
}

////////////////////////////////////////////////////////////////////////////////
