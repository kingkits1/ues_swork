
static uint8_t level; // TODO: 需要变更成真正的数据

uint8_t check_target_load(void)
{
    return 0;
}

// 扫描超声频率，找到合适的频率
void us_scan_freq(void)
{
}

// 停止超声操作！！
void us_stop_operation(void)
{
	// 关闭US输出
	// 关闭声音
	// 恢复到US缺省状态
}


// US控制分析
void us_active_manager(void)
{
    uint8_t load;
	// 处于激发状态时，
    while(true)
    {
	    // FPGA data
	    while(fpga_read_data()==1);//如果没能读到数据则无法调整;
	    // FPGA 数据处理， 激发状态
	    // 激发状态检测
	    check_key_actions();
		// 如果处于未激发状态则退出当前状态
		
	    // 依照激发状态做处理
	    if(is_us_max_actived()) // 高能量激发 (切割)
	    {
	        // 高能量激发下还有不同的能量级别！！！！
	        // 检测负载状态
	        load=check_target_load();
	        // 如果是重载
	        switch(load)
	        {
	            case 2: //重载
	            // 处理重载数据
	            break;
	            
	            case 1: // 中载
	            // 处理中载数据
	            break;
	            
	            default:
	            case 0: // 轻载
	            // 处理轻载数据
	            break;            
	        }        
	    }
	    else // 凝血
	    {
	        // 检测负载状态
	        load=check_target_load(); // 这个有可能跟切割不同！！
	        switch(level)
	        {
	            default:
	            case 1:
	                if(load==2)// 重载
	                {}
	                else // 轻载
	                {}
	                break;
	            case 2:
	                if(load==2)// 重载
	                {}
	                else // 轻载
	                {}
	                break;
	            case 3:
	                if(load==2)// 重载
	                {}
	                else // 轻载
	                {}
	                break;
	            case 4:
	                if(load==2)// 重载
	                {}
	                else // 轻载
	                {}
	                break;
	            case 5:
	                if(load==2)// 重载
	                {}
	                else // 轻载
	                {}
	                break;
	        }
	    }
    }
}
