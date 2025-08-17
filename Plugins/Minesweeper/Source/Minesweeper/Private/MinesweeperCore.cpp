// Copyright Epic Games, Inc. All Rights Reserved.


#include "MinesweeperCore.h"

#include "MinesweeperLog.h"

FMinesweeperCore::FMinesweeperCore()
	: CurrentGameState(EMinesweeperGameState::NotStarted)
	, RevealedTileCount(0)
	, FlaggedTileCount(0) {}

FMinesweeperCore::~FMinesweeperCore()
{
	GameBoardTiles.Empty();
}

// ==== Game Management

void FMinesweeperCore::InitializeGame(const FMinesweeperGameSettings& InSettings)
{
	GameSettings = InSettings;
	GameSettings.ValidateAndClamp();

	ResetGame();
	GenerateBoardTiles();

	CurrentGameState = EMinesweeperGameState::Active;
	MS_DISPLAY("Game initialized with %dx%d grid and %d bombs", GameSettings.GridWidth, GameSettings.GridHeight, GameSettings.BombCount);
}

void FMinesweeperCore::ResetGame()
{
	CurrentGameState = EMinesweeperGameState::NotStarted;
	RevealedTileCount = 0;
	FlaggedTileCount = 0;
	GameBoardTiles.Empty();
}

// ==== Tile Operations

bool FMinesweeperCore::RevealTile(const int32 X, const int32 Y)
{
	if (!IsGameActive() || !IsValidCoordinate(X, Y))
		return false;

	FMinesweeperTile& Tile = GameBoardTiles[GetTileIndex(X, Y)];
	if (Tile.bIsRevealed || Tile.bIsFlagged)
		return false;

	Tile.bIsRevealed = true;
	RevealedTileCount++;

	MS_DISPLAY("Revealed tile at [%d, %d]", X, Y);

	if (Tile.bIsBomb)
	{
		EndGame(false);
		return true;
	}

	// Auto-reveal adjacent tiles if this tile has no adjacent bombs
	if (Tile.AdjacentBombs == 0)
	{
		RevealAdjacentTiles(X, Y);
	}

	CheckWinCondition();
	return true;
}

void FMinesweeperCore::ToggleFlag(const int32 X, const int32 Y)
{
	if (!IsGameActive() || !IsValidCoordinate(X, Y))
		return;

	FMinesweeperTile& Tile = GameBoardTiles[GetTileIndex(X, Y)];

	// Can't flag already revealed tiles
	if (Tile.bIsRevealed)
		return;

	if (Tile.bIsFlagged)
	{
		// Remove flag
		Tile.bIsFlagged = false;
		FlaggedTileCount--;
		MS_DISPLAY("Removed flag from tile [%d, %d]", X, Y);
	}
	else
	{
		// Add flag (only if we haven't exceeded bomb count)
		if (FlaggedTileCount < GameSettings.BombCount)
		{
			Tile.bIsFlagged = true;
			FlaggedTileCount++;
			MS_DISPLAY("Added flag to tile [%d, %d]", X, Y);
		}
	}

	CheckWinCondition();
}

// ==== Tile Queries

const FMinesweeperTile* FMinesweeperCore::GetTile(const int32 X, const int32 Y) const
{
	if (!IsValidCoordinate(X, Y))
	{
		return nullptr;
	}

	return &GameBoardTiles[GetTileIndex(X, Y)];
}

bool FMinesweeperCore::IsValidCoordinate(const int32 X, const int32 Y) const
{
	return X >= 0 && X < GameSettings.GridWidth && Y >= 0 && Y < GameSettings.GridHeight;
}

int32 FMinesweeperCore::GetTileIndex(const int32 X, const int32 Y) const
{
	return Y * GameSettings.GridWidth + X;
}

// ==== Internal Logic

void FMinesweeperCore::EndGame(const bool bWon)
{
	CurrentGameState = bWon ? EMinesweeperGameState::Won : EMinesweeperGameState::Lost;

	// Reveal all bombs and flags for end game display
	for (FMinesweeperTile& Tile : GameBoardTiles)
	{
		if (Tile.bIsBomb || Tile.bIsFlagged)
		{
			if (bWon)
			{
				Tile.bIsFlagged = true;
			}
			Tile.bIsRevealed = true;
		}
	}

	MS_DISPLAY("Game ended - %s", bWon ? TEXT("Won") : TEXT("Lost"));
}

void FMinesweeperCore::CheckWinCondition()
{
	if (!IsGameActive())
	{
		return;
	}

	const int32 TotalNonBombTiles = GameSettings.GetTotalTiles() - GameSettings.BombCount;
	const bool bAllNonBombTilesRevealed = RevealedTileCount >= TotalNonBombTiles;

	// Check if all bombs are correctly flagged
	int32 CorrectlyFlaggedBombs = 0;
	bool bHasIncorrectFlag = false;

	for (const FMinesweeperTile& Tile : GameBoardTiles)
	{
		if (Tile.bIsBomb && Tile.bIsFlagged)
		{
			CorrectlyFlaggedBombs++;
		}
		else if (!Tile.bIsBomb && Tile.bIsFlagged)
		{
			bHasIncorrectFlag = true;
			break;
		}
	}

	const bool bPerfectlyFlagged = CorrectlyFlaggedBombs == GameSettings.BombCount && !bHasIncorrectFlag;

	if (bAllNonBombTilesRevealed || bPerfectlyFlagged)
	{
		EndGame(true);
	}
}

void FMinesweeperCore::GenerateBoardTiles()
{
	const int32 TotalTiles = GameSettings.GetTotalTiles();
	GameBoardTiles.Empty(TotalTiles);
	GameBoardTiles.SetNum(TotalTiles);

	// Initialize all tiles
	for (FMinesweeperTile& Tile : GameBoardTiles)
	{
		Tile = FMinesweeperTile();
	}

	PlaceBombsRandomly();
	CalculateAdjacentBombs();
}

void FMinesweeperCore::PlaceBombsRandomly()
{
	TArray<int32> AvailableIndices;
	AvailableIndices.Reserve(GameBoardTiles.Num());

	for (int32 i = 0; i < GameBoardTiles.Num(); ++i)
	{
		AvailableIndices.Add(i);
	}

	// Ensure bomb count doesn't exceed available tiles
	const int32 BombsToPlace = FMath::Min(GameSettings.BombCount, GameBoardTiles.Num() - 1);
	for (int32 i = 0; i < BombsToPlace; ++i)
	{
		if (AvailableIndices.Num() == 0)
			break;

		const int32 RandomIndex = FMath::RandRange(0, AvailableIndices.Num() - 1);
		const int32 BoardIndex = AvailableIndices[RandomIndex];

		GameBoardTiles[BoardIndex].bIsBomb = true;
		AvailableIndices.RemoveAt(RandomIndex);
	}
}

void FMinesweeperCore::CalculateAdjacentBombs()
{
	for (int32 Y = 0; Y < GameSettings.GridHeight; ++Y)
	{
		for (int32 X = 0; X < GameSettings.GridWidth; ++X)
		{
			FMinesweeperTile& CurrentTile = GameBoardTiles[GetTileIndex(X, Y)];
			if (CurrentTile.bIsBomb)
			{
				continue;
			}

			int32 AdjacentBombs = 0;

			// Check all 8 adjacent tiles
			for (int32 DY = -1; DY <= 1; ++DY)
			{
				for (int32 DX = -1; DX <= 1; ++DX)
				{
					if (DX == 0 && DY == 0)
						continue;

					const int32 CheckX = X + DX;
					const int32 CheckY = Y + DY;

					if (IsValidCoordinate(CheckX, CheckY))
					{
						if (GameBoardTiles[GetTileIndex(CheckX, CheckY)].bIsBomb)
						{
							AdjacentBombs++;
						}
					}
				}
			}

			CurrentTile.AdjacentBombs = AdjacentBombs;
		}
	}
}

void FMinesweeperCore::RevealAdjacentTiles(const int32 X, const int32 Y)
{
	// Check all 8 adjacent tiles
	for (int32 DY = -1; DY <= 1; ++DY)
	{
		for (int32 DX = -1; DX <= 1; ++DX)
		{
			if (DX == 0 && DY == 0)
				continue;

			const int32 CheckX = X + DX;
			const int32 CheckY = Y + DY;

			if (IsValidCoordinate(CheckX, CheckY))
			{
				RevealTile(CheckX, CheckY);
			}
		}
	}
}
