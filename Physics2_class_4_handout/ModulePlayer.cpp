#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball_texture = flipper = NULL;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	ball_texture = App->textures->Load("Assets/ball.png");
	flipper = App->textures->Load("Assets/flippers.png");

	//Left flipper

	b2RevoluteJointDef joint;
	leftFlipper = App->physics->CreateRectangle(150, 485, 48, 10, 20 * DEGTORAD, b2_dynamicBody);   // Creates flipper to location
	leftPivot = App->physics->CreateCircle(153, 480, 7, b2_staticBody);				// Pivot is needed to rotate flipper
	leftFlipper->body->SetGravityScale(20.0f);

	joint.bodyA = leftFlipper->body;
	joint.bodyB = leftPivot->body;
	joint.Initialize(joint.bodyA, joint.bodyB, leftFlipper->body->GetWorldCenter());


	joint.localAnchorA.Set(PIXEL_TO_METERS(-20), PIXEL_TO_METERS(-4));		// Set flipper position correctly
	joint.localAnchorB.Set(0, 0);
	joint.collideConnected = false;

	joint.upperAngle = 45 * DEGTORAD;		//Sets angle limits for the flipper
	joint.lowerAngle = 0 * DEGTORAD;
	joint.enableLimit = true;

	joint.motorSpeed = 1500.0f * DEGTORAD;
	joint.maxMotorTorque = 1500;
	joint.enableMotor = false;

	leftJoint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&joint);

	//Right flippers
	b2RevoluteJointDef joint2;
	rightFlipper = App->physics->CreateRectangle(270, 485, 48, 10, -20 * DEGTORAD, b2_dynamicBody);   // Creates flipper to location
	rightPivot = App->physics->CreateCircle(265, 478, 7, b2_staticBody);				// Pivot is needed to rotate flipper
	rightFlipper->body->SetGravityScale(20.0f);

	joint2.bodyA = rightFlipper->body;
	joint2.bodyB = rightPivot->body;
	joint2.Initialize(joint2.bodyA, joint2.bodyB, rightFlipper->body->GetWorldCenter());

	joint2.localAnchorA.Set(PIXEL_TO_METERS(26), PIXEL_TO_METERS(-3));		// Set flipper position correctly
	joint2.localAnchorB.Set(0, 0);
	joint2.collideConnected = false;

	joint2.upperAngle = -45 * DEGTORAD;		//Sets angle limits for the flipper
	joint2.lowerAngle = -20 * DEGTORAD;
	joint2.enableLimit = true;

	joint2.motorSpeed = 1500.0f * DEGTORAD;
	joint2.maxMotorTorque = 1500;
	joint2.enableMotor = false;

	rightJoint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&joint2);


	// LAUNCHER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//b2PrismaticJointDef joint3;
	//launcherBody = App->physics->CreateRectangle(388, 422, 18, 28, 0, b2_dynamicBody);
	//launcherPivot = App->physics->CreateRectangle(100, 100, 18, 10, 0, b2_staticBody);

	//joint3.bodyA = launcherBody->body;
	//joint3.bodyA = launcherPivot->body;

	//joint3.Initialize(joint3.bodyA, joint3.bodyB, launcherBody->body->GetWorldCenter(), );

	//joint3.collideConnected = false;

	//joint3.enableLimit = true;
	//joint3.upperTranslation = PIXEL_TO_METERS(30);
	//joint3.lowerTranslation = 0;

	//joint3.motorSpeed = 2500.0f;
	//joint3.maxMotorForce = 750;

	//launcherJoint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&joint3);


	CreateBall();

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	App->textures->Unload(ball_texture);
	App->textures->Unload(flipper);

	return true;
}

void ModulePlayer::CreateBall()
{
	// Create Ball
	ball = App->physics->CreateCircle(153, 200, 8, b2_dynamicBody);
	ball->listener = this;
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


	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		rightJoint->EnableMotor(true);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		rightJoint->EnableMotor(false);
	}

	//Blit left flipper
	int posX, posY;
	leftFlipper->GetPosition(posX, posY);
	App->renderer->Blit(flipper, posX, posY, NULL, 1.0f, leftFlipper->GetRotation());

	//Blit right flipper. UNFINISHED
	int posX2, posY2;
	rightFlipper->GetPosition(posX2, posY2);
	App->renderer->Blit(flipper, posX2, posY2, NULL, 1.0f, leftFlipper->GetRotation() - 180);
	

	// Set Ball texture
	ball->GetPosition(posX, posY);
	App->renderer->Blit(ball_texture, posX, posY, NULL, 1.0f, ball->GetRotation());

	// ----------------------------------------------------------


	return UPDATE_CONTINUE;
}

