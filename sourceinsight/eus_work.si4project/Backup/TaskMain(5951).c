#include "main.h"
#include "cmsis_os.h"
#include "../global.h"

__weak void init_global_data(void){}
__weak void init_hardware_module(void){}
void StartTaskMain(void *argument)
{
  init_global_data();
  init_hardware_module();
  for(;;)
  {
    osDelay(1);
  }
}
