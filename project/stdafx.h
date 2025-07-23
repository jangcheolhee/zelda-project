#pragma once
#include "stdafx.h"
// STL
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <functional>

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// FRAMEWORK
#include "json.hpp"
using json = nlohmann::json;

#include "Defines.h"
#include "Variables.h"
#include "Utils.h"
#include "Singleton.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "GameObject.h"
#include "Scene.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "Framework.h"
#include "Player.h"
#include "Enemy.h"
#include "Character.h"
#include "AnimationApp.h"
#include "AnimationClipManager.h"
#include "AnimationPlayer.h"
#include "AnimationEditorUI.h"
