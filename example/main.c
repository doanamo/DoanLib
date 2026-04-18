#include "dn/shared.h"
#include "dn/application.h"

int main(int argc, char* argv[]) {
  DN_UNUSED(argc);
  DN_UNUSED(argv);

  DnAppConfig config = {
    .memory = {
      .tempReservedSize = DN_MEM_GB(1),
    },
    .system = {
      .windowTitle = "DoanLib Example",
      .windowWidth = 1024,
      .windowHeight = 768,
    }
  };

  return DnApp_Run(&config);
}
