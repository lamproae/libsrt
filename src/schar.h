#ifndef SCHAR_H
#define SCHAR_H
#ifdef __cplusplus
extern "C" {
#endif

/*
 * schar.h
 *
 * Unicode processing helper functions.
 *
 * Copyright (c) 2015-2016, F. Aragon. All rights reserved. Released under
 * the BSD 3-Clause License (see the doc/LICENSE file included).
 *
 * Designed to be used by libraries or wrapped by some abstraction (e.g.
 * sstring), not as general-purpose direct usage.
 *
 * Features:
 *
 * - Unicode <-> UTF-8 character conversion.
 * - Compute Unicode required size for UTF-8 representation.
 * - Count Unicode characters into UTF-8 stream.
 * - Lowercase/uppercase conversions.
 * - Not relies on C library nor OS Unicode support ("locale").
 * - Small memory footprint (not using hash tables).
 * - Fast
 *
 * Observations:
 * - Turkish (tr_CY/tr_TR) is an especial case that breaks the possibility
 *   of generic case conversion, because of the 'i' <-> 'I' collision with
 *   other case conversion for other languages using the latin alphabet, as
 *   in that locale there are two lowercase 'i', with and without dot, and
 *   also two uppercase 'I' with, and without dot, so the 'i' <-> 'I' don't
 *   applies. For covering that case, equivalent to having the tr_* locale
 *   set, sc_tolower_tr()/sc_toupper_tr() are provided.
 */ 

#include "scommon.h"

enum SSUTF8 {
	SSU8_SX = 0x80,
	SSU8_S1 = 0x00,
	SSU8_S2 = 0xc0,
	SSU8_S3 = 0xe0,
	SSU8_S4 = 0xf0,
	SSU8_S5 = 0xf8,
	SSU8_S6 = 0xfc,
	SSUB_MX = 0x3f,
	SSU8_M1 = 0x80,
	SSU8_M2 = 0xe0,
	SSU8_M3 = 0xf0,
	SSU8_M4 = 0xf8,
	SSU8_M5 = 0xfc,
	SSU8_M6 = 0xfe,
	SSU8_MAX_SIZE = 6
};

#define SSU8_VALID_START(c)     (((c) & 0xc0) != SSU8_SX)

size_t sc_utf8_char_size(const char *s, const size_t off, const size_t max_off);
size_t sc_utf8_count_chars(const char *s, const size_t s_size);
size_t sc_wc_to_utf8_size(const int32_t c);
size_t sc_wc_to_utf8(const int c, char *s, const size_t off, const size_t max_off);
size_t sc_utf8_to_wc(const char *s, const size_t off, const size_t max_off, int *unicode_out, int *encoding_errors);
size_t sc_unicode_count_to_utf8_size(const char *, const size_t off, const size_t max_off, const size_t unicode_count, size_t *actual_unicode_count);
ssize_t sc_utf8_calc_case_extra_size(const char *s, const size_t off, const size_t s_size, int32_t (*ssc_toX)(const int32_t));
int32_t sc_tolower(const int32_t c);
int32_t sc_toupper(const int32_t c);
int32_t sc_tolower_tr(const int32_t c);
int32_t sc_toupper_tr(const int32_t c);
size_t sc_parallel_toX(const char *s, size_t off, const size_t max, unsigned *o, int32_t (*ssc_toX)(const int32_t));

#ifdef __cplusplus
}      /* extern "C" { */
#endif
#endif	/* SCHAR_H */
