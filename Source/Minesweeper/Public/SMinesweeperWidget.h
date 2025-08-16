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
	void OnWidthChanged(const uint32 NewValue);
	void OnHeightChanged(const uint32 NewValue);
	void OnBombCountChanged(const uint32 NewValue);

private:
	static constexpr int GameGridMin = 5;
	static constexpr int GameGridMax = 50;
	static constexpr int BombCountMax = 30;
	
	// Game states
	TArray<FMinesweeperTile> GameBoardTiles;
	uint32 GridWidth = 10;
	uint32 GridHeight = 10;
	uint32 BombCount = 3;
	uint32 RevealedTiles = 0;
	bool bGameActive = false;
	bool bGameWon = false;

	// UI Components
	TSharedPtr<class SUniformGridPanel> GameBoardGrid;
	TSharedPtr<SSpinBox<uint32>> WidthSpinBox;
	TSharedPtr<SSpinBox<uint32>> HeightSpinBox;
	TSharedPtr<SSpinBox<uint32>> BombCountSpinBox;
};
