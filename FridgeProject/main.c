#include <inttypes.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/timer.h"
#include "driverlib/eeprom.h"
#include "driverlib/interrupt.h"
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "helper_library/helper.h"

volatile static uint8_t fridge_open = false;

void init_sensor_kit_led()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2);
}

void turn_on_led()
{
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0xFF);
}

void turn_off_led()
{
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0x00);
}

void Timer0Isr()
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    fridge_open = true;
}

void init_timer()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, 20 * 16000000);
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0Isr);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

void init_sensor_kit_button()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_3);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPD);
}

void init_buzzer()
{

    /* Init MWPWM0, which will drive the buzzer */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    GPIOPinConfigure(GPIO_PB6_M0PWM0);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
    PWMGenConfigure(PWM0_BASE, PWM_GEN_0,
    PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 4000);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 2000);
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
}

void fridge()
{
    bool fridge_button = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3);

    uint32_t tmr1Value;
    uint32_t seconds;

    if (fridge_button)
    {
        TimerEnable(TIMER0_BASE, TIMER_A);
        turn_on_led();
        tmr1Value = TimerValueGet(TIMER0_BASE, TIMER_A);
        seconds = tmr1Value / 16000000;
        if (fridge_open == true && seconds % 2 == 0)
        {
            PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
        }
        else
        {
            PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, false);
        }
    }

    else if (!fridge_button)
    {
        PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, false);
        TimerDisable(TIMER0_BASE, TIMER_A);
        turn_off_led();
        fridge_open = false;
    }

    else
    {
        // do nothing
    }
}

int main(void)
{
    helper_library_init();
    init_timer();
    init_buzzer();
    init_sensor_kit_button();
    init_sensor_kit_led();

    while (1)
    {
        fridge();
    }
}
