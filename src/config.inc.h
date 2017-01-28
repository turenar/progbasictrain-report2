#pragma once

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef __attribute_malloc__
#define __attribute_malloc__ __attribute__((__malloc__))
#endif

#ifndef __attribute_format__
#ifdef __GNUC__
#define __attribute_format__(type, string_index, first_to_check) __attribute((format(type, string_index, first_to_check)))
#else
#define __attribute_format__(a,b,c) /**/
#endif
#endif

#ifdef __cplusplus
#define UNINIT_WARNING_SUPPRESSOR {}
#else
#define UNINIT_WARNING_SUPPRESSOR {0}
#endif

#define UNUSED_VAR(x) ((void)x)
