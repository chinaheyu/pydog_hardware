#include "pstwo.h"
/*********************************************************     
**********************************************************/	 

// 结果
int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY;
uint8_t RedLightMode;

#define DELAY_TIME  DWT_Delay_us(5);
uint16_t Handkey;	// 按键值读取，零时存储。
uint8_t Comd[2]={0x01,0x42};	//开始命令。请求数据
uint8_t Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //数据存储数组
uint16_t MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
	};	//按键值与按键明


void PS2_Init(void)
{
    HAL_GPIO_WritePin(PS2_DO_GPIO_Port, PS2_DO_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PS2_CLK_GPIO_Port, PS2_CLK_Pin, GPIO_PIN_SET);
    
}

//向手柄发送命令
void PS2_Cmd(uint8_t CMD)
{
	volatile uint16_t ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			HAL_GPIO_WritePin(PS2_DO_GPIO_Port, PS2_DO_Pin, GPIO_PIN_SET);                   //输出一位控制位
		}
		else HAL_GPIO_WritePin(PS2_DO_GPIO_Port, PS2_DO_Pin, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(PS2_CLK_GPIO_Port, PS2_CLK_Pin, GPIO_PIN_SET);                        //时钟拉高
		DELAY_TIME;
		HAL_GPIO_WritePin(PS2_CLK_GPIO_Port, PS2_CLK_Pin, GPIO_PIN_RESET);
		DELAY_TIME;
		HAL_GPIO_WritePin(PS2_CLK_GPIO_Port, PS2_CLK_Pin, GPIO_PIN_SET);
		if(HAL_GPIO_ReadPin(PS2_DI_GPIO_Port, PS2_DI_Pin))
			Data[1] = ref|Data[1];
	}
	DWT_Delay_us(16);
}
//判断是否为红灯模式,0x41=模拟绿灯，0x73=模拟红灯
//返回值；1，红灯模式
//		  其他，其他模式
uint8_t PS2_RedLight(void)
{
	HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_RESET);
	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据
	HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_SET);
	if( Data[1] == 0x73)
        return 1;
    return 0;
}
//读取手柄数据
void PS2_ReadData(void)
{
	volatile uint8_t byte=0;
	volatile uint16_t ref=0x01;
	HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_RESET);
	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据
	for(byte=2;byte<9;byte++)          //开始接受数据
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			HAL_GPIO_WritePin(PS2_CLK_GPIO_Port, PS2_CLK_Pin, GPIO_PIN_SET);
			DELAY_TIME;
			HAL_GPIO_WritePin(PS2_CLK_GPIO_Port, PS2_CLK_Pin, GPIO_PIN_RESET);
			DELAY_TIME;
			HAL_GPIO_WritePin(PS2_CLK_GPIO_Port, PS2_CLK_Pin, GPIO_PIN_SET);
		      if(HAL_GPIO_ReadPin(PS2_DI_GPIO_Port, PS2_DI_Pin))
		      Data[byte] = ref|Data[byte];
		}
        DWT_Delay_us(16);
	}
	HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_SET);
}

//对读出来的PS2的数据进行处理,只处理按键部分  
//只有一个按键按下时按下为0， 未按下为1
uint8_t PS2_DataKey()
{
	uint8_t index;

	Handkey=(Data[4]<<8)|Data[3];     //这是16个按键  按下为0， 未按下为1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;
	}
	return 0;          //没有任何按键按下
}

//得到一个摇杆的模拟量	 范围0~256
uint8_t PS2_AnologData(uint8_t button)
{
	return Data[button];
}

//清除数据缓冲区
void PS2_ClearData()
{
	uint8_t a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}
/******************************************************
Function:    void PS2_Vibration(uint8_t motor1, uint8_t motor2)
Description: 手柄震动函数，
Calls:		 void PS2_Cmd(uint8_t CMD);
Input: motor1:右侧小震动电机 0x00关，其他开
	   motor2:左侧大震动电机 0x40~0xFF 电机开，值越大 震动越大
******************************************************/
void PS2_Vibration(uint8_t motor1, uint8_t motor2)
{
	HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_RESET);
	DWT_Delay_us(16);
    PS2_Cmd(0x01);  //开始命令
	PS2_Cmd(0x42);  //请求数据
	PS2_Cmd(0X00);
	PS2_Cmd(motor1);
	PS2_Cmd(motor2);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_SET);
	DWT_Delay_us(16);  
}
//short poll
void PS2_ShortPoll(void)
{
	HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_RESET);
	DWT_Delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x42);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x00);
	HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_SET);
	DWT_Delay_us(16);	
}
//进入配置
void PS2_EnterConfing(void)
{
    HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_RESET);
	DWT_Delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01);
	PS2_Cmd(0x00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_SET);
	DWT_Delay_us(16);
}
//发送模式设置
void PS2_TurnOnAnalogMode(void)
{
	HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_RESET);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x44);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x01); //analog=0x01;digital=0x00  软件设置发送模式
	PS2_Cmd(0x03); //Ox03锁存设置，即不可通过按键“MODE”设置模式。
				   //0xEE不锁存软件设置，可通过按键“MODE”设置模式。
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	PS2_Cmd(0X00);
	HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_SET);
	DWT_Delay_us(16);
}
//振动设置
void PS2_VibrationMode(void)
{
	HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_RESET);
	DWT_Delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x4D);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0X01);
	HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_SET);
	DWT_Delay_us(16);	
}
//完成并保存配置
void PS2_ExitConfing(void)
{
  HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_RESET);
	DWT_Delay_us(16);
	PS2_Cmd(0x01);  
	PS2_Cmd(0x43);  
	PS2_Cmd(0X00);
	PS2_Cmd(0x00);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	PS2_Cmd(0x5A);
	HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_SET);
	DWT_Delay_us(16);
}

//手柄配置初始化
void PS2_SetInit(void)
{
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_ShortPoll();
	PS2_EnterConfing();		//进入配置模式
	//PS2_TurnOnAnalogMode();	//“红绿灯”配置模式，并选择是否保存
	//PS2_VibrationMode();	//开启震动模式
	PS2_ExitConfing();		//完成并保存配置
}

//读取手柄信息
void PS2_Receive(void)
{
    PS2_ClearData();
	PS2_ReadData();
    PS2_KEY=PS2_DataKey();
    PS2_LX=PS2_AnologData(PSS_LX);
    PS2_LY=PS2_AnologData(PSS_LY);
    PS2_RX=PS2_AnologData(PSS_RX);
    PS2_RY=PS2_AnologData(PSS_RY);
    RedLightMode = PS2_RedLight();
}


