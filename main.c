#include <stdio.h>
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/bootrom.h"

#include "mpu6050.c"
#include "stepper.c"

/*

component pin-out
motor
imu


left side:
GP0  | 01 UART0 TX
GP1  | 02 UART0 RX
GND  | 03 
GP2  | 04 
GP3  | 05 
GP4  | 06 
GP5  | 07 
GND  | 08 
GP6  | 09 MA_apwm
GP7  | 10 MA_adir
GP8  | 11 MA_bpwm
GP9  | 12 MA_bdir
GND  | 13 
GP10 | 14 MB_apwm 
GP11 | 15 MB_adir
GP12 | 16 MB_bpwm
GP13 | 17 MB_bdir
GND  | 18 
GP14 | 19 MD_apwm
GP15 | 20 MD_adir

right side:
| VBUS    | 40 
| VSYS    | 39 
| GND     | 38 
| 3V3_EN  | 37 
| 3V3(OUT)| 36 
| ADC_VREF| 35 
| GP28    | 34 
| GND     | 33 
| GP27    | 32 
| GP26    | 31 
| RUN     | 30 
| GP22    | 29 
| GND     | 28 
| GP21    | 27 MC_bdir
| GP20    | 26 MC_bpwm
| GP19    | 25 MC_adir
| GP18    | 24 MC_apwm
| GND     | 23 
| GP17    | 22 MD_bpwm
| GP16    | 21 MD_bdir


*/

int pico_w_led_on(){
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    return 0;
}

int main() {
    // init stdio
    stdio_init_all();

    // turn on LED
    if(pico_w_led_on()){
        return -1;
    }

    mpu6050_init();

    // TODO: motor layout:
    
    // setup stepper pins, sensor pins
    //
    Stepper motor_a = stepper_default();
    motor_a.gpio_a_pwm = 6;
    motor_a.gpio_a_dir = 7;
    motor_a.gpio_b_pwm = 8;
    motor_a.gpio_b_dir = 9;
    
    Stepper motor_b = stepper_default();
    motor_b.gpio_a_pwm = 10;
    motor_b.gpio_a_dir = 11;
    motor_b.gpio_b_pwm = 12;
    motor_b.gpio_b_dir = 13;

    Stepper motor_c = stepper_default();
    motor_c.gpio_a_pwm = 18;
    motor_c.gpio_a_dir = 19;
    motor_c.gpio_b_pwm = 20;
    motor_c.gpio_b_dir = 21;

    Stepper motor_d = stepper_default();
    motor_d.gpio_a_pwm = 14;
    motor_d.gpio_a_dir = 15;
    motor_d.gpio_b_pwm = 17;
    motor_d.gpio_b_dir = 16;

    stepper_init(&motor_a);
    stepper_init(&motor_b);
    stepper_init(&motor_c);
    stepper_init(&motor_d);



    int counter = 0;
    int dir = 1;
    Stepper *motors[4] = {&motor_a, &motor_b, &motor_c, &motor_d};
    // rotate motor based on sensor values
    while(1){
        mpu6050_print_raw_values();

#if 0
        int i = counter % 4;
        int j = (counter+2) % 4;
        for(int x = 0; x < 2; x++){
            stepper_step_no_delay(motors[i], dir);
            stepper_step_no_delay(motors[j], dir);
            sleep_ms(1);
        }
        sleep_ms(10);
        stepper_stop(motors[i]);
        stepper_stop(motors[j]);
#endif
#if 0
        stepper_step_no_delay(&motor_a, dir);
        stepper_step_no_delay(&motor_b, dir);
        sleep_ms(10);
        stepper_stop(&motor_a);
        stepper_stop(&motor_b);
        stepper_step_no_delay(&motor_c, dir);
        stepper_step_no_delay(&motor_d, dir);
        sleep_ms(10);
        stepper_stop(&motor_c);
        stepper_stop(&motor_d);
#endif
#if 0
        stepper_step(&motor_a, dir);
        stepper_step(&motor_b, dir);
        stepper_step(&motor_c, dir);
        stepper_step(&motor_d, dir);
#endif
#if 0
        // runs at >2A when all motors run simultaneously
        stepper_step_no_delay(&motor_a, dir);
        stepper_step_no_delay(&motor_b, dir);
        stepper_step_no_delay(&motor_c, dir);
        stepper_step_no_delay(&motor_d, dir);
        sleep_ms(2);
#endif
#if 1
        stepper_rotate(&motor_a, 45);
        stepper_rotate(&motor_b, 45);
        stepper_rotate(&motor_c, 45);
        stepper_rotate(&motor_d, 45);
        counter += 44;
#endif

        counter++;
        if (counter > 200){
            dir = -1;
        }
        if (counter > 400){
            break;
        }
    }
    reset_usb_boot(0,0);
}
