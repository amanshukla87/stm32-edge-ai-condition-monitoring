/*
 * ST7789 TFT colour-test firmware
 * Target: STM32F446RE / NUCLEO-F446RE
 *
 * Wiring:
 *   TFT SCK  -> PB13 (SPI2_SCK, AF5)
 *   TFT MOSI -> PB15 (SPI2_MOSI, AF5)
 *   TFT CS   -> PB12
 *   TFT DC   -> PC0
 *   TFT RST  -> PC1
 *   TFT GND  -> GND
 *   TFT VCC/BL -> only as supported by your exact module
 *
 * IMPORTANT:
 * This is a generic ST7789 240x320 starting point, not hardware-verified.
 * ST7789 modules vary in resolution, offsets, colour order and init sequence.
 * If your panel is not 240x320, update TFT_WIDTH/TFT_HEIGHT and offsets.
 * Create/use an STM32CubeIDE HAL project for STM32F446RE and add this file
 * as main.c. Do not add it alongside another main.c.
 */

#include "stm32f4xx_hal.h"
#include <stdint.h>

/* ---------- Display configuration: confirm for your module ---------- */
#define TFT_WIDTH       240U
#define TFT_HEIGHT      320U
#define TFT_X_OFFSET    0U
#define TFT_Y_OFFSET    0U

#define TFT_CS_GPIO     GPIOB
#define TFT_CS_PIN      GPIO_PIN_12
#define TFT_DC_GPIO     GPIOC
#define TFT_DC_PIN      GPIO_PIN_0
#define TFT_RST_GPIO    GPIOC
#define TFT_RST_PIN     GPIO_PIN_1

#define TFT_SPI_TIMEOUT 1000U

SPI_HandleTypeDef hspi2;

static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void Error_Handler(void);

static void TFT_Reset(void);
static void TFT_WriteCommand(uint8_t command);
static void TFT_WriteData(const uint8_t *data, uint16_t length);
static void TFT_WriteDataByte(uint8_t data);
static void TFT_Init(void);
static void TFT_SetAddressWindow(uint16_t x0, uint16_t y0,
                                 uint16_t x1, uint16_t y1);
static void TFT_FillScreen(uint16_t colour);
static void TFT_Delay(uint32_t ms);

static void TFT_CS_Low(void)
{
    HAL_GPIO_WritePin(TFT_CS_GPIO, TFT_CS_PIN, GPIO_PIN_RESET);
}

static void TFT_CS_High(void)
{
    HAL_GPIO_WritePin(TFT_CS_GPIO, TFT_CS_PIN, GPIO_PIN_SET);
}

static void TFT_DC_Command(void)
{
    HAL_GPIO_WritePin(TFT_DC_GPIO, TFT_DC_PIN, GPIO_PIN_RESET);
}

static void TFT_DC_Data(void)
{
    HAL_GPIO_WritePin(TFT_DC_GPIO, TFT_DC_PIN, GPIO_PIN_SET);
}

static void TFT_Delay(uint32_t ms)
{
    HAL_Delay(ms);
}

static void TFT_WriteCommand(uint8_t command)
{
    TFT_CS_Low();
    TFT_DC_Command();
    if (HAL_SPI_Transmit(&hspi2, &command, 1U, TFT_SPI_TIMEOUT) != HAL_OK)
        Error_Handler();
    TFT_CS_High();
}

static void TFT_WriteData(const uint8_t *data, uint16_t length)
{
    if (length == 0U) return;

    TFT_CS_Low();
    TFT_DC_Data();
    if (HAL_SPI_Transmit(&hspi2, (uint8_t *)data, length,
                         TFT_SPI_TIMEOUT) != HAL_OK)
        Error_Handler();
    TFT_CS_High();
}

static void TFT_WriteDataByte(uint8_t data)
{
    TFT_WriteData(&data, 1U);
}

static void TFT_Reset(void)
{
    HAL_GPIO_WritePin(TFT_RST_GPIO, TFT_RST_PIN, GPIO_PIN_SET);
    TFT_Delay(10U);
    HAL_GPIO_WritePin(TFT_RST_GPIO, TFT_RST_PIN, GPIO_PIN_RESET);
    TFT_Delay(20U);
    HAL_GPIO_WritePin(TFT_RST_GPIO, TFT_RST_PIN, GPIO_PIN_SET);
    TFT_Delay(120U);
}

static void TFT_Init(void)
{
    TFT_Reset();

    /* Software reset and sleep-out */
    TFT_WriteCommand(0x01U);
    TFT_Delay(150U);
    TFT_WriteCommand(0x11U);
    TFT_Delay(120U);

    /* Generic ST7789 setup. Panel-specific tuning may be required. */
    TFT_WriteCommand(0xB2U); /* Porch control */
    {
        const uint8_t d[] = {0x0CU, 0x0CU, 0x00U, 0x33U, 0x33U};
        TFT_WriteData(d, sizeof(d));
    }

    TFT_WriteCommand(0xB7U); /* Gate control */
    TFT_WriteDataByte(0x35U);

    TFT_WriteCommand(0xBBU); /* VCOM setting */
    TFT_WriteDataByte(0x19U);

    TFT_WriteCommand(0xC0U); /* LCM control */
    TFT_WriteDataByte(0x2CU);

    TFT_WriteCommand(0xC2U); /* VDV and VRH command enable */
    TFT_WriteDataByte(0x01U);

    TFT_WriteCommand(0xC3U); /* VRH set */
    TFT_WriteDataByte(0x12U);

    TFT_WriteCommand(0xC4U); /* VDV set */
    TFT_WriteDataByte(0x20U);

    TFT_WriteCommand(0xC6U); /* Frame rate control */
    TFT_WriteDataByte(0x0FU);

    TFT_WriteCommand(0xD0U); /* Power control */
    {
        const uint8_t d[] = {0xA4U, 0xA1U};
        TFT_WriteData(d, sizeof(d));
    }

    /* Positive gamma */
    TFT_WriteCommand(0xE0U);
    {
        const uint8_t d[] = {
            0xD0U, 0x04U, 0x0DU, 0x11U, 0x13U, 0x2BU,
            0x3FU, 0x54U, 0x4CU, 0x18U, 0x0DU, 0x0BU,
            0x1FU, 0x23U
        };
        TFT_WriteData(d, sizeof(d));
    }

    /* Negative gamma */
    TFT_WriteCommand(0xE1U);
    {
        const uint8_t d[] = {
            0xD0U, 0x04U, 0x0CU, 0x11U, 0x13U, 0x2CU,
            0x3FU, 0x44U, 0x51U, 0x2FU, 0x1FU, 0x1FU,
            0x20U, 0x23U
        };
        TFT_WriteData(d, sizeof(d));
    }

    /* 16-bit RGB565 pixel format */
    TFT_WriteCommand(0x3AU);
    TFT_WriteDataByte(0x55U);

    /* Memory access control. If red/blue are swapped, try 0x08 or 0x00. */
    TFT_WriteCommand(0x36U);
    TFT_WriteDataByte(0x00U);

    /* Inversion on is common for ST7789 panels; adjust if image looks wrong. */
    TFT_WriteCommand(0x21U);
    TFT_WriteCommand(0x13U); /* Normal display mode */
    TFT_Delay(10U);
    TFT_WriteCommand(0x29U); /* Display on */
    TFT_Delay(100U);
}

static void TFT_SetAddressWindow(uint16_t x0, uint16_t y0,
                                 uint16_t x1, uint16_t y1)
{
    uint8_t data[4];

    x0 = (uint16_t)(x0 + TFT_X_OFFSET);
    x1 = (uint16_t)(x1 + TFT_X_OFFSET);
    y0 = (uint16_t)(y0 + TFT_Y_OFFSET);
    y1 = (uint16_t)(y1 + TFT_Y_OFFSET);

    TFT_WriteCommand(0x2AU); /* Column address set */
    data[0] = (uint8_t)(x0 >> 8);
    data[1] = (uint8_t)(x0 & 0xFFU);
    data[2] = (uint8_t)(x1 >> 8);
    data[3] = (uint8_t)(x1 & 0xFFU);
    TFT_WriteData(data, sizeof(data));

    TFT_WriteCommand(0x2BU); /* Row address set */
    data[0] = (uint8_t)(y0 >> 8);
    data[1] = (uint8_t)(y0 & 0xFFU);
    data[2] = (uint8_t)(y1 >> 8);
    data[3] = (uint8_t)(y1 & 0xFFU);
    TFT_WriteData(data, sizeof(data));

    TFT_WriteCommand(0x2CU); /* Memory write */
}

static void TFT_FillScreen(uint16_t colour)
{
    /* 256 pixels per transfer = 512-byte buffer. */
    uint8_t pixelBuffer[512];
    const uint32_t totalPixels = (uint32_t)TFT_WIDTH * (uint32_t)TFT_HEIGHT;
    uint32_t remaining = totalPixels;
    uint16_t i;

    for (i = 0U; i < 256U; i++)
    {
        pixelBuffer[(uint16_t)(2U * i)] =
            (uint8_t)(colour >> 8);
        pixelBuffer[(uint16_t)(2U * i + 1U)] =
            (uint8_t)(colour & 0xFFU);
    }

    TFT_SetAddressWindow(0U, 0U, TFT_WIDTH - 1U, TFT_HEIGHT - 1U);

    TFT_CS_Low();
    TFT_DC_Data();

    while (remaining > 0U)
    {
        uint16_t pixels = (remaining >= 256U) ? 256U : (uint16_t)remaining;
        uint16_t bytes = (uint16_t)(pixels * 2U);

        if (HAL_SPI_Transmit(&hspi2, pixelBuffer, bytes,
                             TFT_SPI_TIMEOUT) != HAL_OK)
        {
            TFT_CS_High();
            Error_Handler();
        }
        remaining -= pixels;
    }

    TFT_CS_High();
}

/* RGB565 colour values */
#define COLOR_RED     0xF800U
#define COLOR_GREEN   0x07E0U
#define COLOR_BLUE    0x001FU
#define COLOR_WHITE   0xFFFFU
#define COLOR_BLACK   0x0000U

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI2_Init();

    TFT_Init();

    while (1)
    {
        TFT_FillScreen(COLOR_RED);
        TFT_Delay(1000U);

        TFT_FillScreen(COLOR_GREEN);
        TFT_Delay(1000U);

        TFT_FillScreen(COLOR_BLUE);
        TFT_Delay(1000U);

        TFT_FillScreen(COLOR_WHITE);
        TFT_Delay(1000U);

        TFT_FillScreen(COLOR_BLACK);
        TFT_Delay(1000U);
    }
}

static void MX_SPI2_Init(void)
{
    __HAL_RCC_SPI2_CLK_ENABLE();

    hspi2.Instance = SPI2;
    hspi2.Init.Mode = SPI_MODE_MASTER;
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi2.Init.NSS = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial = 10;

    if (HAL_SPI_Init(&hspi2) != HAL_OK)
        Error_Handler();
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (spiHandle->Instance == SPI2)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_SPI2_CLK_ENABLE();

        GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* PB14 MISO is not required for write-only display operation. */
    }
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    HAL_GPIO_WritePin(GPIOB, TFT_CS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, TFT_DC_PIN | TFT_RST_PIN, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = TFT_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = TFT_DC_PIN | TFT_RST_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* HSI 16 MHz -> PLL -> SYSCLK 168 MHz */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        Error_Handler();

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK |
                                  RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 |
                                  RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
        Error_Handler();
}

static void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
        /* Stop here on HAL/SPI/clock error; debug with a breakpoint. */
    }
}
