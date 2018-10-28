#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleSceneIntro.h"
#include "ModuleAudio.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball_texture = flipper = flipper2 = NULL;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	ball_texture = App->textures->Load("Assets/ball.png");
	flipper = App->textures->Load("Assets/flippers.png");
	flipper2 = App->textures->Load("Assets/flippers.png");
	flippers_FX = App->audio->LoadFx("Audio/fx_flipper.wav");

	touching_deathzone = false;

	CreateBall(403, 200);
	CreateFlippers();
	CreateLauncher();

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

void ModulePlayer::CreateBall(int x, int y)
{
	// Create Ball
	if (tries > 0)
	{
		ball = App->physics->CreateCircle(x, y, 8, b2_dynamicBody);
		ball->listener = this;
	}
}

void ModulePlayer::CreateLauncher()
{
	b2Vec2 axis(0.0f, 1.0f);

	// LAUNCHER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	b2PrismaticJointDef joint;
	launcherBody = App->physics->CreateRectangle(403, 440, 15, 5, 0, b2_dynamicBody);
	launcherPivot = App->physics->CreateRectangle(395, 440, 15, 1, 0, b2_staticBody);

	joint.bodyA = launcherBody->body;
	joint.bodyB = launcherPivot->body;

	joint.Initialize(joint.bodyA, joint.bodyB, launcherBody->body->GetWorldCenter(), axis);

	joint.localAnchorA.Set(0, 0);
	joint.localAnchorB.Set(0, 0);
	joint.collideConnected = false;

	joint.upperTranslation = PIXEL_TO_METERS(-100);
	joint.lowerTranslation = 0;
	joint.enableLimit = true;

	joint.enableMotor = false;
	joint.motorSpeed = 5000;
	joint.maxMotorForce = 150;

	launcherJoint = (b2PrismaticJoint*)App->physics->world->CreateJoint(&joint);
}

void ModulePlayer::CreateFlippers()
{
	//Left flipper

	b2RevoluteJointDef joint;
	leftFlipper = App->physics->CreateRectangle(150, 485, 48, 10, 20 * DEGTORAD, b2_dynamicBody);   // Creates flipper to location
	leftPivot = App->physics->CreateCircle(153, 480, 7, b2_staticBody);				// Pivot is needed to rotate flipper
	leftFlipper->body->SetGravityScale(20.0f);

	joint.bodyA = leftFlipper->body;
	joint.bodyB = leftPivot->body;
	joint.Initialize(joint.bodyA, joint.bodyB, leftFlipper->body->GetWorldCenter());


	joint.localAnchorA.Set(PIXEL_TO_METERS(-20), PIXEL_TO_METERS(0));		// Set flipper position correctly
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
	rightFlipper = App->physics->CreateRectangle(230, 485, 48, 10, 25 * DEGTORAD, b2_dynamicBody);   // Creates flipper to location
	rightPivot = App->physics->CreateCircle(265, 480, 7, b2_staticBody);				// Pivot is needed to rotate flipper
	rightFlipper->body->SetGravityScale(20.0f);

	joint.bodyA = rightFlipper->body;
	joint.bodyB = rightPivot->body;
	joint.Initialize(joint.bodyA, joint.bodyB, rightFlipper->body->GetWorldCenter());

	joint.localAnchorA.Set(PIXEL_TO_METERS(20), PIXEL_TO_METERS(0));		// Set flipper position correctly
	joint.localAnchorB.Set(0, 0);
	joint.collideConnected = false;

	joint.upperAngle = 45 * DEGTORAD;		//Sets angle limits for the flipper
	joint.lowerAngle = 0 * DEGTORAD;
	joint.enableLimit = true;

	joint.motorSpeed = -1500.0f * DEGTORAD;
	joint.maxMotorTorque = 1500;
	joint.enableMotor = false;

	rightJoint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&joint);
}

void ModulePlayer::OnCollision(PhysBody * bodyA, PhysBody * bodyB) 
{
	if (bodyB == App->scene_intro->deathzone) {
		touching_deathzone = true;
		tries -= 1;
	}

	for (int i = 0; i < 2; i++)//Loop to check collision with triangles
	{
		if (bodyB == App->scene_intro->nugget_bouncers_sensors[i])
		{
			App->scene_intro->touch_nuggets[i] = true;
		}
	}
}



// Update: draw background
update_status ModulePlayer::Update()
{

	//Inputs
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN && tries > 0)
	{
		leftJoint->EnableMotor(true);
		App->audio->PlayFx(flippers_FX);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP && tries > 0)
	{
		leftJoint->EnableMotor(false);
	}


	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && tries > 0)
	{
		rightJoint->EnableMotor(true);
		App->audio->PlayFx(flippers_FX);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP && tries > 0)
	{
		rightJoint->EnableMotor(false);
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		launcherJoint->EnableMotor(true);
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		launcherJoint->EnableMotor(false);
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		LOG("Reset tries");
		tries = 5;	
	}

	//Blit left flipper
	int posX, posY;
	leftFlipper->GetPosition(posX, posY);
	App->renderer->Blit(flipper, posX, posY, NULL, 1.0f, leftFlipper->GetRotation());

	//Blit right flipper. UNFINISHED
	rightFlipper->GetPosition(posX, posY);
	App->renderer->Blit(flipper2, posX, posY, NULL, 1.0f, rightFlipper->GetRotation() - 180);
	

	// Set Ball texture
	ball->GetPosition(posX, posY);
	App->renderer->Blit(ball_texture, posX, posY, NULL, 1.0f, ball->GetRotation());

	// ----------------------------------------------------------

	if (touching_deathzone)
	{
		App->physics->world->DestroyBody(ball->body);
		CreateBall(403, 200);
		touching_deathzone = false;
	}

	return UPDATE_CONTINUE;
}

