#include "peripherals/timer.h"

TIM_HandleTypeDef htim1 = {0};
TIM_HandleTypeDef htim2 = {0};

static void timer1_init() {
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim1.Instance = TIM1;


    htim1.Init.Prescaler = 100 - 1;
    htim1.Init.Period = 100 - 1;

    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
        Error_Handler();
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

    if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }

    HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

    HAL_TIM_Base_Start_IT(&htim1);
}

static void timer2_init() {
    TIM_Encoder_InitTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 0;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 4294967295;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    sClockSourceConfig.EncoderMode = TIM_ENCODERMODE_TI12;
    sClockSourceConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
    sClockSourceConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sClockSourceConfig.IC1Prescaler = TIM_ICPSC_DIV1;
    sClockSourceConfig.IC1Filter = 15;
    sClockSourceConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
    sClockSourceConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sClockSourceConfig.IC2Prescaler = TIM_ICPSC_DIV1;
    sClockSourceConfig.IC2Filter = 15;

    if (HAL_TIM_Encoder_Init(&htim2, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL) != HAL_OK) {
        Error_Handler();
    }
}

void timer_init() {
    timer1_init();
    timer2_init();
}

void timer_deinit() {
    if (HAL_TIM_Encoder_DeInit(&htim1) != HAL_OK) {
        Error_Handler();
    }

    if (HAL_TIM_Encoder_DeInit(&htim2) != HAL_OK) {
        Error_Handler();
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base) {
    if (htim_base->Instance == TIM1) {
        __HAL_RCC_TIM1_CLK_ENABLE();
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base) {
    if (htim_base->Instance == TIM1) {
        __HAL_RCC_TIM1_CLK_DISABLE();
    }
}

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_encoder){
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (htim_encoder->Instance == TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}


void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* htim_encoder){
    if (htim_encoder->Instance == TIM2) {
        __HAL_RCC_TIM2_CLK_DISABLE();

        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0 | GPIO_PIN_1);
    }
}
