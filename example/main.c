#include "dn/main.h"
#include "dn/app.h"

DN_DEFINE_MAIN_ENTRY() {
  DnAppConfig config = {
    .windowTitle = DN_STR_VIEW_LITERAL("DoanLib Example"),
    .windowWidth = 1024,
    .windowHeight = 576,
  };

  return DnApp_Run(&config);
}
