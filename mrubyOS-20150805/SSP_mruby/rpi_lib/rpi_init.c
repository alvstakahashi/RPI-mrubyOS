#include "rpi.h"

void rpi_init(void){
	// bssのクリア
	clearBss();
	// システムタイマーを初期化
	init_syst();
	// GPIOをすべてINPUT_PULLUPに設定
//	init_gpio();

	// 起動確認用
#if defined(RPI2) || defined(RPIBPLUS) 
	pinMode(47,OUTPUT);
	digitalWrite(47, LOW);
#else
	pinMode(16,OUTPUT);
	digitalWrite(16, LOW);
#endif
	// UARTを有効
	Serial_begin(115200);
}
