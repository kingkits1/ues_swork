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
7. 24bit时，实际存储上每3个字节为一个数据，写到缓冲区时，需要写入4字节（最高字节的数据是无效的）而且需要处理数据不够一个缓冲区时的问题！！！这时要考虑是否为循环数据！！！！


#2022-7-25
1. 声音播放有两个停止模式：
   - 连续播放时，只有主动请求停止播放才可以停止
   - 非连续播放时，切换声音会停止当前的声音播放，并换到其他声音
   - 非连续播放时，当声音播放到末尾时，读取数据的函数会把数据状态切换到失效，再次读取数据将停止播放（DMA传送）
2. uint16_t wav_buffill(uint8_t *buf,uint16_t size,uint8_t bits);已经完成
   24bit时，实际存储上每3个字节为一个数据，拷贝到缓冲区时，需要写入4字节（其中最高字节的数据是无效的，我们默认填0），这里是3字节转4字节！而且需要处理数据不够一个缓冲区时的问题！这时要考虑是否为循环数据！
3. 开始修正 void wav_sai_dma_tx_callback(void) DMA回调函数！
   - 需要修正wav_buffill，当声音数据读取到结束时，非连续循环播放的读取操作需要设置结束标记！供DMA回调函数使用！！！
   - 回调函数负责停止非连续播放的声音
   - 增加 wavctrl.stop_buf_flag
   - 增加 结束标记的定义
```
  typedef enum 
  {
      EM_WAV_NO_END,        // 无停止数据
      EM_WAV_TWO_BUFFER_END0,// 双缓存停止在BUF0
      EM_WAV_TWO_BUFFER_END1,// 双缓存停止在BUF1
      EM_WAV_SINGLE_END
  }ENUM_WAV_STOP_BUFFER_DEFINES;
```
4. 开始修正 uint8_t wav_play_song()
5. 需要编写测试函数
6. 需要准备音乐数据---*.wav 转换成 *.c

##### 后面需要做的工作
I. SAI 初始化需要测试一下！！！
II. SDRAM 初始化
