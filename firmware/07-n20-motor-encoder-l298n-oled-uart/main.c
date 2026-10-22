#include <stdint.h>
#include <stdio.h>

/* STM32F446RE bare-metal example: L298N + N20 encoder + SSD1306 OLED + UART2.
   Assumes core/APB timer clock is 16 MHz (HSI), OLED address 0x3C, I2C1 PB8/PB9.
   Encoder count: rising edges on PC4 only. Set ENCODER_PULSES_PER_OUTPUT_REV correctly. */

#define REG32(a) (*(volatile uint32_t *)(a))

/* RCC */
#define RCC_AHB1ENR REG32(0x40023830U)
#define RCC_APB1ENR REG32(0x40023840U)
#define RCC_APB2ENR REG32(0x40023844U)

/* GPIOA */
#define GPIOA_MODER REG32(0x40020000U)
#define GPIOA_OTYPER REG32(0x40020004U)
#define GPIOA_OSPEEDR REG32(0x40020008U)
#define GPIOA_PUPDR REG32(0x4002000CU)
#define GPIOA_IDR REG32(0x40020010U)
#define GPIOA_ODR REG32(0x40020014U)
#define GPIOA_AFRL REG32(0x40020020U)
#define GPIOA_AFRH REG32(0x40020024U)
/* GPIOB */
#define GPIOB_MODER REG32(0x40020400U)
#define GPIOB_OTYPER REG32(0x40020404U)
#define GPIOB_OSPEEDR REG32(0x40020408U)
#define GPIOB_PUPDR REG32(0x4002040CU)
#define GPIOB_AFRH REG32(0x40020424U)
/* GPIOC */
#define GPIOC_MODER REG32(0x40020800U)
#define GPIOC_PUPDR REG32(0x4002080CU)
#define GPIOC_IDR REG32(0x40020810U)
#define GPIOC_ODR REG32(0x40020814U)
/* USART2 */
#define USART2_SR REG32(0x40004400U)
#define USART2_DR REG32(0x40004404U)
#define USART2_BRR REG32(0x40004408U)
#define USART2_CR1 REG32(0x4000440CU)
#define USART2_CR2 REG32(0x40004410U)
#define USART2_CR3 REG32(0x40004414U)
/* I2C1 */
#define I2C1_CR1 REG32(0x40005400U)
#define I2C1_CR2 REG32(0x40005404U)
#define I2C1_DR REG32(0x40005410U)
#define I2C1_SR1 REG32(0x40005414U)
#define I2C1_SR2 REG32(0x40005418U)
#define I2C1_CCR REG32(0x4000541CU)
#define I2C1_TRISE REG32(0x40005420U)
/* TIM1 */
#define TIM1_CR1 REG32(0x40010000U)
#define TIM1_EGR REG32(0x40010014U)
#define TIM1_CCMR1 REG32(0x40010018U)
#define TIM1_CCER REG32(0x40010020U)
#define TIM1_PSC REG32(0x40010028U)
#define TIM1_ARR REG32(0x4001002CU)
#define TIM1_CCR1 REG32(0x40010034U)
#define TIM1_BDTR REG32(0x40010044U)
/* SysTick */
#define SYST_CSR REG32(0xE000E010U)
#define SYST_RVR REG32(0xE000E014U)
#define SYST_CVR REG32(0xE000E018U)
/* EXTI/SYSCFG/NVIC */
#define SYSCFG_EXTICR2 REG32(0x4001380CU)
#define EXTI_IMR REG32(0x40013C00U)
#define EXTI_RTSR REG32(0x40013C08U)
#define EXTI_FTSR REG32(0x40013C0CU)
#define EXTI_PR REG32(0x40013C14U)
#define NVIC_ISER0 REG32(0xE000E100U)

#define OLED_ADDR 0x3CU
#define ENCODER_PULSES_PER_OUTPUT_REV 450U /* Example only: verify your exact motor */
#define PWM_PERIOD 999U
#define MOTOR_PWM_PERCENT 50U

volatile uint32_t encoder_pulses = 0U;
volatile uint8_t encoder_phase_b = 0U;

static void delay_init(void) {
    SYST_RVR = 16000U - 1U; /* 1 ms at 16 MHz */
    SYST_CVR = 0U;
    SYST_CSR = 5U; /* processor clock, enable */
}
static void delay_ms(uint32_t ms) {
    while (ms--) {
        while ((SYST_CSR & (1U << 16)) == 0U) { }
    }
}

static void uart_init(void) {
    RCC_AHB1ENR |= 1U;
    RCC_APB1ENR |= (1U << 17);
    GPIOA_MODER = (GPIOA_MODER & ~(3U << 4)) | (2U << 4);
    GPIOA_AFRL = (GPIOA_AFRL & ~(0xFU << 8)) | (7U << 8); /* PA2 AF7 */
    USART2_CR1 = 0U; USART2_CR2 = 0U; USART2_CR3 = 0U;
    USART2_BRR = 139U; /* 115200 baud at 16 MHz */
    USART2_CR1 = (1U << 3) | (1U << 13); /* TE, UE */
}
static void uart_putc(char c) {
    while ((USART2_SR & (1U << 7)) == 0U) { }
    USART2_DR = (uint8_t)c;
}
static void uart_puts(const char *s) { while (*s) uart_putc(*s++); }
static void uart_u32(uint32_t n) {
    char b[11]; uint32_t i = 0U;
    if (!n) { uart_putc('0'); return; }
    while (n && i < sizeof(b)) { b[i++] = (char)('0' + n % 10U); n /= 10U; }
    while (i) uart_putc(b[--i]);
}

static uint8_t i2c_start(void) {
    uint32_t t = 100000U;
    while (I2C1_SR2 & 2U) if (--t == 0U) return 0U;
    I2C1_CR1 |= (1U << 8);
    t = 100000U; while (!(I2C1_SR1 & 1U)) if (--t == 0U) return 0U;
    return 1U;
}
static void i2c_stop(void) { I2C1_CR1 |= (1U << 9); }
static uint8_t i2c_addr(uint8_t addr) {
    uint32_t t = 100000U; I2C1_DR = addr;
    while (!(I2C1_SR1 & ((1U << 1) | (1U << 10)))) if (--t == 0U) return 0U;
    if (I2C1_SR1 & (1U << 10)) { (void)I2C1_SR1; return 0U; }
    (void)I2C1_SR1; (void)I2C1_SR2; return 1U;
}
static uint8_t i2c_write(uint8_t d) {
    uint32_t t = 100000U; I2C1_DR = d;
    while (!(I2C1_SR1 & (1U << 7))) if (--t == 0U) return 0U;
    t = 100000U; while (!(I2C1_SR1 & (1U << 2))) if (--t == 0U) return 0U;
    return 1U;
}
static void i2c_init(void) {
    RCC_AHB1ENR |= (1U << 1); RCC_APB1ENR |= (1U << 21);
    GPIOB_MODER = (GPIOB_MODER & ~((3U<<16)|(3U<<18))) | (2U<<16) | (2U<<18);
    GPIOB_OTYPER |= (1U<<8)|(1U<<9);
    GPIOB_OSPEEDR |= (3U<<16)|(3U<<18);
    GPIOB_PUPDR = (GPIOB_PUPDR & ~((3U<<16)|(3U<<18))) | (1U<<16) | (1U<<18);
    GPIOB_AFRH = (GPIOB_AFRH & ~0xFFU) | 0x44U; /* PB8/PB9 AF4 */
    I2C1_CR1 = 0U; I2C1_CR2 = 16U; I2C1_CCR = 80U; I2C1_TRISE = 17U; I2C1_CR1 |= 1U;
}
static uint8_t oled_cmd(uint8_t c) {
    if (!i2c_start()) return 0U;
    if (!i2c_addr(OLED_ADDR << 1)) { i2c_stop(); return 0U; }
    if (!i2c_write(0x00U) || !i2c_write(c)) { i2c_stop(); return 0U; }
    i2c_stop(); return 1U;
}
static void oled_data(uint8_t d) {
    if (!i2c_start()) return;
    if (!i2c_addr(OLED_ADDR << 1)) { i2c_stop(); return; }
    if (!i2c_write(0x40U)) { i2c_stop(); return; }
    (void)i2c_write(d); i2c_stop();
}
static void oled_init(void) {
    delay_ms(100U);
    oled_cmd(0xAE); oled_cmd(0xD5); oled_cmd(0x80); oled_cmd(0xA8); oled_cmd(0x3F);
    oled_cmd(0xD3); oled_cmd(0x00); oled_cmd(0x40); oled_cmd(0x8D); oled_cmd(0x14);
    oled_cmd(0x20); oled_cmd(0x02); oled_cmd(0xA1); oled_cmd(0xC8); oled_cmd(0xDA);
    oled_cmd(0x12); oled_cmd(0x81); oled_cmd(0x7F); oled_cmd(0xD9); oled_cmd(0xF1);
    oled_cmd(0xDB); oled_cmd(0x40); oled_cmd(0xA4); oled_cmd(0xA6); oled_cmd(0xAF);
}
static void oled_pos(uint8_t page, uint8_t col) {
    oled_cmd((uint8_t)(0xB0U | page)); oled_cmd((uint8_t)(col & 0x0FU));
    oled_cmd((uint8_t)(0x10U | ((col >> 4) & 0x0FU)));
}
static const uint8_t digits[10][5] = {
 {0x3E,0x51,0x49,0x45,0x3E},{0x00,0x42,0x7F,0x40,0x00},{0x42,0x61,0x51,0x49,0x46},
 {0x21,0x41,0x45,0x4B,0x31},{0x18,0x14,0x12,0x7F,0x10},{0x27,0x45,0x45,0x45,0x39},
 {0x3C,0x4A,0x49,0x49,0x30},{0x01,0x71,0x09,0x05,0x03},{0x36,0x49,0x49,0x49,0x36},{0x06,0x49,0x49,0x29,0x1E}};
static void oled_char(char c) {
    uint8_t i, f[5] = {0,0,0,0,0};
    if (c >= '0' && c <= '9') { for (i=0;i<5;i++) f[i]=digits[c-'0'][i]; }
    else if (c=='R') { f[0]=0x7F; f[1]=0x09; f[2]=0x19; f[3]=0x29; f[4]=0x46; }
    else if (c=='P') { f[0]=0x7F; f[1]=0x09; f[2]=0x09; f[3]=0x09; f[4]=0x06; }
    else if (c=='M') { f[0]=0x7F; f[1]=0x02; f[2]=0x0C; f[3]=0x02; f[4]=0x7F; }
    else if (c=='U') { f[0]=0x3F; f[1]=0x40; f[2]=0x40; f[3]=0x40; f[4]=0x3F; }
    else if (c=='L') { f[0]=0x7F; f[1]=0x40; f[2]=0x40; f[3]=0x40; f[4]=0x40; }
    else if (c=='S') { f[0]=0x26; f[1]=0x49; f[2]=0x49; f[3]=0x49; f[4]=0x32; }
    else if (c=='E') { f[0]=0x7F; f[1]=0x49; f[2]=0x49; f[3]=0x49; f[4]=0x41; }
    else if (c=='=') { f[0]=0x14; f[1]=0x14; f[2]=0x14; f[3]=0x14; f[4]=0x14; }
    else if (c==':') { f[1]=0x36; f[2]=0x36; }
    else if (c==' ') { }
    for (i=0;i<5;i++) oled_data(f[i]); oled_data(0x00U);
}
static void oled_str(const char *s) { while (*s) oled_char(*s++); }
static void oled_clear(void) {
    uint8_t p; uint16_t i;
    for (p=0;p<8;p++) { oled_pos(p,0); if (!i2c_start()) continue;
        if (!i2c_addr(OLED_ADDR<<1)) { i2c_stop(); continue; }
        if (!i2c_write(0x40U)) { i2c_stop(); continue; }
        for (i=0;i<128;i++) (void)i2c_write(0); i2c_stop(); }
}
static void oled_u32(uint32_t n) {
    char b[11]; uint8_t i=0;
    if (!n) { oled_char('0'); return; }
    while (n && i<sizeof(b)) { b[i++]=(char)('0'+n%10U); n/=10U; }
    while (i) oled_char(b[--i]);
}

static void motor_encoder_init(void) {
    RCC_AHB1ENR |= (1U<<0)|(1U<<2); /* GPIOA and GPIOC */
    RCC_APB2ENR |= (1U<<0)|(1U<<14); /* TIM1 and SYSCFG */
    /* PA8 = TIM1_CH1 AF1 */
    GPIOA_MODER = (GPIOA_MODER & ~(3U<<16)) | (2U<<16);
    GPIOA_AFRH = (GPIOA_AFRH & ~0xFU) | 1U;
    /* PC2/PC3 output */
    GPIOC_MODER = (GPIOC_MODER & ~((3U<<4)|(3U<<6))) | (1U<<4)|(1U<<6);
    GPIOC_ODR &= ~((1U<<2)|(1U<<3));
    /* PC4/PC5 input, pull-up */
    GPIOC_MODER &= ~((3U<<8)|(3U<<10));
    GPIOC_PUPDR = (GPIOC_PUPDR & ~((3U<<8)|(3U<<10))) | (1U<<8)|(1U<<10);
    /* TIM1 PWM at approx 1 kHz if TIM1 clock = 16 MHz */
    TIM1_PSC=15U; TIM1_ARR=PWM_PERIOD; TIM1_CCR1=0U;
    TIM1_CCMR1 = (TIM1_CCMR1 & ~(7U<<4)) | (6U<<4) | (1U<<3);
    TIM1_CCER |= 1U; TIM1_BDTR |= (1U<<15); TIM1_CR1 |= (1U<<7);
    TIM1_EGR=1U; TIM1_CR1 |= 1U;
    /* EXTI4 from PC4 */
    SYSCFG_EXTICR2 = (SYSCFG_EXTICR2 & ~0xFU) | 2U;
    EXTI_IMR |= (1U<<4); EXTI_RTSR |= (1U<<4); EXTI_FTSR &= ~(1U<<4); EXTI_PR=(1U<<4);
    NVIC_ISER0 = (1U<<10); /* EXTI4_IRQn */
}
void EXTI4_IRQHandler(void) {
    if (EXTI_PR & (1U<<4)) {
        encoder_phase_b = (GPIOC_IDR & (1U<<5)) ? 1U : 0U;
        encoder_pulses++;
        EXTI_PR = (1U<<4);
    }
}
static void motor_forward(void) { GPIOC_ODR |= (1U<<2); GPIOC_ODR &= ~(1U<<3); }
static void motor_set_speed(uint8_t percent) {
    if (percent>100U) percent=100U;
    TIM1_CCR1=((PWM_PERIOD+1U)*percent)/100U;
}

int main(void) {
    uint32_t previous_count=0U, count, delta, rpm;
    delay_init(); uart_init(); i2c_init(); oled_init(); oled_clear(); motor_encoder_init();
    uart_puts("STM32F446RE MOTOR ENCODER MONITOR\r\n");
    uart_puts("Motor rated: 6V 100RPM\r\n");
    uart_puts("Check encoder PPR setting before relying on RPM.\r\n");
    motor_forward(); motor_set_speed(MOTOR_PWM_PERCENT);
    while (1) {
        delay_ms(1000U);
        __asm volatile ("cpsid i" ::: "memory");
        count=encoder_pulses;
        __asm volatile ("cpsie i" ::: "memory");
        delta=count-previous_count; previous_count=count;
        rpm=(delta*60U)/ENCODER_PULSES_PER_OUTPUT_REV;
        uart_puts("RPM="); uart_u32(rpm); uart_puts("  Pulse Count="); uart_u32(count); uart_puts("\r\n");
        oled_clear();
        oled_pos(1U,0U); oled_str("RPM="); oled_u32(rpm);
        oled_pos(4U,0U); oled_str("PULSES="); oled_u32(count);
    }
}
