#include "huff_codes.h"


huff_code *new_huff_code(huff_code code)
{
  huff_code *node = CALLOC(1, sizeof(*node));
  node->numbits = code.numbits;
  memcpy(&node->bits, &code.bits, 8);
  return node;
_err:
  ERROR_MSG();
  ERROR_RETURN(NULL);
}
