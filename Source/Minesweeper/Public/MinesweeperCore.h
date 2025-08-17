// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MinesweeperTypes.h"

/**
 * Core game logic for Minesweeper
 * Handles all game state management and rules
 */
class MINESWEEPER_API FMinesweeperCore
{
public:
	FMinesweeperCore();
	~FMinesweeperCore();

	// Game Management
	void InitializeGame(const FMinesweeperGameSettings& InSettings);
	void ResetGame();

	// Tile Operations
	bool RevealTile(const int32 X, const int32 Y);
	void ToggleFlag(const int32 X, const int32 Y);

	// Game State Queries
	EMinesweeperGameState GetGameState() const { return CurrentGameState; }
	const FMinesweeperGameSettings& GetGameSettings() const { return GameSettings; }
	bool IsGameActive() const { return CurrentGameState == EMinesweeperGameState::Active; }
	bool IsGameWon() const { return CurrentGameState == EMinesweeperGameState::Won; }

	// Tile Queries
	const FMinesweeperTile* GetTile(const int32 X, const int32 Y) const;
	bool IsValidCoordinate(const int32 X, const int32 Y) const;
	int32 GetRevealedTileCount() const { return RevealedTileCount; }
	int32 GetFlaggedTileCount() const { return FlaggedTileCount; }
	int32 GetRemainingFlags() const { return GameSettings.BombCount - FlaggedTileCount; }
	int32 GetTileIndex(const int32 X, const int32 Y) const;

private:
	// Internal Logic
	void EndGame(const bool bWon);
	void CheckWinCondition();
	void GenerateBoardTiles();
	void PlaceBombsRandomly();
	void CalculateAdjacentBombs();
	void RevealAdjacentTiles(const int32 X, const int32 Y);

private:
	/** Current game state */
	EMinesweeperGameState CurrentGameState;

	/** Game configuration settings */
	FMinesweeperGameSettings GameSettings;

	/** Array of all tiles on the board */
	TArray<FMinesweeperTile> GameBoardTiles;

	/** Statistics */
	int32 RevealedTileCount;
	int32 FlaggedTileCount;
};
