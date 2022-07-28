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


# 2022-7-26
1. 查找音乐素材，都是收费的，而且是双声道数据，需要专门的程序转换格式和播放时间以符合实际需要。
2. 编写数据转换程序
3. 程序使用的所有声音文件都是单声道的？且最大长度不超过64K bytes
4. 为了省事，生成的文件需要手动重新修改数据长度！！！
   为 0xD0,0xFF,0x00
5. 

## 简单梳理一下WAV的文件格式

![](./markdownpic/wav-head.png)
### RIFF Chunk(0x00)   文件标识块

名称|偏移地址|字节数|端序|内容
----|-------|------|----|---
ID|0x00|4|大端|RIFF (0x52494646)
Size|0x04|4|小端|fileSize - 8
Type|0x08|4|大端|WAVE(0x57415645)

### Format Chunk(0xc)   文件属性块
|名称 |偏移地址|字节数|端序|内容|
|-----|-------|-----|-----|---|
|ID|0x00|4Byte|大端|'fmt '(0x666D7420)|
|Size|0x04|4Byte|小端|16|
|AudioFormat|0x08|2Byte|小端|音频格式|
|NumChannels|0x0A|2Byte|小端|声道数|
|SampleRate|0x0C|4Byte|小端|采样率|
|ByteRate|0x10|4Byte|小端|每秒数据字节数|
|BlockAlign|0x14|2Byte|小端|数据块对齐|
|BitsPerSample|0x16|2Byte|小端|采样位数|


### Data Chunk(0x24)文件数据块

该区块是真正保存wav数据的地方，以'data'作为该Chunk的标示。

名称|字节|端序|
----|---|----|----
ID|4字节|大端|以'data'为标识
Size|4字节|小端|表示音频数据的长度
Data|可变|小端|音频数据


##### 后面需要做的工作

# 2022-7-27

1. SDRAM 移植完成
2. 修正SDRAM读写函数为 16bits读写缓冲区
<font color=red>I. SAI 初始化需要测试一下！！！</font>
<font color=green>II. SDRAM 初始化</font>
III. SDRAM 数据解析！！


### 不要OS！！！
1. <font color=red>需要重新规划系统</font>

# 2022-7-28
----


