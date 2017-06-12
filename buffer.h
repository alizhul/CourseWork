/**
 * @file       buffer.h
 * @author     Alina Zhulanova
 * @date       26 May 2017
 * @brief      Function prototypes for working with buffer.
 *
 * @copyright  Copyright (c) 2017, Alina Zhulanova
 * @license    This file is released under the GNU Public license
 * @bug        No known bugs.
 */


#ifndef FILE_IO_
#define FILE_IO_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <endian.h>
#include <fcntl.h>
#include "error_handler.h"
#include "macros.h"

typedef enum { BUFFER_READ_MODE, BUFFER_WRITE_MODE } buffer_mode_t;


#define UINT64_BIT (64)
#define BUFF_MAX_SIZE (1024*1024*200)
#define CHUNK_SIZE CHAR_BIT


 /**
  * @struct buffer_t
  * @brief This struct used for file reading
  */
typedef struct buffer_t {
  union {
    uint8_t  *buffer;               /**< Read chunk that store part of file */
    uint64_t *buffer64;             /**< Write chunk that store part of file */
  };
  uint64_t buffer_position;         /**< Current chunk in buffer */
  uint64_t buffer_size;             /**< Count of chunks */
  uint32_t bit_position;            /**< Bit position in current chunk */
  int      file;                    /**< File from wich buffer takes data */
} buffer_t;

 /**
  * @brief Buffer initilization
  * @details Choose mode for buffer. If in read mode then file open with "r" flag else if
  * in open file in "w+" mode. Allocates memory for this buffer
  *
  * @param file_path Path to file which buffer will use.
  * @param buff_mode Mode wich choose type of buffer.
  *
  * @return Pointer to buffer or NULL if failed.
  */
buffer_t* buffer_init(const char *file_path, buffer_mode_t buff_mode, uint64_t buffer_size);

/**
 * @brief Get nex block of file
 * @details Try to read next MAX_BUFF_SZIE bytes
 *
 * @param buff buffer_t
 *
 * @return pointer to buffer string on success and NULL on error or EOF
 */
uint64_t* buffer_get(buffer_t *buff);

/**
 * @brief Free buffer
 * @details [long description]
 *
 * @param buff main struct
 *
 * @return NULL
 */
buffer_t* buffer_destroy(buffer_t *buff);


/**
 * Macros for rewind buffer file.
 */
#define BUFFER_REWIND(buff)                                                    \
      ({                                                                       \
        lseek(buff->file, 0, SEEK_SET);                                        \
      })

/**
 * Macros to write buffer to file from buffer start to buffer position
 * as write chunks.
 * After set buffer position and buffer bit position to 0.
 */
#define BUFFER_WRITE(buff)                                                     \
      ({                                                                       \
        WRITE(buff->buffer64, sizeof(*buff->buffer64),                         \
              buff->buffer_position, buff->file);                              \
        buff->buffer_position = 0;                                             \
        buff->bit_position = 0;                                                \
      })


/**
 * Macros to write last part of buffer as wite chunks. Write from buffer
 * start to buffer position and then write left bytes.
 */
#define BUFFER_WRITE_END(buff)                                                 \
      ({                                                                       \
        WRITE(buff->buffer64, sizeof(*buff->buffer64),                         \
              buff->buffer_position, buff->file);                              \
        WRITE(&buff->buffer64[output_buff->buffer_position],                   \
          sizeof(*buff->buffer), (buff->bit_position / CHAR_BIT),              \
           buff->file);                                                        \
      })


/**
 * Macros to read file to buffer as read chunks.
 * Set buffer position to 0 and buffer bit position to read chunk size
 * Return pointer to buffer if buffer not empty, else return NULL.
 */
#define BUFFER_READ(buff)                                                      \
      ({                                                                       \
        buff->buffer_size = READ(buff->buffer, sizeof(*buff->buffer),          \
                                 BUFF_MAX_SIZE, buff->file);                   \
        buff->buffer_position = 0;                                             \
        buff->bit_position = CHAR_BIT;                                         \
        buff->buffer_size == 0 ? NULL : buff->buffer;                          \
})


/**
 * Macros to check write buffer for overflow. If buffer full write to file.
 */
#define BUFFER_CHECK_W_OVERFLOW(buff)                                          \
      ({                                                                       \
        if (buff->buffer_position == (BUFF_MAX_SIZE/8)) {                      \
          BUFFER_WRITE(buff);                                                  \
        }                                                                      \
      })


/**
 * Mactros to check read buffer for overflow. If buffer full read next part
 */
#define BUFFER_CHECK_R_OVERFLOW(buff)                                          \
      ({                                                                       \
        if (buff->buffer_position == BUFF_MAX_SIZE) {                          \
          BUFFER_READ(buff);                                                   \
        }                                                                      \
      })


/**
 * Macros to correct move buffer position to next write chunk.
 * Convert chunk to big endian and move position to next.
 * Set buffet bit position to 0.
 * Check for buffer write overflow.
 */
#define BUFFER_NEXT_W_CHUNK(buff)                                              \
      ({                                                                       \
        buff->buffer64[buff->buffer_position] =                                \
                               htobe64(buff->buffer64[buff->buffer_position]); \
        buff->buffer_position++;                                               \
        buff->bit_position = 0;                                                \
        BUFFER_CHECK_W_OVERFLOW(buff);                                         \
        buff->buffer64[buff->buffer_position] = 0;                             \
      })


/**
 * Mactos to correct move buffe position to next read chunk.
 * Move position to next and set buffet bit position to read chunk size.
 * Check for buffer overflow.
 */
#define BUFFER_NEXT_R_CHUNK(buff)                                              \
      ({                                                                       \
        buff->buffer_position++;                                               \
        buff->bit_position = CHAR_BIT;                                         \
        BUFFER_CHECK_R_OVERFLOW(buff);                                         \
      })


/**
 * Macros to append char.
 * Wrapper to BUFFER_APPENDS_BITS.
 */
#define BUFFER_APPEND_CHAR(buff, ch)                                           \
      ({                                                                       \
        BUFFER_APPEND_BITS(buff, ch, CHAR_BIT);                                \
      })


/**
 * Macros to append huff_code.
 * Wrapper to BUFFER_APPENDS_BITS.
 */
#define BUFFER_APPEND_HUFF_CODE(buff, hc)                                      \
      ({                                                                       \
        BUFFER_APPEND_BITS(buff, hc->bits, hc->numbits);                       \
      })

/**
 * Macros to append char bits.
 */
#define BUFFER_APPEND_BITS(buff, bits, numbits)                                \
      ({                                                                       \
        uint32_t buff_bits_left = UINT64_BIT - buff->bit_position;             \
        if (buff_bits_left > numbits) {                                        \
          buff->buffer64[buff->buffer_position] <<= numbits;                   \
          buff->buffer64[buff->buffer_position] |= bits;                       \
          buff->bit_position += numbits;                                       \
        } else if (buff_bits_left < numbits){                                  \
          buff->buffer64[buff->buffer_position] <<= buff_bits_left;            \
          buff->buffer64[buff->buffer_position] |= bits >>                     \
                                                   (numbits - buff_bits_left); \
          BUFFER_NEXT_W_CHUNK(buff);                                           \
          buff->buffer64[buff->buffer_position] |= bits;                       \
          buff->bit_position = numbits - buff_bits_left;                       \
        } else {                                                               \
          buff->buffer64[buff->buffer_position] <<= numbits;                   \
          buff->buffer64[buff->buffer_position] |= bits;                       \
          BUFFER_NEXT_W_CHUNK(buff);                                           \
        }                                                                      \
      })


/**
 * Macros to get char from buffer.
 * Return char.
 */
#define BUFFER_GET_CHAR(buff)                                                  \
      ({                                                                       \
        uint8_t temp = 0;                                                      \
        uint32_t shift = buff->bit_position <= CHAR_BIT ?                      \
                                               buff->bit_position : CHAR_BIT;  \
        uint32_t mv_pos = CHAR_BIT - shift;                                    \
        temp |= buff->buffer[buff->buffer_position] << mv_pos;                 \
        BUFFER_NEXT_R_CHUNK(buff);                                             \
        temp |= buff->buffer[buff->buffer_position] >> shift;                  \
        buff->bit_position -= mv_pos;                                          \
        temp;                                                                  \
      })


/* Buffer bit operations */

/**
 * Macros to check write chunk for full.
 */
#define BUFFER_BIT_CHECK_CHUNK_FULL(buff_bit)                                  \
      ({                                                                       \
        buff_bit->bit_position == UINT64_BIT ? 1 : 0;                          \
      })


/**
 * Macros to check read chunk for empty.
 */
#define BUFFER_BIT_CHECK_EMPTY(buff_bit)                                       \
      ({                                                                       \
        buff_bit->bit_position == 0 ? 1 : 0;                                   \
      })


/**
 * Appends to buffer bit 0.
 */
#define BUFFER_BIT_APPEND_0(buff_bit)                                          \
      ({                                                                       \
        buff_bit->bit_position++;                                              \
        buff_bit->buffer64[buff_bit->buffer_position] <<= 1ULL;                \
        if (BUFFER_BIT_CHECK_CHUNK_FULL(buff_bit)) {                           \
          BUFFER_NEXT_W_CHUNK(buff_bit);                                       \
        }                                                                      \
      })

/**
 * Appends to buffer bit 1.
 */
#define BUFFER_BIT_APPEND_1(buff_bit)                                          \
      ({                                                                       \
        buff_bit->bit_position++;                                              \
        buff_bit->buffer64[buff_bit->buffer_position] <<= 1ULL;                \
        buff_bit->buffer64[buff_bit->buffer_position] |= 1ULL;                 \
        if (BUFFER_BIT_CHECK_CHUNK_FULL(buff_bit)) {                           \
          BUFFER_NEXT_W_CHUNK(buff_bit);                                       \
        }                                                                      \
      })


/**
 * Mactos to set buffer bit position.
 */
#define BUFFER_BIT_SET_POSITION(buff_bit, position)                            \
      ({                                                                       \
        buff_bit->bit_position = position;                                     \
      })

/**
 * Macros to get bit from current position.
 */
#define BUFFER_BIT_GET_POSITION(buff_bit)                                      \
      ({                                                                       \
        buff_bit->buffer[buff_bit->buffer_position]                            \
                                >> buff_bit->bit_position & 1;                 \
      })

/**
 * Macros to get bit and move to next.
 */
#define BUFFER_BIT_NEXT_POSITION(buff_bit)                                     \
      ({                                                                       \
        uint32_t bit = buff_bit->buffer[buff_bit->buffer_position]             \
                                >> (buff_bit->bit_position - 1) & 1;           \
                                buff_bit->bit_position--;                      \
        if (BUFFER_BIT_CHECK_EMPTY(buff_bit)) {                                \
          BUFFER_NEXT_R_CHUNK(buff_bit);                                       \
        }                                                                      \
        bit;                                                                   \
      })

#endif /* FILE_IO_ */
