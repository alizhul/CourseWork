/**
 * @file       eof.h
 * @author     Alina Zhulanova
 * @date       26 May 2017
 * @brief      Macroses for write eof of file and skip them
 *
 * @copyright  Copyright (c) 2017, Alina Zhulanova
 * @license    This file is released under the GNU Public license
 * @bug        No known bugs.
 */

#ifndef EOF_H_
#define EOF_H_

#include "buffer.h"

/**
 * Macros to skip 1 write chunk to write metainfo later
 */
#define BUFFER_SKIP_EOF(buff)                                                  \
      ({                                                                       \
        BUFFER_NEXT_W_CHUNK(buff);                                             \
      })

/**
 * Macros to writing end of file and metainfo
 */
#define BUFFER_WRITE_EOF(buff, file_size)                                      \
      ({                                                                       \
        buff->buffer64[buff->buffer_position] <<=                              \
                                UINT64_BIT - buff->bit_position;               \
        buff->buffer64[buff->buffer_position] =                                \
                                htobe64(buff->buffer64[buff->buffer_position]);\
        buff->buffer_position+=2;                                              \
        BUFFER_WRITE(buff);                                                    \
        BUFFER_REWIND(buff);                                                   \
        WRITE(&file_size, sizeof(file_size), 1, buff->file);                   \
      })

/**
 * Macros to read eof metainfo
 */
#define BUFFER_READ_EOF(buff)                                                  \
      ({                                                                       \
        uint64_t file_size;                                                    \
        READ(&file_size, sizeof(file_size), 1, buff->file);                    \
        file_size;                                                             \
      })


#endif /* EOF_H_ */
