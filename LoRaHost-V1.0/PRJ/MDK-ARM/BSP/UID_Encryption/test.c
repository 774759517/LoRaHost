#include "test.h"
uint32_t idAddr[]=
{
	0x1FFFF7AC,//F0
	0x1FFFF7E8,//F1
	0x1FFF7A10,//F2
	0x1FFFF7AC,//F3
	0x1FFF7A10,//F4
	0x1FF0F420,//F7
	0x1FF80050,//L0
	0x1FF80050,//L1
	0x1FFF7590,//L4
	0x1FF0F420,//H7
};
/*获取MCU唯一ID*/
void GetSTM32MCUUID(uint32_t *id,teMCUTypedef type)
{
	if(id!=NULL)
	{
		id[0]=*(uint32_t*)(idAddr[type]);
		id[1]=*(uint32_t*)(idAddr[type]+4);
		id[2]=*(uint32_t*)(idAddr[type]+8);
		user_info("本机ID：%08X%08X%08X",id[0],id[1],id[2]);
	}
}

