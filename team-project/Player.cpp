#include "stdafx.h"
#include "Player.h"
#include "AnimationClip.h"

Player::Player(const std::string& name)
	: GameObject(name)
{
}

void Player::OnCollide(Enemy* enemy)
{
}

void Player::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Player::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Player::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Player::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Player::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Player::Init()
{
	

	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
	
	std::string texPath = "graphics/Link.png";
	if (!TEXTURE_MGR.Exists(texPath))
	{
		TEXTURE_MGR.Load(texPath);
	}
	texture = &TEXTURE_MGR.Get(texPath);
	body.setTexture(*texture);
	// Up
	animations[Direction::Up] = AnimationIO::loadFromCSV("animations/Link_up.csv");

	// Down
	animations[Direction::Down] = AnimationIO::loadFromCSV("animations/Link_down.csv");
	
	
	// Right
	animations[Direction::Right] = AnimationIO::loadFromCSV("animations/Link_right.csv");
	// Left
	animations[Direction::Left] = animations[Direction::Right];

	currentDirection = Direction::Down;
	currentFrame = 0;
	elapsedTime = 0.f;
	frameTime = 1.f / 8.f;   // 예: 초당 8프레임

	auto& vec = animations[currentDirection];
	if (!vec.empty()) {
		body.setTextureRect(vec[currentFrame]);

	}
	else {
		std::cerr << "[Init] 빈 애니메이션: Direction="
			<< static_cast<int>(currentDirection)
			<< "\n";
	}

	
	body.setScale(1.0f, 1.0f); // ũ�� ����
	
	// ��Ʈ�ڽ� �ʱ�ȭ
	hitBox.UpdateTransform(body, body.getLocalBounds());
}

void Player::Release()
{
	GAME_MGR.SetPlayerData(hp, position);
}

void Player::Reset()
{
	hp = GAME_MGR.GetPlayerData().hp;
	SetPosition(GAME_MGR.GetPlayerData().position);
	wantsToInteract = false;
	isInteract = false;
	bool isMovingLeft = false;
	bool isRightPressed = false;
	bool isLeftPressed = false;

}
void Player::Update(float dt)
{
	
	sf::Vector2f movement(0.f, 0.f);
	bool moving = false;
	bool isMovingLeft = false;


	if (!isHoldingDirection)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			currentDirection = Direction::Up;
			isHoldingDirection = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			currentDirection = Direction::Down;
			isHoldingDirection = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			currentDirection = Direction::Left;
			isHoldingDirection = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			currentDirection = Direction::Right;
			isHoldingDirection = true;
		}

		if (isHoldingDirection) {
			//currentDirection = heldDirection;
			currentFrame = 0;
			elapsedTime = 0.f;
		}
	}

	// 2. 이동 방향은 3개까지 입력 가능 (움직임만)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		movement.x -= speed * dt;
		moving = true;
		if (currentDirection == Direction::Left)
			isMovingLeft = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		movement.x += speed * dt;
		moving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		movement.y -= speed * dt;
		moving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		movement.y += speed * dt;
		moving = true;
	}
	
	// 키가 모두 떨어졌는지 체크
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		isHoldingDirection = false;

	}
	
	// 2) **딱 한번** 벡터를 가져오고
	auto& vec = animations[currentDirection];
	if (vec.empty()) {
		// 애니메이션 데이터 없으면 이동만 처리
		body.move(movement);
		hitBox.UpdateTransform(body, body.getLocalBounds());
		return;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
	{
		hitBox.visible = !hitBox.visible;
		
	}
	
	
	if (moving) {
		elapsedTime += dt;
		if (elapsedTime >= frameTime) 
		{
			auto& vec = animations[currentDirection];
			currentFrame = (currentFrame + 1) % vec.size();
			elapsedTime = 0.f;
		}
	
	}
	else {
		
		currentFrame = 0;
		
	} 
	// 5) 최종으로 텍스처 사각형 결정
	if (currentDirection == Direction::Left)
	{
		// 오른쪽 프레임을 가져와서 뒤집는다
		auto& rightVec = animations[Direction::Right];
		if (!rightVec.empty())
		{
			sf::IntRect r = rightVec[currentFrame];
			// 뒤집기: 왼쪽에서 width 만큼 옮긴 뒤 너비를 음수로
			r.left = r.left + r.width;
			r.width = -r.width;
			body.setTextureRect(r);
		}
	}
	else
	{
		// Up, Down, Right 는 그대로
		auto& vec = animations[currentDirection];
		if (!vec.empty())
			body.setTextureRect(vec[currentFrame]);
	}
	
	body.move(movement);
	hitBox.UpdateTransform(body, body.getLocalBounds());

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
		hitBox.visible = !hitBox.visible;
	}
	// interactable과 x키 누르면 상호작용
	if (InputMgr::GetKeyDown(sf::Keyboard::X))
	{
		wantsToInteract = true;
	}
	else
	{
		wantsToInteract = false;
	}


}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
}

bool Player::checkCollision(const HitBox& other)
{
	return hitBox.rect.getGlobalBounds().intersects(other.rect.getGlobalBounds());
}