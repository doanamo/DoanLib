#include "dn/app.h"

#include "dn/mem/virtual.h"

int main(int argc, char* argv[]) {
  DN_UNUSED(argc);
  DN_UNUSED(argv);

  DN_LOG_INFO("System page size: %llu", DnMemVirtualPageSize());
  DN_LOG_INFO("Default alignment: %llu", DN_DEFAULT_ALIGNMENT);

  return DnAppRun();
}
