#pragma once

#ifdef GITHUB_ASSERT
#include <libassert/assert.hpp>
#else
#include <cassert>
#define DEUBG_ASSERT(condition, message) assert(condition)
#endif
