#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleWindow.h"
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

	ball_eater_FX = App->audio->LoadFx("Audio/fx_balleater.wav");
	flippers_FX = App->audio->LoadFx("Audio/fx_flipper.wav");
	bouncers_FX = App->audio->LoadFx("Audio/fx_bouncer.wav");
	kicker_fx = App->audio->LoadFx("Audio/fx_kicker.wav");

	leftAutoKicker = App->physics->CreateRectangle(89, 425, 10, 10, 0, b2_staticBody, 2.0f);
	rightAutoKicker = App->physics->CreateRectangle(327, 425, 10, 10, 0, b2_staticBody, 2.0f);

	CreateBall(403, 380);
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

void ModulePlayer::CreateBall(int x, int y, float vx, float vy)
{
	// Create Ball
	
	ball = App->physics->CreateCircle(x, y, 8, b2_dynamicBody, 0, vx, vy);
	ball->listener = this;
	
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

	joint.upperTranslation = PIXEL_TO_METERS(10);
	joint.lowerTranslation = 0;
	joint.enableLimit = true;

	joint.enableMotor = false;
	joint.motorSpeed = 100;
	joint.maxMotorForce = 125;

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
	//deathzone
	if (bodyB == App->scene_intro->deathzone) {
		App->scene_intro->touching_deathzone = true;
		lives -= 1;
	}

	//tornado
	if (bodyB == App->scene_intro->tornado) {
		App->scene_intro->touching_tornado = true;
		if (App->scene_intro->combo)
			score += 4000;
		else score += 2000;
	}

	//balleater
	if (bodyB == App->scene_intro->balleater_sense) {
		App->scene_intro->touching_balleater = true;
		if(App->scene_intro->combo)
		score += 1000;
		else score += 500;

	}

	//Stars --------------------------------------
	if (bodyB == App->scene_intro->leftStar) {
		App->scene_intro->touching_leftStar = true;
	}
	if (bodyB == App->scene_intro->middleStar) {
		App->scene_intro->touching_middleStar = true;
	}
	if (bodyB == App->scene_intro->rightStar) {
		App->scene_intro->touching_rightStar = true;
	}
	//Bumpers
	if (bodyB == App->scene_intro->leftBumper) {
		App->audio->PlayFx(bouncers_FX);

		if (App->scene_intro->combo)
			score += 200;
		else score += 100;
	}
	if (bodyB == App->scene_intro->midBumper) {
		App->audio->PlayFx(bouncers_FX);
		if (App->scene_intro->combo)
			score += 200;
		else score += 100;
	}
	if (bodyB == App->scene_intro->rightBumper) {
		App->audio->PlayFx(bouncers_FX);

		if (App->scene_intro->combo)
			score += 200;
		else score += 100;
	}


	//Launcher sensor
	if (bodyB == App->scene_intro->launcherSensor) {
		App->scene_intro->touching_launcherSensor = false;
	}

	//Autokickers ----------------------
	if (bodyB == leftAutoKicker)
	{
		App->audio->PlayFx(flippers_FX);
		leftKickerActivations -= 1;
	}
	if (bodyB == rightAutoKicker)
	{
		App->audio->PlayFx(flippers_FX);
		rightKickerActivations -= 1;
	}
	//-------------------------------------
	
	if (bodyB == App->scene_intro->right_nugget_sens)
	{
		App->scene_intro->touching_right_nugget = true;
		if (App->scene_intro->combo)
			score += 500;
		else score += 250;
	}
	if (bodyB == App->scene_intro->left_nugget_sens)
	{
		App->scene_intro->touching_left_nugget = true;
		if (App->scene_intro->combo)
			score += 500;
		else score += 250;
	}
}



// Update: draw background
update_status ModulePlayer::Update()
{

	//Inputs
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN && lives > 0)
	{
		leftJoint->EnableMotor(true);
		App->audio->PlayFx(flippers_FX);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP && lives > 0)
	{
		leftJoint->EnableMotor(false);
	}


	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && lives > 0)
	{
		rightJoint->EnableMotor(true);
		App->audio->PlayFx(flippers_FX);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP && lives > 0)
	{
		rightJoint->EnableMotor(false);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && App->scene_intro->touching_launcherSensor)
	{
		launcherJoint->EnableMotor(true);
		App->audio->PlayFx(kicker_fx);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		launcherJoint->EnableMotor(false);

	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		LOG("Reset lives");
		if (lives > 0) App->physics->world->DestroyBody(ball->body);

		lives = 5;
		CreateBall(403, 380);
		App->scene_intro->touching_deathzone = App->scene_intro->combo = App->scene_intro->touching_leftStar =
			App->scene_intro->touching_middleStar = App->scene_intro->touching_rightStar = false;

		leftKickerActivations = rightKickerActivations = 3;
		score = 0;
		App->scene_intro->touching_launcherSensor = true;
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
	if (lives > 0)  App->renderer->Blit(ball_texture, posX, posY, NULL, 1.0f, ball->GetRotation());

	App->renderer->Blit(App->scene_intro->piece_tornado, 57, 104, NULL, 1.0f);

	// ----------------------------------------------------------
	//Ball to deathzone
	if (App->scene_intro->touching_deathzone)
	{
		App->physics->world->DestroyBody(ball->body);
		if(lives > 0) CreateBall(403, 380);

		App->scene_intro->touching_launcherSensor = true;
		App->scene_intro->touching_deathzone = false;
	}
	//Tornado Action
	if (App->scene_intro->touching_tornado)
	{
		App->physics->world->DestroyBody(ball->body);
		CreateBall(203, 71);
		App->scene_intro->touching_tornado = false;
	}
	
	//Ball Eater
	if (App->scene_intro->touching_balleater)
	{
		i++;
		if(i == 1)
			App->physics->world->DestroyBody(ball->body);
		if (i == 100)
		{
			CreateBall(203, 325, 2.0f, 10.0f);
			App->scene_intro->touching_balleater = false;
		}
		else if (i == 60)
		{
			//fx
			App->audio->PlayFx(ball_eater_FX);
		}
	}
	if (i > 100)i = 0;


	if (leftKickerActivations == 0)

	{
		App->physics->world->DestroyBody(leftAutoKicker->body);

		leftAutoKicker = App->physics->CreateRectangle(1000, 1000, 10, 10, 0, b2_staticBody, 1.1f);

	}

	if (rightKickerActivations == 0)

	{
		App->physics->world->DestroyBody(rightAutoKicker->body);

		rightAutoKicker = App->physics->CreateRectangle(-15, -15, 10, 10, 0, b2_staticBody, 1.1f);
	}

	if (App->scene_intro->touching_right_nugget)
	{
		App->audio->PlayFx(bouncers_FX);
		App->scene_intro->touching_right_nugget = false;

	}
	if (App->scene_intro->touching_left_nugget)
	{
		App->audio->PlayFx(bouncers_FX);
		App->scene_intro->touching_left_nugget = false;
	}

	
	title->create("Pepsi Pinball / Score: %d / Lives left: %d / %d / %d", score, lives, leftKickerActivations, rightKickerActivations);
	App->window->SetTitle(title->GetString());
	

	return UPDATE_CONTINUE;
}

