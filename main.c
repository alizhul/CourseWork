#include <stdio.h>
#include <getopt.h>
#include "huffman.h"

static void print_usage();

int main(int argc, char *const *argv) {
  int opt;

  if (argc != 4) {
    print_usage();
    return 0;
  }

  while ((opt = getopt(argc, argv, "cx")) != -1) {
    switch (opt) {
      case 'c':
        huffman_encode_file(argv[1], argv[3]);
        break;
      case 'x':
        huffman_decode_file(argv[1], argv[3]);
        break;
      default:
        print_usage();
        break;
    }
  }

  return 0;
}

static void print_usage() {
  puts("Usage: huff ifile [-c | -x] ofile\n"
      "ifile - input file\n"
      "ofile - output file\n"
      "-c - compress ifile to ofile\n"
      "-x - decompress ifile to ofile\n");
}
