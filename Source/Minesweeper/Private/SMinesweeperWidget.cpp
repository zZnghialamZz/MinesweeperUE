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
					SAssignNew(WidthSpinBox, SSpinBox<uint32>)
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
					SAssignNew(HeightSpinBox, SSpinBox<uint32>)
					.MinValue(GameGridMin)
					.MaxValue(GameGridMax)
					.Value(GridHeight)
					.OnValueChanged(this, &SMinesweeperWidget::OnHeightChanged)
				]
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(10, 5, 0 , 5)
			[
				SNew(STextBlock).Text(FText::FromString(TEXT("Number of Mines:")))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.MinWidth(100)
			[
				SAssignNew(BombCountSpinBox, SSpinBox<uint32>)
				.MinValue(0)
				.MaxValue(BombCountMax)
				.Value(BombCount)
				.OnValueChanged(this, &SMinesweeperWidget::OnBombCountChanged)
			]
		];
}

TSharedRef<SWidget> SMinesweeperWidget::CreateGameBoard()
{
	return SAssignNew(GameBoardGrid, SUniformGridPanel);
}

void SMinesweeperWidget::OnWidthChanged(const uint32 NewValue)
{
	GridWidth = FMath::Clamp(NewValue, GameGridMin, GameGridMax);
}

void SMinesweeperWidget::OnHeightChanged(const uint32 NewValue)
{
	GridHeight = FMath::Clamp(NewValue, GameGridMin, GameGridMax);
}

void SMinesweeperWidget::OnBombCountChanged(const uint32 NewValue)
{
	BombCount = FMath::Clamp(NewValue, 0, BombCountMax);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
