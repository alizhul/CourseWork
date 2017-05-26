#include "huff_nodes.h"

static huff_node* new_leaf_huff_node(unsigned char ch) {
  huff_node *node = CALLOC(1, sizeof(*node));
  node->symbol = ch;
  node->frequency = 0;
  node->is_leaf = true;
  return node;
_err:
  ERROR_MSG();
  ERROR_RETURN(NULL);
}

static huff_node* new_nonleaf_huff_node(huff_node *p1, huff_node *p2) {
  huff_node *node = CALLOC(1, sizeof(*node));
  node->left = p1;
  node->right = p2;
  node->is_leaf = false;
  node->frequency = p1->frequency + p2->frequency;
  return node;
_err:
  ERROR_MSG();
  ERROR_RETURN(NULL);
}

static huff_node* new_null_huff_node() {
  huff_node *node = CALLOC(1, sizeof(*node));
  return node;
_err:
  ERROR_MSG();
  ERROR_RETURN(NULL);
}

static huff_node* set_symbol_huff_node(huff_node *hn, unsigned char ch) {
  hn->symbol = ch;
  hn->is_leaf = true;
  return hn;
}

uint32_t huff_nodes_init(huff_node *hnf[]) {
  size_t i;
  for (i = 0; i < MAX_SYMBOLS; i++) {
    hnf[i] = new_leaf_huff_node(i);
  }
  return 0;
}

int64_t clalculate_symbol_frequancy(huff_node *hnf[], buffer_t *buff) {
  uint64_t file_size = 0;
  size_t i;
  uint8_t *ch;
  while (NULL != BUFFER_READ(buff)) {
    ch = buff->buffer;
    file_size += buff->buffer_size;
    for (i = buff->buffer_size; --i; ch++) {
      hnf[*ch]->frequency++;
    }
  }
  return file_size;
_err:
  ERROR_MSG();
  ERROR_RETURN(-1);
}


int32_t cmp_huff_nodes(const void *p1, const void *p2) {
  const huff_node *hn1 = *(const huff_node**)p1;
	const huff_node *hn2 = *(const huff_node**)p2;

	if(hn1 == NULL && hn2 == NULL) {
		return 0;
	} else if(hn1 == NULL) {
		return 1;
	} else if(hn2 == NULL) {
		return -1;
  }

	return (hn2->frequency - hn1->frequency);
}

int32_t construct_tree(huff_node *hnf[]) {
  unsigned int symbols_count = 0;
  int i;
  int j;
  qsort(hnf, MAX_SYMBOLS, sizeof(*hnf), cmp_huff_nodes);

  for (i = 0; i < MAX_SYMBOLS && (hnf[i]->frequency != 0); i++, symbols_count++) {
    continue;
  }

  for (i = symbols_count - 1; i > 0; --i) {
  	huff_node *tmp_node = new_nonleaf_huff_node(hnf[i-1], hnf[i]);
  	for (j = i - 1; j > 0; --j) {
  		if (tmp_node->frequency <= hnf[j-1]->frequency) {
  			break;
  		}
  	}
  	memmove(&hnf[j+1], &hnf[j], sizeof(*hnf) * (i - 1 - j));
  	hnf[j] = tmp_node;
  }
  return symbols_count;
}


int32_t write_tree(huff_node *hn, buffer_t *buff_out, huff_code **hnct) {
  static huff_code code_bst = { 0, 0 };

  if (!hn) {
    BUFFER_BIT_APPEND_0(buff_out);
  } else if (hn->is_leaf) {
    BUFFER_BIT_APPEND_0(buff_out);
    BUFFER_BIT_APPEND_0(buff_out);

    BUFFER_APPEND_CHAR(buff_out, hn->symbol);
    hnct[hn->symbol] = new_huff_code(code_bst);
  } else {
    HUFF_CODE_APPEND_ZERO(code_bst);

    BUFFER_BIT_APPEND_1(buff_out);
    write_tree(hn->left, buff_out, hnct);

    HUFF_CODE_LAS_BIT_TO_ONE(code_bst);

    BUFFER_BIT_APPEND_1(buff_out);
    write_tree(hn->right, buff_out, hnct);

    HUFF_CODE_RM_LAS_BIT(code_bst);
  }
  return 0;
_err:
  ERROR_MSG();
  ERROR_RETURN(-1);
}



int32_t read_tree(huff_node **hn, buffer_t *buff_in) {
  *hn = new_null_huff_node();

  if (BUFFER_BIT_NEXT_POSITION(buff_in)) {
    read_tree(&(*hn)->left, buff_in);
  }
  if (BUFFER_BIT_NEXT_POSITION(buff_in)) {
    read_tree(&(*hn)->right, buff_in);
    return 0;
  }
  uint8_t added_char = BUFFER_GET_CHAR(buff_in);
  *hn = set_symbol_huff_node(*hn, added_char);

  return 0;
_err:
  ERROR_MSG();
  ERROR_RETURN(-1);
}


int32_t read_huff_code(huff_node *tree, buffer_t *buff_in, uint8_t *ch) {
  huff_node *temp = tree;
  while (1) {
    if (!temp->is_leaf) {
        int32_t bit = BUFFER_BIT_NEXT_POSITION(buff_in);
        if (!bit) {
          temp = temp->left;
        } else {
          temp = temp->right;
        }
    } else  {
      *ch = temp->symbol;
      return 0;
    }
  }
  return 0;
_err:
  ERROR_MSG();
  ERROR_RETURN(-1);
}
