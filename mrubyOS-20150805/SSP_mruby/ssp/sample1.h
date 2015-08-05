#ifndef TOPPERS_MACRO_ONLY
extern void	task1(intptr_t arg);
extern void	task2(intptr_t arg);
extern void cyclic_handler(intptr_t exinf);

extern void alarm_handler(intptr_t exinf);		//alvstakahashi
extern void alarm_handler2(intptr_t exinf);		//alvstakahashi
extern void 	mrb_ssp_thread_call(intptr_t arg);

extern void 	mrb_ssp_cyclic_call(intptr_t arg);
extern void 	mrb_ssp_alarm_call(intptr_t arg);


#endif /* TOPPERS_MACRO_ONLY */
