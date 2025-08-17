// Copyright Epic Games, Inc. All Rights Reserved.

#include "Minesweeper.h"
#include "MinesweeperStyle.h"
#include "MinesweeperCommands.h"
#include "MinesweeperLog.h"
#include "Widgets/SMinesweeperWidget.h"

#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName MinesweeperTabName("Minesweeper");

#define LOCTEXT_NAMESPACE "FMinesweeperModule"

void FMinesweeperModule::StartupModule()
{
	MS_DISPLAY("Starting Minesweeper Module!");

	// Initialize style system
	FMinesweeperStyle::Initialize();
	FMinesweeperStyle::ReloadTextures();

	// Register UI Commands
	FMinesweeperCommands::Register();

	// Create and setup command list
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FMinesweeperCommands::Get().OpenMineSweeperWindow,
		FExecuteAction::CreateRaw(this, &FMinesweeperModule::OnMineSweeperButtonClicked),
		FCanExecuteAction());

	// Register UIs
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMinesweeperModule::RegisterToolBarMenus));
	RegisterTabSpawner();
}

void FMinesweeperModule::ShutdownModule()
{
	UnregisterToolBarMenus();
	UnregisterTabSpawner();
	FMinesweeperStyle::Shutdown();
	FMinesweeperCommands::Unregister();

	MS_DISPLAY("Minesweeper module shutdown complete");
}

void FMinesweeperModule::OnMineSweeperButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperTabName);
}

void FMinesweeperModule::RegisterToolBarMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);
	{
		// Add to main menu
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMinesweeperCommands::Get().OpenMineSweeperWindow, PluginCommands);
		}
	}

	{
		// Add to Toolbar menu
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMinesweeperCommands::Get().OpenMineSweeperWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

void FMinesweeperModule::UnregisterToolBarMenus()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
}

void FMinesweeperModule::RegisterTabSpawner()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MinesweeperTabName, FOnSpawnTab::CreateRaw(this, &FMinesweeperModule::OnSpawnMinesweeperTab))
	                        .SetDisplayName(NSLOCTEXT("MinesweeperEditor", "MinesweeperTabTitle", "Minesweeper"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMinesweeperModule::UnregisterTabSpawner()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MinesweeperTabName);
}

TSharedRef<SDockTab> FMinesweeperModule::OnSpawnMinesweeperTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SMinesweeperWidget)
		];
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMinesweeperModule, Minesweeper)
