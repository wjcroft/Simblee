/*
 * Copyright (c) 2015 RF Digital Corp. All Rights Reserved.
 *
 * The source code contained in this file and all intellectual property embodied in
 * or covering the source code is the property of RF Digital Corp. or its licensors.
 * Your right to use this source code and intellectual property is non-transferable,
 * non-sub licensable, revocable, and subject to terms and conditions of the
 * SIMBLEE SOFTWARE LICENSE AGREEMENT.
 * http://www.simblee.com/licenses/SimbleeSoftwareLicenseAgreement.txt
 *
 * THE SOURCE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.
 *
 * This heading must NOT be removed from this file.
 */

#include "nrf51.h"
#include "startup_nrf51822.h"
#include "system_nrf51.h"

/* Initialize segments */
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

/* C library initialization */
void __libc_init_array(void);

/** \cond DOXYGEN_SHOULD_SKIP_THIS */
int main(void);
/** \endcond */

void Reset_Handler(void);
void Default_Handler(void);

/* Cortex-M0 core handlers */
void NMI_Handler        ( void ) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler  ( void ) __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler        ( void ) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler     ( void ) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler    ( void ) __attribute__ ((weak, alias("Default_Handler")));

/* Peripherals handlers */
void POWER_CLOCK_IRQHandler ( void ) __attribute__ ((weak, alias("Default_Handler")));
void RADIO_IRQHandler       ( void ) __attribute__ ((weak, alias("Default_Handler")));
void UART0_IRQHandler       ( void ) __attribute__ ((weak, alias("Default_Handler")));
void SPI0_TWI0_IRQHandler   ( void ) __attribute__ ((weak, alias("Default_Handler")));
void SPI1_TWI1_IRQHandler   ( void ) __attribute__ ((weak, alias("Default_Handler")));
void GPIOTE_IRQHandler      ( void ) __attribute__ ((weak, alias("Default_Handler")));
void ADC_IRQHandler         ( void ) __attribute__ ((weak, alias("Default_Handler")));
void TIMER0_IRQHandler      ( void ) __attribute__ ((weak, alias("Default_Handler")));
void TIMER1_IRQHandler      ( void ) __attribute__ ((weak, alias("Default_Handler")));
void TIMER2_IRQHandler      ( void ) __attribute__ ((weak, alias("Default_Handler")));
void RTC0_IRQHandler        ( void ) __attribute__ ((weak, alias("Default_Handler")));
void TEMP_IRQHandler        ( void ) __attribute__ ((weak, alias("Default_Handler")));
void RNG_IRQHandler         ( void ) __attribute__ ((weak, alias("Default_Handler")));
void ECB_IRQHandler         ( void ) __attribute__ ((weak, alias("Default_Handler")));
void CCM_AAR_IRQHandler     ( void ) __attribute__ ((weak, alias("Default_Handler")));
void WDT_IRQHandler         ( void ) __attribute__ ((weak, alias("Default_Handler")));
void RTC1_IRQHandler        ( void ) __attribute__ ((weak, alias("Default_Handler")));
void QDEC_IRQHandler        ( void ) __attribute__ ((weak, alias("Default_Handler")));
void LPCOMP_IRQHandler      ( void ) __attribute__ ((weak, alias("Default_Handler")));
void SWI0_IRQHandler        ( void ) __attribute__ ((weak, alias("Default_Handler")));
void SWI1_IRQHandler        ( void ) __attribute__ ((weak, alias("Default_Handler")));
void SWI2_IRQHandler        ( void ) __attribute__ ((weak, alias("Default_Handler")));
void SWI3_IRQHandler        ( void ) __attribute__ ((weak, alias("Default_Handler")));
void SWI4_IRQHandler        ( void ) __attribute__ ((weak, alias("Default_Handler")));
void SWI5_IRQHandler        ( void ) __attribute__ ((weak, alias("Default_Handler")));

/* Vector Table declaration */
__attribute__ ((section(".vectors")))
const vector_table_t vector_table = {
    /* Configure Initial Stack Pointer, using linker-generated symbols */
    (void*) (&_estack),
    (void*) Reset_Handler,
    (void*) NMI_Handler,
    (void*) HardFault_Handler,
    (void*) (0x5b1ee001),    /* Reserved */
    (void*) (0UL),           /* Reserved */
    (void*) (0UL),           /* Reserved */
    (void*) (0UL),           /* Reserved */
    (void*) (0UL),           /* Reserved */
    (void*) (0UL),           /* Reserved */
    (void*) (0UL),           /* Reserved */
    (void*) SVC_Handler,
    (void*) (0UL),           /* Reserved */
    (void*) (0UL),           /* Reserved */
    (void*) PendSV_Handler,
    (void*) SysTick_Handler,

    /* Configurable interrupts */
    (void*) POWER_CLOCK_IRQHandler,    /* 0  Power Clock */
    (void*) RADIO_IRQHandler,          /* 1  Radio Controller */
    (void*) UART0_IRQHandler,          /* 2  UART0 Controller */
    (void*) SPI0_TWI0_IRQHandler,      /* 3  SPI0 & TWI0 Controller */
    (void*) SPI1_TWI1_IRQHandler,      /* 4  SPI1 & TWI1 Controller */
    (void*) (0UL),                     /* 5  Reserved */
    (void*) GPIOTE_IRQHandler,         /* 6  GPIO */
    (void*) ADC_IRQHandler,            /* 7  ADC Controller */
    (void*) TIMER0_IRQHandler,         /* 8  Timer Counter 0 */
    (void*) TIMER1_IRQHandler,         /* 9  Timer Counter 1 */
    (void*) TIMER2_IRQHandler,         /* 10 Timer Counter 2 */
    (void*) RTC0_IRQHandler,           /* 11 Real Time Clock Controller 0 */
    (void*) TEMP_IRQHandler,           /* 12 Temperature */
    (void*) RNG_IRQHandler,            /* 13 Random Number Generator */
    (void*) ECB_IRQHandler,            /* 14 ECB */
    (void*) CCM_AAR_IRQHandler,        /* 15 CCM_AAR */
    (void*) WDT_IRQHandler,            /* 16 Watchdog Timer */
    (void*) RTC1_IRQHandler,           /* 17 Real Time Clock Controller 1 */
    (void*) QDEC_IRQHandler,           /* 18 QDEC */
    (void*) LPCOMP_IRQHandler,         /* 19 LPCOMP */
    (void*) SWI0_IRQHandler,           /* 20 Software Interrupt 0 */
    (void*) SWI1_IRQHandler,           /* 21 Software Interrupt 1 */
    (void*) SWI2_IRQHandler,           /* 22 Software Interrupt 2 */
    (void*) SWI3_IRQHandler,           /* 23 Software Interrupt 3 */
    (void*) SWI4_IRQHandler,           /* 24 Software Interrupt 4 */
    (void*) SWI5_IRQHandler,           /* 25 Software Interrupt 5 */
    (void*) (0UL),                     /* 26 Reserved */
    (void*) (0UL),                     /* 27 Reserved */
    (void*) (0UL),                     /* 28 Reserved */
    (void*) (0UL),                     /* 29 Reserved */
    (void*) (0UL),                     /* 30 Reserved */
    (void*) (0UL)                      /* 31 Reserved */
};

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void Reset_Handler(void)
{
    uint32_t *pSrc, *pDest;

    /* Initialize the relocate segment */
    pSrc = &_etext;
    pDest = &_srelocate;

    if (pSrc != pDest) {
        for (; pDest < &_erelocate;) {
            *pDest++ = *pSrc++;
        }
    }

    /* Clear the zero segment */
    for (pDest = &_szero; pDest < &_ezero;) {
        *pDest++ = 0;
    }

    /* Initialize the C library */
    __libc_init_array();

    /* Call SystemInit */
    SystemInit();

    /* Branch to main function */
    main();

    /* Infinite loop */
    while (1);
}

#ifdef DYNAMIC_HANDLERS

void Default_Dynamic_Handler(void);

/* exception number mirrors vector table */
dynamic_handler_t exception_handlers[48] =
{
    0,                        /* No Exception - Executing Thread */
    Default_Dynamic_Handler,  /* Reset */
    Default_Dynamic_Handler,  /* NMI */
    Default_Dynamic_Handler,  /* Hard Fault */
    0,                        /* Reserved */
    0,                        /* Reserved */
    0,                        /* Reserved */
    0,                        /* Reserved */
    0,                        /* Reserved */
    0,                        /* Reserved */
    0,                        /* Reserved */
    Default_Dynamic_Handler,  /* SVC */
    0,                        /* Reserved */
    0,                        /* Reserved */
    Default_Dynamic_Handler,  /* PendSV */
    Default_Dynamic_Handler   /* SysTick */
};

/* interrupt number: negative for non-configuable interrupts, positive for configurable interrupts */
dynamic_handler_t *dynamic_handlers = &exception_handlers[16];

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Default_Handler(void)
{
  // call the dynamic exception handler registered for the exception number of the the active vector
  // field of the interrupt control and state register of the system control block
  exception_handlers[SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk]();
}

/**
 * \brief Default dynamic interrupt handler for unused IRQs.
 */
void Default_Dynamic_Handler(void)
{
    while (1)
      ;
}

#else

/**
 * \brief Default interrupt handler for unused IRQs.
 */
void Default_Handler(void)
{
    while (1)
      ;
}

#endif
