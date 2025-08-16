// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"

MINESWEEPER_API DECLARE_LOG_CATEGORY_EXTERN(LogMinesweeper, Display, All);

// NOTE(Nghia Lam): Quick log support for this project
// ---
// We can turn all the logging in this project on/off by change this value 1/0

#define MINESWEEPER_LOG_ENABLE 1

#if MINESWEEPER_LOG_ENABLE

#define MS_LOG(type, msg, ...) UE_LOG(LogMinesweeper, type, TEXT("[%s] - %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(msg), ##__VA_ARGS__))
#define MS_DISPLAY(msg, ...) UE_LOG(LogMinesweeper, Display, TEXT("[%s] - %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(msg), ##__VA_ARGS__))
#define MS_WARNING(msg, ...) UE_LOG(LogMinesweeper, Warning, TEXT("[%s] - %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(msg), ##__VA_ARGS__))
#define MS_ERROR(msg, ...) UE_LOG(LogMinesweeper, Error, TEXT("[%s] - %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(msg), ##__VA_ARGS__))
#define MS_FATAL(msg, ...) UE_LOG(LogMinesweeper, Fatal, TEXT("[%s] - %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(msg), ##__VA_ARGS__))

#else

#define MS_LOG(type, msg, ...)
#define MS_DISPLAY(msg, ...) 
#define MS_WARNING(msg, ...)
#define MS_ERROR(msg, ...)
#define MS_FATAL(msg, ...)

#endif
