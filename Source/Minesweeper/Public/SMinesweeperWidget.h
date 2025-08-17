// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MinesweeperCore.h"
#include "MinesweeperTypes.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/SCompoundWidget.h"

/**
 * Main Minesweeper widget that handles the UI and game display
 */
class MINESWEEPER_API SMinesweeperWidget final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	// UI generation
	TSharedRef<SWidget> CreateControlPanel();
	TSharedRef<SWidget> CreateGameSettingsPanel();
	TSharedRef<SWidget> CreateGameInfoPanel();
	TSharedRef<SWidget> CreateGameBoard();
	TSharedRef<SWidget> CreateTileButton(int32 X, int32 Y);

	// UI Update Methods
	void RefreshGameBoardUI();
	void UpdateGameInfoDisplay();
	void UpdateFlagCountDisplay() const;
	void UpdateGameStatusDisplay() const;

	// UI callbacks
	FReply OnGenerateNewGameClicked();
	FReply OnTileClicked(const int32 X, const int32 Y);
	FReply OnTileRightClicked(const int32 X, const int32 Y);
	void OnWidthUIValueChanged(const int32 NewValue);
	void OnHeightUIValueChanged(const int32 NewValue);
	void OnBombCountUIValueChanged(const int32 NewValue);

	// UI Attribute Getters (for dynamic UI updates)
	FText GetTileButtonText(const int32 X, const int32 Y) const;
	FSlateColor GetTileButtonTextColor(const int32 X, const int32 Y) const;
	FSlateColor GetTileButtonBackgroundColor(const int32 X, const int32 Y) const;
	bool IsTileButtonInteractable(const int32 X, const int32 Y) const;

	// Game flow
	void InitializeNewGame();
	void HandleGameStateChange(EMinesweeperGameState NewState);
	void ShowEndGameDialog() const;

private:
	// Core Game Logic
	TUniquePtr<FMinesweeperCore> GameCore;

	// UI State
	FMinesweeperGameSettings PendingGameSettings;

	// UI Components
	TSharedPtr<class SUniformGridPanel> GameBoardGridPanelUI;
	TSharedPtr<SSpinBox<int32>> WidthSpinBoxUI;
	TSharedPtr<SSpinBox<int32>> HeightSpinBoxUI;
	TSharedPtr<SSpinBox<int32>> BombCountSpinBoxUI;
	TSharedPtr<STextBlock> GameStatusTextUI;
	TSharedPtr<STextBlock> FlagCountTextUI;

	// Tile buttons cache for updates
	TMap<int32, TSharedPtr<SButton>> TileButtonsCache;
};
