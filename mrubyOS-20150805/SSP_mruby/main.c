#include <kernel.h>
#include "kernel_cfg.h"

#include "rpi_lib/rpi.h"
#include <stdio.h>
#include <stdint.h>

#include "mrb_read.h"

extern const uint8_t
#if defined __GNUC__
__attribute__((aligned(4)))
#elif defined _MSC_VER
__declspec(align(4))
#endif
rb_main[];


extern void _kernel_signal_time();
// タイマー割り込み処理
void timerIRQ_handler(void)
{
	_kernel_handler(_kernel_signal_time);
}



// IRQ割り込みハンドラ
void IRQ_handler(void)
{
	// Basic IRQ pendingをチェック
	if(*INTERRUPT_IRQ_BASIC_PENDING & 0x01 != 0)
	{
		// タイマー割り込み

        // 割り込みフラグクリア
        *TIMER_IRQ_CLR = 0;

		// タイマ割り込み処理
		timerIRQ_handler();

	}

	return;
}




void task1(intptr_t arg)
{
	printf("task1 start-----------------------------------------------------\n");

  uint8_t *prg_data;
  prg_data = (uint8_t*)rb_main;
#if 1
	pinMode(25,INPUT);
	if (digitalRead(25)==0)
	{
		prg_data = mrb_read_ui();
		if (prg_data == NULL)
		{
			prg_data = (uint8_t*)rb_main;
		}
		else
		{
			printf("mrb head =%8s\n",prg_data);
		}
	}
#endif
	mruby_call((uint8_t *)rb_main);

	printf("task1 End.\n");
}
void task2(intptr_t arg)
{
	int count;
	count = 1;
	printf("task2 Started\n");
	while(1)
	{
	//	printf("task2 running Count= %d\n",count++);
	}
}
