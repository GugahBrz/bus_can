
#ifndef INC_ANEMO_H_
#define INC_ANEMO_H_

#include "main.h"

//uint16_t anemoCount;

void anemo_Timer1Init(void);
int anemo_GetCount(void);
void anemo_ResetCount(void);
void anemo_Step(uint16_t anemoCount);


#endif /* INC_ANEMO_H_ */
