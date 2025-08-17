// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * Custom button widget for Minesweeper tiles
 * Extends SButton to support right-click functionality for flagging mechanic
 */
class MINESWEEPER_API SMinesweeperTileButton : public SButton
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperTileButton)
			: _ClickMethod(EButtonClickMethod::MouseDown)
			, _HAlign(HAlign_Center)
			, _VAlign(VAlign_Center) {}

		/** Called when the button is left-clicked */
		SLATE_EVENT(FOnClicked, OnClicked)

		/** Called when the button is right-clicked */
		SLATE_EVENT(FOnClicked, OnRightClicked)

		/** Content to put in the button */
		SLATE_DEFAULT_SLOT(FArguments, Content)

		/** Color for the button **/
		SLATE_ATTRIBUTE(FSlateColor, ButtonColorAndOpacity)

		/** Sets the rules to use for determining whether the button was clicked.  This is an advanced setting and generally should be left as the default. */
		SLATE_ARGUMENT(EButtonClickMethod::Type, ClickMethod)

		/** Horizontal alignment */
		SLATE_ARGUMENT(EHorizontalAlignment, HAlign)

		/** Vertical alignment */
		SLATE_ARGUMENT(EVerticalAlignment, VAlign)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

public:
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

private:
	/**
	 * Executes the right-click callback if bound, we will use this for toggle flag mechanic in Minesweeper game.
	 * @return Reply indicating whether the event was handled
	 */
	FReply ExecuteOnRightClick() const;

private:
	FOnClicked OnRightClicked;
};
