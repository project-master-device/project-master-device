#ifndef PMD_BYTEARRAY_H_
#define PMD_BYTEARRAY_H_

#include <stdint.h>

/**
 * @struct bytearr_t
 * @brief simple byte array
 */
typedef struct {
	uint8_t* itself;
	uint32_t len;
} bytearr_t;

#endif // PMD_BYTEARRAY_H_
