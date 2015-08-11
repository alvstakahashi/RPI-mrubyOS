#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/string.h"

#include "kernel_impl.h"
#include "task.h"

#include <string.h>

#define MAX_ALARM	8

static mrb_value alarm_self_tbl[MAX_ALARM];

extern mrb_state *mrb_global;


static mrb_value
mrb_ssp_alarm_initialize(mrb_state *mrb, mrb_value self)
{
	  mrb_value id;
	  mrb_value name;
	  char *name_cstr;	
	  mrb_int	id_num;

	  mrb_get_args(mrb, "So",&name, &id);

	  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@alarm_id"),id );
	  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@alarm_name"),name );

	  id_num = mrb_fixnum(id);
	  
	  alarm_self_tbl[id_num-1] = self;

  	  name_cstr = mrb_str_to_cstr(mrb, name);

	  printf("alarm init here id=%d name=%s \n",id_num,name_cstr);
	  return self;
}
static mrb_value
mrb_ssp_alarm_sta(mrb_state *mrb, mrb_value self)
{
	ER retval;
	mrb_int	tout;
	mrb_value id   = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@alarm_id"));
	mrb_int id_num = mrb_fixnum(id);

  	mrb_get_args(mrb, "i", &tout);

	retval = sta_alm(id_num,tout);
	if (retval == E_OK)
	{
		return(mrb_false_value());
	}
	return(mrb_true_value());
}
static mrb_value
mrb_ssp_alarm_stp(mrb_state *mrb, mrb_value self)
{
	ER retval;
	mrb_value id   = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@alarm_id"));
	mrb_int id_num = mrb_fixnum(id);

	retval = stp_alm(id_num);
	if (retval == E_OK)
	{
		return(mrb_false_value());
	}
	return(mrb_true_value());
}

// ssp_thread ‚Å“o˜^‚³‚ê‚étask–{‘Ì
void
mrb_ssp_alarm_call(intptr_t exf)
{
	mrb_int alarm_id = (mrb_int)exf;		// alarmID
	mrb_value self  = alarm_self_tbl[alarm_id-1];
	char *name_cstr[20];
	
	mrb_value name = mrb_iv_get(mrb_global, self, mrb_intern_lit(mrb_global, "@alarm_name"));
	mrb_value id   = mrb_iv_get(mrb_global, self, mrb_intern_lit(mrb_global, "@alarm_id"));
// 	char *name_cstr = mrb_str_to_cstr(mrb_global, name);	
    strncpy(name_cstr, RSTRING_PTR(name), RSTRING_LEN(name));
    name_cstr[RSTRING_LEN(name)]='\0';
	
	printf("alarm id =%d name =%s\n",alarm_id,name_cstr);
#if 0
	mrb_funcall(mrb_global, mrb_top_self(mrb_global), name_cstr,1, id);
#else
	i_lock_cpu();		//May be Interruped in ruby handler are problem.
	mrb_funcall(mrb_global, self, (const char*)name_cstr,1, id);
	i_unlock_cpu();
#endif
}
	
void
mrb_mruby_ssp_alarm_gem_init(mrb_state* mrb) {


	struct RClass *alm;
	alm = mrb_define_class(mrb, "Ssp_cyclic", mrb->object_class);

	/* methods */
	mrb_define_method(mrb, alm, "initialize", mrb_ssp_alarm_initialize, ARGS_REQ(2));
	mrb_define_method(mrb, alm, "start", mrb_ssp_alarm_sta, ARGS_REQ(1));
	mrb_define_method(mrb, alm, "stop" , mrb_ssp_alarm_stp, ARGS_NONE());

}

void
mrb_mruby_ssp_alarm_gem_final(mrb_state* mrb) {
  // finalizer
}
