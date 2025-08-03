#include "stdafx.h"
#include "Player.h"
#include "AnimationClip.h"
#include "HUD.h"
#include "SceneGame.h"
#include "SceneCastle.h"
#include "SceneHidden.h"
#include "SceneBoss.h"
#include "GameMgr.h"


Player::Player(const std::string& name)
	: GameObject(name)
{
	
}

void Player::OnCollide(Enemy* enemy)
{

	//if (!isInvincible)
	//{
	//	hp = std::max(hp - 1, 0);

	//	if (hud) hud->SetHeartCount(hp);

	//	isInvincible = true;
	//	invincibleTime = 1.f;
	//}
	// 칼 히트박스가 활성화되어 있고,
		// 현재 적의 바운딩박스와 충돌한다면
	
	if (swordHitBoxActive &&
		swordHitBox.rect.getGlobalBounds().intersects(enemy->GetHitBox().rect.getGlobalBounds()))
	{
		enemy->OnCollideBySword(); // 적 피격 처리
		enemy->OnDamage(2);
	}
}

void Player::AddRupee(int amount)
{
	rupeeCount += amount;
	GAME_MGR.playerRupee = rupeeCount;   // 전역에도 저장
	if (hud) hud->SetRupee(rupeeCount);
}
void Player::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
	UpdateFixedHitBox();
}

void Player::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
	UpdateFixedHitBox();
}

void Player::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
	UpdateFixedHitBox();
}

void Player::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
	UpdateFixedHitBox();
}

void Player::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
	UpdateFixedHitBox();
}

sf::FloatRect Player::GetGlobalBounds() const
{
	return hitBox.rect.getGlobalBounds();
}

void Player::Init()
{
	sf::FloatRect bounds = body.getLocalBounds();
	body.setOrigin(bounds.width / 2.f, bounds.height);
	
	hp = maxHp; 

	sortingLayer = SortingLayers::Player;
	sortingOrder = 2;

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
	// Push 애니메이션
	pushingAnimations[Direction::Up] = AnimationIO::loadFromCSV("animations/Link_push_up.csv");
	pushingAnimations[Direction::Down] = AnimationIO::loadFromCSV("animations/Link_push_down.csv");
	pushingAnimations[Direction::Right] = AnimationIO::loadFromCSV("animations/Link_push_right.csv");
	// Left는 Right 반전으로 재활용
	pushingAnimations[Direction::Left] = pushingAnimations[Direction::Right];

	for (auto& [dir, frames] : pushingAnimations)
	{
		std::cout << "[DEBUG] pushingAnimations[" << static_cast<int>(dir) << "] = " << frames.size() << " 프레임\n";
	}

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
	for (auto& [dir, frames] : pushingAnimations)
	{
		std::cout << "[DEBUG] pushingAnimations[" << static_cast<int>(dir) << "] = " << frames.size() << " 프레임\n";
	}
	body.setScale(1.0f, 1.0f);

}

void Player::Release()
{
	
}

void Player::Reset()
{
	hp = GAME_MGR.playerHp;
	if (hud) hud->SetHeartCount(hp);
	SetPosition(GAME_MGR.playerSpawnPosition);
	wantsToInteract = false;
	isInteract = false;
	bool isMovingLeft = false;
	bool isRightPressed = false;
	bool isLeftPressed = false;
	SetOrigin(Origins::BC);
	SetActive(true);
}
void Player::Update(float dt)
{
	if (isNpcTalk) return;
	CheckInteractions();
	bool collidingWithPushable = false;

	if (currentInteractable != nullptr)
	{
		sf::Vector2f playerPos = GetPosition();
		sf::Vector2f objPos = currentInteractable->GetPosition();
		float distance = std::sqrt(std::pow(playerPos.x - objPos.x, 2) +
			std::pow(playerPos.y - objPos.y, 2));

		// 밀기는 더 가까운 거리에서만 가능
		if (distance <= 25.0f)
		{
			collidingWithPushable = true;
		}
	}
	
	bool nowPushing = collidingWithPushable && moveDir != sf::Vector2f(0.f, 0.f);
	// 밀고 있는 시간 체크
	if (nowPushing)
	{
		pushTimer += dt;
		if (pushTimer >= pushThreshold)
		{
			isPushing = true;
		}
	}
	else
	{
		pushTimer = 0.f;
		isPushing = false;
	}

	// 상태 전환 처리
	if (isPushing && !wasPushing)
	{
		playerState = PlayerState::Push;
		currentFrame = 0;
		elapsedTime = 0.f;
		std::cout << "➡ 밀기 상태 진입! currentFrame 초기화\n";
	}
	else if (!isPushing && wasPushing)
	{
		// 이동 중이면 Walk, 아니면 Idle
		playerState = (moveDir != sf::Vector2f(0.f, 0.f)) ? PlayerState::Walk : PlayerState::Idle;
		currentFrame = 0;
		elapsedTime = 0.f;
		std::cout << "⬅ 밀기 상태 종료\n";
	}
	wasPushing = isPushing;
	// 이전 상태 갱신
	wasPushing = isPushing;
	if (collidingWithPushable)
	{
		pushTimer += dt;
		if (pushTimer >= pushThreshold)
		{
			if (!isPushing)
				std::cout << "[밀기 상태 전환] 애니메이션 전환 시작!\n";

			isPushing = true;
		}
	}
	else
	{
		if (isPushing)
			std::cout << "[밀기 해제] 더 이상 밀고 있지 않음\n";

		pushTimer = 0.f;
		isPushing = false;
	}

	


	previousPosition = GetPosition();
	timeSinceLastDamage += dt;
	// 상태 판단
	if (isAttacking)
	{
		playerState = PlayerState::Attack;
	}
	else if (isPushing)
	{
		playerState = PlayerState::Push;
	}
	else if (moveDir != sf::Vector2f(0.f, 0.f))
	{
		playerState = PlayerState::Walk;
	}
	else
	{
		playerState = PlayerState::Idle;
	}
	// 깜빡임 무적 처리
	if (isInvincible)
	{
		invincibleElapsed += dt;

		// 깜빡이게 하기: 0.1초마다 표시/숨김 반복
		float blinkCycle = 0.1f;
		bool visible = static_cast<int>(invincibleElapsed / blinkCycle) % 2 == 0;
		body.setColor(visible ? sf::Color::White : sf::Color(255, 255, 255, 0)); // 깜빡임

		if (invincibleElapsed >= invincibleTime)
		{
			isInvincible = false;
			body.setColor(sf::Color::White); // 원래대로
		}
	}

	if (!isAttacking && InputMgr::GetKeyDown(sf::Keyboard::Z))
	{
		playerState = PlayerState::Attack;
		SOUND_MGR.PlaySfx(SOUNDBUFFER_MGR.Get("effects/sword.wav"));
		isAttacking = true;
		attackElapsed = 0.f;
		attackFrameIndex = 0;

		if (swordTexture) body.setTexture(*swordTexture);

		auto& attackVec = attackAnimations[currentDirection];

		if (!attackVec.empty() && attackFrameIndex < attackVec.size())
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

	if (playerState == PlayerState::Attack)
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
					playerState = PlayerState::Idle;
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
					float swordRange = 50.f;  // 칼 거리
					switch (currentDirection)
					{
					case Direction::Up:
						size = { 8.f, 15.f };
						offset = { 0.f, -9.f };
						break;
					case Direction::Down:
						size = { 15.f, 31.f };
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
		UpdateFixedHitBox();
		//------------------------------
		if (SCENE_MGR.GetCurrentSceneId() == SceneIds::Game)
		{
			enemyList = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetEnemy();

		}
		else if (SCENE_MGR.GetCurrentSceneId() == SceneIds::Castle)
		{
			enemyList = dynamic_cast<SceneCastle*>(SCENE_MGR.GetCurrentScene())->GetEnemy();
		}
		else if (SCENE_MGR.GetCurrentSceneId() == SceneIds::Hidden)
		{
			enemyList = dynamic_cast<SceneHidden*>(SCENE_MGR.GetCurrentScene())->GetEnemy();
		}
		else if (SCENE_MGR.GetCurrentSceneId() == SceneIds::Boss)
		{
			enemyList = dynamic_cast<SceneBoss*>(SCENE_MGR.GetCurrentScene())->GetEnemy();
		}

		for (auto& enemy : enemyList)
		{
			OnCollide(enemy);
			
		}//-----------------------------
		return; // 공격 중에는 나머지 처리 스킵
	}
	

	moveDir = { 0.f, 0.f };
	bool moving = false;


	// 2. 이동 방향은 3개까지 입력 가능 (움직임만)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
	{
		currentDirection = Direction::Left;
		moveDir.x -= speed * dt;
		moving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
	{
		currentDirection = Direction::Right;
		moveDir.x += speed * dt;
		moving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) 
	{
		currentDirection = Direction::Up;
		moveDir.y -= speed * dt;
		moving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) 
	{
		currentDirection = Direction::Down;
		moveDir.y += speed * dt;
		moving = true;
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

		if (currentFrame < vec.size()) // ✅ 체크 추가
		{
			sf::IntRect rect = vec[currentFrame];
			if (currentDirection == Direction::Left)
			{
				rect.left += rect.width;
				rect.width = -rect.width;
			}
			body.setTextureRect(rect);
		}
	}
	
	// 애니메이션 데이터 없으면 이동만 처리

	
	float h = InputMgr::GetAxis(Axis::Horizontal);
	float v = InputMgr::GetAxis(Axis::Vertical);

	velocity.x = h * speed;
	velocity.y = v * speed;

	position += velocity * dt;
	SetPosition(position);
	UpdateFixedHitBox();
	

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
	UpdateAnimation(dt);
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

void Player::CheckInteractions()
{
	if (sceneGame == nullptr)
	{
		currentInteractable = nullptr;
		showInteractionUI = false;
		return;
	}

	sf::Vector2f playerPos = GetPosition();
	GameObject* nearestObj = nullptr;
	float nearestDistance = std::numeric_limits<float>::max();

	const float INTERACTION_RANGE = 50.0f;

	for (auto* obj : sceneGame->GetInteractables())
	{
		sf::Vector2f objPos = obj->GetPosition();

		// 거리 계산
		float distance = std::sqrt(std::pow(playerPos.x - objPos.x, 2) +
			std::pow(playerPos.y - objPos.y, 2));

		if (distance <= INTERACTION_RANGE && distance < nearestDistance)
		{
			nearestDistance = distance;
			nearestObj = obj;
		}
	}

	if (nearestObj != currentInteractable)
	{
		currentInteractable = nearestObj;
		showInteractionUI = (currentInteractable != nullptr);
	}
}

void Player::TriggerInteraction(GameObject* obj)
{
	if (obj == nullptr) return;
}

void Player::CollideMoving(HitBox box)
{
	
	sf::Vector2f position = GetPosition();
	sf::Vector2f newPos = position;
	sf::Vector2f deltaMove = position - previousPosition;

	
	newPos.x = previousPosition.x + deltaMove.x;
	SetPosition(newPos);
	UpdateFixedHitBox();
	hitBox.SetPosition(newPos);

	if (Utils::CheckCollision(hitBox.rect, box.rect)) {
		newPos.x = previousPosition.x; 
		UpdateFixedHitBox();
		hitBox.SetPosition(newPos);
	}

	
	newPos.y = previousPosition.y + deltaMove.y;
	SetPosition(newPos);
	UpdateFixedHitBox();
	hitBox.SetPosition(newPos);

	if (Utils::CheckCollision(hitBox.rect, box.rect)) {
		newPos.y = previousPosition.y; 
		SetPosition(newPos);
		UpdateFixedHitBox();
		hitBox.SetPosition(newPos);
	}

	SetPosition(newPos);
	UpdateFixedHitBox();
	
}

void Player::TakeDamageIfPossible(int damage)
{
	if (timeSinceLastDamage >= damageCooldown)
	{
		OnDamage(damage);
		timeSinceLastDamage = 0.f;
	}
}
void Player::OnDamage(int damage)
{
	// 보스 전용 씬이면 데미지 무시
	if (SCENE_MGR.GetCurrentSceneId() == SceneIds::Boss)
		return;

	hp = Utils::Clamp(hp - damage, 0, maxHp);
	SOUND_MGR.PlaySfx(SOUNDBUFFER_MGR.Get("effects/link hurt.wav"));
	std::cout << "[Player] damage! " << damage << " ▶ HP: " << hp << "\n";
	hp -= damage;
	if (hp < 0) hp = 0;
	if (hud != nullptr)
	{
		// 💡 하트 개수 = hp / 2 (2 체력 = 1 하트)
		hud->SetHeartCount(hp);  // ❤️ 하트 동기화
	}

	if (hp <= 0)
	{
		std::cout << "[Player] Die!\n";
		SOUND_MGR.PlaySfx(SOUNDBUFFER_MGR.Get("effects/link dies.wav"));
		SCENE_MGR.ChangeScene(SceneIds::GameOver);
		SetActive(false); // 비활성화 또는 리스폰 처리
		// 여기에 죽었을 때 상태 전환이나 UI 호출 가능
	}
	isInvincible = true;
	invincibleElapsed = 0.0f;

	// 추가로 맞을 수 없도록 쿨타임도 초기화
	timeSinceLastDamage = 0.f;
}

void Player::UpdateFixedHitBox()
{
	hitBox.UpdateTransform(body, GetLocalBounds());
	sf::Vector2f fixedSize = { 8.f, 8.f };              // 원하는 히트박스 크기
	hitBox.rect.setSize(fixedSize);

	sf::Vector2f offset(4.f, 8.f);
	hitBox.rect.setPosition(GetPosition() + offset);
}

bool Player::IsAttacking() const
{
	return isAttacking;
}

void Player::Heal(int amount)
{
	hp = Utils::Clamp(hp + amount, 0, maxHp);
		SOUND_MGR.PlaySfx(SOUNDBUFFER_MGR.Get("effects/heart.wav"));
	if (hud != nullptr)
	{
		hud->SetHeartCount(hp);

	}
}

void Player::UpdateAnimation(float dt)
{
	std::vector<sf::IntRect>* currentVec = nullptr;
	auto& vec = pushingAnimations[currentDirection];
	if (vec.empty())
	{
		std::cerr << "⚠️ Push 애니메이션 비어 있음: 방향 = " << static_cast<int>(currentDirection) << "\n";
		return;
	}

	switch (playerState)
	{
	case PlayerState::Idle:
		currentVec = &animations[currentDirection];
		currentFrame = 0;
		break;

	case PlayerState::Walk:
		currentVec = &animations[currentDirection];
		break;

	case PlayerState::Push:
		currentVec = &pushingAnimations[currentDirection];
		break;

	case PlayerState::Attack:
		return; // 공격 애니메이션은 Update()에서 따로 처리
	}

	// 벡터가 유효한지 확인
	if (!currentVec || currentVec->empty())
	{
		std::cerr << "[애니메이션 오류] state = " << static_cast<int>(playerState)
			<< ", currentDirection = " << static_cast<int>(currentDirection)
			<< ", currentVec is empty or null!\n";
		return;
	}

	// 프레임 시간 계산
	if (playerState == PlayerState::Walk || playerState == PlayerState::Push)
	{
		elapsedTime += dt;
		if (elapsedTime >= frameTime)
		{
			elapsedTime = 0.f;
			currentFrame = (currentFrame + 1) % currentVec->size();
		}
	}

	// 현재 프레임 유효성 확인
	if (currentFrame >= currentVec->size()) currentFrame = 0;


	// 텍스처 적용
	sf::IntRect rect = (*currentVec)[currentFrame];
	if (currentDirection == Direction::Left)
	{
		rect.left += rect.width;
		rect.width = -rect.width;
	}
	body.setTextureRect(rect); 

}

void Player::SetSceneGame(SceneGame* scene)
{
	this->sceneGame = scene;
}


