#pragma once
#define DEF_DEV

enum class SortingLayers
{
	Background, //0, tilemap
	Foreground, //1, interactble ->
	Player, //2, player
	Enemy, //3, enemy
	Default, // 4, hitbox
	UI, //5, UI
};

enum class Origins
{
	TL, TC, TR,	// 0 1 2
	ML, MC, MR,	// 3 4 5
	BL, BC, BR, // 6 7 8
	Custom
};

enum class SceneIds
{
	None = -1,
	Select,
	Game,
	Hidden,
	Castle,
	Boss,
	GameOver,
	Count,
};

enum class Sides
{
	Up,
	Left,
	Down,
	Right,
	None,
};

enum class Direction
{
	None = -1,
	Down, 
	Left,
	Right, 
	Up,

};
