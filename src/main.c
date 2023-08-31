#include "device/kit/driver.h"
#include "device/props/device.h"
#include "platform.h"
#include <stdio.h>

#define L(lvl, file, line, func, ...) printf(__VA_ARGS__)
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
#define OK(e) EXPECT((e) == Device_Ok, "Unexpected failure:\n\t%s", #e)
#define NOTOK(e) EXPECT((e) == Device_Err, "Unexpected success:\n\t%s", #e)

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

  // const char *lib_path = "C:\\Users\\nclack\\src\\acquire\\acquire-driver-"
  //                        "pvcam\\target\\debug\\acquire_driver_pvcam.dll";
  const char *lib_path =
      "/home/nclack/src/"
      "CZI/acquire-driver-pvcam/target/debug/libacquire_driver_pvcam.so";

  CHECK(lib_open(&lib, lib_path));
  acquire_driver_init_v0_t acquire_driver_init_v0 = 0;
  CHECK(acquire_driver_init_v0 = lib_load(&lib, "acquire_driver_init_v0"));
  struct Driver *driver = 0;
  CHECK(driver = acquire_driver_init_v0(reporter));
  CHECK(driver->shutdown);

  CHECK(driver->device_count);
  size_t n_devices = 0;
  LOG("device count: %d\n", (int)(n_devices = driver->device_count(driver)));

  struct DeviceIdentifier identifier = {0};
  for (int i = 0; i < n_devices; ++i) {
    OK(driver->describe(driver, &identifier, i));
    LOG("%3d: %20s %s", i, device_kind_as_string(identifier.kind),
        identifier.name);
  }
  NOTOK(driver->describe(driver, &identifier, n_devices));

  CHECK(driver->shutdown(driver) == Device_Ok);

  return 0;
Error:
  return 1;
}
