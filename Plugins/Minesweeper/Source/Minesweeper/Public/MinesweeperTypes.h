// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// Constants
static constexpr int32 MineSweeperGameGridMin = 5;
static constexpr int32 MineSweeperGameGridMax = 50;
static constexpr int32 MineSweeperBombCountMin = 1;
static constexpr int32 MineSweeperBombCountMax = 500;

enum class EMinesweeperGameState : uint8
{
	NotStarted,
	Active,
	Won,
	Lost
};

struct MINESWEEPER_API FMinesweeperTile
{
	/** Whether this tile contains a bomb */
	bool bIsBomb = false;

	/** Whether this tile has been revealed */
	bool bIsRevealed = false;

	/** Whether this tile is flagged by the player */
	bool bIsFlagged = false;

	/** Number of adjacent bombs */
	int32 AdjacentBombs = 0;

	FMinesweeperTile() = default;
};

struct MINESWEEPER_API FMinesweeperGameSettings
{
	/** Width of the game grid */
	int32 GridWidth = 10;

	/** Height of the game grid */
	int32 GridHeight = 10;

	/** Number of bombs on the board */
	int32 BombCount = 10;

	FMinesweeperGameSettings() = default;

	FMinesweeperGameSettings(const int32 InGridWidth, const int32 InGridHeight, const int32 InBombCount)
		: GridWidth(InGridWidth)
		, GridHeight(InGridHeight)
		, BombCount(InBombCount) {}

	/** Validates and clamps the settings to valid ranges */
	void ValidateAndClamp()
	{
		GridWidth = FMath::Clamp(GridWidth, MineSweeperGameGridMin, MineSweeperGameGridMax);
		GridHeight = FMath::Clamp(GridHeight, MineSweeperGameGridMin, MineSweeperGameGridMax);
		BombCount = FMath::Clamp(BombCount, MineSweeperBombCountMin, FMath::Min(MineSweeperBombCountMax, GridWidth * GridHeight - 1));
	}

	/** Returns the total number of tiles */
	int32 GetTotalTiles() const
	{
		return GridWidth * GridHeight;
	}
};
