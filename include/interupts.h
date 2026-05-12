#pragma once

#include <stm32f4xx_hal.h>

#include "peripherals/dma.h"
#include "error.h"

void EXTI3_IRQHandler();
void DMA2_Stream5_IRQHandler();
void NMI_Handler();
void HardFault_Handler();
void MemManage_Handler();
void BusFault_Handler();
void UsageFault_Handler();
void SVC_Handler();
void DebugMon_Handler();
void PendSV_Handler();
void SysTick_Handler();