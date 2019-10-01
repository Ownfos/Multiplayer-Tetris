#include "TetrisBoard.h"

using namespace ownfos::network;
using namespace ownfos::graphic;

TetrisBoard::TetrisBoard
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
) :
	Object(id), application(application), system(system),
	socket(socket), input(input), active(active), boardX(boardX),
	boardY(boardY), currentBlockType(initialBlockType)
{

}

void TetrisBoard::RenderBlock(int x, int y, int type)
{
	auto color = (type == -1) ? Color(0.5f,0.5f,0.5f,1.0f) : blockColor[type];
	application->RenderBlock
	(
		color,
		boardX + x * blockSize + offsetX,
		boardY + y * blockSize + offsetY,
		0.0f,
		blockSize / 2 - 1,
		blockSize / 2 - 1
	);

	color = Color::Interpolate(color, { 0.0f,0.0f,0.0f,1.0f }, 0.9f);
	application->RenderBlock
	(
		color,
		boardX + x * blockSize + offsetX,
		boardY + y * blockSize + offsetY,
		0.0f,
		blockSize / 2 - 5,
		blockSize / 2 - 5
	);
}

bool TetrisBoard::IsColliding(int type, int rotation, int x, int y)
{
	for (int i = 0; i < 4; i++)
	{
		int boxX = x + blockShape[type][rotation][i][0];
		int boxY = y + blockShape[type][rotation][i][1];

		if (boxX < 0 || boxX>9 || boxY < 0 || boxY>19)
			return true;
		if (board[boxX][boxY] != -1)
			return true;
	}
	return false;
}

void TetrisBoard::CalculateHardDropPosition()
{
	hardDropY = currentBlockY - 1;
	while (!IsColliding(currentBlockType, currentBlockRotation, currentBlockX, hardDropY))
		hardDropY--;
	hardDropY++;
}

int TetrisBoard::PlaceBlock(int type, int rotation, int x, int y)
{
	// place block
	for (int i = 0; i < 4; i++)
	{
		int boxX = x + blockShape[type][rotation][i][0];
		int boxY = y + blockShape[type][rotation][i][1];
		board[boxX][boxY] = type;
	}

	// delete filled lines
	int linesDeleted = 0;
	for (int y = 0; y < 20; y++)
	{
		bool filled = true;
		for (int x = 0; x < 10; x++)
			if (board[x][y] == -1 || board[x][y] == 7)
				filled = false;

		if (filled)
		{
			for (int yy = y + 1; yy < 20; yy++)
				for (int xx = 0; xx < 10; xx++)
					board[xx][yy - 1] = board[xx][yy];
			y--;
			linesDeleted++;
		}
	}

	return linesDeleted;
}

void TetrisBoard::GameOver()
{
	Buffer buffer;
	buffer.Write<int>(9);
	socket->SendData(buffer);

	MessageBox(NULL, "You lost the match!", "Game Over", MB_OK);

	system->EndMatch();
}

void TetrisBoard::DropAndCheckGameOver()
{
	auto newType = application->RandomInt(0, 6);
	auto linesDeleted = HardDrop(newType);

	int linesToSend = 0;
	if (linesDeleted > 0)
	{
		switch (linesDeleted)
		{
		case 2: linesToSend += 1; break;
		case 3: linesToSend += 2; break;
		case 4: linesToSend += 4; break;
		}

		switch (combo)
		{
		case 0: break;
		case 1:
		case 2: linesToSend += 1; break;
		case 3:
		case 4: linesToSend += 2; break;
		case 5:
		case 6: linesToSend += 3; break;
		default: linesToSend += 4;
		}

		combo++;
		timeSinceLastCombo = 0;
	}

	if (IsColliding(currentBlockType, currentBlockRotation, currentBlockX, currentBlockY))
	{
		GameOver();
	}
	else
	{
		int dummyLinesRemoved = 0;
		while (board[0][dummyLinesRemoved] == 7 && dummyLinesRemoved < linesToSend) dummyLinesRemoved++;
		RemoveDummyLines(dummyLinesRemoved);
		linesToSend -= dummyLinesRemoved;

		Buffer buffer;
		buffer.Write<int>(10);
		buffer.Write<int>(newType);
		buffer.Write<int>(linesToSend);
		buffer.Write<int>(dummyLinesRemoved);
		socket->SendData(buffer);
	}
}

std::tuple<int, int> TetrisBoard::CurrentBlockXMinMax()
{
	int minX = 10;
	int maxX = -1;
	for (int i = 0; i < 4; i++)
	{
		int boxX = currentBlockX + blockShape[currentBlockType][currentBlockRotation][i][0];
		if (boxX < minX)
			minX = boxX;
		if (boxX > maxX)
			maxX = boxX;
	}

	return { minX, maxX };
}

int TetrisBoard::HardDrop(int newType)
{
	auto linesDeleted = PlaceBlock(currentBlockType, currentBlockRotation, currentBlockX, hardDropY);
	SetBlockType(newType);
	currentBlockY = 18;

	// adjust x coordinate to prevent new block from getting stuck at wall
	auto [minX, maxX] = CurrentBlockXMinMax();
	if (minX < 0)
		currentBlockX -= minX;
	if (maxX > 9)
		currentBlockX -= (maxX - 9);

	CalculateHardDropPosition();

	return linesDeleted;
}

void TetrisBoard::SetBlockType(int type)
{
	currentBlockType = type;
}

void TetrisBoard::RotateBlock()
{
	int oldRotation = currentBlockRotation;
	currentBlockRotation = (currentBlockRotation + 1) % 4;

	// adjust x coordinate
	auto [minX, maxX] = CurrentBlockXMinMax();
	if (minX < 0)
		currentBlockX -= minX;
	if (maxX > 9)
		currentBlockX -= (maxX - 9);

	// revert changes if inappropriate
	if (IsColliding(currentBlockType, currentBlockRotation, currentBlockX, currentBlockY))
	{
		currentBlockRotation = oldRotation;
		if (minX < 0)
			currentBlockX += minX;
		if (maxX > 9)
			currentBlockX += (maxX - 9);
	}
}

void TetrisBoard::MoveBlock(int dx, int dy)
{
	currentBlockX += dx;
	currentBlockY += dy;
}

void TetrisBoard::AddDummyLines(int count)
{
	for (int i = 0; i < count; i++)
	{
		for (int y = 19; y > 0; y--)
			for (int x = 0; x < 10; x++)
				board[x][y] = board[x][y - 1];

		for (int x = 0; x < 10; x++)
			board[x][0] = 7;
	}
}

void TetrisBoard::RemoveDummyLines(int count)
{
	std::lock_guard guard(dataMutex);

	for (int i = 0; i < count; i++)
		for (int y = 1; y < 20; y++)
			for (int x = 0; x < 10; x++)
				board[x][y - 1] = board[x][y];
}

void TetrisBoard::Deactivate()
{
	active = false;
}

void TetrisBoard::OnCreate()
{
	for (int x = 0; x < 10; x++)
		for (int y = 0; y < 20; y++)
			board[x][y] = -1;
}

void TetrisBoard::OnUpdate(float deltaTime)
{
	if (active)
	{
		// timers
		{
			if (combo > 0)
			{
				timeSinceLastCombo += deltaTime;
				if (timeSinceLastCombo > comboTimeOut)
					combo = 0;
			}

			autoDownTimer++;

			if (input->FrameState(KeyCode::Down) == InputState::ReleasingNow)
				downTimer = 0;
			if (input->FrameState(KeyCode::Down) == InputState::Pressing)
				downTimer++;
			if (input->FrameState(KeyCode::Right) == InputState::ReleasingNow)
				rightTimer = 0;
			if (input->FrameState(KeyCode::Right) == InputState::Pressing)
				rightTimer++;
			if (input->FrameState(KeyCode::Left) == InputState::ReleasingNow)
				leftTimer = 0;
			if (input->FrameState(KeyCode::Left) == InputState::Pressing)
				leftTimer++;
		}

		if (input->FrameState(KeyCode::Up) == InputState::PressedNow)
		{
			RotateBlock();

			Buffer buffer;
			buffer.Write<int>(8);
			socket->SendData(buffer);
		}

		if (input->FrameState(KeyCode::Right) == InputState::PressedNow || rightTimer > 5)
		{
			if (rightTimer > 5)
				rightTimer = 0;

			if (!IsColliding(currentBlockType, currentBlockRotation, currentBlockX + 1, currentBlockY))
			{
				MoveBlock(1, 0);

				Buffer buffer;
				buffer.Write<int>(11);
				buffer.Write<int>(1);
				buffer.Write<int>(0);
				socket->SendData(buffer);
			}
		}

		if (input->FrameState(KeyCode::Left) == InputState::PressedNow || leftTimer > 5)
		{
			if (leftTimer > 5)
				leftTimer = 0;

			if (!IsColliding(currentBlockType, currentBlockRotation, currentBlockX - 1, currentBlockY))
			{
				MoveBlock(-1, 0);

				Buffer buffer;
				buffer.Write<int>(11);
				buffer.Write<int>(-1);
				buffer.Write<int>(0);
				socket->SendData(buffer);
			}
		}

		if (input->FrameState(KeyCode::Down) == InputState::PressedNow || autoDownTimer > 60 || downTimer > 5)
		{
			autoDownTimer = 0;
			if (downTimer > 5)
				downTimer = 0;

			if (!IsColliding(currentBlockType, currentBlockRotation, currentBlockX, currentBlockY - 1))
			{
				MoveBlock(0, -1);

				Buffer buffer;
				buffer.Write<int>(11);
				buffer.Write<int>(0);
				buffer.Write<int>(-1);
				socket->SendData(buffer);
			}
			else
			{
				DropAndCheckGameOver();
			}
		}

		if (input->FrameState(KeyCode::Space) == InputState::PressedNow)
			DropAndCheckGameOver();
	}

	if (currentBlockType != -1)
		CalculateHardDropPosition();
}

void TetrisBoard::OnRender()
{
	// background
	application->RenderBlock(
		{ 36U, 36U, 36U, 255U },
		boardX, boardY, 0.0f,
		blockSize * 5,
		blockSize * 10
	);

	// board
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 20; y++)
		{

			if (board[x][y] == -1)
			{
				auto saturation = 43U;
				if ((x + y) % 2 == 0)
					saturation = 47U;
				
				application->RenderBlock
				(
					{ saturation , saturation , saturation , 255U },
					boardX + x * blockSize + offsetX,
					boardY + y * blockSize + offsetY,
					0.0f,
					blockSize / 2 - 1,
					blockSize / 2 - 1
				);
			}
			else
			{
				RenderBlock(x, y, board[x][y]);
			}
		}
	}

	// hard drop preview
	for (int i = 0; i < 4; i++)
	{
		auto x = currentBlockX + blockShape[currentBlockType][currentBlockRotation][i][0];
		auto y = hardDropY + blockShape[currentBlockType][currentBlockRotation][i][1];
		RenderBlock(x, y, -1);
	}

	// actual block
	for (int i = 0; i < 4; i++)
	{
		auto x = currentBlockX + blockShape[currentBlockType][currentBlockRotation][i][0];
		auto y = currentBlockY + blockShape[currentBlockType][currentBlockRotation][i][1];
		RenderBlock(x, y, currentBlockType);
	}

	 
	// combo counter
	if (active && combo > 0)
		application->RenderText(L"Combo: " + std::to_wstring(combo), { 1.0f,1.0f,1.0f,1.0f }, boardX - blockSize * 6, boardY - blockSize * 9, 0.0f);
}

void TetrisBoard::OnDestroy()
{
}
