#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/string.h"

#include "kernel_impl.h"
#include "task.h"

#include <string.h>

#define MAX_CYCLIC	8

static mrb_value cyclic_self_tbl[MAX_CYCLIC];

extern mrb_state *mrb_global;


static mrb_value
mrb_ssp_cyclic_initialize(mrb_state *mrb, mrb_value self)
{
	  mrb_value id;
	  mrb_value name;
	  char *name_cstr;	
	  mrb_int	id_num;

	  mrb_get_args(mrb, "So",&name, &id);

	  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@cyclic_id"),id );
	  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@cyclic_name"),name );

	  id_num = mrb_fixnum(id);
	  
	  cyclic_self_tbl[id_num-1] = self;

  	  name_cstr = mrb_str_to_cstr(mrb, name);

	  return self;
}
static mrb_value
mrb_ssp_cyclic_sta(mrb_state *mrb, mrb_value self)
{
	ER retval;
	mrb_value id   = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@cyclic_id"));
	mrb_int id_num = mrb_fixnum(id);

	retval = sta_cyc(id_num);
	
	if (retval == E_OK)
	{
		return(mrb_false_value());
	}
	return(mrb_true_value());
}
static mrb_value
mrb_ssp_cyclic_stp(mrb_state *mrb, mrb_value self)
{
	ER retval;
	mrb_value id   = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@cyclic_id"));
	mrb_int id_num = mrb_fixnum(id);

	retval = stp_cyc(id_num);
	
	if (retval == E_OK)
	{
		return(mrb_false_value());
	}
	return(mrb_true_value());
}

// ssp_thread ‚Å“o˜^‚³‚ê‚étask–{‘Ì
void
mrb_ssp_cyclic_call(intptr_t exf)
{
	mrb_int cyclic_id = (mrb_int)exf;		// cyclicID
	mrb_value self  = cyclic_self_tbl[cyclic_id-1];
	char name_cstr[20];
	
	mrb_value name = mrb_iv_get(mrb_global, self, mrb_intern_lit(mrb_global, "@cyclic_name"));
	mrb_value id   = mrb_iv_get(mrb_global, self, mrb_intern_lit(mrb_global, "@cyclic_id"));
// 	char *name_cstr = mrb_str_to_cstr(mrb_global, name);	//ƒƒ‚ƒŠƒŠ[ƒNH@ŠJ•ú‚µ‚È‚¢‚½‚ß‚S‰ñ–Ú‚Å—Ž‚¿‚é
    strncpy(name_cstr, RSTRING_PTR(name), RSTRING_LEN(name));
    name_cstr[RSTRING_LEN(name)]='\0';

#if 0
	mrb_funcall(mrb_global, mrb_top_self(mrb_global), "cyclic",1, id);
#else
	i_lock_cpu();		//May be Interruped in ruby handler are problem.
	mrb_funcall(mrb_global, self, (const char*)name_cstr,1, id);
	i_unlock_cpu();
#endif
}
	
void
mrb_mruby_ssp_cyclic_gem_init(mrb_state* mrb) {


	struct RClass *cyc;
	cyc = mrb_define_class(mrb, "Ssp_cyclic", mrb->object_class);

	/* methods */
	mrb_define_method(mrb, cyc, "initialize", mrb_ssp_cyclic_initialize, ARGS_REQ(2));
	mrb_define_method(mrb, cyc, "start", mrb_ssp_cyclic_sta, ARGS_NONE());
	mrb_define_method(mrb, cyc, "stop" , mrb_ssp_cyclic_stp, ARGS_NONE());

}

void
mrb_mruby_ssp_cyclic_gem_final(mrb_state* mrb) {
  // finalizer
}
