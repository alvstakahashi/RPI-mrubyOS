#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/string.h"

static mrb_value
mrb_callback_initialize(mrb_state *mrb, mrb_value self)
{
	  mrb_value id;
	  mrb_value name;
	  char *name_cstr;	

	  mrb_get_args(mrb, "So",&name, &id);

	  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@task_id"),id );
	  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@task_name"),name );

	name_cstr = mrb_str_to_cstr(mrb, name);

	 printf("init here id=%d name=%s \n",mrb_fixnum(id),name_cstr);
	  return self;
}
static mrb_value
mrb_callback_act(mrb_state *mrb, mrb_value self)
{

	mrb_value name = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@task_name"));
	mrb_value id   = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@task_id"));
	mrb_int id_num = mrb_fixnum(id);

 	char *name_cstr = mrb_str_to_cstr(mrb, name);	
	
	printf("id=%d name =%s\n",id_num,name_cstr);
	mrb_funcall(mrb, mrb_top_self(mrb), name_cstr,1, id);
	return self;	
}

void
mrb_mruby_callback_gem_init(mrb_state* mrb) {


	struct RClass *led;
	led = mrb_define_class(mrb, "Callback", mrb->object_class);

	/* methods */
	mrb_define_method(mrb, led, "initialize", mrb_callback_initialize, ARGS_REQ(2));
	mrb_define_method(mrb, led, "act", mrb_callback_act, ARGS_NONE());

}

void
mrb_mruby_callback_gem_final(mrb_state* mrb) {
  // finalizer
}
