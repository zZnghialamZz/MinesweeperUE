// Copyright Epic Games, Inc. All Rights Reserved.


#include "Widgets/SMinesweeperTileButton.h"

#include "MinesweeperLog.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMinesweeperTileButton::Construct(const FArguments& InArgs)
{
	// Store tile coordinates and game reference
	TileX = InArgs._TileX;
	TileY = InArgs._TileY;
	GameCoreWeak = InArgs._GameCore;

	// Store event delegates
	OnTileRevealed = InArgs._OnTileRevealed;
	OnTileFlagged = InArgs._OnTileFlagged;

	// Configure button arguments with provided settings
	SButton::FArguments ButtonArgs;
	ButtonArgs.ClickMethod(InArgs._ClickMethod);
	ButtonArgs.OnClicked_Raw(this, &SMinesweeperTileButton::ExecuteOnLeftClick);
	ButtonArgs.ButtonColorAndOpacity(this, &SMinesweeperTileButton::GetTileBackgroundColor);
	ButtonArgs.HAlign(InArgs._HAlign);
	ButtonArgs.VAlign(InArgs._VAlign);
	ButtonArgs.Content()
	[
		SNew(SBox)
		.WidthOverride(20)
		.HeightOverride(20)
		[
			SNew(STextBlock)
			.Text(this, &SMinesweeperTileButton::GetTileDisplayText)
			.ColorAndOpacity(this, &SMinesweeperTileButton::GetTileTextColor)
			.Font(FAppStyle::Get().GetFontStyle("BoldFont"))
			.Justification(ETextJustify::Center)
		]
	];

	// Construct the base button with configured arguments
	SButton::Construct(ButtonArgs);
}

FReply SMinesweeperTileButton::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	// Support handling Mouse Right Click, this is used for Minesweeper right click game play feature.
	if (IsEnabled() && MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		Press();
		return ExecuteOnRightClick();
	}

	// If not mouse right click, check the usual behavior
	return SButton::OnMouseButtonDown(MyGeometry, MouseEvent);
}

FReply SMinesweeperTileButton::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	// Handle right mouse button release
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		// Clear the pressed button state for right clicks
		Release();
		// Don't mark as handled here - let the below event handle the logic
		return FReply::Unhandled();
	}
	return SButton::OnMouseButtonUp(MyGeometry, MouseEvent);
}

FReply SMinesweeperTileButton::ExecuteOnLeftClick() const
{
	MS_DISPLAY("Left click on tile [%d, %d]", TileX, TileY);

	PlayClickedSound();

	if (OnTileRevealed.IsBound())
	{
		OnTileRevealed.Execute(TileX, TileY);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply SMinesweeperTileButton::ExecuteOnRightClick() const
{
	MS_DISPLAY("Right click on tile [%d, %d]", TileX, TileY);

	PlayClickedSound();

	if (OnTileFlagged.IsBound())
	{
		OnTileFlagged.Execute(TileX, TileY);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FText SMinesweeperTileButton::GetTileDisplayText() const
{
	const FMinesweeperTile* TileData = GetCurrentTileData();
	if (TileData == nullptr)
		return FText::GetEmpty();

	// Show flag if tile is flagged
	if (TileData->bIsFlagged)
		return FText::FromString(TEXT("🚩"));

	// Show nothing if not revealed
	if (!TileData->bIsRevealed)
		return FText::GetEmpty();

	// Show bomb if revealed and is bomb
	if (TileData->bIsBomb)
		return FText::FromString(TEXT("💣"));

	// Show number if there are adjacent bombs
	if (TileData->AdjacentBombs > 0)
		return FText::AsNumber(TileData->AdjacentBombs);

	// Empty for revealed safe tiles with no adjacent bombs
	return FText::GetEmpty();
}

FSlateColor SMinesweeperTileButton::GetTileTextColor() const
{
	const FMinesweeperTile* TileData = GetCurrentTileData();
	if (TileData == nullptr)
		return FSlateColor::UseForeground();

	// Red color for revealed bombs
	if (TileData->bIsBomb && TileData->bIsRevealed)
		return FSlateColor(FLinearColor::Red);

	// Color code numbers based on adjacent bomb count
	if (TileData->bIsRevealed && TileData->AdjacentBombs > 0)
		return GetTileTextColorBasedOnAdjacentBomb(TileData->AdjacentBombs);

	return FSlateColor::UseForeground();
}

FSlateColor SMinesweeperTileButton::GetTileBackgroundColor() const
{
	const FMinesweeperTile* TileData = GetCurrentTileData();
	if (TileData == nullptr)
		return FSlateColor::UseForeground();

	// Red background for revealed bombs (not flagged)
	if (TileData->bIsBomb && TileData->bIsRevealed && !TileData->bIsFlagged)
		return FSlateColor(FLinearColor::Red);

	// Red background for incorrectly flagged tiles (flagged but not a bomb, and revealed)
	if (TileData->bIsFlagged && TileData->bIsRevealed && !TileData->bIsBomb)
		return FSlateColor(FLinearColor::Red);

	// Dark gray for revealed tiles
	if (TileData->bIsRevealed)
		return FSlateColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.0f));

	// Default button color for unrevealed tiles
	return FSlateColor(FLinearColor(0.7f, 0.7f, 0.7f, 1.0f));
}

FSlateColor SMinesweeperTileButton::GetTileTextColorBasedOnAdjacentBomb(const int32 AdjacentBombCount) const
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

const FMinesweeperTile* SMinesweeperTileButton::GetCurrentTileData() const
{
	const TSharedPtr<FMinesweeperCore> GameCore = GameCoreWeak.Pin();
	if (!GameCore.IsValid())
	{
		return nullptr;
	}

	return GameCore->GetTile(TileX, TileY);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
