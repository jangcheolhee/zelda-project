#pragma once
#define DEF_DEV

enum class SortingLayers
{
	Background,
	Foreground,
	Default,
	UI,
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
	Game,
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
	None,
	Down, 
	Left,
	Right, 
	Up,
	None,
};