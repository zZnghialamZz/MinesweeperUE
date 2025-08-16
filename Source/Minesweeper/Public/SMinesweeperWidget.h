// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SSpinBox.h"

UENUM()
enum class EMinesweeperTileState : uint8
{
	Hidden,
	Revealed,
	Bomb
};

struct FMinesweeperTile
{
	bool bIsBomb = false;
	bool bIsRevealed = false;
	int32 AdjacentBombs = 0;
	EMinesweeperTileState State = EMinesweeperTileState::Hidden;

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

	// UI callbacks
	FReply OnGenerateNewGameClicked();
	void OnWidthChanged(const int32 NewValue);
	void OnHeightChanged(const int32 NewValue);
	void OnBombCountChanged(const int32 NewValue);

	// Game logic
	void GenerateBoardTiles();
	void PlaceBombsRandomly();
	void CalculateAdjacentBombs();
	bool IsValidCoordinate(const int32 X, const int32 Y) const;
	int32 GetTileIndex(const int32 X, const int32 Y) const;

private:
	static constexpr int GameGridMin = 5;
	static constexpr int GameGridMax = 50;
	static constexpr int BombCountMax = 30;
	
	// Game states
	TArray<FMinesweeperTile> GameBoardTiles;
	int32 GridWidth = 10;
	int32 GridHeight = 10;
	int32 BombCount = 3;
	int32 RevealedTiles = 0;
	bool bGameActive = false;
	bool bGameWon = false;

	// UI Components
	TSharedPtr<class SUniformGridPanel> GameBoardGridUI;
	TSharedPtr<SSpinBox<int32>> WidthSpinBoxUI;
	TSharedPtr<SSpinBox<int32>> HeightSpinBoxUI;
	TSharedPtr<SSpinBox<int32>> BombCountSpinBoxUI;
	TSharedPtr<STextBlock> GameStatusTextUI;
};
