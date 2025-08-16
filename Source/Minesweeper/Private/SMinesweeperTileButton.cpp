// Copyright Epic Games, Inc. All Rights Reserved.


#include "SMinesweeperTileButton.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMinesweeperTileButton::Construct(const FArguments& InArgs)
{
	OnRightClicked = InArgs._OnRightClicked;
	
	SButton::FArguments ButtonArgs;
	ButtonArgs.OnClicked(InArgs._OnClicked);
	ButtonArgs.ClickMethod(EButtonClickMethod::MouseDown)
	[
		InArgs._Content.Widget
	];
	ButtonArgs.ButtonColorAndOpacity(InArgs._ButtonColorAndOpacity);
	ButtonArgs.HAlign(HAlign_Center);
	ButtonArgs.VAlign(VAlign_Center);

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

FReply SMinesweeperTileButton::ExecuteOnRightClick() const
{
	PlayClickedSound();
	if (OnRightClicked.IsBound())
	{
		FReply Reply = OnRightClicked.Execute();
		return Reply;
	}
	return FReply::Unhandled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
