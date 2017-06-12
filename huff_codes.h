/**
 * @file       huff_codes.h
 * @author     Alina Zhulanova
 * @date       26 May 2017
 * @brief      Function prototypes manage huffman codes.
 *
 * @copyright  Copyright (c) 2017, Alina Zhulanova
 * @license    This file is released under the GNU Public license
 * @bug        No known bugs.
 */

#ifndef HUFF_CODE_H_
#define HUFF_CODE_H_

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "error_handler.h"
#include "buffer.h"

#define MAX_SYMBOLS 256


/**
 * @struct huff_code
 * @brief This struct store huffman code
 */
typedef struct huff_code {
  uint64_t bits;              /**< 64 bits to store huffman code */
  uint32_t numbits;           /**<  Size of huffman code*/
} huff_code;

/**
 * @brief Create new huff code
 * @details Allocates memory for new struct and init
 * 
 * @param  code store code with huffman code 
 * @return Pointer to new huff_code struct with code 
 */
huff_code *new_huff_code(huff_code code);


/**
 * Macros to append bit 0 to huff_code.
 */
#define HUFF_CODE_APPEND_ZERO(bit_st)                                          \
      ({                                                                       \
        bit_st.bits <<= 1;                                                     \
        bit_st.numbits++;                                                      \
      })

/**
 * Macros to remove last bit from huff_code.
 */
#define HUFF_CODE_RM_LAS_BIT(bit_st)                                           \
      ({                                                                       \
        bit_st.numbits--;                                                      \
        bit_st.bits >>= 1;                                                     \
      })


/**
 * Macros to flip last bit to 1 huff_code.
 */
#define HUFF_CODE_LAS_BIT_TO_ONE(bit_st)                                       \
      ({                                                                       \
        bit_st.bits |= 0x1LL;                                                  \
      })


#endif/* HUFF_CODE_H_ */
