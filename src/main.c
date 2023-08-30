#include "device/kit/driver.h"
#include "device/props/device.h"
#include "platform.h"
#include <stdio.h>

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
  printf("%5s: %s:%d %s() - %s\n", is_error ? "Error" : "", file, line,
         function, msg);
}

typedef struct Driver *(*acquire_driver_init_v0_t)(
    void (*reporter)(int is_error, const char *file, int line,
                     const char *function, const char *msg));

int main(int argc, char *argv[]) {
  struct lib lib = {};
  CHECK(lib_open(&lib, "C:\\Users\\nclack\\src\\acquire\\acquire-driver-"
                       "pvcam\\target\\debug\\acquire_driver_pvcam.dll"));
  acquire_driver_init_v0_t acquire_driver_init_v0 = 0;
  CHECK(acquire_driver_init_v0 = lib_load(&lib, "acquire_driver_init_v0"));
  struct Driver *driver = 0;
  CHECK(driver = acquire_driver_init_v0(reporter));
  CHECK(driver->shutdown);
  CHECK(driver->device_count);

  LOG("device count: %d\n", driver->device_count(driver));
  CHECK(driver->shutdown(driver) == Device_Ok);

  return 0;
Error:
  return 1;
}
