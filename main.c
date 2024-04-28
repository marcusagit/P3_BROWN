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

    // setup stepper pins, sensor pins
    Stepper motor_a = stepper_default();
    motor_a.gpio_a_pwm = 13;
    motor_a.gpio_a_dir = 12;
    motor_a.gpio_b_pwm = 11;
    motor_a.gpio_b_dir = 10;

    stepper_init(&motor_a);



    // rotate motor based on sensor values
    while(1){
        mpu6050_print_raw_values();
        stepper_rotate(&motor_a, 360);
        sleep_ms(500);
        stepper_rotate(&motor_a, -360);
        sleep_ms(500);
    }
}
