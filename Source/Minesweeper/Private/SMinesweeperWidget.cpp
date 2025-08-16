// Fill out your copyright notice in the Description page of Project Settings.


#include "SMinesweeperWidget.h"

#include "SlateOptMacros.h"
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
				.MaxDesiredWidth(800)
				.MinDesiredHeight(600)
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
					.MinValue(0)
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
			.HAlign(HAlign_Right)
			.Padding(8, 4)
			[
				SAssignNew(GameStatusTextUI, STextBlock)
				.Text(FText::FromString(TEXT("Ready to play! Click the button above to start!")))
				.Justification(ETextJustify::Right)
			];
}

TSharedRef<SWidget> SMinesweeperWidget::CreateGameBoard()
{
	return SAssignNew(GameBoardGridUI, SUniformGridPanel);
}

TSharedRef<SWidget> SMinesweeperWidget::CreateTileButton(const int32 X, const int32 Y)
{
	TSharedRef<SButton> NewTileButton = SNew(SButton)
		//.ButtonStyle(FAppStyle::Get(), "FlatButton")
		.ContentPadding(0)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.OnClicked(this, &SMinesweeperWidget::OnTileClicked, X, Y)
		[
			SNew(SBox)
			.WidthOverride(25)
			.HeightOverride(25)
			[
				SNew(STextBlock)
				.Text(this, &SMinesweeperWidget::GetTileButtonText, X, Y)
				.ColorAndOpacity(this, &SMinesweeperWidget::GetTileButtonColor, X, Y)
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
		return FText::GetEmpty();

	const FMinesweeperTile& Tile = GameBoardTiles[GetTileIndex(X, Y)];
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

FSlateColor SMinesweeperWidget::GetTileButtonColor(const int32 X, const int32 Y) const
{
	if (!IsValidCoordinate(X, Y))
		return FSlateColor::UseForeground();

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
				return FSlateColor(FLinearColor::Blue);
			case 2:
				return FSlateColor(FLinearColor::Green);
			case 3:
				return FSlateColor(FLinearColor::Red);
			case 4:
				return FSlateColor(FLinearColor(0.5f, 0.0f, 0.5f, 1.0f)); // Purple
			case 5:
				return FSlateColor(FLinearColor(0.5f, 0.0f, 0.0f, 1.0f)); // Maroon
			case 6:
				return FSlateColor(FLinearColor(0.0f, 0.5f, 0.5f, 1.0f)); // Teal
			case 7:
				return FSlateColor(FLinearColor::Black);
			case 8:
				return FSlateColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f)); // Gray
			default:
				return FSlateColor::UseForeground();
		}
	}

	return FSlateColor::UseForeground();
}

FReply SMinesweeperWidget::OnGenerateNewGameClicked()
{
	// Maintain these value at the time user press generate new grid, the ui value can be changed afterward.
	// But the value use for game is only updated when user press this button.
	GridWidth = GridWidthUIValue;
	GridHeight = GridHeightUIValue;
	BombCount = BombCountUIValue;
	ResetGame();
	GenerateBoardTiles();
	return FReply::Handled();
}

FReply SMinesweeperWidget::OnTileClicked(int32 X, int32 Y)
{
	// TODO
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
	BombCountUIValue = FMath::Clamp(NewValue, 0, BombCountMax);
}

void SMinesweeperWidget::InitializeGame()
{
	ResetGame();
	GenerateBoardTiles();
}

void SMinesweeperWidget::ResetGame()
{
	bGameActive = false;
	bGameWon = false;
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

	bGameActive = true;
	RevealedTiles = 0;

	if (GameStatusTextUI.IsValid())
	{
		GameStatusTextUI->SetText(FText::FromString(TEXT("Game Started! Click tiles to reveal them.")));
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

bool SMinesweeperWidget::IsValidCoordinate(const int32 X, const int32 Y) const
{
	return X >= 0 && X < GridWidth && Y >= 0 && Y < GridHeight;
}

int32 SMinesweeperWidget::GetTileIndex(const int32 X, const int32 Y) const
{
	return Y * GridWidth + X;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
