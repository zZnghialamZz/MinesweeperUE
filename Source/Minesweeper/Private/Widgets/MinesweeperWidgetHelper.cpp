// Copyright Epic Games, Inc. All Rights Reserved.


#include "Widgets/MinesweeperWidgetHelper.h"

// ==== Text Formatting

FText FMinesweeperWidgetHelper::GetTileDisplayText(const FMinesweeperTile& Tile)
{
	// Show flag if tile is flagged
	if (Tile.bIsFlagged)
	{
		return FText::FromString(TEXT("🚩"));
	}

	// Show nothing if not revealed
	if (!Tile.bIsRevealed)
	{
		return FText::GetEmpty();
	}

	// Show bomb if revealed and is bomb
	if (Tile.bIsBomb)
	{
		return FText::FromString(TEXT("💣"));
	}

	// Show number if there are adjacent bombs
	if (Tile.AdjacentBombs > 0)
	{
		return FText::AsNumber(Tile.AdjacentBombs);
	}

	// Empty for revealed safe tiles with no adjacent bombs
	return FText::GetEmpty();
}

FText FMinesweeperWidgetHelper::GetGameStatusText(const EMinesweeperGameState GameState)
{
	switch (GameState)
	{
		case EMinesweeperGameState::NotStarted:
			return FText::FromString(TEXT("Ready to play! Click the button above to start!"));

		case EMinesweeperGameState::Active:
			return FText::FromString(TEXT("Game Started! Left-click to reveal, right-click to flag."));

		case EMinesweeperGameState::Won:
			return FText::FromString(TEXT("Congratulations! You won!"));

		case EMinesweeperGameState::Lost:
			return FText::FromString(TEXT("Game Over! You hit a mine."));

		default:
			return FText::GetEmpty();
	}
}

FText FMinesweeperWidgetHelper::GetFlagCountText(const int32 FlaggedCount, const int32 TotalBombs)
{
	const int32 RemainingFlags = TotalBombs - FlaggedCount;
	FString FlagText = FString::Printf(TEXT("Flags: %d/%d"), FlaggedCount, TotalBombs);

	if (RemainingFlags >= 0)
	{
		FlagText += FString::Printf(TEXT(" (Remaining: %d)"), RemainingFlags);
	}

	return FText::FromString(FlagText);
}

// ==== Color Schemes

FSlateColor FMinesweeperWidgetHelper::GetTileTextColor(const FMinesweeperTile& Tile)
{
	// Red color for revealed bombs
	if (Tile.bIsBomb && Tile.bIsRevealed)
		return FSlateColor(FLinearColor::Red);

	// Color code numbers based on adjacent bomb count
	if (Tile.bIsRevealed && Tile.AdjacentBombs > 0)
		return GetTileTextColorBasedOnAdjacentBomb(Tile.AdjacentBombs);

	return FSlateColor::UseForeground();
}

FSlateColor FMinesweeperWidgetHelper::GetTileBackgroundColor(const FMinesweeperTile& Tile)
{
	// Red background for revealed bombs (not flagged)
	if (Tile.bIsBomb && Tile.bIsRevealed && !Tile.bIsFlagged)
	{
		return FSlateColor(FLinearColor::Red);
	}

	// Red background for incorrectly flagged tiles (flagged but not a bomb, and revealed)
	if (Tile.bIsFlagged && Tile.bIsRevealed && !Tile.bIsBomb)
	{
		return FSlateColor(FLinearColor::Red);
	}

	// Dark gray for revealed tiles
	if (Tile.bIsRevealed)
	{
		return FSlateColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.0f));
	}

	// Default button color for unrevealed tiles
	return FSlateColor::UseForeground();
}

FSlateColor FMinesweeperWidgetHelper::GetTileTextColorBasedOnAdjacentBomb(const int AdjacentBombCount)
{
	// Color code numbers based on count
	switch (AdjacentBombCount)
	{
		case 1:
			return FSlateColor(FLinearColor(0.0f, 1.0f, 1.0f, 1.0f)); // Cyan
		case 2:
			return FSlateColor(FLinearColor::Green);
		case 3:
			return FSlateColor(FLinearColor::Yellow);
		case 4:
			return FSlateColor(FLinearColor(1.0f, 0.5f, 0.0f, 1.0f)); // Orange
		case 5:
			return FSlateColor(FLinearColor(1.0f, 0.7f, 0.7f, 1.0f)); // Pink
		case 6:
			return FSlateColor(FLinearColor(0.5f, 0.0f, 0.5f, 1.0f)); // Purple
		case 7:
			return FSlateColor(FLinearColor(0.5f, 0.0f, 0.0f, 1.0f)); // Maroon
		case 8:
			return FSlateColor(FLinearColor::Red);
		default:
			return FSlateColor::UseForeground();
	}
}
