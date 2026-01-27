#include "shared.h"
#include "platform/window.h"
#include "application.h"

bool g_exit = false;
int g_exit_code = 1;

void close_callback() {
  g_exit = true;
}

int main(int argc, char* argv[]) {
  UNUSED(argc);
  UNUSED(argv);

  g_platform_window_close_callback = &close_callback;
  if (!platform_window_init("Game", 1024, 576)) {
    goto error;
  }

  if (!application_init()) {
    goto error;
  }

  LOG_INFO("Starting main loop");
  platform_window_show();

  while(!g_exit) {
    platform_window_process_messages();
    application_update(0.0f);
    application_render(1.0f);
  }

  g_exit_code = 0;

error:
  LOG_INFO("Exiting application");
  application_deinit();
  platform_window_deinit();
  return g_exit_code;
}
