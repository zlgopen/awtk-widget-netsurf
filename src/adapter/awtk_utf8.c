#include "utils/nsoption.h"
#include "utils/utf8.h"
#include "netsurf/utf8.h"
#include "awtk_utf8.h"

static nserror utf8_to_local(const char* string, size_t len, char** result) {
  *result = strndup(string, len);
  return NSERROR_OK;
}

static nserror utf8_from_local(const char* string, size_t len, char** result) {
  *result = strndup(string, len);
  return NSERROR_OK;
}

static struct gui_utf8_table utf8_table = {
    .utf8_to_local = utf8_to_local,
    .local_to_utf8 = utf8_from_local,
};

struct gui_utf8_table* awtk_get_utf8_table(void) {
  return &utf8_table;
}
