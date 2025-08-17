// Copyright Epic Games, Inc. All Rights Reserved.


#include "Widgets/SMinesweeperTileButton.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMinesweeperTileButton::Construct(const FArguments& InArgs)
{
	// Store the right-click delegate
	OnRightClicked = InArgs._OnRightClicked;

	// Configure button arguments with provided settings
	SButton::FArguments ButtonArgs;
	ButtonArgs.OnClicked(InArgs._OnClicked);
	ButtonArgs.ClickMethod(InArgs._ClickMethod);
	ButtonArgs.ButtonColorAndOpacity(InArgs._ButtonColorAndOpacity);
	ButtonArgs.HAlign(InArgs._HAlign);
	ButtonArgs.VAlign(InArgs._VAlign);
	ButtonArgs.Content()
	[
		InArgs._Content.Widget
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

FReply SMinesweeperTileButton::ExecuteOnRightClick() const
{
	PlayClickedSound();

	if (OnRightClicked.IsBound())
	{
		return OnRightClicked.Execute();
	}
	return FReply::Unhandled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
