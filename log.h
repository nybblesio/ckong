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

#pragma once

typedef enum {
    category_app,
    category_error,
    category_assert,
    category_system,
    category_audio,
    category_video,
    category_render,
    category_input,
    category_test,
    category_reserved1,
    category_reserved2,
    category_custom
} log_category_t;

void log_init(void);

void log_warn(log_category_t category, const char* fmt, ...);

void log_debug(log_category_t category, const char* fmt, ...);

void log_error(log_category_t category, const char* fmt, ...);

void log_message(log_category_t category, const char* fmt, ...);

void log_critical(log_category_t category, const char* fmt, ...);
