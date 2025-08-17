// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperCommands.h"

#define LOCTEXT_NAMESPACE "FMinesweeperModule"

void FMinesweeperCommands::RegisterCommands()
{
	UI_COMMAND(OpenMineSweeperWindow, "Minesweeper", "Open Minesweeper Game Window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
