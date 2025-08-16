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
						.Value(GridWidth)
						.OnValueChanged(this, &SMinesweeperWidget::OnWidthChanged)
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
						.Value(GridHeight)
						.OnValueChanged(this, &SMinesweeperWidget::OnHeightChanged)
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
					.Value(BombCount)
					.OnValueChanged(this, &SMinesweeperWidget::OnBombCountChanged)
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

FReply SMinesweeperWidget::OnGenerateNewGameClicked()
{
	GenerateBoardTiles();
	return FReply::Handled();
}

void SMinesweeperWidget::OnWidthChanged(const int32 NewValue)
{
	GridWidth = FMath::Clamp(NewValue, GameGridMin, GameGridMax);
}

void SMinesweeperWidget::OnHeightChanged(const int32 NewValue)
{
	GridHeight = FMath::Clamp(NewValue, GameGridMin, GameGridMax);
}

void SMinesweeperWidget::OnBombCountChanged(const int32 NewValue)
{
	BombCount = FMath::Clamp(NewValue, 0, BombCountMax);
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
		// TODO: Generate Tile UI
		GameBoardGridUI->ClearChildren();
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
