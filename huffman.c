#include "huffman.h"

static int32_t write_huff_codes(huff_code *hnct[], buffer_t *buff_in, buffer_t *buff_out) {
  uint8_t *pbuff_in;
  while ((pbuff_in = BUFFER_READ(buff_in)) != NULL) {
    uint64_t i;
    for (i = 0; i < buff_in->buffer_size; i++) {
      BUFFER_APPEND_HUFF_CODE(buff_out, hnct[pbuff_in[i]]);
    }
  }
  return 0;
_err:
  ERROR_MSG();
  ERROR_RETURN(-1);
}

static int32_t read_huff_codes(huff_node *tree, buffer_t *buff_in, buffer_t *buff_out, uint64_t file_size) {
  uint8_t decoded_char;
  uint32_t j = file_size;
  while (j--) {
    read_huff_code(tree, buff_in, &decoded_char);
    BUFFER_APPEND_CHAR(buff_out, decoded_char);

  }
  buff_out++;
  return 0;
_err:
  ERROR_MSG();
  ERROR_RETURN(-1);
}

int32_t huffman_encode_file(const char *path_in, const char *path_out) {
  buffer_t *input_buff;
  buffer_t *output_buff;
  huff_node *hnt[MAX_SYMBOLS] = {NULL};
  huff_code *hnc[MAX_SYMBOLS] = {NULL};

  input_buff = buffer_init(path_in, BUFFER_READ_MODE, BUFF_MAX_SIZE);
  output_buff = buffer_init(path_out, BUFFER_WRITE_MODE, BUFF_MAX_SIZE);


  BUFFER_SKIP_EOF(output_buff);

  huff_nodes_init(hnt);

  uint64_t file_size = clalculate_symbol_frequancy(hnt, input_buff);

  construct_tree(hnt);

  BUFFER_REWIND(input_buff);

  write_tree(hnt[0], output_buff, hnc);

  write_huff_codes(hnc, input_buff, output_buff);

  BUFFER_WRITE_EOF(output_buff, file_size);

  buffer_destroy(input_buff);
  buffer_destroy(output_buff);
  return 0;

_err:
  ERROR_RETURN(-1);
}




int32_t huffman_decode_file(const char *path_in, const char *path_out) {
  buffer_t *input_buff;
  buffer_t *output_buff;
  huff_node *tree = NULL;

  input_buff = buffer_init(path_in, BUFFER_READ_MODE, BUFF_MAX_SIZE);
  output_buff = buffer_init(path_out, BUFFER_WRITE_MODE, BUFF_MAX_SIZE);

  uint64_t file_size = BUFFER_READ_EOF(input_buff);

  BUFFER_READ(input_buff);

  BUFFER_BIT_SET_POSITION(input_buff, 8);

  read_tree(&tree, input_buff);


  read_huff_codes(tree, input_buff, output_buff, file_size);

  BUFFER_WRITE_END(output_buff);


  buffer_destroy(input_buff);
  buffer_destroy(output_buff);
  return 0;
_err:
  ERROR_MSG();
  ERROR_RETURN(-1);
}
