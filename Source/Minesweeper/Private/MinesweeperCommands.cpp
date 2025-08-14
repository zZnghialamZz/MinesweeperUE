// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperCommands.h"

#define LOCTEXT_NAMESPACE "FMinesweeperModule"

void FMinesweeperCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Minesweeper", "Execute Minesweeper action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
