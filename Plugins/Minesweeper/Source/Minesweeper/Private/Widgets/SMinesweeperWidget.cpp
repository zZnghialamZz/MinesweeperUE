// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widgets/SMinesweeperWidget.h"

#include "MinesweeperLog.h"
#include "Widgets/SMinesweeperTileButton.h"

#include "SlateOptMacros.h"
#include "Widgets/Layout/SUniformGridPanel.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMinesweeperWidget::Construct(const FArguments& InArgs)
{
	// Initialize game logic
	GameCore = MakeShared<FMinesweeperCore>();

	// Initialize UI settings
	PendingGameSettings = FMinesweeperGameSettings(10, 10, 10);

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(8.0f)
		[
			SNew(SVerticalBox)

			// Control Panel
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 0, 0, 8)
			[
				CreateControlPanel()
			]

			// Separator
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 8)
			[
				SNew(SSeparator)
				.Orientation(Orient_Horizontal)
			]

			// Game Board
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SNew(SBox)
				.MinDesiredWidth(1024)
				.MinDesiredHeight(1024)
				[
					CreateGameBoard()
				]
			]
		]
	];

	// Initialize the first game
	InitializeNewGame();
}

// ==== UI generation

TSharedRef<SWidget> SMinesweeperWidget::CreateControlPanel()
{
	return SNew(SVerticalBox)

			// Game Settings Panel
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				CreateGameSettingsPanel()
			]

			// Game Info Panel
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 8.0f, 0.0f, 0.0f)
			[
				CreateGameInfoPanel()
			];
}

TSharedRef<SWidget> SMinesweeperWidget::CreateGameSettingsPanel()
{
	return SNew(SVerticalBox)

			// Width & Height Row
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 5.0f)
			[
				SNew(SHorizontalBox)

				// Width Setting
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(10.0f, 0.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(0.0f, 5.0f)
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("Minesweeper", "WidthLabel", "Width: "))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.MinWidth(100.0f)
					[
						SAssignNew(WidthSpinBoxUI, SSpinBox<int32>)
						.MinValue(MineSweeperGameGridMin)
						.MaxValue(MineSweeperGameGridMax)
						.Value(PendingGameSettings.GridWidth)
						.OnValueChanged(this, &SMinesweeperWidget::OnWidthUIValueChanged)
					]
				]

				// Height Setting
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(10.0f, 0.0f)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(0.0f, 5.0f)
					[
						SNew(STextBlock)
						.Text(NSLOCTEXT("Minesweeper", "HeightLabel", "Height: "))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.MinWidth(100.0f)
					[
						SAssignNew(HeightSpinBoxUI, SSpinBox<int32>)
						.MinValue(MineSweeperGameGridMin)
						.MaxValue(MineSweeperGameGridMax)
						.Value(PendingGameSettings.GridHeight)
						.OnValueChanged(this, &SMinesweeperWidget::OnHeightUIValueChanged)
					]
				]
			]

			// Bomb Count Row
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 5.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(10.0f, 5.0f, 0.0f, 5.0f)
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("Minesweeper", "BombCountLabel", "Number of Mines: "))
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.MinWidth(100.0f)
				[
					SAssignNew(BombCountSpinBoxUI, SSpinBox<int32>)
					.MinValue(MineSweeperBombCountMin)
					.MaxValue(MineSweeperBombCountMax)
					.Value(PendingGameSettings.BombCount)
					.OnValueChanged(this, &SMinesweeperWidget::OnBombCountUIValueChanged)
				]
			]

			// Generate New Game Button
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 10.0f, 0.0f, 5.0f)
			[
				SNew(SButton)
				.Text(NSLOCTEXT("Minesweeper", "GenerateNewGame", "Generate New Game"))
				.OnClicked(this, &SMinesweeperWidget::OnGenerateNewGameClicked)
			];
}

TSharedRef<SWidget> SMinesweeperWidget::CreateGameInfoPanel()
{
	return SNew(SHorizontalBox)

			// Flag Count Display
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			[
				SAssignNew(FlagCountTextUI, STextBlock)
				.Text(GetFlagCountText(0, 10))
				.Justification(ETextJustify::Left)
			]

			// Game Status Display
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			[
				SAssignNew(GameStatusTextUI, STextBlock)
				.Text(GetGameStatusText(EMinesweeperGameState::NotStarted))
				.Justification(ETextJustify::Right)
			];
}

TSharedRef<SWidget> SMinesweeperWidget::CreateGameBoard()
{
	return SAssignNew(GameBoardGridPanelUI, SUniformGridPanel);
}

TSharedRef<SWidget> SMinesweeperWidget::CreateTileButton(const int32 X, const int32 Y)
{
	if (!GameCore.IsValid())
	{
		MS_FATAL("GameCore is not valid when creating tile button, please re-check !");
		return SNew(SButton);
	}

	TSharedRef<SButton> NewTileButton = SNew(SMinesweeperTileButton)
		.IsEnabled(this, &SMinesweeperWidget::IsTileButtonInteractable, X, Y)
		.TileX(X)
		.TileY(Y)
		.GameCore(GameCore)
		.OnTileRevealed(this, &SMinesweeperWidget::OnTileRevealed)
		.OnTileFlagged(this, &SMinesweeperWidget::OnTileFlagged);

	return NewTileButton;
}

// ==== UI Update Methods

void SMinesweeperWidget::RefreshGameBoardUI()
{
	if (!GameBoardGridPanelUI.IsValid() || !GameCore.IsValid())
	{
		MS_ERROR("Invalid GameBoardGridPanel or GameCore when trying to refresh game board");
		return;
	}

	// Clear existing UI
	GameBoardGridPanelUI->ClearChildren();

	const FMinesweeperGameSettings& Settings = GameCore->GetGameSettings();

	// Create new tile buttons
	for (int32 Y = 0; Y < Settings.GridHeight; ++Y)
	{
		for (int32 X = 0; X < Settings.GridWidth; ++X)
		{
			TSharedRef<SWidget> TileWidget = CreateTileButton(X, Y);
			GameBoardGridPanelUI->AddSlot(X, Y)
			[
				TileWidget
			];
		}
	}
}

void SMinesweeperWidget::UpdateGameInfoDisplay() const
{
	UpdateFlagCountDisplay();
	UpdateGameStatusDisplay();
}

void SMinesweeperWidget::UpdateFlagCountDisplay() const
{
	if (FlagCountTextUI.IsValid() && GameCore.IsValid())
	{
		const int32 FlaggedCount = GameCore->GetFlaggedTileCount();
		const int32 TotalBombs = GameCore->GetGameSettings().BombCount;
		FlagCountTextUI->SetText(GetFlagCountText(FlaggedCount, TotalBombs));
	}
}

void SMinesweeperWidget::UpdateGameStatusDisplay() const
{
	if (GameStatusTextUI.IsValid() && GameCore.IsValid())
	{
		const EMinesweeperGameState CurrentState = GameCore->GetGameState();
		GameStatusTextUI->SetText(GetGameStatusText(CurrentState));
	}
}

// ==== UI callbacks

FReply SMinesweeperWidget::OnGenerateNewGameClicked()
{
	// Validate and apply pending settings
	PendingGameSettings.ValidateAndClamp();
	InitializeNewGame();
	return FReply::Handled();
}

void SMinesweeperWidget::OnTileRevealed(const int32 X, const int32 Y)
{
	MS_DISPLAY("Try revealing tile [%d - %d]", X, Y);
	if (!GameCore.IsValid())
		return;

	const bool bTileRevealed = GameCore->RevealTile(X, Y);
	if (bTileRevealed)
	{
		UpdateGameInfoDisplay();
		HandleGameStateChange(GameCore->GetGameState());
	}
}

void SMinesweeperWidget::OnTileFlagged(const int32 X, const int32 Y)
{
	if (!GameCore.IsValid())
		return;

	MS_DISPLAY("Toggling flag on tile [%d, %d]", X, Y);

	GameCore->ToggleFlag(X, Y);
	UpdateGameInfoDisplay();
	HandleGameStateChange(GameCore->GetGameState());
}

void SMinesweeperWidget::OnWidthUIValueChanged(const int32 NewValue)
{
	PendingGameSettings.GridWidth = NewValue;
	PendingGameSettings.ValidateAndClamp();
}

void SMinesweeperWidget::OnHeightUIValueChanged(const int32 NewValue)
{
	PendingGameSettings.GridHeight = NewValue;
	PendingGameSettings.ValidateAndClamp();
}

void SMinesweeperWidget::OnBombCountUIValueChanged(const int32 NewValue)
{
	PendingGameSettings.BombCount = NewValue;
	PendingGameSettings.ValidateAndClamp();
}

// ==== UI Attribute Getters (for dynamic UI updates)

FText SMinesweeperWidget::GetGameStatusText(const EMinesweeperGameState GameState) const
{
	switch (GameState)
	{
		case EMinesweeperGameState::NotStarted:
			return FText::FromString(TEXT("Ready to play! Click the button above to start!"));

		case EMinesweeperGameState::Active:
			return FText::FromString(TEXT("Game Started! Left-click to reveal, right-click to flag."));

		case EMinesweeperGameState::Won:
			return FText::FromString(TEXT("Congratulations! You won!"));

		case EMinesweeperGameState::Lost:
			return FText::FromString(TEXT("Game Over! You hit a mine."));

		default:
			return FText::GetEmpty();
	}
}

FText SMinesweeperWidget::GetFlagCountText(const int32 FlaggedCount, const int32 TotalBombs) const
{
	const int32 RemainingFlags = TotalBombs - FlaggedCount;
	FString FlagText = FString::Printf(TEXT("Flags: %d/%d"), FlaggedCount, TotalBombs);

	if (RemainingFlags >= 0)
	{
		FlagText += FString::Printf(TEXT(" (Remaining: %d)"), RemainingFlags);
	}

	return FText::FromString(FlagText);
}

bool SMinesweeperWidget::IsTileButtonInteractable(const int32 X, const int32 Y) const
{
	if (!GameCore.IsValid() || !GameCore->IsGameActive())
		return false;

	const FMinesweeperTile* Tile = GameCore->GetTile(X, Y);
	if (!Tile)
		return false;

	// Only allow interaction with unrevealed tiles
	return !Tile->bIsRevealed;
}

// ==== Game flow

void SMinesweeperWidget::InitializeNewGame()
{
	if (!GameCore.IsValid())
	{
		MS_ERROR("GameCore is invalid when trying to initialize new game");
		return;
	}

	GameCore->InitializeGame(PendingGameSettings);

	// Refresh the UI
	RefreshGameBoardUI();
	UpdateGameInfoDisplay();
}

void SMinesweeperWidget::HandleGameStateChange(const EMinesweeperGameState NewState)
{
	switch (NewState)
	{
		case EMinesweeperGameState::Won:
		case EMinesweeperGameState::Lost:
		{
			// Add a slight delay to allow UI updates before showing dialog
			FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this](float) -> bool {
				ShowEndGameDialog();
				return false; // Remove ticker after execution
			}), 0.05f);
			break;
		}

		case EMinesweeperGameState::Active:
		case EMinesweeperGameState::NotStarted:
		default:
			// No special handling needed for these states yet
			break;
	}
}

void SMinesweeperWidget::ShowEndGameDialog() const
{
	if (!GameCore.IsValid())
		return;

	const bool bGameWon = GameCore->IsGameWon();
	const FString MessageText = bGameWon
		? TEXT("Congratulations! You won!\nClick on the Generate New Game button to play again!")
		: TEXT("Game Over! You hit a mine.\nClick on the Generate New Game button to try again!");

	const FString DialogTitle = TEXT("Minesweeper");
	const EAppMsgCategory MessageCategory = bGameWon ? EAppMsgCategory::Success : EAppMsgCategory::Warning;

	FMessageDialog::Open(MessageCategory, EAppMsgType::Ok, FText::FromString(MessageText), FText::FromString(DialogTitle));
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
