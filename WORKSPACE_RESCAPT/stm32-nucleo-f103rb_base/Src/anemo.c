
#include "anemo.h"


TIM_HandleTypeDef htim1;

//================================================================

void anemo_Timer1Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_IC_InitTypeDef     sICConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 200;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim1);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_TI1;//TIM_CLOCKSOURCE_ETRMODE1;// TIM_CLOCKSOURCE_ETRMODE2; //TIM_CLOCKSOURCE_TI1
  sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
  sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
  sClockSourceConfig.ClockFilter = 0xF;
  HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);

  sICConfig.ICPolarity  = TIM_ICPOLARITY_RISING;
  sICConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sICConfig.ICPrescaler = TIM_ICPSC_DIV1;
  sICConfig.ICFilter    = 0;
  HAL_TIM_IC_ConfigChannel(&htim1, &sICConfig, TIM_CHANNEL_1);

  TIM1->CNT=0; 			//valeur initiale
  TIM1->CR1|=0x0001; 	//timer 1 en ON.
}

//================================================================
int anemo_GetCount(void)
{
	return TIM1->CNT;
}
//================================================================
void anemo_ResetCount(void)
{
	  TIM1->CNT=0;
}
//================================================================

void anemo_Step()
{
	//term_printf("Anemo: %d Hz/km/h \n\r", anemo_GetCount());
	anemoCount = anemo_GetCount();
	anemo_ResetCount();

	//char lowerByte = anemoCount & 0xFF;
	//char upperByte = anemoCount >> 8;

	/*CAN_Message      txMsg;

	txMsg.id = 0x01; //Card ID

	//Send vent vitesse
	txMsg.data[0] = lowerByte;//measure from sensor
	txMsg.data[1] = upperByte;

	txMsg.len = 8;
	txMsg.format = CANStandard;
	txMsg.type = CANData;

	can_Write(txMsg);*/
}
//================================================================



