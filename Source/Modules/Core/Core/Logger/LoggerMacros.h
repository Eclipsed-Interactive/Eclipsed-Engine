#pragma once

#include "DebugMessageFactory.h"
#include "DebugLogger.h"

#ifdef ECL_GAME
	#define LOG_FUNC Eclipse::Core::DebugLogger::AddGameLogMessage
#else
	#define LOG_FUNC Eclipse::Core::DebugLogger::AddEngineLogMessage
#endif

#ifndef CHANGE_THIS_TO_A_SHIPPING_DEFINE

#define LOG(message) LOG_FUNC(Eclipse::DebugMessageFactory::ConstructSourceCodeMessage(message, __FILE__, Eclipse::DiagnosticSeverity::Info), __LINE__);
#define LOG_WARNING(message) LOG_FUNC(Eclipse::DebugMessageFactory::ConstructSourceCodeMessage(message, __FILE__, Eclipse::DiagnosticSeverity::Warning), __LINE__);
#define LOG_ERROR(message) LOG_FUNC(Eclipse::DebugMessageFactory::ConstructSourceCodeMessage(message, __FILE__, Eclipse::DiagnosticSeverity::Error), __LINE__);

#else
#define LOG(MESSAGE)
#define LOG_WARNING(MESSAGE)
#define LOG_ERROR(MESSAGE)
#endif