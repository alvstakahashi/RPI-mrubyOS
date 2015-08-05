#include <stdint.h>
#include "mruby.h"
#include "mruby/proc.h"
#include "mruby/data.h"
#include "mruby/compile.h"
#include "mruby/string.h"
#include "mruby/numeric.h"
#include "mruby/dump.h"
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#include <kernel.h>

mrb_state *mrb_global;
mrb_irep *idx_code1 = 0;



void mruby_call(uint8_t *mrb_p )
{
    mrb_global = mrb_open();
	if (mrb_global == NULL) 
	{
		return ;
	}
	mrb_value return_value1;
	if (idx_code1 == 0) 
	{
		idx_code1 = mrb_read_irep(mrb_global, (const uint8_t*)mrb_p);
		if (idx_code1 == 0) 
		{
			// irep_error(mrb, idx_code1);
			return ;
		}
	}
	if (idx_code1 != 0) 
	{
//		int ai = mrb_gc_arena_save(mrb_global);
		return_value1 =  mrb_run(mrb_global, mrb_proc_new(mrb_global, idx_code1),mrb_top_self(mrb_global));
//		mrb_gc_arena_restore(mrb_global, ai);
	}
}
