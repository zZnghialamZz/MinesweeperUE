// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperStyle.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMinesweeperStyle::StyleInstance = nullptr;

const ISlateStyle& FMinesweeperStyle::Get()
{
	return *StyleInstance;
}

FName FMinesweeperStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MinesweeperStyle"));
	return StyleSetName;
}

void FMinesweeperStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMinesweeperStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

TSharedRef<FSlateStyleSet> FMinesweeperStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("MinesweeperStyle"));
	
	// Set content root to plugin's Resources directory
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("Minesweeper")->GetBaseDir() / TEXT("Resources"));

	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);

	// Register plugin icon
	Style->Set("Minesweeper.OpenMineSweeperWindow", new IMAGE_BRUSH_SVG(TEXT("SweepIcon"), Icon20x20));
	return Style;
}

void FMinesweeperStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

#undef RootToContentDir
