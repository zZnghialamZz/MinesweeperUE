// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MinesweeperCore.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_TwoParams(FOnTileInteraction, const int32, const int32);

/**
 * Custom button widget for Minesweeper tiles
 * Extends SButton to support right-click functionality for flagging mechanic
 * Handles its own state, appearance.
 */
class MINESWEEPER_API SMinesweeperTileButton : public SButton
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperTileButton)
			: _TileX(0)
			, _TileY(0)
			, _HAlign(HAlign_Center)
			, _VAlign(VAlign_Center)
			, _ClickMethod(EButtonClickMethod::MouseDown) {}

		/** Tile coordinates **/
		SLATE_ARGUMENT(int32, TileX)
		SLATE_ARGUMENT(int32, TileY)

		/** Reference to game core for tile data access **/
		SLATE_ARGUMENT(TWeakPtr<FMinesweeperCore>, GameCore)

		/** Called when tile is left-clicked (reveal) **/
		SLATE_EVENT(FOnTileInteraction, OnTileRevealed)

		/** Called when tile is right-clicked (flag toggle) **/
		SLATE_EVENT(FOnTileInteraction, OnTileFlagged)

		/** Content to put in the button **/
		SLATE_DEFAULT_SLOT(FArguments, Content)

		/** Horizontal alignment **/
		SLATE_ARGUMENT(EHorizontalAlignment, HAlign)

		/** Vertical alignment **/
		SLATE_ARGUMENT(EVerticalAlignment, VAlign)

		/** Sets the rules to use for determining whether the button was clicked.  This is an advanced setting and generally should be left as the default. */
		SLATE_ARGUMENT(EButtonClickMethod::Type, ClickMethod)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

public:
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

private:
	/** Internal event handlers **/
	FReply ExecuteOnLeftClick() const;
	FReply ExecuteOnRightClick() const;

	/** UI update for this tile **/
	FText GetTileDisplayText() const;
	FSlateColor GetTileTextColor() const;
	FSlateColor GetTileBackgroundColor() const;
	FSlateColor GetTileTextColorBasedOnAdjacentBomb(const int32 AdjacentBombCount) const;

	/** Helper to get current tile data safely **/
	const FMinesweeperTile* GetCurrentTileData() const;

private:
	/** Tile coordinates */
	int32 TileX = 0;
	int32 TileY = 0;

	/** Event delegates */
	FOnTileInteraction OnTileRevealed;
	FOnTileInteraction OnTileFlagged;

	/** Weak reference to game core */
	TWeakPtr<FMinesweeperCore> GameCoreWeak;
};
