// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"

MINESWEEPER_API DECLARE_LOG_CATEGORY_EXTERN(LogMinesweeper, Display, All);

/**
 * Logging system for the Minesweeper plugin
 * 
 * Provides convenient macros for different log levels with automatic function name inclusion
 * Can be easily enabled/disabled for the entire plugin via MINESWEEPER_LOG_ENABLE
 */

// Master switch for all logging in this plugin (1 = enabled, 0 = disabled)
#define MINESWEEPER_LOG_ENABLE 1

#if MINESWEEPER_LOG_ENABLE

#define MS_LOG(type, msg, ...) UE_LOG(LogMinesweeper, type, TEXT("[%s] - %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(msg), ##__VA_ARGS__))
#define MS_DISPLAY(msg, ...) UE_LOG(LogMinesweeper, Display, TEXT("[%s] - %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(msg), ##__VA_ARGS__))
#define MS_WARNING(msg, ...) UE_LOG(LogMinesweeper, Warning, TEXT("[%s] - %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(msg), ##__VA_ARGS__))
#define MS_ERROR(msg, ...) UE_LOG(LogMinesweeper, Error, TEXT("[%s] - %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(msg), ##__VA_ARGS__))
#define MS_FATAL(msg, ...) UE_LOG(LogMinesweeper, Fatal, TEXT("[%s] - %s"), *FString(__FUNCTION__), *FString::Printf(TEXT(msg), ##__VA_ARGS__))

#else

// When logging is disabled, all macros become no-ops

#define MS_LOG(type, msg, ...)
#define MS_DISPLAY(msg, ...)
#define MS_WARNING(msg, ...)
#define MS_ERROR(msg, ...)
#define MS_FATAL(msg, ...)

#endif // MINESWEEPER_LOG_ENABLE
