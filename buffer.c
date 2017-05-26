#include "buffer.h"


buffer_t* buffer_init(const char *file_path, buffer_mode_t buff_mode, uint64_t buffer_size) {
  int mode;
  switch (buff_mode) {
    case BUFFER_READ_MODE: mode = O_RDONLY; break;
    case BUFFER_WRITE_MODE: mode = O_CREAT| O_RDWR |O_TRUNC ; break;
    default: eprintf("Wrong arg mode\n"); mode = O_RDWR ; break;
  }
  buffer_t *buff = CALLOC(1, sizeof(*buff));
  buff->buffer = CALLOC(buffer_size, sizeof(*buff->buffer));
  buff->file = OPEN(file_path, mode);
  return buff;
_err:
  ERROR_MSG();
  ERROR_RETURN(NULL);
}


buffer_t* buffer_destroy(buffer_t *buff) {
  CLOSE(buff->file);
  FREE(buff->buffer);
  FREE(buff);
  return buff;
_err:
  ERROR_MSG();
  ERROR_RETURN(NULL);
}
