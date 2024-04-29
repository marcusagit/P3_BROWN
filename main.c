#include <stdio.h>
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/bootrom.h"

#include "mpu6050.c"
#include "stepper.c"


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
    Stepper motor_a = stepper_default();
    motor_a.gpio_a_pwm = 9;
    motor_a.gpio_a_dir = 8;
    motor_a.gpio_b_pwm = 7;
    motor_a.gpio_b_dir = 6;
    
    Stepper motor_b = stepper_default();
    motor_b.gpio_a_pwm = 13;
    motor_b.gpio_a_dir = 12;
    motor_b.gpio_b_pwm = 11;
    motor_b.gpio_b_dir = 10;

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



    // rotate motor based on sensor values
    while(1){
        mpu6050_print_raw_values();
        stepper_rotate(&motor_a, 360);
        sleep_ms(500);
        stepper_rotate(&motor_b, 360);
        sleep_ms(500);
        stepper_rotate(&motor_c, 360);
        sleep_ms(500);
        stepper_rotate(&motor_d, 360);
        sleep_ms(500);
    }
}
