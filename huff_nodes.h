/**
 * @file       huff_nodes.h
 * @author     Alina Zhulanova
 * @date       26 May 2017
 * @brief      Function prototypes for working with huffman nodes.
 *
 * @copyright  Copyright (c) 2017, Alina Zhulanova
 * @license    This file is released under the GNU Public license
 * @bug        No known bugs.
 */


#ifndef HUFF_NODE_H_
#define HUFF_NODE_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "error_handler.h"
#include "huff_codes.h"
#include "buffer.h"


#define MAX_SYMBOLS 256 /// Count of maxsimumx

 /**
  * @struct huff_node
  * @brief This struct used for construct tree and store frequency
  */
typedef struct huff_node {
  bool is_leaf;                       /**< Check for leaf */
  uint64_t frequency;                 /**< Frequency of symbol */
  union {
    uint8_t symbol;                   /**< Symbol */
    struct  {
      struct huff_node *left;         /**< Left part of node (0) */
      struct huff_node *right;        /**< Right part of node (1) */
    };
  };
} huff_node;



uint32_t huff_nodes_init(huff_node *hnf[]);

/**
 * @brief Calculating frequency of symbols
 * @details Read file to EOF and calculation frequency of each symbol and store and in hnt
 *
 * @param hnf store frequency of symbols
 * @param buff buffer for to read symbols
 *
 * @return 0 on success and 1 if faild
 */
int64_t clalculate_symbol_frequancy(huff_node *hnf[], buffer_t *buff);

/**
 * @brief Compare to symbols by frequency
 *
 * @param p1 first huff_node
 * @param p2 second huff_node
 *
 * @return 0 if equal, >0 if p2 > p1, <0 if p1 > p2
 */
int32_t cmp_huff_nodes(const void *p1, const void *p2);

/**
 * @brief Create tree from huff
 * @details Sort frequency table and construct tree by group them of 2 elements
 *
 * @param hnf Array of frequency each element
 * @return Symbol count
 * @return 0 on success and 1 if faild
 */
int32_t construct_tree(huff_node *hnf[]);

/**
 * @brief Write tree to output buffer
 *
 * @param hn Element of tree
 * @param buff_out File buffer to write tree
 * @param hnct Huffman code table
 * @return 0 on success and 1 if faild 
 */
int32_t write_tree(huff_node *hn, buffer_t *buff_out, huff_code **hnct);

/**
 * @brief Read tree from input buffer
 *
 * @param hn Element of tree
 * @param buff_in File buffer to read tree
 * @return 0 on success and 1 if faild
 */
int32_t read_tree(huff_node **hn, buffer_t *buff_in);

/**
 * @brief Read character from input buffer
 * @details Going by tree and 
 * 
 * @param tree Huffman codes tree
 * @param buff_in Input buffer
 * @param ch Char that will store read charater
 * @return 0 on success and 1 if faild
 */
int32_t read_huff_code(huff_node *tree, buffer_t *buff_in, uint8_t *ch);

#endif /* HUFF_NODE_H_ */
