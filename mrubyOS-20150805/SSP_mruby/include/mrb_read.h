/***********************************************************************/
/*                                                                     */
/*  FILE        :mrb_read.h                                           */
/*  DATE        :Thu, Jul 07, 2015                                     */
/*  DESCRIPTION :mrb binary SCI0 read to buffer                        */
/*  CPU TYPE    :RX63N                                                 */
/*                                                                     */
/***********************************************************************/

#ifndef __MRB_READ_H__
#define __MRB_READ_H__


extern int read_mrb(char *buff,int bfsz);
extern uint8_t *mrb_read_ui(void);


#endif //__MRB_READ_H__

