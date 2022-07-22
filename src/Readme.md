#2022-7-22
------------
1. wavplay.c/h 中 增加一个变量(current_data_ptr)在结构体 wavx中
2. 考虑到按键声音是短播放时间，而切割提示音为长播放时间（可能一个数据段循环播放），所以还需要增加如下变量：
  a. 增加一个判定是否为循环播放的变量（bool loop_sound_flag;）在结构体 wavx中。
  b. 增加一个声音数据是否有效的变量 （bool actived_flag;）
3. 变更wav_decode_init的调用参数 uint8_t wav_decode_init(ENUM_WAVE_TYPES wav_type)
4. 完成wav_decode_init() 函数
5. 尝试修正：uint32_t wav_buffill(uint8_t *buf,uint16_t size,uint8_t bits) 函数
6. 在修正上面函数时，要考虑循环数据的衔接问题！还有按键声音等短时间提示音的终止问题
7. 