// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"

MINESWEEPER_API DECLARE_LOG_CATEGORY_EXTERN(LogMinesweeper, Display, All);

// NOTE(Nghia Lam): Quick log support for this project
// ---

#define MS_LOG(type, msg, ...) UE_LOG(LogMinesweeper, type, TEXT("[%s] - %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(msg), ##__VA_ARGS__))
#define MS_DISPLAY(msg, ...) UE_LOG(LogMinesweeper, Display, TEXT("[%s] - %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(msg), ##__VA_ARGS__))
#define MS_WARNING(msg, ...) UE_LOG(LogMinesweeper, Warning, TEXT("[%s] - %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(msg), ##__VA_ARGS__))
#define MS_ERROR(msg, ...) UE_LOG(LogMinesweeper, Error, TEXT("[%s] - %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(msg), ##__VA_ARGS__))
#define MS_FATAL(msg, ...) UE_LOG(LogMinesweeper, Fatal, TEXT("[%s] - %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(msg), ##__VA_ARGS__))
