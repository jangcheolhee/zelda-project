#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "TileMap.h"
#include "BasicEnemy.h"
#include "Bush.h"
#include "Npc.h"
#include "Chest.h"
#include "Rupee.h"
#include "JumpWall.h"
#include "Heart.h"
#include "HUD.h"
#include <istream>

SceneGame::SceneGame()
	:Scene(SceneIds::Game)
{
}

void SceneGame::InitZones()
{
	mapZones.clear();

	mapZones.push_back({
		//zone1 origin
		sf::FloatRect(0, 260, 511, 499), //zone 1_confirm
		1,
		[this]()
		  {
			  std::cout << "Zone 1 Enter" << std::endl;
			  sf::Vector2f enemyPos = tileMapGame->getPosition(2, 18585);
			  SpawnEnemy(enemyPos, Enemy::Types::Basic);

		  },
		[this]()
		  {
			  std::cout << "Zone 1 Exit" << std::endl;
			  DeleteInteractables();
			  DeleteEnemy();
		  },
		false
		});
	// Zone 2 origin_left1
	mapZones.push_back({
		sf::FloatRect(-512, 260, 511,499),
		2,
		[this]()
		{
			std::cout << "Zone 2 Enter" << std::endl;
		},
		[this]()
		{
			std::cout << "Zone 2 Exit" << std::endl;
			DeleteEnemy();
		},
		false
		});
	// Zone 3 origin_up1
	mapZones.push_back({
		sf::FloatRect(0, -240, 511, 499),
		3,
		[this]()
		{
			std::cout << "Zone 3 Enter" << std::endl;
		},
		[this]()
		{
			std::cout << "Zone 3 Exit" << std::endl;
			DeleteEnemy();
		},
		false
		});
	// Zone 4 origin_up2
	mapZones.push_back({
		sf::FloatRect(0, -740, 511, 499),
		4,
		[this]()
		{
			std::cout << "Zone 4 Enter" << std::endl;
		},
		[this]()
		{
			std::cout << "Zone 4 Exit" << std::endl;
			DeleteEnemy();
		},
		false
		});
}

void SceneGame::UpdateZones()
{
	sf::Vector2f playerPos = player->GetGlobalBounds().getPosition();
	for (auto& zone : mapZones)
	{
		bool nowInZone = zone.bounds.contains(playerPos);
		if (nowInZone && !zone.entered)
		{
			zone.entered = true;
			zoneID = zone.zoneId;

			if (zone.onEnter)
			{
				zone.entered = true;
				zoneID = zone.zoneId;

				changeZone = true;
				zone.onEnter();

				SpawnInteractableObject(zone.bounds);
				SpawnFlowers(zone.bounds);
				SpawnSquareHitBox();

			}
		}
		else if (!nowInZone && zone.entered)
		{
			zone.entered = false;



			
			if (zone.onExit)
			{
				zone.onExit();
			}
			DeleteInteractables();

			for (auto f : flowers)
			{
				RemoveGameObject(f);
			}
			flowers.clear();



		}
	}
}
void SceneGame::UpdateBehaviorZone(float dt)
{
	float x = Utils::Clamp(player->GetGlobalBounds().getPosition().x, mapZones[zoneID - 1].bounds.left + worldView.getSize().x / 2, mapZones[zoneID - 1].bounds.left + mapZones[0].bounds.width - worldView.getSize().x / 2);
	float y = Utils::Clamp(player->GetGlobalBounds().getPosition().y, mapZones[zoneID - 1].bounds.top + worldView.getSize().y / 2, mapZones[zoneID - 1].bounds.top + mapZones[0].bounds.height - worldView.getSize().y / 2);
	if (changeZone)
	{
		worldView.setCenter(Utils::Lerp(worldView.getCenter(), { x,y }, dt * 2));
		if (Utils::Distance(worldView.getCenter(), { x,y }) > 1 && Utils::Distance(worldView.getCenter(), { x,y }) < 5)
		{
			//std::cout << Utils::Distance(worldView.getCenter(), { x,y }) << std::endl;
			changeZone = false;
		}
	}
	else
	{
		worldView.setCenter({ x, y });
	}
	//switch (zoneID)
	//{//player 기준
	//case 1:
	//break;
	//}
	
}

void SceneGame::DeleteInteractables()
{
	for (Interactable* inter : interactList)
	{
		inter->SetActive(false);
		interactPool[inter->GetType()].push_back(inter);
	}
	interactList.clear();


}
void SceneGame::RecycleEnemy(Enemy* enemy)
{
	if (enemy)
	{
		enemy->SetActive(false);
		enemyPools[enemy->GetType()].push_back(enemy);
	}
}

void SceneGame::SpawnEnemy(sf::Vector2f pos, Enemy::Types type)
{
	Enemy* enemy = nullptr;

	auto& pool = enemyPools[type];
	if (!pool.empty())
	{
		enemy = pool.front();
		pool.pop_front();
	}
	else
	{
		switch (type)
		{
		case Enemy::Types::Basic:
			enemy = (Enemy*)AddGameObject(new BasicEnemy());
			break;
		case Enemy::Types::Count:
			break;
		default:
			break;
		}
		enemy->Init();
	}
	enemy->Reset();
	enemy->SetPosition(pos);
	enemy->SetActive(true);

	enemyList.push_back(enemy);
}

void SceneGame::SpawnInteractable(sf::Vector2f pos, Interactable::Type type)
{
	Interactable* inter = nullptr;
	auto& pool = interactPool[type];
	if (!pool.empty())
	{
		inter = pool.front();
		pool.pop_front();
	}
	else
	{
		switch (type)
		{
		case Interactable::Type::None:
			break;
		case Interactable::Type::Throw:
			inter = (Bush*)AddGameObject(new Bush());
			break;
		case Interactable::Type::Chest:
			break;
		case Interactable::Type::JumpWall:
		{
			JumpWall* j = new JumpWall();
			j->SetBounds(wallX, wallY, wallWithdh, wallHeight);
			inter = (Interactable*)AddGameObject(j);
			break;
		}
		case Interactable::Type::Heart:
			inter = (Heart*)AddGameObject(new Heart());
			break;
		case Interactable::Type::Rupee:
			inter = (Rupee*)AddGameObject(new Rupee());
			break;
		case Interactable::Type::Npc:
			inter = (Npc*)AddGameObject(new Npc());
			break;
		default:
			break;
		}
		inter->Init();

	}
	
	inter->Reset();
	inter->SetActive(true);
	inter->SetPosition(pos);
	interactList.push_back(inter);

}

void SceneGame::SpawnEnemyAtTile(int layerIndex, int targetGid, Enemy::Types type)
{
	std::vector<sf::Vector2f> positions = tileMapGame->getPositions(layerIndex, targetGid);
	for (const auto& pos : positions)
	{
		SpawnEnemy(pos, type);
	}
}

void SceneGame::SpawnFlowers(sf::FloatRect zone)
{
	std::vector<sf::Vector2f> positions = tileMapGame->getPositions(4, 24696);
	for (const auto& pos : positions)
	{
		if (zone.contains(pos))
		{
			auto flower = new SpriteGo();
			flower->SetName("Flower");
			flower->Init();

			flower->GetSprite().setTexture(TEXTURE_MGR.Get("graphics/Overworld.png"));
			flower->GetSprite().setTextureRect({ 760, 41, 8, 8 });

			flower->SetActive(flowerBool);
			flower->SetOrigin(Origins::TL);
			flower->SetPosition(pos);
			AddGameObject(flower);
			flowers.push_back(flower);

		}
	}
}

void SceneGame::FlowerBreath(float dt)
{
	flowerTimer += dt;
	if (flowerTimer >= flowerRate)
	{
		flowerBool = !flowerBool;
		for (auto& flower : flowers)
		{
			flower->SetActive(flowerBool);
		}
		flowerTimer = 0.f;
	}
}

void SceneGame::CheckCollison()
{
	for (auto& enemy : enemyList)
	{
		if (player->GetGlobalBounds().intersects(enemy->GetGlobalBounds()))
		{
			player->OnCollide(enemy);
			enemy->OnCollide(player);
		}
	}

	sf::FloatRect rect = player->GetGlobalBounds();
	rect.left -= 2.f;
	rect.top -= 2.f;
	rect.width += 4.f;
	rect.height += 4.f;
	for (auto& obj : interactList)
	{

		for (auto& enemy : enemyList)
		{

			if (obj->GetGlobalBounds().intersects(enemy->GetGlobalBounds()))
			{
				enemy->SetPosition(enemy->GetPos());
			}
		}
		if (obj->GetType() == Interactable::Type::Throw)
		{
			if (rect.intersects(obj->GetGlobalBounds()))
			{
				if (player->WantsToInteract() && !player->IsInteract())
				{
					int r = Utils::RandomRange(0, 3);

					switch (r)
					{
					case 0:
					{

						SpawnInteractable(obj->GetPosition() + sf::Vector2f({ 3,-16 }), Interactable::Type::Rupee);

						break;
					}

					case 1:
					{

						SpawnInteractable(obj->GetPosition() + sf::Vector2f({ 4,-4 }), Interactable::Type::Heart);
						break;
					}
					}
					obj->OnInteract();
					continue;
				}

			}
		}

		if (player->GetGlobalBounds().intersects(obj->GetGlobalBounds()))
		{
			if (obj->GetType() == Interactable::Type::Heart || obj->GetType() == Interactable::Type::Rupee)
			{
				obj->OnInteract();
				continue;
			}
			player->SetPosition(player->GetPos());
			if (obj->GetType() == Interactable::Type::Chest || obj->GetType() == Interactable::Type::JumpWall)
			{
				obj->OnInteract();
			}

		}

		if (player->GetGlobalBounds().intersects(obj->GetGlobalBounds()))
		{
			if (obj->GetType() == Interactable::Type::Rupee || obj->GetType() == Interactable::Type::Heart)
			{
				obj->OnInteract();
				continue;
			}
			player->SetMovable(false);
			if (obj->GetType() == Interactable::Type::Chest || obj->GetType() == Interactable::Type::JumpWall)
			{
				obj->OnInteract();
			}
		}

	}
}
void SceneGame::SpawnSquareHitBox()
{
	HitboxCorners corners;
	if (!tileMapGame->LoadHitboxLayer(corners)) return;
	// 검색 범위와 허용 오차 확대
	const float searchWidth = 500.f;  // 더 큰 히트박스 허용
	const float searchHeight = 500.f;
	const float epsilon = 2.f;        // 코너 매칭 허용 오차 증가
	const float minSize = 4.f;        // 최소 히트박스 크기 (너무 작은 히트박스 제외)

	auto nearlyEqual = [epsilon](float a, float b) {
		return std::abs(a - b) < epsilon;
		};

	// 생성된 hitbox들의 영역을 추적하여 겹침 방지
	std::vector<sf::FloatRect> createdRects;

	// 겹침 검사를 더 엄격하게 수정 - 완전히 포함되거나 상당 부분이 겹치는 경우만 겹침으로 판단
	auto isOverlapping = [](const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
		if (!rect1.intersects(rect2)) return false;

		// 교집합 영역 계산
		sf::FloatRect intersection;
		if (rect1.intersects(rect2, intersection)) {
			float intersectionArea = intersection.width * intersection.height;
			float rect1Area = rect1.width * rect1.height;
			float rect2Area = rect2.width * rect2.height;
			float minArea = std::min(rect1Area, rect2Area);

			// 작은 사각형의 50% 이상이 겹치거나, 교집합이 8픽셀 이상인 경우만 겹침으로 판단
			return (intersectionArea / minArea > 0.5f) || (intersectionArea > 64.0f);
		}
		return false;
		};

	// 이미 처리된 좌상단 점들을 추적
	std::set<std::pair<int, int>> processedTopLefts;

	// 모든 가능한 hitbox 후보들을 수집
	struct HitboxCandidate {
		sf::Vector2f topLeft;
		sf::FloatRect rect;
		int priority; // 낮을수록 우선순위 높음
		std::string method;
	};

	std::vector<HitboxCandidate> allCandidates;

	// 모든 좌상단 점에 대해 가능한 hitbox들을 수집
	for (const auto& topLeft : corners.topLefts)
	{
		// 방법 1: 완전한 4코너 사각형 - 가장 가까운 점들부터 매칭
		// 가장 가까운 우상단 점 찾기
		sf::Vector2f closestTopRight;
		float minTopRightDistance = searchWidth;
		bool foundTopRight = false;

		for (const auto& topRight : corners.topRights)
		{
			if (nearlyEqual(topRight.y, topLeft.y) && topRight.x > topLeft.x)
			{
				float distance = topRight.x - topLeft.x;
				if (distance <= searchWidth && distance < minTopRightDistance)
				{
					minTopRightDistance = distance;
					closestTopRight = topRight;
					foundTopRight = true;
				}
			}
		}

		if (foundTopRight)
		{
			// 가장 가까운 좌하단 점 찾기
			sf::Vector2f closestBottomLeft;
			float minBottomLeftDistance = searchHeight;
			bool foundBottomLeft = false;

			for (const auto& bottomLeft : corners.bottomLefts)
			{
				if (nearlyEqual(bottomLeft.x, topLeft.x) && bottomLeft.y > topLeft.y)
				{
					float distance = bottomLeft.y - topLeft.y;
					if (distance <= searchHeight && distance < minBottomLeftDistance)
					{
						minBottomLeftDistance = distance;
						closestBottomLeft = bottomLeft;
						foundBottomLeft = true;
					}
				}
			}

			if (foundBottomLeft)
			{
				// 예상되는 우하단 위치
				sf::Vector2f expectedBottomRight(closestTopRight.x, closestBottomLeft.y);

				bool foundBottomRight = false;
				for (const auto& bottomRight : corners.bottomRights)
				{
					if (nearlyEqual(bottomRight.x, expectedBottomRight.x) &&
						nearlyEqual(bottomRight.y, expectedBottomRight.y))
					{
						sf::FloatRect rect(topLeft.x, topLeft.y,
							closestTopRight.x - topLeft.x,
							closestBottomLeft.y - topLeft.y);

						// 최소 크기 체크
						if (rect.width < minSize || rect.height < minSize) break;

						// 우선순위 결정: 큰 히트박스와 완전한 4코너 사각형에 높은 우선순위 부여
						float area = rect.width * rect.height;
						float ratio = std::max(rect.width / rect.height, rect.height / rect.width);

						int priority = 0;
						if (area > 50000.0f) priority = 0; // 매우 큰 히트박스 (최우선)
						else if (area > 20000.0f) priority = 1; // 큰 히트박스
						else if (ratio > 8.0f) priority = 2; // 매우 얇고 긴 것
						else if (ratio > 4.0f) priority = 3; // 얇고 긴 것
						else priority = 4; // 일반적인 것

						allCandidates.push_back({ topLeft, rect, priority, "4-corner" });
						foundBottomRight = true;
						break;
					}
				}
			}
		}

		// 방법 2: 좌상단 + 우하단 대각선 매칭 - 가장 가까운 우하단 찾기
		sf::Vector2f closestBottomRight;
		float minBottomRightDistance = searchWidth + searchHeight;
		bool foundBottomRight = false;

		for (const auto& bottomRight : corners.bottomRights)
		{
			if (bottomRight.x > topLeft.x && bottomRight.y > topLeft.y &&
				bottomRight.x - topLeft.x <= searchWidth &&
				bottomRight.y - topLeft.y <= searchHeight)
			{
				float distance = std::sqrt(std::pow(bottomRight.x - topLeft.x, 2) +
					std::pow(bottomRight.y - topLeft.y, 2));
				if (distance < minBottomRightDistance)
				{
					minBottomRightDistance = distance;
					closestBottomRight = bottomRight;
					foundBottomRight = true;
				}
			}
		}

		if (foundBottomRight)
		{
			sf::FloatRect rect(topLeft.x, topLeft.y,
				closestBottomRight.x - topLeft.x,
				closestBottomRight.y - topLeft.y);

			if (rect.width >= minSize && rect.height >= minSize)
			{
				float area = rect.width * rect.height;
				float ratio = std::max(rect.width / rect.height, rect.height / rect.width);

				int priority = 10;
				if (area > 50000.0f) priority = 5; // 매우 큰 히트박스
				else if (area > 20000.0f) priority = 6; // 큰 히트박스
				else if (ratio > 8.0f) priority = 7;
				else if (ratio > 4.0f) priority = 8;

				allCandidates.push_back({ topLeft, rect, priority, "diagonal" });
			}
		}

		// 방법 3: 좌상단 + 우상단 (세로로 가장 가까운 하단 찾기)
		if (foundTopRight)
		{
			float minBottomY = topLeft.y + searchHeight;
			bool foundBottom = false;

			for (const auto& bottomLeft : corners.bottomLefts)
			{
				if (nearlyEqual(bottomLeft.x, topLeft.x) && bottomLeft.y > topLeft.y &&
					bottomLeft.y < minBottomY)
				{
					minBottomY = bottomLeft.y;
					foundBottom = true;
				}
			}

			for (const auto& bottomRight : corners.bottomRights)
			{
				if (nearlyEqual(bottomRight.x, closestTopRight.x) && bottomRight.y > topLeft.y &&
					bottomRight.y < minBottomY)
				{
					minBottomY = bottomRight.y;
					foundBottom = true;
				}
			}

			if (foundBottom)
			{
				sf::FloatRect rect(topLeft.x, topLeft.y,
					closestTopRight.x - topLeft.x,
					minBottomY - topLeft.y);

				if (rect.width >= minSize && rect.height >= minSize)
				{
					float area = rect.width * rect.height;
					float ratio = std::max(rect.width / rect.height, rect.height / rect.width);

					int priority = 15;
					if (area > 50000.0f) priority = 9; // 매우 큰 히트박스
					else if (area > 20000.0f) priority = 10; // 큰 히트박스
					else if (ratio > 8.0f) priority = 11;
					else if (ratio > 4.0f) priority = 12;

					allCandidates.push_back({ topLeft, rect, priority, "horizontal" });
				}
			}
		}

		// 방법 4: 좌상단 + 좌하단 (가로로 가장 가까운 우측 찾기)
		// 가장 가까운 좌하단 점 찾기 (이미 위에서 구했을 수도 있음)
		sf::Vector2f finalBottomLeft;
		float finalMinBottomLeftDistance = searchHeight;
		bool finalFoundBottomLeft = false;

		for (const auto& bottomLeft : corners.bottomLefts)
		{
			if (nearlyEqual(bottomLeft.x, topLeft.x) && bottomLeft.y > topLeft.y)
			{
				float distance = bottomLeft.y - topLeft.y;
				if (distance <= searchHeight && distance < finalMinBottomLeftDistance)
				{
					finalMinBottomLeftDistance = distance;
					finalBottomLeft = bottomLeft;
					finalFoundBottomLeft = true;
				}
			}
		}

		if (finalFoundBottomLeft)
		{
			float minRightX = topLeft.x + searchWidth;
			bool foundRight = false;

			for (const auto& topRight : corners.topRights)
			{
				if (nearlyEqual(topRight.y, topLeft.y) && topRight.x > topLeft.x &&
					topRight.x < minRightX)
				{
					minRightX = topRight.x;
					foundRight = true;
				}
			}

			for (const auto& bottomRight : corners.bottomRights)
			{
				if (nearlyEqual(bottomRight.y, finalBottomLeft.y) && bottomRight.x > topLeft.x &&
					bottomRight.x < minRightX)
				{
					minRightX = bottomRight.x;
					foundRight = true;
				}
			}

			if (foundRight)
			{
				sf::FloatRect rect(topLeft.x, topLeft.y,
					minRightX - topLeft.x,
					finalBottomLeft.y - topLeft.y);

				if (rect.width >= minSize && rect.height >= minSize)
				{
					float area = rect.width * rect.height;
					float ratio = std::max(rect.width / rect.height, rect.height / rect.width);

					int priority = 16;
					if (area > 50000.0f) priority = 13; // 매우 큰 히트박스
					else if (area > 20000.0f) priority = 14; // 큰 히트박스
					else if (ratio > 8.0f) priority = 15;
					else if (ratio > 4.0f) priority = 16;

					allCandidates.push_back({ topLeft, rect, priority, "vertical" });
				}
			}
		}
	}

	// 우선순위별로 정렬 - 수동으로 구현
	for (size_t i = 0; i < allCandidates.size(); ++i)
	{
		for (size_t j = i + 1; j < allCandidates.size(); ++j)
		{
			bool shouldSwap = false;
			if (allCandidates[i].priority > allCandidates[j].priority)
			{
				shouldSwap = true;
			}
			else if (allCandidates[i].priority == allCandidates[j].priority)
			{
				float areaI = allCandidates[i].rect.width * allCandidates[i].rect.height;
				float areaJ = allCandidates[j].rect.width * allCandidates[j].rect.height;
				if (areaI < areaJ)
				{
					shouldSwap = true;
				}
			}

			if (shouldSwap)
			{
				HitboxCandidate temp = allCandidates[i];
				allCandidates[i] = allCandidates[j];
				allCandidates[j] = temp;
			}
		}
	}
	// 우선순위 순서대로 겹치지 않는 hitbox들을 생성
	for (const auto& candidate : allCandidates)
	{
		std::pair<int, int> tlKey = std::make_pair(static_cast<int>(std::round(candidate.topLeft.x)),
			static_cast<int>(std::round(candidate.topLeft.y)));

		// 겹침 체크
		bool overlaps = false;
		for (const auto& existingRect : createdRects)
		{
			if (isOverlapping(candidate.rect, existingRect))
			{
				overlaps = true;
				break;
			}
		}

		// 이미 이 좌상단 점을 처리했는지 확인 (겹침이 없는 경우에만)
		if (!overlaps && processedTopLefts.find(tlKey) != processedTopLefts.end()) overlaps = true;

		if (!overlaps && candidate.rect.width > 0 && candidate.rect.height > 0)
		{
			HitBox hitbox;
			hitbox.UpdateTransformCollision(collisionBox, candidate.rect,
				sf::Vector2f(candidate.rect.left, candidate.rect.top));
			collisions.push_back(hitbox);
			createdRects.push_back(candidate.rect);
			processedTopLefts.insert(tlKey);
		}
	}
}

void SceneGame::SpawnInteractableObject(sf::FloatRect zone)
{
	//layer 1 : bush
	int layer1Gid[] = { 24670, 24590 };
	for (int id : layer1Gid)
	{
		std::vector <sf::Vector2f> positions = tileMapGame->getPositions(1, id);
		for (const auto& pos : positions)
		{
			if ((pos.x >= zone.left && pos.x <= (zone.left + zone.width)) && (pos.y >= zone.top && pos.y <= zone.top + zone.height))
			{//bush
				SpawnInteractable(pos, Interactable::Type::Throw);
			}
		}
	}

	//layer 2 : npc
	int layer2Gid[] = { 24638 };
	for (int id : layer2Gid)
	{
		std::vector <sf::Vector2f> positions = tileMapGame->getPositions(2, id);
		for (const auto& pos : positions)
		{//npc
			if ((pos.x >= zone.left && pos.x <= (zone.left + zone.width)) && (pos.y >= zone.top && pos.y <= zone.top + zone.height))
			{
				SpawnInteractable(pos, Interactable::Type::Npc);
			}
		}
	}
	//layer3: collision
	int gid[] = { 25075,25067,24699,25068,24592 };
	for (int id : gid)
	{
		std::vector <sf::Vector2f> positions = tileMapGame->getPositions(3, id);
		for (const auto& pos : positions)
		{
			if ((pos.x >= zone.left && pos.x <= (zone.left + zone.width)) && (pos.y >= zone.top && pos.y <= zone.top + zone.height))
			{
				/*auto inter = new JumpWall();
				switch (id)
				{
				case 25075:
					inter->SetDirection(Direction::Down);
					break;
				case 25067:
					inter->SetDirection(Direction::Up);
					break;
				case 24699:
					inter->SetDirection(Direction::Left);
					break;
				case 25068: case 24592:
					inter->SetDirection(Direction::None);
					break;
				default:
					break;
				}
				AddGameObject(inter);
				interactables.push_back(inter);
				inter->SetOrigin(Origins::TC);
				inter->Reset();
				inter->SetPosition(pos);*/

			}
		}
	}
}

void SceneGame::DeleteEnemy()
{
	for (Enemy* e : enemyList)
	{
		RecycleEnemy(e);
	}
	enemyList.clear();
}

void SceneGame::Init()
{
	texIds.push_back("graphics/sprite_sheet.png");
	texIds.push_back("graphics/bush.png");
	texIds.push_back("graphics/Overworld.png");
	texIds.push_back("graphics/npc.png");
	texIds.push_back("graphics/Enemy_sheet.png");
	texIds.push_back("graphics/Items.png");
	texIds.push_back("graphics/HUD.png");
	texIds.push_back("graphics/flower.png");
	texIds.push_back("graphics/Effects.png");
	texIds.push_back("graphics/Death.png");
	//fontIds.push_back("fonts/DS-DIGIT.ttf");
	ANI_CLIP_MGR.Load("animations/bush2.csv");
	ANI_CLIP_MGR.Load("animations/EnemyDeath.csv");
	//ANI_CLIP_MGR.Load("animations/run.csv");
	//ANI_CLIP_MGR.Load("animations/jump.csv");

	player = new Player("Player");
	// 3) 타일맵도 만들고 Init()
	tileMapGame = new TileMap("TileMap", "data/originalMap.tmj");
	tileMapGame->Init();
	AddGameObject(new HUD());
	AddGameObject(player);
	AddGameObject(tileMapGame);

	InitZones();

	endPos = tileMapGame->getPosition(4, 24590);
	endHole = sf::FloatRect(endPos.x - 16, endPos.y - 16, 32, 32);

	flowerTimer = 0.f;
	flowerBool = true;

	Scene::Init();
}

void SceneGame::Exit()
{
	GAME_MGR.SetPlayerData(player->GetHp(), player->GetPosition());
	//GAME_MGR.SaveGame("data/data.json");
	for (Enemy* enemy : enemyList)
	{
		enemy->SetActive(false);
		enemyPools[enemy->GetType()].push_back(enemy);
	}
	enemyList.clear();
	Scene::Exit();
}

void SceneGame::Enter()
{

	auto size = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f center{ size.x * 0.5f, size.y * 0.5f };
	uiView.setSize(size);
	uiView.setCenter(center);
	worldView.setSize({ size.x * .5f, size.y * .5f });
	worldView.setCenter(player->GetGlobalBounds().getPosition());
	sf::Vector2f startPos = tileMapGame->getPosition(2, 18585);
	Scene::Enter();
	player->SetPosition(startPos);
}

void SceneGame::Update(float dt)
{

	Scene::Update(dt);
	auto it = enemyList.begin();
	while (it != enemyList.end())
	{
		if (!(*it)->GetActive())
		{
			RecycleEnemy(*it);
			it = enemyList.erase(it);
		}
		else
		{
			++it;
		}
	}
	auto it1 = interactList.begin();
	while (it1 != interactList.end())
	{
		if (!(*it1)->GetActive())
		{
			(*it1)->SetActive(false);
			interactPool[(*it1)->GetType()].push_back(*it1);
			it1 = interactList.erase(it1);
		}
		else
		{
			++it1;
		}
	}
	CheckCollison();
	UpdateZones();
	UpdateBehaviorZone(dt);
	FlowerBreath(dt);


	if (endHole.contains(player->GetGlobalBounds().getPosition()))
	{
		std::cout << "Hidden" << std::endl;
		SCENE_MGR.ChangeScene(SceneIds::Hidden);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F2))
	{
		std::cout << "Hidden" << std::endl;
		SCENE_MGR.ChangeScene(SceneIds::Hidden);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F1))
	{
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F3))
	{
		std::cout << "PlayerPosition" << player->GetPosition().x << ", " << player->GetPosition().y << ")" << std::endl;
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
	window.setView(worldView);
	for (auto& col : collisions)
	{
		col.Draw(window);
	}
}
