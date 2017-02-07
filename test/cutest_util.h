#pragma once

#include <cutest/CuTest.h>
#include <stddef.h>

void CuAssertSizeTEquals_LineMsg(CuTest* tc, const char* file, int line, const char* message,
                                 size_t expected, size_t actual);

#define CuAssertSizeTEquals(tc, ex, ac) CuAssertSizeTEquals_LineMsg((tc),__FILE__,__LINE__,NULL,(ex),(ac))
