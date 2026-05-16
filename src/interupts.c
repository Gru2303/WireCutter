#include "interupts.h"

void TIM1_UP_TIM10_IRQHandler() {
    HAL_TIM_IRQHandler(&htim1);
}

void NMI_Handler() {
    Error_Handler();
}

void HardFault_Handler() {
    Error_Handler();
}

void MemManage_Handler() {
    Error_Handler();
}

void BusFault_Handler() {
    Error_Handler();
}

void UsageFault_Handler() {
    Error_Handler();
}

void SVC_Handler() {

}

void DebugMon_Handler() {

}

void PendSV_Handler() {

}

void SysTick_Handler() {
    HAL_IncTick();
}