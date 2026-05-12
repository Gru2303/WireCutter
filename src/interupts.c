#include "interupts.h"

void EXTI3_IRQHandler() {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_3) {

    }
}

void DMA2_Stream5_IRQHandler() {
    HAL_DMA_IRQHandler(&hdma_tim1_up);
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