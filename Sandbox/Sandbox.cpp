#include "../Strave Engine/Sandbox.h"

#define NUMBER_OF_ASSETS 200

namespace nSandbox {

	Strave::Mouse* mouse;
	Strave::Keyboard* keyboard;

	// Animations
	Strave::Texture* AssetsTexture;
	Strave::Texture* AppleTexture;
	Strave::Texture* FireTexture;
	Strave::Texture* PlayerTexture;
	Strave::Texture* EnemyTexture;
	Strave::Texture* GridTexture;
	Strave::Texture* ButtonTexture;

	Strave::AnimationTexture* PlayerAnimationTexture;
	Strave::AnimationTexture* TreeAnimationTexture;
	Strave::AnimationTexture* EnemyAnimationTexture;
	Strave::AnimationTexture* FireAnimationTexture;
	Strave::Animation* PlayerAnimation;
	Strave::Animation* TreeAnimation[NUMBER_OF_ASSETS];
	Strave::Animation* EnemyAnimation;
	Strave::Animation* FireAnimation;

	Strave::GameObject2D* apple;
	Strave::GameObject2D* tree;
	Strave::GameObject2D* fireObj;

	Strave::Player2D* player;
	Strave::RigidBody2D* enemy;

	Strave::GameObject2D* assets[NUMBER_OF_ASSETS];

	Strave::Input* input;
	Strave::CharacterController2D* characterController;
	Strave::GameCamera2D* characterCamera;

	Strave::GUI::Grid* inventory;
	Strave::GUI::Button* addItemButton;
	Strave::GUI::Button* removeItemButton;

	Strave::Collider* PlayerCollider;

	float	movementForward, movementBackward,
		movementLeft, movementRight;

	bool itemInserted = false;
	bool itemsInsertedCounter = 0;

	int selectedItemIndex = -1;

	enum PLAYER_ANIMATION {
		RUN_LEFT = 1,
		RUN_RIGHT
	};

	bool doubleClicked = false;
	PLAYER_ANIMATION lastCastedAnimation = PLAYER_ANIMATION::RUN_RIGHT;

	// Use this for initialization
	void Sandbox::Start(void) const {
		AssetsTexture = Strave::Texture::LoadTexture("D:\\Documents\\Strave Pictures\\texture_pack\\tree_1");
		AppleTexture = Strave::Texture::LoadTexture("D:\\Documents\\Strave Pictures\\texture_pack\\apple");
		FireTexture = Strave::Texture::LoadTexture("D:\\Documents\\Strave Pictures\\texture_pack\\empty");
		PlayerTexture = Strave::Texture::LoadTexture("D:\\Documents\\Strave Pictures\\texture_pack\\player");
		EnemyTexture = Strave::Texture::LoadTexture("D:\\Documents\\Strave Pictures\\texture_pack\\enemy");
		GridTexture = Strave::Texture::LoadTexture("D:\\Documents\\Strave Pictures\\texture_pack\\inventory");
		ButtonTexture = Strave::Texture::LoadTexture("D:\\Documents\\Strave Pictures\\texture_pack\\button");

		srand(time(NULL));
		int x;
		int y;
		float x_f;
		float y_f;

		for (unsigned int i = 0; i != NUMBER_OF_ASSETS; i++) {

			x = rand() % (5000 - 10 + 1) + 10;
			y = rand() % (5000 - 10 + 1) + 10;
			x_f = (float)x;
			y_f = (float)y;

			assets[i] = Strave::GameObject2D::Create(*AssetsTexture, Strave::Vector2f(x_f, y_f), Strave::Vector2f(0.25f, 0.25f));
		}

		mouse = new Strave::Mouse(*Strave::StraveApplication->GetWindow());
		keyboard = new Strave::Keyboard();

		apple = Strave::GameObject2D::Create(*AppleTexture, Strave::Vector2f(450.0f, 300.0f), Strave::Vector2f(0.10f, 0.10f));
		fireObj = Strave::GameObject2D::Create(*FireTexture, Strave::Vector2f(800.0f, 500.0f), Strave::Vector2f(1.0f, 1.0f));
		player = Strave::Player2D::Create("Mandarinni", *PlayerTexture, Strave::Vector2f(750.0f, 550.0f), Strave::Vector2f(1.5f, 1.5f));
		enemy = Strave::RigidBody2D::Create(*EnemyTexture, Strave::Vector2f(300.0f, 300.0f), Strave::Vector2f(1.0f, 1.0f));
		input = new Strave::Input();
		characterController = new Strave::CharacterController2D();
		characterCamera = Strave::GameCamera2D::Create(player->GetSpritePosition(), Strave::StraveApplication->GetWindow()->GetResolutionInFloats());
		inventory = Strave::GUI::Grid::Create(
			Strave::Vector2u(8, 4),
			*GridTexture,
			*GridTexture,
			Strave::Vector2f(20.0f, 20.0f),
			Strave::Vector2f(250.0f, 500.0f)
		);
		addItemButton = Strave::GUI::Button::Create(*ButtonTexture, Strave::Vector2f(35.0f, 500.0f), Strave::Vector2f(100.0f, 70.0f));
		removeItemButton = Strave::GUI::Button::Create(*ButtonTexture, Strave::Vector2f(150.0f, 500.0f), Strave::Vector2f(100.0f, 70.0f));

		// sv collider testing
		Strave::ColliderMesh::Box::Constraints con1 = {
			SV_DEF_COLLIDERMESH_ALIGNMENT,
			SV_DEF_COLLIDERMESH_POSITION,
			SV_DEF_COLLIDERMESH_ROTATION,
			SV_DEF_COLLIDERMESH_BOX_SIZE
		};
		Strave::ColliderMesh::Box::Constraints con2 = {
			SV_DEF_COLLIDERMESH_ALIGNMENT,
			SV_DEF_COLLIDERMESH_POSITION,
			SV_DEF_COLLIDERMESH_ROTATION,
			SV_DEF_COLLIDERMESH_BOX_SIZE
		};
		Strave::ColliderMesh::Box* playerColliderMesh[] = {
			Strave::ColliderMesh::Box::Create(con1),
			Strave::ColliderMesh::Box::Create(con2)
		};
		PlayerCollider = Strave::Collider::Create(*playerColliderMesh, 2);
		player->AssignCollider(*PlayerCollider);
		// sv collider testing

		characterController->AssignCharacter(*player);
		characterCamera->AssignTo(*player);
		inventory->SetMargins(Strave::Vector2f(15.0f, 15.0f));
		inventory->SetGridPosition(Strave::Vector2f(20.0f, 20.0f));

		movementForward = input->GetAxisDirection(Strave::InputDirection::Up);
		movementBackward = input->GetAxisDirection(Strave::InputDirection::Down);
		movementLeft = input->GetAxisDirection(Strave::InputDirection::Left);
		movementRight = input->GetAxisDirection(Strave::InputDirection::Right);

		PlayerAnimationTexture = Strave::Texture::LoadTexture("D:\\Documents\\Strave Pictures\\animation_pack\\player_animation");
		TreeAnimationTexture = Strave::Texture::LoadTexture("D:\\Documents\\Strave Pictures\\animation_pack\\tree_animation");
		EnemyAnimationTexture = Strave::Texture::LoadTexture("D:\\Documents\\Strave Pictures\\animation_pack\\enemy\\enemy_prepare_to_fight");
		FireAnimationTexture = Strave::Texture::LoadTexture("D:\\Documents\\Strave Pictures\\animation_pack\\fire");

		Strave::Animation::Constraints PlayerAnimationConstraints[] = {
			SV_DEF_ANIMATION_CONSTRAINTS,
			{ 6, 0.20f },
			{ 6, 0.20f },
		};

		PlayerAnimation = Strave::Animation::Create(*PlayerAnimationTexture, 3, PlayerAnimationConstraints);
		EnemyAnimation = Strave::Animation::Create(*EnemyAnimationTexture, 6, 0.25f);
		FireAnimation = Strave::Animation::Create(*FireAnimationTexture, 8, 0.25f);

		for (int i = 0; i != NUMBER_OF_ASSETS; i++) {
			TreeAnimation[i] = Strave::Animation::Create(*TreeAnimationTexture, 4, 0.15f);
		}

		PlayerAnimation->AssignTo(*player);
		EnemyAnimation->AssignTo(*enemy);
		FireAnimation->AssignTo(*fireObj);

		for (int i = 0; i != NUMBER_OF_ASSETS; i++) {
			TreeAnimation[i]->AssignTo(*assets[i]);
		}
	}

	// Update is called once per frame
	void Sandbox::Update(void) const {
		if (Strave::Shapes::fPoint::DistanceBetweenPoints(apple->GetSpritePosition(), player->GetSpritePosition()) < 100.0f) {
			if (!itemInserted) {
				inventory->Insert(*apple);
				itemInserted = true;
			}

		}
		else {
			if (itemInserted) {
				itemInserted = false;
			}
		}

		if (mouse->DoubleClick(Strave::Mouse::Button::Left, *player)) {
			mouse->Drag(*player);
		}

		for (unsigned int i = 0; i != inventory->GetSize(); i++) {
			if (inventory->GetCell(i).MouseClick(Strave::Mouse::Button::Middle)) {
				selectedItemIndex = i;
				break;
			}
		}

		if (addItemButton->MouseReleased(Strave::Mouse::Button::Left)) {
			inventory->Insert(*apple);
		}
		if (removeItemButton->MouseClick(Strave::Mouse::Button::Left)) {
			inventory->Erase(selectedItemIndex);
		}

		if (addItemButton->MouseClick(Strave::Mouse::Button::Right)) {
			addItemButton->MouseDrag();
		}
		if (removeItemButton->MouseClick(Strave::Mouse::Button::Right)) {
			removeItemButton->MouseDrag();
		}

		//if (inventory->MouseDoubleClick(Strave::Mouse::Button::Right)) {
		//	inventory->MouseDrag();
		//}

		if (inventory->MouseDoubleClick(Strave::Mouse::Button::Left)) {
			doubleClicked = true;
		}
		if (doubleClicked == true) {
			if (inventory->MouseHold(Strave::Mouse::Button::Left)) {
				inventory->MouseDrag();
			}
			else {
				doubleClicked = false;
			}
		}

		if (keyboard->IsKeyPressed(Strave::Keyboard::KEY::W)) {
			characterController->Move(Strave::InputAxis::Vertical, 2.0f * movementForward * Strave::EngineClocks::deltaTime);
			PlayerAnimation->Animate(lastCastedAnimation);
		}
		if (keyboard->IsKeyPressed(Strave::Keyboard::KEY::S)) {
			characterController->Move(Strave::InputAxis::Vertical, 2.0f * movementBackward * Strave::EngineClocks::deltaTime);
			PlayerAnimation->Animate(lastCastedAnimation);
		}
		if (keyboard->IsKeyPressed(Strave::Keyboard::KEY::A)) {
			characterController->Move(Strave::InputAxis::Horizontal, 2.0f * movementLeft * Strave::EngineClocks::deltaTime);
			PlayerAnimation->Animate(lastCastedAnimation = PLAYER_ANIMATION::RUN_RIGHT);
		}
		if (keyboard->IsKeyPressed(Strave::Keyboard::KEY::D)) {
			characterController->Move(Strave::InputAxis::Horizontal, 2.0f * movementRight * Strave::EngineClocks::deltaTime);
			PlayerAnimation->Animate(lastCastedAnimation = PLAYER_ANIMATION::RUN_LEFT);
		}

		Strave::RigidBody2D::OnTriggerCollision(*player);
	}
	// Use this for uninitialization
	void Sandbox::End(void) const {

		delete mouse;
		delete keyboard;
		delete AssetsTexture;
		delete AppleTexture;
		delete FireTexture;
		delete PlayerTexture;
		delete EnemyTexture;
		delete GridTexture;
		delete ButtonTexture;
		delete PlayerAnimationTexture;
		delete TreeAnimationTexture;
		delete EnemyAnimationTexture;
		delete FireAnimationTexture;
		delete PlayerAnimation;
		delete EnemyAnimation;
		delete FireAnimation;
		delete apple;
		delete tree;
		delete fireObj;
		delete player;
		delete enemy;
		delete input;
		delete characterController;
		delete characterCamera;
		delete inventory;
		delete addItemButton;
		delete removeItemButton;

		for (unsigned int i = 0; i != NUMBER_OF_ASSETS; i++) {
			delete assets[i];
			delete TreeAnimation[i];
		}

	}

}
