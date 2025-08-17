// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FMinesweeperModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** UI Command Handler **/
	void OnMineSweeperButtonClicked();
	
	void RegisterToolBarMenus();
	void UnregisterToolBarMenus();
	void RegisterTabSpawner();
	void UnregisterTabSpawner();

	/** Tab Management **/
	TSharedRef<class SDockTab> OnSpawnMinesweeperTab(const FSpawnTabArgs& Args);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
