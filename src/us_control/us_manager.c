
static uint8_t level; // TODO: ��Ҫ���������������

uint8_t check_target_load(void)
{
    return 0;
}

// US���Ʒ���
void us_manager(void)
{
    uint8_t load;
    
    // FPGA data
    while(fpga_read_data()==1);//���û�ܶ����������޷�����;
    
    // ����״̬���
    check_key_actions();
    // ���ռ���״̬������
    if(is_us_max_actived()) // ���������� (�и�)
    {
        // �����������»��в�ͬ���������𣡣�����
        // ��⸺��״̬
        load=check_target_load();
        // ���������
        switch(load)
        {
            case 2: //����
            // ������������
            break;
            
            case 1: // ����
            // ������������
            break;
            
            default:
            case 0: // ����
            // ������������
            break;            
        }        
    }
    else // ��Ѫ
    {
        // ��⸺��״̬
        load=check_target_load(); // ����п��ܸ��иͬ����
        switch(level)
        {
            default:
            case 1:
                if(load==2)// ����
                {}
                else // ����
                {}
                break;
            case 2:
                if(load==2)// ����
                {}
                else // ����
                {}
                break;
            case 3:
                if(load==2)// ����
                {}
                else // ����
                {}
                break;
            case 4:
                if(load==2)// ����
                {}
                else // ����
                {}
                break;
            case 5:
                if(load==2)// ����
                {}
                else // ����
                {}
                break;
        }
    }
}
