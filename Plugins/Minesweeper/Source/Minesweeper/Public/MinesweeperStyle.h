// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Styling/SlateStyle.h"

/**
 * Style management for the Minesweeper plugin
 * 
 * Handles loading and managing visual resources like icons and brushes
 * Follows Epic's standard pattern for plugin styling
 */
class FMinesweeperStyle
{
public:
	static const ISlateStyle& Get();
	
	static FName GetStyleSetName();
	
	static void Initialize();
	static void Shutdown();
	static void ReloadTextures();

private:
	static TSharedRef<class FSlateStyleSet> Create();

private:
	static TSharedPtr<class FSlateStyleSet> StyleInstance;
};
