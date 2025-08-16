// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SSpinBox.h"

struct FMinesweeperTile
{
	bool bIsBomb = false;
	bool bIsRevealed = false;
	bool bIsFlagged = false;
	int32 AdjacentBombs = 0;

	FMinesweeperTile() = default;
};

class MINESWEEPER_API SMinesweeperWidget final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	// UI generation
	TSharedRef<SWidget> CreateControlPanel();
	TSharedRef<SWidget> CreateGameBoard();
	TSharedRef<SWidget> CreateTileButton(const int32 X, const int32 Y);
	FText GetTileButtonText(const int32 X, const int32 Y) const;
	FSlateColor GetTileButtonTextColor(const int32 X, const int32 Y) const;
	FSlateColor GetTileButtonBackgroundColor(const int32 X, const int32 Y) const;
	bool IsTileButtonInteractable(const int32 X, const int32 Y) const;
	void ShowEndGameDialog() const;
	void UpdateFlagCountDisplay();

	// UI callbacks
	FReply OnGenerateNewGameClicked();
	FReply OnTileClicked(const int32 X, const int32 Y);
	FReply OnTileRightClicked(const int32 X, const int32 Y);
	void OnWidthUIValueChanged(const int32 NewValue);
	void OnHeightUIValueChanged(const int32 NewValue);
	void OnBombCountUIValueChanged(const int32 NewValue);

	// Game logic
	void InitializeGame();
	void ResetGame();
	void EndGame(const bool bWon);
	void CheckWinCondition();
	void GenerateBoardTiles();
	void PlaceBombsRandomly();
	void CalculateAdjacentBombs();
	void RevealAllBombsAndFlag(const bool bRevealBombAsFlag);
	void RevealTile(const int32 X, const int32 Y);
	void RevealAdjacentTiles(const int32 X, const int32 Y);
	void ToggleFlag(const int32 X, const int32 Y);
	bool IsValidCoordinate(const int32 X, const int32 Y) const;
	int32 GetTileIndex(const int32 X, const int32 Y) const;

private:
	static constexpr int GameGridMin = 5;
	static constexpr int GameGridMax = 50;
	static constexpr int BombCountMin = 1;
	static constexpr int BombCountMax = 500;
	
	// Game states
	TArray<FMinesweeperTile> GameBoardTiles;
	int32 GridWidth = 10;
	int32 GridHeight = 10;
	int32 BombCount = 10;
	int32 RevealedTiles = 0;
	int32 FlaggedTiles = 0;
	bool bGameActive = false;
	bool bGameWon = false;

	// UI Components
	TSharedPtr<class SUniformGridPanel> GameBoardGridUI;
	TSharedPtr<SSpinBox<int32>> WidthSpinBoxUI;
	TSharedPtr<SSpinBox<int32>> HeightSpinBoxUI;
	TSharedPtr<SSpinBox<int32>> BombCountSpinBoxUI;
	TSharedPtr<STextBlock> GameStatusTextUI;
	TSharedPtr<STextBlock> FlagCountTextUI;
	int32 GridWidthUIValue = 10;
	int32 GridHeightUIValue = 10;
	int32 BombCountUIValue = 10;

	// Tile buttons cache for updates
	TMap<int32, TSharedPtr<SButton>> TileButtons;
};
