/***********************************************************************/
/*                                                                     */
/*  FILE        :mrb_read.c                                           */
/*  DATE        :Thu, Jul 07, 2015                                     */
/*  DESCRIPTION :mrb binary SCI0 read to buffer                        */
/*  CPU TYPE    :RX63N                                                 */
/*                                                                     */
/***********************************************************************/

#include <stdint.h>
#include <string.h>

#include <kernel.h>
#include "kernel_cfg.h"
#include "rpi_lib/rpi.h"

#include "mrb_read.h"

#define MRBSZ		4096		// 4096�o�C�g

extern const uint8_t
#if defined __GNUC__
__attribute__((aligned(4)))
#elif defined _MSC_VER
__declspec(align(4))
#endif
rb_main[];

uint8_t	
#if defined __GNUC__
__attribute__((aligned(4)))
#elif defined _MSC_VER
__declspec(align(4))
#endif
prg_buff[MRBSZ];

uint8_t	cmd_buff[256];


#define TIMEOUT		3000		// 3�b
#define	START_TOUT	10000		//�@�ŏ���10�b
#define HEADERSZ	10			// �w�b�_�T�C�Y10�o�C�g
#define	SIZESZ		4			// SIZE�t�B�[���h�T�C�Y4�o�C�g

#define CPU_UartPuts			uart0_puts
#define STATUSBYTES		32			// 32�o�C�g��.
int CPU_UartGetch_t(RELTIM tout)
{
	char data;
	SYSTIM targetTime,nowTime;
	
	get_tim(&targetTime);		//���ݎ���
	targetTime += (tout);		// ������ms�P��
								//�I�[�o�[�t���[�͂Ƃ肠�����Ή����Ȃ�
	
	while(1){
		if (Serial_available()) {	/* �f�[�^����M������ */
			data = *UART0_DR;
			break;
		}
		get_tim(&nowTime);
		if (targetTime <= nowTime)
		{
			return(-1);
		}
	}
	return ((int)(data & 0x00ff));
}

int CPU_UartRead(char *inbuf,int sz,RELTIM tout)
{
	int c;
	int count = 0;
	int statusCount = STATUSBYTES;

	do {
		c = CPU_UartGetch_t(tout);
		if (c == -1)
		{
			return(count);
		}
		*inbuf++ = c;
		count++;
#if 1
		if (statusCount--)
		{
			CPU_UartPuts(".");
			statusCount = STATUSBYTES;
		}
#endif
		
	} while (count < sz);
	return count;
}
/* �R���\�[�����������(CR�R�[�h�܂�/�ő�255����/�G�R�[�o�b�N����) */
int CPU_UartGets(char *buffer)
{
	char data;
	int i;
	i=0;
	while(1){
		data = uart0_getc();	/* 1�������� */
		*buffer = data;
		uart0_putc(data);	/* 1�����o��(�G�R�[�o�b�N) */
		buffer++;
		i++;
		if (i > 255) break;		/* �ő啶�����ɒB���� */
		if (data == 0x0D) break;/* CR�R�[�h�܂Ŏ�M */
	}
	*buffer=(unsigned char)0;	/* null�R�[�h */
	return i;					/* ���͕����� */
}

int read_mrb(char *buff,int bfsz)
{
	int readCount = 0;
	uint32_t mrb_size = 0;

	readCount = CPU_UartRead(buff,bfsz,START_TOUT);
	if (readCount < 0)
	{
		return(-1);
	}
	return(readCount);
}

uint8_t *mrb_read_ui(void)
{
	int commandCount = 0;

	int loadCount = 0;

	printf("\nmrubyOS ver0.001 By AliveVisionSoftwareKK\n");
	while(1)
	{
		printf("for Command L to load mrb binary\n");
		printf("E to exit & go load program \n");
		printf("Q to exit & run for internal program \n");
		if (loadCount)
		{
			printf("Loaded now\n");
		}
		else
		{
			printf("No program load\n");
		}
		CPU_UartPuts(">>>");

		commandCount = CPU_UartGets(cmd_buff);
		if (commandCount > 0)
		{
			switch(cmd_buff[0])
			{
			case 'L':			// load
				memset(prg_buff,0,MRBSZ);
				loadCount = read_mrb(prg_buff,MRBSZ);
				printf("Load Finish!! LoadCount = %d\n",loadCount);
				break;
			case 'E':
				if (loadCount)
				{
					return(prg_buff);
				}
				else
				{
					return((uint8_t *)NULL);
				}
				break;
			case 'Q':
				return((uint8_t *)NULL);
				break;
			case 'C':
				if (memcmp(prg_buff,rb_main,loadCount) == 0)
				{
					printf("Compare OK\n");
				}
				else
				{
					printf("Compare NG\n");
				}
				break;
				
			default:
				printf("command err\n");
				break;
			}
		}
	}
}
