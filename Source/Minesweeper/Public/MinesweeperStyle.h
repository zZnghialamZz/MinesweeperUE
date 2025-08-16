// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Styling/SlateStyle.h"

class FMinesweeperStyle
{
public:
	static const ISlateStyle& Get();
	
	static void Initialize();
	static void Shutdown();
	
	static FName GetStyleSetName();
	static void ReloadTextures();

private:
	static TSharedRef<class FSlateStyleSet> Create();

private:
	static TSharedPtr<class FSlateStyleSet> StyleInstance;
};
