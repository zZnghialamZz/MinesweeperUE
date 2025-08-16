// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "MinesweeperStyle.h"

class FMinesweeperCommands final : public TCommands<FMinesweeperCommands>
{
public:
	FMinesweeperCommands()
		: TCommands(TEXT("Minesweeper"), NSLOCTEXT("Contexts", "Minesweeper", "Minesweeper Plugin"), NAME_None, FMinesweeperStyle::GetStyleSetName()) {}

	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> OpenMineSweeperWindow;
};
