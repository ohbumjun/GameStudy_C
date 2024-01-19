#pragma once

#include <stddef.h>
#include <cstdint>

#define   likely(expr) __builtin_expect((bool)(expr),true )
#define unlikely(expr) __builtin_expect((bool)(expr),false)