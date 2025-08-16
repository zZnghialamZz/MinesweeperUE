// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class MINESWEEPER_API SMinesweeperTileButton : public SButton
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperTileButton) {}

		/** Content to put in the button */
		SLATE_DEFAULT_SLOT(FArguments, Content)

		/** Called when the button is left-clicked */
		SLATE_EVENT(FOnClicked, OnClicked)

		/** Called when the button is right-clicked */
		SLATE_EVENT(FOnClicked, OnRightClicked)

		/** Color for the button **/
		SLATE_ATTRIBUTE(FSlateColor, ButtonColorAndOpacity)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

public:
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

private:
	FReply ExecuteOnRightClick() const;

private:
	FOnClicked OnRightClicked;
};
