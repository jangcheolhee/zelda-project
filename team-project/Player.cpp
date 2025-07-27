#include "stdafx.h"
#include "Player.h"
#include "AnimationClip.h"

Player::Player(const std::string& name)
	: GameObject(name)
{
}

void Player::OnCollide(Enemy* enemy)
{
	// 칼 히트박스가 활성화되어 있고,
		// 현재 적의 바운딩박스와 충돌한다면
		if (swordHitBoxActive &&
			swordHitBox.rect.getGlobalBounds().intersects(enemy->GetGlobalBounds()))
		{
			enemy->OnCollideBySword(); // 적 피격 처리
		}
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

	std::string swordTexPath = "graphics/Sword_ani.png";
	if (!TEXTURE_MGR.Exists(swordTexPath))
	{
		TEXTURE_MGR.Load(swordTexPath);
	}
	swordTexture = &TEXTURE_MGR.Get(swordTexPath);

	//Sord_Up
	attackAnimations[Direction::Up] = AnimationIO::loadFromCSV("animations/Sword_attack_u.csv");

	//Sord_Down
	attackAnimations[Direction::Down] = AnimationIO::loadFromCSV("animations/Sword_attack_d.csv");

	//sord_Right
	attackAnimations[Direction::Right] = AnimationIO::loadFromCSV("animations/Sword_attack_r.csv");

	//Sord_Left
	auto& attackVec = (currentDirection == Direction::Left)
		? attackAnimations[Direction::Right]
		: attackAnimations[currentDirection];

	for (auto& [dir, vec] : attackAnimations)
	{
		std::cout << "[Init] Loaded attack animation for Direction "
			<< static_cast<int>(dir)
			<< " → " << vec.size() << " frames\n";
	}

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
	if (!isAttacking && InputMgr::GetKeyDown(sf::Keyboard::Z))
	{
		state = PlayerState::Attack;
		isAttacking = true;
		attackElapsed = 0.f;
		attackFrameIndex = 0;

		if (swordTexture)
			body.setTexture(*swordTexture);

		auto& attackVec = attackAnimations
			[currentDirection == Direction::Left? Direction::Right
			: currentDirection];

		if (!attackVec.empty())
		{
			sf::IntRect rect = attackVec[attackFrameIndex];
			if (currentDirection == Direction::Left)
			{
				rect.left += rect.width;
				rect.width = -rect.width;
			}
			body.setTextureRect(rect);

		}
		
	}	

	if (state == PlayerState::Attack)
	{
		attackElapsed += dt;

		auto& attackVec = attackAnimations[currentDirection];
		if (attackVec.empty() && currentDirection == Direction::Left)
			attackVec = attackAnimations[Direction::Right]; // 좌우 반전 처리용

		if (!attackVec.empty() && attackFrameIndex < attackVec.size())
		{
			if (attackElapsed >= attackFrameTime)
			{
				attackElapsed = 0.f;
				attackFrameIndex++;



				if (attackFrameIndex >= attackVec.size())
				{
					isAttacking = false;
					state = PlayerState::Idle;
					currentFrame = 0;
					elapsedTime = 0.f;

					// ⭐ 공격 끝나면 원래 텍스처(Link.png)로 복귀
					if (texture)
						body.setTexture(*texture);
					// 히트박스 비활성화
					swordHitBoxActive = false;
					swordHitBox.visible = false;
				}
				else
				{
					// 다음 공격 프레임
					sf::IntRect rect = attackVec[attackFrameIndex];
					if (currentDirection == Direction::Left)
					{
						rect.left += rect.width;
						rect.width = -rect.width;
					}
					body.setTextureRect(rect);

					// ✅ 히트박스 위치 갱신
					sf::FloatRect bodyBounds = body.getGlobalBounds();
					sf::Vector2f swordPos = { bodyBounds.left + bodyBounds.width * 0.5f,
											  bodyBounds.top + bodyBounds.height * 0.5f };
					sf::Vector2f offset;
					sf::Vector2f size;
					float swordRange = 20.f;  // 칼 거리
					switch (currentDirection)
					{
					case Direction::Up:
						size = { 8.f, 15.f };
						offset = { 0.f, -9.f };
						break;
					case Direction::Down:
						size = { 8.f, 15.f };
						offset = { 0.f, +9.f };
						break;
					case Direction::Left:
						size = { 15.f, 8.f };
						offset = { -9.f, 0.f };
						break;
					case Direction::Right:
						size = { 15.f, 8.f };      // ← 크기 줄임
						offset = { +9.f, 0.f };    // ← 위치 안쪽으로
						break;
					}
					swordHitBox.rect.setSize(size);
					swordHitBox.rect.setOrigin(size * 0.5f);
					swordHitBox.rect.setPosition(swordPos + offset);
					swordHitBox.visible = true;
					swordHitBoxActive = true;
				}
			}
		}
		

	
		body.move(sf::Vector2f(0.f, 0.f)); // 공격 중엔 이동 없음
		hitBox.UpdateTransform(body, body.getLocalBounds());
		return; // 공격 중에는 나머지 처리 스킵
			
		}

	sf::Vector2f movement(0.f, 0.f);
	bool moving = false;
	bool isMovingLeft = false;
	// 방향 키 입력 시 방향 결정
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

	// ================== 이동 애니메이션 프레임 처리 ==================
	auto& vec = animations[currentDirection];
	
		if (!vec.empty())
		{
			if (moving)
			{
				elapsedTime += dt;
				if (elapsedTime >= frameTime)
				{
					elapsedTime = 0.f;
					currentFrame = (currentFrame + 1) % vec.size();
				}
			}
			else
			{
				currentFrame = 0;
			}

			sf::IntRect rect = vec[currentFrame];
			if (currentDirection == Direction::Left)
			{
				rect.left += rect.width;
				rect.width = -rect.width;
			}
			body.setTextureRect(rect);
		}
		// 애니메이션 데이터 없으면 이동만 처리
		body.move(movement);
		hitBox.UpdateTransform(body, body.getLocalBounds());
		
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
	{
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
	if (swordHitBoxActive)
	{
		swordHitBox.Draw(window);
	}
}

bool Player::checkCollision(const HitBox& other)
{
	return hitBox.rect.getGlobalBounds().intersects(other.rect.getGlobalBounds());
}