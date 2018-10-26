#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball = flipper = NULL;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	ball = App->textures->Load("Assets/ball.png");
	flipper = App->textures->Load("Assets/flippers.png");

	//Flippers

	b2RevoluteJointDef joint;


	leftFlipper = App->physics->CreateRectangle(150, 485, 48, 30, 32 * DEGTORAD);
	leftPivot = App->physics->CreateCircle(150, 485, 15, b2_staticBody);
	leftFlipper->body->SetGravityScale(0.0f);

	joint.bodyA = leftFlipper->body;
	joint.bodyB = leftPivot->body;

	joint.localAnchorA.Set(PIXEL_TO_METERS(-25), 0);		// Set the pivot point of the rectangle where the center of the circle is
	joint.localAnchorB.Set(0, 0);						// Set the pivot point of the circle on its center
	joint.collideConnected = false;

	joint.upperAngle = 35 * DEGTORAD;			// Angle limits
	joint.lowerAngle = -35 * DEGTORAD;
	joint.enableLimit = true;

	joint.motorSpeed = 1500.0f * DEGTORAD;		// Motor
	joint.maxMotorTorque = 1500;
	joint.enableMotor = false;

	leftJoint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&joint);


	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	App->textures->Unload(ball);
	App->textures->Unload(flipper);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	//Inputs
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		leftJoint->EnableMotor(true);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		leftJoint->EnableMotor(false);
	}

	return UPDATE_CONTINUE;
}



