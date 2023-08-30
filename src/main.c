#include "platform.h"
#include <printf.h>

#define L(lvl, file, line, func, ...) (printf(__VA_ARGS__))
#define LOG(...) L(0, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOGE(...) L(1, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define EXPECT(e, ...)                                                         \
  do {                                                                         \
    if (!(e)) {                                                                \
      LOGE(__VA_ARGS__);                                                       \
      goto Error;                                                              \
    }                                                                          \
  } while (0)
#define CHECK(e) EXPECT(e, "Expression evaluated as false:\n\t%s", #e)

void reporter(int is_error, const char *file, int line, const char *function,
              const char *msg) {
  printf("%5s: %s:%d - %s", is_error ? "Error" : "", file, line, function, msg);
}

typedef struct Driver *(*acquire_driver_init_v0)(
    void (*reporter)(int is_error, const char *file, int line,
                     const char *function, const char *msg));

int main(int argc, char *argv[]) {
  struct lib lib = {};
  CHECK(lib_open_by_name(&lib, "acquire-driver-pvcam"));
  lib_load(&lib, "acquire_driver_init_v0");
Error:
  return 1;
}
