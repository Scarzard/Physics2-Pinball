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
	//-----------THE PROBLEM MUST BE THAT JOINT NEEDS TO BE INICIALIZED AND THE JOINT AND FLIPPERBODY ARE CREATED IN A WRONG WAY------------ //
	//joint.Initialize(bodyA, bodyB, leftFlipper->body->GetWorldCenter())


	leftFlipper = App->physics->CreateRectangle(150, 485, 48, 30, 20 * DEGTORAD);   // Creates flipper to location
	leftPivot = App->physics->CreateCircle(150, 485, 7, b2_staticBody);				// Pivot is needed to rotate flipper
	leftFlipper->body->SetGravityScale(20.0f);

	joint.bodyA = leftFlipper->body;
	joint.bodyB = leftPivot->body;

	joint.localAnchorA.Set(PIXEL_TO_METERS(-15), PIXEL_TO_METERS(-4));		// Set pivot position correctly
	joint.localAnchorB.Set(0, 0);						
	joint.collideConnected = false;

	joint.upperAngle = 35 * DEGTORAD;		//Sets angle limits for the flipper
	joint.lowerAngle = -35 * DEGTORAD;
	joint.enableLimit = true;

	joint.motorSpeed = 1500.0f * DEGTORAD;	
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



	int posX, posY;


	leftFlipper->GetPosition(posX, posY);
	App->renderer->Blit(flipper, posX, posY, NULL, 1.0f, leftFlipper->GetRotation());

	return UPDATE_CONTINUE;
}



