#include <stdint.h>
#include <stddef.h>
#include "bits.h"

typedef struct {
	volatile uint32_t GPIO_MODER;		// 0x00
	volatile uint32_t GPIO_OTYPER;		// 0x04
	volatile uint32_t GPIO_OSPEEDR;		// 0x08
	volatile uint32_t GPIO_PUPDR;		// 0x0C
	volatile uint32_t GPIO_IDR;			// 0x10
	volatile uint32_t GPIO_ODR;			// 0x14
	volatile uint32_t GPIO_BSRR;		// 0x18
	volatile uint32_t GPIO_LCKR;		// 0x1C
	volatile uint32_t GPIO_AFRL;		// 0x20
	volatile uint32_t GPIO_AFRH;		// 0x24
} MY_GPIO_TypeDef;

typedef struct {
	volatile uint32_t RCC_CR;			// 0x00
	volatile uint32_t RCC_PLLCFGR;  	// 0x04
	volatile uint32_t RCC_CFGR;			// 0x08
	volatile uint32_t RCC_CIR;			// 0x0C
	volatile uint32_t RCC_AHB1RSTR;		// 0x10
	volatile uint32_t RCC_AHB2RSTR;		// 0x14
	volatile uint32_t RCC_AHB3RSTR;		// 0x18
	volatile uint32_t RESERVED0;		// 0x1C
	volatile uint32_t RCC_APB1RSTR;		// 0x20
	volatile uint32_t RCC_APB2RSTR;		// 0x24
	volatile uint32_t RESERVED1;		// 0x28
	volatile uint32_t RESERVED2;		// 0x2C
	volatile uint32_t RCC_AHB1ENR;		// 0x30
	volatile uint32_t RCC_AHB2ENR;		// 0x34
	volatile uint32_t RCC_AHB3ENR;		// 0x38
	volatile uint32_t RESERVED3;		// 0x3C
	volatile uint32_t RCC_APB1ENR;		// 0x40
	volatile uint32_t RCC_APB2ENR;		// 0x44
	volatile uint32_t RESERVED4;		// 0x48
	volatile uint32_t RESERVED5;		// 0x4C
	volatile uint32_t RCC_AHB1LPENR;	// 0x50
	volatile uint32_t RCC_AHB2LPENR;	// 0x54
	volatile uint32_t RCC_AHB3LPENR;	// 0x58
	volatile uint32_t RESERVED6;		// 0x5C
	volatile uint32_t RCC_APB1LPENR;	// 0x60
	volatile uint32_t RCC_APB2LPENR;	// 0x64
	volatile uint32_t RESERVED7;		// 0x68
	volatile uint32_t RESERVED8;		// 0x6C
	volatile uint32_t RCC_BDCR;			// 0x70
	volatile uint32_t RCC_CSR;			// 0x74
	volatile uint32_t RESERVED9;		// 0x78
	volatile uint32_t RESERVED10;		// 0x7C
	volatile uint32_t RCC_SSCGR;		// 0x80
	volatile uint32_t RCC_PLLI2SCFGR;	// 0x84
	volatile uint32_t RCC_PLLSAICFGR;	// 0x88
	volatile uint32_t RCC_DCKCFGR;		// 0x8C
	volatile uint32_t RCC_CKGATENR;		// 0x90
	volatile uint32_t RCC_DCKCFGR2;		// 0x94
} MY_RCC_TypeDef;

_Static_assert(offsetof(MY_RCC_TypeDef, RCC_AHB1ENR) == 0x30u, "AHB1ENR offset");
_Static_assert(offsetof(MY_GPIO_TypeDef, GPIO_IDR) == 0x10u, "IDR offset");
_Static_assert(offsetof(MY_GPIO_TypeDef, GPIO_ODR) == 0x14u, "ODR offset");
_Static_assert(offsetof(MY_RCC_TypeDef, RCC_DCKCFGR2) == 0x94u, "DCKCFGR2 offset");

#define MY_GPIOA ((MY_GPIO_TypeDef *)0x40020000u)
#define MY_GPIOC ((MY_GPIO_TypeDef *)0x40020800u)
#define MY_RCC ((MY_RCC_TypeDef *)0x40023800u)

#define LED_PIN 5u
#define LED_MODER_POS (LED_PIN * 2u)
#define LED_MODE_OUTPUT 1u
#define LED_MODER_WIDTH 2u
#define GPIOAEN_POS 0u

#define B1_PIN 13u
#define B1_MODER_POS (B1_PIN * 2u)
#define B1_MODE_INPUT 0u
#define B1_MODER_WIDTH 2u
#define B1_PRESSED_LEVEL 0u
#define B1_RELEASED_LEVEL (1u - B1_PRESSED_LEVEL)
#define GPIOCEN_POS 2u

#define DELAY_SLOW 120000u
#define DELAY_MEDIUM 80000u
#define DELAY_FAST 40000u
static const uint32_t delays[] = {DELAY_SLOW, DELAY_MEDIUM, DELAY_FAST};

static void led_init(void) {
	MY_RCC->RCC_AHB1ENR = set_bit(MY_RCC->RCC_AHB1ENR, GPIOAEN_POS);
	MY_GPIOA->GPIO_MODER = set_field(MY_GPIOA->GPIO_MODER, LED_MODE_OUTPUT, LED_MODER_POS, LED_MODER_WIDTH);
}

static void button_init(void) {
	MY_RCC->RCC_AHB1ENR = set_bit(MY_RCC->RCC_AHB1ENR, GPIOCEN_POS);
	MY_GPIOC->GPIO_MODER = set_field(MY_GPIOC->GPIO_MODER, B1_MODE_INPUT, B1_MODER_POS, B1_MODER_WIDTH);
}

//static void led_follow_button(void) {
//	const unsigned int b1_state = test_bit(MY_GPIOC->GPIO_IDR, B1_PIN);
//	if(b1_state == B1_PRESSED_LEVEL) {
//		MY_GPIOA->GPIO_ODR = set_bit(MY_GPIOA->GPIO_ODR, LED_PIN);
//	} else {
//		MY_GPIOA->GPIO_ODR = clear_bit(MY_GPIOA->GPIO_ODR, LED_PIN);
//	}
//}
//
//static void button_toggle_led(void) {
//	const unsigned b1_state = test_bit(MY_GPIOC->GPIO_IDR, B1_PIN);
//	static unsigned button_state = B1_RELEASED_LEVEL;
//	if((b1_state == B1_PRESSED_LEVEL) && (button_state == B1_RELEASED_LEVEL)) {
//		MY_GPIOA->GPIO_ODR = toggle_bit(MY_GPIOA->GPIO_ODR, LED_PIN);
//	}
//	button_state = b1_state;
//}

static uint32_t speed_steps(void) {
    const size_t array_size = sizeof(delays) / sizeof(delays[0]);

    const unsigned b1_state = test_bit(MY_GPIOC->GPIO_IDR, B1_PIN);
    static unsigned button_state = B1_RELEASED_LEVEL;
    static unsigned index = 0u;

    if((b1_state == B1_PRESSED_LEVEL) && (button_state == B1_RELEASED_LEVEL)) {
    	index = (index + 1u) % array_size;
    }

    button_state = b1_state;
    return delays[index];
}

static void blink_led(void) {
	MY_GPIOA->GPIO_ODR = toggle_bit(MY_GPIOA->GPIO_ODR, LED_PIN);
}

int main(void) {
    led_init();
    button_init();
	for(;;) {
		blink_led();
		uint32_t speed = speed_steps();
		uint32_t loop_bound = speed;
		for (volatile uint32_t i = 0; i < loop_bound; i++) {
			(void)speed_steps();
		}
	}
}
