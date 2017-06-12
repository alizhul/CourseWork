/**
 * @file       error_handler.h
 * @author     Alina Zhulanova
 * @date       26 May 2017
 * @brief      Function prototypes for errorhandking
 *
 * @copyright  Copyright (c) 2017, Alina Zhulanova
 * @license    This file is released under the GNU Public license
 * @bug        No known bugs.
 */

#ifndef ERROR_HANDLER_
#define ERROR_HANDLER_

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

#define ERROR_INT -1
#define ERROR_PTR NULL

/**
 * Macros to printf to error strem.
 */
#define eprintf(...)                                                           \
      fprintf(stderr, __VA_ARGS__);

/**
 * Mactos to goto error.
 */
#define ERROR_GOTO(val)  goto _err;


/**
 * Macros to print errno.
 */
#define ERROR_MSG()                                                            \
      ({                                                                       \
           eprintf("Error type: %s\n", strerror(errno));                       \
      })


/**
 * Macros to return error value.
 */
#define ERROR_RETURN(err_val)                                                  \
      ({                                                                       \
        return err_val;                                                        \
      })


#endif /* ERROR_HANDLER_ */
