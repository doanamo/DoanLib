#include "dn/shared.h"
#include "dn/application.h"
#include "dn/memory.h"

int main(int argc, char* argv[]) {
  DN_UNUSED(argc);
  DN_UNUSED(argv);

  DN_LOG_INFO("System page size: %llu", DnMemVirtual_GetPageSize());
  DN_LOG_INFO("Default alignment: %llu", DN_DEFAULT_ALIGNMENT);

  return DnApp_Run();
}
