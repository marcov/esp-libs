#ifndef __WDT_H__
#define __WDT_H__



void wdtInit(unsigned timeout_ms);
void wdtEnable(void);
void wdtDisable(void);
void wdtKick(void);

#endif /* #ifndef __WDT_H__ */
