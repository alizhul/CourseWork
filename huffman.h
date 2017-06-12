/**
 * @file       huffman.h
 * @author     Alina Zhulanova
 * @date       26 May 2017
 * @brief      Function prototypes for encoding and decoding file. 
 *
 * @copyright  Copyright (c) 2017, Alina Zhulanova
 * @license    This file is released under the GNU Public license
 * @bug        No known bugs.
 */


#ifndef HUFFFMAN_H_
#define HUFFFMAN_H_

#include <endian.h>
#include "huff_nodes.h"
#include "error_handler.h"
#include "eof.h"
#include "buffer.h"


 /**
  * @brief Encoding file that is on path_in and writing to path_out
  * @details Read file from path_in and calculating frequncy of each 
  * character and stre that in frequancy table. Then construct tree from
  * frequancy table. After that writing tree in output file and create 
  * huffman codes table. Read input file from start and encode each cahr with 
  * huffman codes talble.  
  * 
  * @param path_in Path to file for encoding
  * @param path_out Path to file for save encoding
  *
  * @return 0 if success or 1 if failed
  */
int32_t huffman_encode_file(const char *path_in, const char *path_out);

/**
  * @brief Decoding file that is on path_in and writing to path_out
  * @details Read huffman tree from input file and decode input file with that tree
  *
  * @param path_in Path to file for decoding
  * @param path_out Path to file for save decoding
  *
  * @return 0 if success or 1 if failed
  */
int32_t huffman_decode_file(const char *path_in, const char *path_out);


#endif /* HUFFFMAN_H_ */
