#include "main.h"
#include "cmsis_os.h"
#include "../global.h"

__weak void init_global_data(void){}
__weak void init_hardware_module(void){}
void wav_play_manager(void);
void StartTaskMain(void *argument)
{
  init_global_data();
  init_hardware_module();
  for(;;)
  {
    // Debug

    // sound
    wav_play_manager();
    
    osDelay(3);
  }
}
