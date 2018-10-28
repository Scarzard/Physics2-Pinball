#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleTextures.h"
#include "Box2D/Box2D/Box2D.h"
#include <string>

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void CreateBall(int x, int y);
	void CreateFlippers();
	void CreateLauncher();

public:

	//Textures
	SDL_Texture* ball_texture;
	SDL_Texture* flipper;
	SDL_Texture* flipper2;

	//Player attributes
	uint score = 0;
	uint tries = 5;

	//fx
	uint flippers_FX;

	//Flippers
	PhysBody* leftFlipper;
	PhysBody* leftPivot;
	b2RevoluteJoint* leftJoint;

	PhysBody* rightFlipper;
	PhysBody* rightPivot;
	b2RevoluteJoint* rightJoint;

	PhysBody* launcherBody;
	PhysBody* launcherPivot;
	b2PrismaticJoint* launcherJoint;
	b2Vec2 axis;

	
private:

	PhysBody * ball;
};