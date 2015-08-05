
#include "mruby.h"
#include "mruby/variable.h"

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

#ifndef RPI2
#define GPFSEL0 0x20200000
#define GPFSEL1 0x20200004
#define GPFSEL2 0x20200008
#define GPFSEL4 0x20200010
#define GPSET0  0x2020001C
#define GPSET1  0x20200020
#define GPCLR0  0x20200028
#define GPCLR1  0x2020002C
#else
#define GPFSEL0 0x3F200000
#define GPFSEL1 0x3F200004
#define GPFSEL2 0x3F200008
#define GPFSEL4 0x3F200010
#define GPSET0  0x3F20001C
#define GPSET1  0x3F200020
#define GPCLR0  0x3F200028
#define GPCLR1  0x3F20002C
#endif

#define INTERN_LEN	1
#define PIN_LEN		2

unsigned int ra;
uint16_t flg = 0;

static mrb_value
mrb_rs_led_initialize(mrb_state *mrb, mrb_value self)
{
	  mrb_int n;
	  mrb_value pin;
	  mrb_get_args(mrb, "i", &n);
	  pin = mrb_fixnum_value(n);
	  mrb_iv_set(mrb, self, mrb_intern(mrb, "@pin",PIN_LEN), pin);
	  mrb_iv_set(mrb, self, mrb_intern(mrb, "@status",INTERN_LEN), mrb_false_value());

	  return self;

}

static mrb_value
mrb_rs_led_on(mrb_state *mrb, mrb_value self)
{
	uint16_t n;
	mrb_value pin;
	pin = mrb_iv_get(mrb, self, mrb_intern(mrb, "@pin",PIN_LEN));
	n = mrb_fixnum(pin);

	if (n > 31)
	{
		PUT32(GPSET1,1<<(n-32));
	}
	else
	{	
		PUT32(GPSET0,1<<n);
	}
	mrb_iv_set(mrb, self, mrb_intern(mrb, "@status",INTERN_LEN), mrb_true_value());
	return self;
}

static mrb_value
mrb_rs_led_off(mrb_state *mrb, mrb_value self)
{
	uint16_t n;
	mrb_value pin;
	pin = mrb_iv_get(mrb, self, mrb_intern(mrb, "@pin",PIN_LEN));
	n = mrb_fixnum(pin);
	if (n > 31)
	{
		PUT32(GPCLR1,(1<<(n-32)));
	}
	else
	{
		PUT32(GPCLR0,1<<n);
	}
	mrb_iv_set(mrb, self, mrb_intern(mrb, "@status",INTERN_LEN), mrb_false_value());

    return self;
}


static mrb_value
mrb_rs_led_toggle(mrb_state *mrb, mrb_value self)
{
	mrb_value status;

	status = mrb_iv_get(mrb, self, mrb_intern(mrb, "@status",INTERN_LEN));
	if(mrb_bool(status)){
		mrb_rs_led_off(mrb, self);
	}else{
		mrb_rs_led_on(mrb, self);
	}

	return self;
}


void
mrb_mruby_rs_led_gem_init(mrb_state* mrb) {


	struct RClass *led;
	led = mrb_define_class(mrb, "Led", mrb->object_class);

	/* define led collar 8,16,17,18,23 */
#if defined(RPI2) || defined(RPIBPLUS) 
	mrb_define_const(mrb,led, "ACT", mrb_fixnum_value(47));
#else
	mrb_define_const(mrb,led, "ACT", mrb_fixnum_value(16));
#endif
	mrb_define_const(mrb,led, "GREEN", mrb_fixnum_value(17));
	mrb_define_const(mrb,led, "ORANGE", mrb_fixnum_value(18));
	mrb_define_const(mrb,led, "RED", mrb_fixnum_value(23));
	mrb_define_const(mrb,led, "BLUE", mrb_fixnum_value(8));

	/* methods */
	mrb_define_method(mrb, led, "initialize", mrb_rs_led_initialize, ARGS_REQ(1));
	mrb_define_method(mrb, led, "on", mrb_rs_led_on, ARGS_NONE());
	mrb_define_method(mrb, led, "off", mrb_rs_led_off, ARGS_NONE());
	mrb_define_method(mrb, led, "toggle", mrb_rs_led_toggle, ARGS_NONE());

#if defined(RPI2) || defined(RPIBPLUS) 
	//47 B+
	ra=GET32(GPFSEL4);
	ra&=~(7<<21);
	ra|=1<<21;
	PUT32(GPFSEL4,ra);
#else
	//16
	ra=GET32(GPFSEL1);
	ra&=~(7<<18);
	ra|=1<<18;
	PUT32(GPFSEL1,ra);
#endif

	//17
	ra=GET32(GPFSEL1);
	ra&=~(7<<21);
	ra|=1<<21;
	PUT32(GPFSEL1,ra);

	//18
	ra=GET32(GPFSEL1);
	ra&=~(7<<24);
	ra|=1<<24;
	PUT32(GPFSEL1,ra);

	//23
	ra=GET32(GPFSEL2);
	ra&=~(7<<9);	//23 -> 3*3
	ra|=1<<9;
	PUT32(GPFSEL2,ra);

	//25
	ra=GET32(GPFSEL0);
	ra&=~(7<<24);	//25 -> 3*4
	ra|=1<<24;
	PUT32(GPFSEL0,ra);

#if defined(RPI2) || defined(RPIBPLUS) 
	PUT32(GPCLR1,1<<(47-32));
#else
	PUT32(GPCLR0,1<<16);
#endif
	PUT32(GPCLR0,1<<17);
	PUT32(GPCLR0,1<<18);
	PUT32(GPCLR0,1<<23);
	PUT32(GPCLR0,1<<8);

}

void
mrb_mruby_rs_led_gem_final(mrb_state* mrb) {
  // finalizer
}
