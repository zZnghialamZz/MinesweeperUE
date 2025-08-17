// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MinesweeperTypes.h"

/**
 * Static utility class for Minesweeper UI styling and text formatting
 */
class MINESWEEPER_API FMinesweeperWidgetHelper
{
public:
	// Text Formatting
	static FText GetTileDisplayText(const FMinesweeperTile& Tile);
	static FText GetGameStatusText(const EMinesweeperGameState GameState);
	static FText GetFlagCountText(const int32 FlaggedCount, const int32 TotalBombs);

	// Color Schemes
	static FSlateColor GetTileTextColor(const FMinesweeperTile& Tile);
	static FSlateColor GetTileBackgroundColor(const FMinesweeperTile& Tile);

private:
	static FSlateColor GetTileTextColorBasedOnAdjacentBomb(const int32 AdjacentBombCount);
};
