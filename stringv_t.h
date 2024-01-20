/*MIT License

Copyright (c) 2024 yuI4140

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#ifndef STRVDEF
#define STRVDEF
#endif

typedef struct {
  size_t count;
  const char *data;
} Stringv;
#define STRV(cstr_lit) strv_init(cstr_lit, sizeof(cstr_lit) - 1)
#define STRV_STATIC(cstr_lit)                                                  \
  { sizeof(cstr_lit) - 1, (cstr_lit) }

#define STRV_NULL strv_init(NULL, 0)

// printf macros for Stringv
#define STRV_Fmt "%.*s"
#define STRV_Arg(strv) (int)(strv).count, (strv).data
// Example:
//   Stringv hi = strv_init("hello, world!\n",14);
//   printf(""STRV_Fmt"\n", STRV_Arg(hi));
STRVDEF Stringv strv_initz(void);
STRVDEF char at_strv(Stringv *strv, size_t index);
STRVDEF Stringv strv_init(const char *data, size_t size);
STRVDEF Stringv strv_from_cstr(const char *cstr);
STRVDEF Stringv strv_trim_left(Stringv strv);
STRVDEF Stringv strv_trim_right(Stringv strv);
STRVDEF Stringv strv_trim(Stringv strv);
STRVDEF Stringv strv_take_left_while(Stringv strv, bool (*predicate)(char x));
STRVDEF Stringv strv_chop_left(Stringv *strv, size_t n);
STRVDEF Stringv strv_chop_right(Stringv *strv, size_t n);
STRVDEF Stringv strv_chop_left_while(Stringv *strv, bool (*predicate)(char x));
STRVDEF bool strv_try_chop_by_delim(Stringv *strv, char delim, Stringv *chunk);
STRVDEF bool strv_chop_by_delim(Stringv *strv, char delim,Stringv *chunk);
STRVDEF Stringv strv_chop_by_strv(Stringv *strv, Stringv thicc_delim);
STRVDEF void str_cut(Stringv *src, uint32_t start, uint32_t end);
STRVDEF bool strv_index_of(Stringv strv, char c, size_t *index);
STRVDEF bool strv_eq(Stringv a, Stringv b);
STRVDEF bool strv_eq_ignorecase(Stringv a, Stringv b);
STRVDEF bool strv_starts_with(Stringv strv, Stringv prefix);
STRVDEF bool strv_ends_with(Stringv strv, Stringv suffix);
STRVDEF uint64_t strv_to_u64(Stringv strv);
STRVDEF int strv_cpy(Stringv *dest, const Stringv *src);
STRVDEF int strv_cat(Stringv *dest, const Stringv *src);
STRVDEF Stringv strv_dup(const Stringv *strv);
uint64_t strv_chop_u64(Stringv *strv);
#ifdef STRV_UTILS
#define STRV_ASSERT assert
#define STRV_REALLOC realloc
#define STRV_FREE free
#define MV_CHAR(dest, src)                                                     \
  do {                                                                         \
    if (dest != src) {                                                         \
      memcpy(&dest, &src, sizeof(char));                                       \
      src = 0;                                                                 \
    }                                                                          \
  } while (0)
#define CP_CHAR(dest, src)                                                     \
  do {                                                                         \
    if (dest != src) {                                                         \
      memcpy(&dest, &src, sizeof(char));                                       \
    }                                                                          \
  } while (0)
#define fiterate_strv(strv) for (size_t it = 0; it < strv->count; ++it)
#define witerate_strv(strv)                                                    \
  size_t it = 0;                                                               \
  while (str->data[it] != '\0')
#define strv_da_append(da, item)                                                          \
    do {                                                                                 \
        if ((da)->count >= (da)->capacity) {                                             \
            (da)->capacity = (da)->capacity == 0 ? STRV_DA_INIT_CAP : (da)->capacity*2;   \
            (da)->items = STRV_REALLOC((da)->items, (da)->capacity*sizeof(*(da)->items)); \
            STRV_ASSERT((da)->items != NULL && "Buy more RAM lol");                       \
        }                                                                                \
                                                                                         \
        (da)->items[(da)->count++] = (item);                                             \
    } while (0)

#define strv_da_free(da) STRV_FREE((da).items)
#define STRV_ARRAY_LEN(array) (sizeof(array)/sizeof(array[0]))
#define STRV_ARRAY_GET(array, index) \
    (STRV_ASSERT(index >= 0), STRV_ASSERT(index < STRV_ARRAY_LEN(array)), array[index])
#endif // STRV_UTILS
#ifdef STRV_IMP
STRVDEF Stringv strv_initz(void) {
  Stringv strv = {0};
  return strv;
}
STRVDEF Stringv strv_init(const char *data, size_t size) {
  Stringv strv;
  strv.count = size;
  strv.data = data;
  return strv;
}

STRVDEF Stringv strv_from_cstr(const char *cstr) {
  return strv_init(cstr, strlen(cstr));
}

STRVDEF Stringv strv_trim_left(Stringv strv) {
  size_t i = 0;
  while (i < strv.count && isspace(strv.data[i])) {
    i += 1;
  }

  return strv_init(strv.data + i, strv.count - i);
}

STRVDEF Stringv strv_trim_right(Stringv strv) {
  size_t i = 0;
  while (i < strv.count && isspace(strv.data[strv.count - 1 - i])) {
    i += 1;
  }
  return strv_init(strv.data, strv.count - i);
}

STRVDEF Stringv strv_trim(Stringv strv) {
  return strv_trim_right(strv_trim_left(strv));
}

STRVDEF Stringv strv_chop_left(Stringv *strv, size_t n) {
  if (n > strv->count) {
    n = strv->count;
  }

  Stringv result = strv_init(strv->data, n);
  strv->data += n;
  strv->count -= n;
  return result;
}

STRVDEF Stringv strv_chop_right(Stringv *strv, size_t n) {
  if (n > strv->count) {
    n = strv->count;
  }
  Stringv result = strv_init(strv->data + strv->count - n, n);
  strv->count -= n;
  return result;
}

STRVDEF bool strv_index_of(Stringv strv, char c, size_t *index) {
  size_t i = 0;
  while (i < strv.count && strv.data[i] != c) {
    i += 1;
  }

  if (i < strv.count) {
    if (index) {
      *index = i;
    }
    return true;
  } else {
    return false;
  }
}

STRVDEF bool strv_chop_by_delim(Stringv *strv, char delim,Stringv *chunk) {
        size_t i = 0;
    while (i < strv->count && strv->data[i] != delim) {
        i += 1;
    }
    Stringv result = strv_init(strv->data, i);
    if (i < strv->count) {
        strv->count -= i + 1;
        strv->data  += i + 1;
        if (chunk) {
            *chunk = result;
        }
        return true;
    }
    return false;
}

STRVDEF Stringv strv_chop_by_strv(Stringv *strv, Stringv thicc_delim) {
  Stringv window = strv_init(strv->data, thicc_delim.count);
  size_t i = 0;
  while (i + thicc_delim.count < strv->count &&
         !(strv_eq(window, thicc_delim))) {
    i++;
    window.data++;
  }

  Stringv result = strv_init(strv->data, i);

  if (i + thicc_delim.count == strv->count) {
    // include last <thicc_delim.count> characters if they aren't
    //  equal to thicc_delim
    result.count += thicc_delim.count;
  }

  // Chop!
  strv->data += i + thicc_delim.count;
  strv->count -= i + thicc_delim.count;

  return result;
}

STRVDEF bool strv_starts_with(Stringv strv, Stringv expected_prefix) {
  if (expected_prefix.count <= strv.count) {
    Stringv actual_prefix = strv_init(strv.data, expected_prefix.count);
    return strv_eq(expected_prefix, actual_prefix);
  }

  return false;
}

STRVDEF bool strv_ends_with(Stringv strv, Stringv expected_suffix) {
  if (expected_suffix.count <= strv.count) {
    Stringv actual_suffix = strv_init(
        strv.data + strv.count - expected_suffix.count, expected_suffix.count);
    return strv_eq(expected_suffix, actual_suffix);
  }

  return false;
}

STRVDEF bool strv_eq(Stringv a, Stringv b) {
  if (a.count != b.count) {
    return false;
  } else if (a.data && b.data) {
    return memcmp(a.data, b.data, a.count) == 0;
  } else if (a.count == 0) {
    return true;
  } else {
    return false;
  }
}

STRVDEF bool strv_eq_ignorecase(Stringv a, Stringv b) {
  if (a.count != b.count) {
    return false;
  }

  char x, y;
  for (size_t i = 0; i < a.count; i++) {
    x = 'A' <= a.data[i] && a.data[i] <= 'Z' ? a.data[i] + 32 : a.data[i];

    y = 'A' <= b.data[i] && b.data[i] <= 'Z' ? b.data[i] + 32 : b.data[i];

    if (x != y)
      return false;
  }
  return true;
}

STRVDEF uint64_t strv_to_u64(Stringv strv) {
  uint64_t result = 0;

  for (size_t i = 0; i < strv.count && isdigit(strv.data[i]); ++i) {
    result = result * 10 + (uint64_t)strv.data[i] - '0';
  }

  return result;
}

uint64_t strv_chop_u64(Stringv *strv) {
  uint64_t result = 0;
  while (strv->count > 0 && isdigit(*strv->data)) {
    result = result * 10 + *strv->data - '0';
    strv->count -= 1;
    strv->data += 1;
  }
  return result;
}

STRVDEF Stringv strv_chop_left_while(Stringv *strv, bool (*predicate)(char x)) {
  size_t i = 0;
  while (i < strv->count && predicate(strv->data[i])) {
    i += 1;
  }
  return strv_chop_left(strv, i);
}

STRVDEF Stringv strv_take_left_while(Stringv strv, bool (*predicate)(char x)) {
  size_t i = 0;
  while (i < strv.count && predicate(strv.data[i])) {
    i += 1;
  }
  return strv_init(strv.data, i);
}
STRVDEF int strv_cpy(Stringv *dest, const Stringv *src) {
  if (dest == NULL || src == NULL || src->data == NULL) {
    return -1; // Invalid arguments
  }

  dest->count = src->count;
  dest->data = (const char *)malloc(dest->count + 1);

  if (dest->data != NULL) {
    memcpy((void *)dest->data, src->data, dest->count);
    ((char *)dest->data)[dest->count] = '\0';
    return 0;
  } else {
    dest->count = 0;
    return -3; // Memory allocation failure
  }
}

STRVDEF int strv_cat(Stringv *dest, const Stringv *src) {
  if (dest == NULL || src == NULL || src->data == NULL) {
    return -1; // Invalid arguments
  }

  size_t new_size = dest->count + src->count;

  if (new_size + 1 > SIZE_MAX || dest->count > SIZE_MAX - src->count) {
    return -2; // Buffer overflow
  }

  char *new_data = (char *)malloc(new_size + 1);

  if (new_data != NULL) {
    memcpy(new_data, dest->data, dest->count);
    memcpy(new_data + dest->count, src->data, src->count);
    new_data[new_size] = '\0';

    free((void *)dest->data);
    dest->count = new_size;
    dest->data = new_data;

    return 0;
  } else {
    dest->count = 0;
    return -3; // Memory allocation failure
  }
}

STRVDEF Stringv strv_dup(const Stringv *strv) {
  if (strv == NULL || strv->data == NULL) {
    // Rückgabe einer leeren Stringv für ungültige Argumente
    Stringv empty_strv = {0, NULL};
    return empty_strv;
  }

  size_t length = strlen(strv->data);
  char *duplicate = (char *)malloc(length + 1);

  if (duplicate != NULL) {
    memcpy(duplicate, strv->data, length);
    duplicate[length] = '\0';
  }

  Stringv result = {length, duplicate};
  return result;
}
STRVDEF void mv_strv(Stringv *dest, Stringv *src) {
  strv_cpy(dest, src);
  free(src);
}
STRVDEF char at_strv(Stringv *strv, size_t index) {
  if (index > strv->count) {
  }
  return strv->data[index];
}
#endif // STRV_IMP
