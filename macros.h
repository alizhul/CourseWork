/**
 * @file       macros.h
 * @author     Alina Zhulanova
 * @date       26 May 2017
 * @brief      Macros for debug.
 *
 * @copyright  Copyright (c) 2017, Alina Zhulanova
 * @license    This file is released under the GNU Public license
 * @bug        No known bugs.
 */

#ifndef MACROS_H_
#define MACROS_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>
#include "error_handler.h"

/* Printf macroses */

#define debug_printf(...);

/* Errors catcher for stdlib */

/**
 * Macros to handle error for calling calloc
 */
#define CALLOC(nmemb, size)                                                    \
      ({                                                                       \
        void *tmp_ptr = calloc(nmemb, size);                                   \
        if (!tmp_ptr) {                                                        \
          eprintf("Cannot allocate memory\n");                                 \
          ERROR_GOTO();                                                        \
        }                                                                      \
        tmp_ptr;                                                               \
      })

/**
 * Macros to handle error for calling malloc
 */
#define MALLOC(size)                                                           \
      ({                                                                       \
        void *tmp_ptr = malloc(size);                                          \
        if (!tmp_ptr) {                                                        \
          eprintf("Cannot allocate memory\n");                                 \
          ERROR_GOTO();                                                        \
        }                                                                      \
        tmp_ptr;                                                               \
      })

/**
 * Macros to correct free pointer
 */
#define FREE(ptr)                                                              \
      ({                                                                       \
        if (ptr) {                                                             \
          free(ptr);                                                           \
          ptr = NULL;                                                          \
        }                                                                      \
      })


/* Errors catcher for stdio */

/**
 * Macros to handle error for calling open
 */
#define OPEN(path, mode)                                                       \
      ({                                                                       \
        int tmp_file_desc = open(path, mode, S_IRUSR | S_IWUSR |               \
                                             S_IRGRP | S_IROTH);               \
        if (!tmp_file_desc) {                                                  \
          eprintf("Cant open file %s\n", path);                                \
          ERROR_GOTO();                                                        \
        }                                                                      \
        tmp_file_desc;                                                         \
      })

/**
 * Macros to handle error for calling read
 */
#define READ(buf, size, nmemb, fildes)                                         \
      ({                                                                       \
        ssize_t readed_bytes = read(fildes, buf, size * nmemb);                \
        if (readed_bytes < 0) {                                                \
          eprintf("Cannot read from file");                                    \
          ERROR_GOTO();                                                        \
        }                                                                      \
        readed_bytes;                                                          \
      })

/**
 * Macros to handle error for calling write
 */
#define WRITE(buf, size, nmemb, fildes)                                        \
      ({                                                                       \
        ssize_t writed_bytes = write(fildes, buf, size * nmemb);               \
        if (writed_bytes < 0 ) {                                               \
          eprintf("Cannot write to file\n");                                   \
          ERROR_GOTO();                                                        \
        }                                                                      \
        writed_bytes;                                                          \
      })

/**
 * Macros to handle error for calling close
 */
#define CLOSE(fildes)                                                          \
      ({                                                                       \
        if (fildes) {                                                          \
          if (close(fildes) < 0) {                                             \
            eprintf("Error while closing fildes");                             \
            ERROR_GOTO();                                                      \
          }                                                                    \
          fildes = 0;                                                          \
        }                                                                      \
      })

/* DEBUG_MODE */
#ifdef DEBUG_MODE

#undef debug_printf

/**
 * Macros to print debug info
 */
#define debug_printf(...)                                                      \
      fprintf(stderr, __VA_ARGS__);

/**
 * Macros to check for valid pointer
 */
#define CHKPTR(ptr)                                                            \
      ({                                                                       \
        if (ptr == NULL) {                                                     \
          eprnitf("%s:%u  func  %s", __FILE__, __LINE__, __func__);            \
          ERROR_GOTO();                                                        \
        }                                                                      \
      })

#endif /* DEBUG_MODE */

#endif /* MACROS_H_ */
