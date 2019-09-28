#pragma once

#include "..//library/winsock playground/Socket.h"
#include "..//library/directx11 playground/BasicInputListener.h"

#include "Object.h"
#include "MyApplication.h"
#include "System.h"

namespace
{
	constexpr int blockSize			= 20;
	constexpr int offsetX			= -(blockSize * 5 - blockSize / 2);
	constexpr int offsetY			= -(blockSize * 10 - blockSize / 2);
	constexpr float comboTimeOut	= 3.0f;
}

class TetrisBoard : public Object
{
private:
	MyApplication*	application;
	System*			system;
	
	std::shared_ptr<ownfos::network::Socket>				socket;
	std::shared_ptr<ownfos::graphic::BasicInputListener>	input;
	
	bool	active;
	int		boardX;
	int		boardY;
	
	int board[10][20] = {};
	
	int autoDownTimer	= 0;
	int downTimer		= 0;
	int rightTimer		= 0;
	int leftTimer		= 0;
	
	int currentBlockType;
	int currentBlockRotation	= 0;
	int currentBlockX			= 5;
	int currentBlockY			= 18;

	// lowest position's y which coordinate current block can go
	int hardDropY = 0;

	int combo = 0;

	float timeSinceLastCombo = 0;

	// [type][rotation][index][x : 0, y : 1]
	int blockShape[7][4][4][2] =
	{
		{{{0,0}, { 1,0}, {-1, 0}, { 0, 1}}, {{0,0}, { 1, 0}, {0, 1}, {0,-1}}, {{0,0}, { 1,0}, { 0,-1}, {-1, 0}}, {{0,0}, {-1, 0}, {0, 1}, { 0,-1}}},
		{{{0,0}, { 1,0}, { 1, 1}, { 0,-1}}, {{0,0}, {-1, 0}, {0,-1}, {1,-1}}, {{0,0}, { 0,1}, {-1, 0}, {-1,-1}}, {{0,0}, { 1, 0}, {0, 1}, {-1, 1}}},
		{{{0,0}, {-1,0}, {-1, 1}, { 0,-1}}, {{0,0}, {-1, 0}, {0, 1}, {1, 1}}, {{0,0}, { 0,1}, { 1, 0}, { 1,-1}}, {{0,0}, { 1, 0}, {0,-1}, {-1,-1}}},
		{{{0,0}, { 0,1}, { 0,-1}, { 0,-2}}, {{0,0}, {-1, 0}, {1, 0}, {2, 0}}, {{0,0}, { 0,1}, { 0,-1}, { 0,-2}}, {{0,0}, {-1, 0}, {1, 0}, { 2, 0}}},
		{{{0,0}, { 1,0}, { 1, 1}, { 0, 1}}, {{0,0}, { 1, 0}, {1, 1}, {0, 1}}, {{0,0}, { 1,0}, { 1, 1}, { 0, 1}}, {{0,0}, { 1, 0}, {1, 1}, { 0, 1}}},
		{{{0,0}, { 1,0}, {-1, 0}, { 1, 1}}, {{0,0}, { 0, 1}, {0,-1}, {1,-1}}, {{0,0}, { 1,0}, {-1, 0}, {-1,-1}}, {{0,0}, { 0, 1}, {0,-1}, {-1, 1}}},
		{{{0,0}, {-1,0}, { 1, 0}, {-1, 1}}, {{0,0}, { 0,-1}, {0, 1}, {1, 1}}, {{0,0}, {-1,0}, { 1, 0}, { 1,-1}}, {{0,0}, { 0,-1}, {0, 1}, {-1,-1}}}
	};
	ownfos::graphic::Color blockColor[8] =
	{
		{0xa349a3ff},
		{0xee2424ff},
		{0x21b14eff},
		{0x00a2eaff},
		{0xffc90dff},
		{0xfd8026ff},
		{0x3f47ccff},
		{0x90918cff}
	};

	void RenderBlock(int x, int y, int type);

	// checks collision of specified block with placed blocks
	bool IsColliding(int type, int rotation, int x, int y);

	// calculates y coordinate of hard drop position
	void CalculateHardDropPosition();

	// place block and delete filled lines.
	// returns number of lines deleted
	int PlaceBlock(int type, int rotation, int x, int y);

	// end match and notify opponent
	void GameOver();

	// perform hard drop, game over check, then notify opponent
	void DropAndCheckGameOver();

	// returns minimum and maximum x coordinate of current block.
	// used to adjust x coordinate after rotating current block or creating new block.
	std::tuple<int, int> CurrentBlockXMinMax();

public:
	TetrisBoard
	(
		ObjectID												id,
		MyApplication*											application,
		System*													system,
		std::shared_ptr<ownfos::network::Socket>				socket,
		std::shared_ptr<ownfos::graphic::BasicInputListener>	input,
		bool													active,
		int														boardX,
		int														boardY,
		int														initialBlockType
	);

	// drop block to the most low position
	int HardDrop(int newType);

	void SetBlockType(int type);

	// rotate and adjust x position.
	// doesn't make change if invalid
	void RotateBlock();

	void MoveBlock(int dx, int dy);

	void AddDummyLines(int count);

	void RemoveDummyLines(int count);

	// stops OnUpdate() from doing actions
	void Deactivate();

	void OnCreate() override;

	void OnUpdate(float deltaTime) override;

	void OnRender() override;

	void OnDestroy() override;
};

