// --------------------------------------------------------------------------
//
// C Kong
// Copyright (C) 2018 Jeff Panici
// All rights reserved.
//
// This software source file is licensed according to the
// MIT License.  Refer to the LICENSE file distributed along
// with this source file to learn more.
//
// --------------------------------------------------------------------------

#include <stdarg.h>
#include <SDL_log.h>
#include "log.h"

void log_init(void) {
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
}

void log_warn(log_category_t category, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    SDL_LogWarn(category, fmt, args);
    va_end(args);
}

void log_debug(log_category_t category, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    SDL_LogDebug(category, fmt, args);
    va_end(args);
}

void log_error(log_category_t category, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    SDL_LogError(category, fmt, args);
    va_end(args);
}

void log_message(log_category_t category, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    SDL_LogMessageV(category, SDL_LOG_PRIORITY_INFO, fmt, args);
    va_end(args);
}

void log_critical(log_category_t category, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    SDL_LogCritical(category, fmt, args);
    va_end(args);
}