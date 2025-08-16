// Copyright Epic Games, Inc. All Rights Reserved.

#include "SMinesweeperWidget.h"

#include "MinesweeperLog.h"
#include "SlateOptMacros.h"
#include "SMinesweeperTileButton.h"
#include "Widgets/Layout/SUniformGridPanel.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMinesweeperWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(8.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 0, 0, 8)
			[
				CreateControlPanel()
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 8)
			[
				SNew(SSeparator)
				.Orientation(Orient_Horizontal)
			]
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

	InitializeGame();
}

TSharedRef<SWidget> SMinesweeperWidget::CreateControlPanel()
{
	return SNew(SVerticalBox)
			// Width & Height input field area
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 5)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(10, 0)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(0, 5)
					[
						SNew(STextBlock).Text(FText::FromString("Width: "))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.MinWidth(100)
					[
						SAssignNew(WidthSpinBoxUI, SSpinBox<int32>)
						.MinValue(GameGridMin)
						.MaxValue(GameGridMax)
						.Value(GridWidthUIValue)
						.OnValueChanged(this, &SMinesweeperWidget::OnWidthUIValueChanged)
					]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(10, 0)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(0, 5)
					[
						SNew(STextBlock).Text(FText::FromString("Height: "))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.MinWidth(100)
					[
						SAssignNew(HeightSpinBoxUI, SSpinBox<int32>)
						.MinValue(GameGridMin)
						.MaxValue(GameGridMax)
						.Value(GridHeightUIValue)
						.OnValueChanged(this, &SMinesweeperWidget::OnHeightUIValueChanged)
					]
				]
			]
			// Number of Mines input field area
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 5)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(10, 5, 0, 5)
				[
					SNew(STextBlock).Text(FText::FromString(TEXT("Number of Mines: ")))
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.MinWidth(100)
				[
					SAssignNew(BombCountSpinBoxUI, SSpinBox<int32>)
					.MinValue(BombCountMin)
					.MaxValue(BombCountMax)
					.Value(BombCountUIValue)
					.OnValueChanged(this, &SMinesweeperWidget::OnBombCountUIValueChanged)
				]
			]
			// Generate new grid button area
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 10, 0, 5)
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("Generate new grid")))
				.OnClicked(this, &SMinesweeperWidget::OnGenerateNewGameClicked)
			]
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			.Padding(8, 4)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				[
					SAssignNew(FlagCountTextUI, STextBlock)
					.Text(FText::FromString(TEXT("Flags: 0/10")))
					.Justification(ETextJustify::Left)
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				[
					SAssignNew(GameStatusTextUI, STextBlock)
					.Text(FText::FromString(TEXT("Ready to play! Click the button above to start!")))
					.Justification(ETextJustify::Right)
				]
			];
}

TSharedRef<SWidget> SMinesweeperWidget::CreateGameBoard()
{
	return SAssignNew(GameBoardGridUI, SUniformGridPanel);
}

TSharedRef<SWidget> SMinesweeperWidget::CreateTileButton(const int32 X, const int32 Y)
{
	TSharedRef<SButton> NewTileButton = SNew(SMinesweeperTileButton)
		.IsEnabled(this, &SMinesweeperWidget::IsTileButtonInteractable, X, Y)
		.ButtonColorAndOpacity(this, &SMinesweeperWidget::GetTileButtonBackgroundColor, X, Y)
		.OnClicked(this, &SMinesweeperWidget::OnTileClicked, X, Y)
		.OnRightClicked(this, &SMinesweeperWidget::OnTileRightClicked, X, Y)
		[
			SNew(SBox)
			.WidthOverride(20)
			.HeightOverride(20)
			[
				SNew(STextBlock)
				.Text(this, &SMinesweeperWidget::GetTileButtonText, X, Y)
				.ColorAndOpacity(this, &SMinesweeperWidget::GetTileButtonTextColor, X, Y)
				.Font(FAppStyle::Get().GetFontStyle("BoldFont"))
				.Justification(ETextJustify::Center)
			]
		];

	const int32 TileIndex = GetTileIndex(X, Y);

	TileButtons.Add(TileIndex, NewTileButton);
	return NewTileButton;
}

FText SMinesweeperWidget::GetTileButtonText(const int32 X, const int32 Y) const
{
	if (!IsValidCoordinate(X, Y))
	{
		MS_WARNING("Invalid tile coordinate at [%d, %d]", X, Y);
		return FText::GetEmpty();
	}

	const FMinesweeperTile& Tile = GameBoardTiles[GetTileIndex(X, Y)];

	// Show flag if tile is flagged
	if (Tile.bIsFlagged)
	{
		return FText::FromString(TEXT("🚩"));
	}
	if (!Tile.bIsRevealed)
	{
		return FText::FromString(TEXT(""));
	}
	if (Tile.bIsBomb)
	{
		return FText::FromString(TEXT("💣"));
	}

	if (Tile.AdjacentBombs > 0)
	{
		return FText::AsNumber(Tile.AdjacentBombs);
	}
	return FText::GetEmpty();
}

FSlateColor SMinesweeperWidget::GetTileButtonTextColor(const int32 X, const int32 Y) const
{
	if (!IsValidCoordinate(X, Y))
	{
		MS_WARNING("Invalid tile coordinate at [%d, %d]", X, Y);
		return FSlateColor::UseForeground();
	}

	const FMinesweeperTile& Tile = GameBoardTiles[GetTileIndex(X, Y)];

	if (Tile.bIsBomb && Tile.bIsRevealed)
	{
		return FSlateColor(FLinearColor::Red);
	}

	if (Tile.bIsRevealed && Tile.AdjacentBombs > 0)
	{
		// Color code numbers based on count
		switch (Tile.AdjacentBombs)
		{
			case 1:
				return FSlateColor(FLinearColor(0.0f, 1.0f, 1.0f, 1.0f)); // Cyan
			case 2:
				return FSlateColor(FLinearColor::Green);
			case 3:
				return FSlateColor(FLinearColor::Yellow);
			case 4:
				return FSlateColor(FLinearColor(1.0f, 0.5f, 0.0f, 1.0f)); // Orange
			case 5:
				return FSlateColor(FLinearColor(1.0f, 0.7f, 0.7f, 1.0f)); // Pink
			case 6:
				return FSlateColor(FLinearColor(0.5f, 0.0f, 0.5f, 1.0f)); // Purple
			case 7:
				return FSlateColor(FLinearColor(0.5f, 0.0f, 0.0f, 1.0f)); // Maroon
			case 8:
				return FSlateColor(FLinearColor::Red);
			default:
				return FSlateColor::UseForeground();
		}
	}

	return FSlateColor::UseForeground();
}

FSlateColor SMinesweeperWidget::GetTileButtonBackgroundColor(const int32 X, const int32 Y) const
{
	if (!IsValidCoordinate(X, Y))
	{
		MS_WARNING("Invalid tile coordinate at [%d, %d]", X, Y);
		return FSlateColor::UseForeground();
	}

	const FMinesweeperTile& Tile = GameBoardTiles[GetTileIndex(X, Y)];
	if (Tile.bIsBomb && Tile.bIsRevealed && !Tile.bIsFlagged)
	{
		return FSlateColor(FLinearColor::Red);
	}
	if (Tile.bIsFlagged && Tile.bIsRevealed && !Tile.bIsBomb)
	{
		return FSlateColor(FLinearColor::Red);
	}
	if (Tile.bIsRevealed)
	{
		return FSlateColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.0f)); // Dark Gray
	}
	return FSlateColor::UseForeground();
}

bool SMinesweeperWidget::IsTileButtonInteractable(const int32 X, const int32 Y) const
{
	// Only allow clicks when game is active
	if (!bGameActive)
		return false;

	// Don't allow clicking already revealed tiles
	if (IsValidCoordinate(X, Y))
	{
		const FMinesweeperTile& Tile = GameBoardTiles[GetTileIndex(X, Y)];
		return !Tile.bIsRevealed;
	}
	return false;
}

void SMinesweeperWidget::UpdateFlagCountDisplay()
{
	if (FlagCountTextUI.IsValid())
	{
		const int32 RemainingFlags = BombCount - FlaggedTiles;
		FString FlagText = FString::Printf(TEXT("Flags: %d/%d"), FlaggedTiles, BombCount);
		if (RemainingFlags >= 0)
		{
			FlagText += FString::Printf(TEXT(" (Remaining: %d)"), RemainingFlags);
		}
		FlagCountTextUI->SetText(FText::FromString(FlagText));
	}
}

void SMinesweeperWidget::ShowEndGameDialog() const
{
	FString MessageText = bGameWon ? TEXT("Congratulations! You won!") : TEXT("Game Over! You hit a mine.");
	MessageText += TEXT("\nClick on the Generated new grid button to generate a new game!");

	const FString DialogTitle = TEXT("Minesweeper Notice");
	const EAppMsgCategory MessageCategory = bGameWon ? EAppMsgCategory::Success : EAppMsgCategory::Warning;
	FMessageDialog::Open(MessageCategory, EAppMsgType::Ok, FText::FromString(MessageText), FText::FromString(DialogTitle));
}

FReply SMinesweeperWidget::OnGenerateNewGameClicked()
{
	// Update these value at the time user press generate new grid, the ui value can be changed afterward.
	// But the value use for game is only updated when user press this button. This is to prevent any bug
	// happens when the player is still playing on current board but decide to update the UI value.
	GridWidth = GridWidthUIValue;
	GridHeight = GridHeightUIValue;
	BombCount = FMath::Min(BombCountUIValue, GridWidthUIValue * GridHeightUIValue - 1); // Bomb shouldn't go all over board

	InitializeGame();
	return FReply::Handled();
}

FReply SMinesweeperWidget::OnTileClicked(const int32 X, const int32 Y)
{
	MS_DISPLAY("Try revealing tile [%d - %d]", X, Y);
	RevealTile(X, Y);
	CheckWinCondition();
	return FReply::Handled();
}

FReply SMinesweeperWidget::OnTileRightClicked(const int32 X, const int32 Y)
{
	MS_DISPLAY("Toggle flag in tile [%d - %d]", X, Y);
	ToggleFlag(X, Y);
	CheckWinCondition();
	return FReply::Handled();
}

void SMinesweeperWidget::OnWidthUIValueChanged(const int32 NewValue)
{
	GridWidthUIValue = FMath::Clamp(NewValue, GameGridMin, GameGridMax);
}

void SMinesweeperWidget::OnHeightUIValueChanged(const int32 NewValue)
{
	GridHeightUIValue = FMath::Clamp(NewValue, GameGridMin, GameGridMax);
}

void SMinesweeperWidget::OnBombCountUIValueChanged(const int32 NewValue)
{
	BombCountUIValue = FMath::Clamp(NewValue, BombCountMin, BombCountMax);
}

void SMinesweeperWidget::InitializeGame()
{
	ResetGame();
	GenerateBoardTiles();
	UpdateFlagCountDisplay();
}

void SMinesweeperWidget::ResetGame()
{
	bGameActive = false;
	bGameWon = false;
}

void SMinesweeperWidget::EndGame(const bool bWon)
{
	bGameActive = false;
	bGameWon = bWon;

	RevealAllBombsAndFlag(bWon);
	FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this](float) -> bool {
		// Delay show dialog to allow Slate UI update its visual
		ShowEndGameDialog();
		return false; // Return false to remove the ticker
	}), 0.05f);

	if (GameStatusTextUI.IsValid())
	{
		const FString StatusText = bWon ? TEXT("Congratulations! You won!") : TEXT("Game Over! You hit a mine.");
		GameStatusTextUI->SetText(FText::FromString(StatusText));
	}
}

void SMinesweeperWidget::CheckWinCondition()
{
	if (!bGameActive)
	{
		return;
	}

	const int32 TotalNonBombTiles = GridWidth * GridHeight - BombCount;
	const bool bAllNonBombTilesRevealed = RevealedTiles >= TotalNonBombTiles;

	// Also check if all bombs are correctly flagged
	int32 CorrectlyFlaggedBombs = 0;
	bool bPlaceWrongFlag = false;
	for (const auto& Tile : GameBoardTiles)
	{
		if (Tile.bIsBomb && Tile.bIsFlagged)
		{
			CorrectlyFlaggedBombs++;
		}
		// Check for incorrectly flagged tiles (flagged but not a bomb)
		else if (!Tile.bIsBomb && Tile.bIsFlagged)
		{
			// If place wrong flag, the user still not win yet
			bPlaceWrongFlag = true;
			break;
		}
	}
	const bool bPerfectlyFlagged = CorrectlyFlaggedBombs == BombCount && !bPlaceWrongFlag;

	if (bAllNonBombTilesRevealed || bPerfectlyFlagged)
	{
		EndGame(true);
	}
}

void SMinesweeperWidget::GenerateBoardTiles()
{
	GameBoardTiles.Empty();
	GameBoardTiles.SetNum(GridWidth * GridHeight);
	for (FMinesweeperTile& Tile : GameBoardTiles)
	{
		Tile = FMinesweeperTile();
	}

	PlaceBombsRandomly();
	CalculateAdjacentBombs();

	// Rebuild the UI
	if (GameBoardGridUI.IsValid())
	{
		GameBoardGridUI->ClearChildren();
		TileButtons.Empty();

		for (int32 Y = 0; Y < GridHeight; ++Y)
		{
			for (int32 X = 0; X < GridWidth; ++X)
			{
				TSharedRef<SWidget> TileWidget = CreateTileButton(X, Y);
				GameBoardGridUI->AddSlot(X, Y)
				[
					TileWidget
				];
			}
		}
	}
	else
	{
		MS_ERROR("GameBoardGridUI is not valid, please re-check UI code!");
		return;
	}

	bGameActive = true;
	RevealedTiles = 0;
	FlaggedTiles = 0;

	if (GameStatusTextUI.IsValid())
	{
		GameStatusTextUI->SetText(FText::FromString(TEXT("Game Started! Left-click to reveal, right-click to flag.")));
	}
}

void SMinesweeperWidget::PlaceBombsRandomly()
{
	TArray<int32> AvailableIndices;
	for (int32 i = 0; i < GameBoardTiles.Num(); ++i)
	{
		AvailableIndices.Add(i);
	}

	// Make sure again the number of bombs shouldn't cover all the board
	const int32 BombsToPlace = FMath::Min(BombCount, GridWidth * GridHeight - 1);
	for (int32 i = 0; i < BombsToPlace; ++i)
	{
		if (AvailableIndices.Num() == 0)
			break;

		const int32 RandomIndex = FMath::RandRange(0, AvailableIndices.Num() - 1);
		const int32 BoardIndex = AvailableIndices[RandomIndex];

		GameBoardTiles[BoardIndex].bIsBomb = true;
		AvailableIndices.RemoveAt(RandomIndex);
	}
}

void SMinesweeperWidget::CalculateAdjacentBombs()
{
	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		for (int32 X = 0; X < GridWidth; ++X)
		{
			if (!GameBoardTiles[GetTileIndex(X, Y)].bIsBomb)
			{
				int32 AdjacentBombs = 0;

				// Check all 8 adjacent tiles
				for (int32 DY = -1; DY <= 1; ++DY)
				{
					for (int32 DX = -1; DX <= 1; ++DX)
					{
						if (DX == 0 && DY == 0)
							continue;

						const int32 CheckX = X + DX;
						const int32 CheckY = Y + DY;

						if (IsValidCoordinate(CheckX, CheckY))
						{
							if (GameBoardTiles[GetTileIndex(CheckX, CheckY)].bIsBomb)
							{
								AdjacentBombs++;
							}
						}
					}
				}

				GameBoardTiles[GetTileIndex(X, Y)].AdjacentBombs = AdjacentBombs;
			}
		}
	}
}

void SMinesweeperWidget::RevealAllBombsAndFlag(const bool bRevealBombAsFlag)
{
	// Reveal all bombs and flagged tiles
	for (int32 i = 0; i < GameBoardTiles.Num(); ++i)
	{
		if (GameBoardTiles[i].bIsBomb || GameBoardTiles[i].bIsFlagged)
		{
			if (bRevealBombAsFlag)
				GameBoardTiles[i].bIsFlagged = true;
			GameBoardTiles[i].bIsRevealed = true;
		}
	}
}

void SMinesweeperWidget::RevealTile(const int32 X, const int32 Y)
{
	if (!bGameActive || !IsValidCoordinate(X, Y))
	{
		return;
	}

	FMinesweeperTile& Tile = GameBoardTiles[GetTileIndex(X, Y)];
	if (Tile.bIsRevealed || Tile.bIsFlagged)
		return;

	Tile.bIsRevealed = true;
	RevealedTiles++;

	if (Tile.bIsBomb)
	{
		EndGame(false);
		return;
	}

	// Auto-reveal adjacent tiles if this tile has no adjacent bombs
	if (Tile.AdjacentBombs == 0)
	{
		RevealAdjacentTiles(X, Y);
	}
}

void SMinesweeperWidget::RevealAdjacentTiles(const int32 X, const int32 Y)
{
	for (int32 DY = -1; DY <= 1; ++DY)
	{
		for (int32 DX = -1; DX <= 1; ++DX)
		{
			if (DX == 0 && DY == 0)
				continue;

			const int32 CheckX = X + DX;
			const int32 CheckY = Y + DY;

			if (IsValidCoordinate(CheckX, CheckY))
			{
				RevealTile(CheckX, CheckY);
			}
		}
	}
}

void SMinesweeperWidget::ToggleFlag(const int32 X, const int32 Y)
{
	if (!bGameActive || !IsValidCoordinate(X, Y))
		return;

	FMinesweeperTile& Tile = GameBoardTiles[GetTileIndex(X, Y)];

	// Can't flag already revealed tiles
	if (Tile.bIsRevealed)
		return;

	if (Tile.bIsFlagged)
	{
		// Remove flag
		Tile.bIsFlagged = false;
		FlaggedTiles--;
	}
	else
	{
		// Add flag (only if we haven't exceeded bomb count)
		if (FlaggedTiles < BombCount)
		{
			Tile.bIsFlagged = true;
			FlaggedTiles++;
		}
	}

	UpdateFlagCountDisplay();
}

bool SMinesweeperWidget::IsValidCoordinate(const int32 X, const int32 Y) const
{
	return X >= 0 && X < GridWidth && Y >= 0 && Y < GridHeight;
}

int32 SMinesweeperWidget::GetTileIndex(const int32 X, const int32 Y) const
{
	return Y * GridWidth + X;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
