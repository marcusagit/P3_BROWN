#include <stdio.h>
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/bootrom.h"

// full step phase:
// 1: A+ B+
// 2: A- B+
// 3: A- B-
// 4: A+ B-
//
// Step angle = 1.80/360
// Steps per rotation = 200.0

// GPIO
#define PWM_A_GPIO 2
#define DIR_A_GPIO 3
#define PWM_B_GPIO 4
#define DIR_B_GPIO 5
#define MOTOR_1_PWM_CHAN PWM_CHAN_A


static int current_step = 0;
const int steps_per_rotation = 200;
const uint a_step[] = {
    1, 0, 0, 1
};
const uint b_step[] = {
    1, 1, 0, 0
};

void stop_motor(uint a_dir, uint a_pwm, uint b_dir, uint b_pwm){
    current_step = 0;
    gpio_put(a_dir, 0);
    gpio_put(b_dir, 0);
    pwm_set_gpio_level(a_pwm, 0);
    pwm_set_gpio_level(b_pwm, 0);
}


void step_motor(uint a_dir, uint b_dir, int dir, uint delay_ms){
    if(dir > 0){
        current_step += 1;
        current_step = current_step % 4;
        gpio_put(a_dir, a_step[current_step]);
        gpio_put(b_dir, b_step[current_step]);
    } else {
        current_step -= 1;
        if(current_step < 0){ current_step = 3; }
        gpio_put(a_dir, a_step[current_step]);
        gpio_put(b_dir, b_step[current_step]);
    }
    int step_delay_ms = 1;
    if(step_delay_ms < 1){
        step_delay_ms = 1;
    }
    sleep_ms(delay_ms);
}

void rotate_motor(uint a_dir, uint b_dir, int deg, int delay_ms){
    if (deg == 0){ return; }
    int steps = steps_per_rotation/(360/deg);
    uint usteps = steps;
    if (steps < 0){
        usteps = -steps;
    }
    for(size_t i = 0; i < usteps; i++){
        step_motor(a_dir, b_dir, steps, delay_ms);
    }
}

int main() {


    // setup pwm pins
    gpio_set_function(PWM_A_GPIO, GPIO_FUNC_PWM);
    gpio_set_function(PWM_B_GPIO, GPIO_FUNC_PWM);

    // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
    uint slice_a = pwm_gpio_to_slice_num(PWM_A_GPIO);
    uint slice_b = pwm_gpio_to_slice_num(PWM_B_GPIO);


    pwm_set_wrap(slice_a, 3);
    pwm_set_wrap(slice_b, 3);
    pwm_set_chan_level(slice_a, MOTOR_1_PWM_CHAN, 3);
    pwm_set_chan_level(slice_b, MOTOR_1_PWM_CHAN, 3);

    pwm_set_enabled(slice_a, true);
    pwm_set_enabled(slice_b, true);

    int step_counter = 0;

    const uint a_dir = DIR_A_GPIO;
    gpio_init(a_dir);
    gpio_set_dir(a_dir, GPIO_OUT);
    gpio_put(a_dir, 1);

    const uint b_dir = DIR_B_GPIO;
    gpio_init(b_dir);
    gpio_set_dir(b_dir, GPIO_OUT);
    gpio_put(b_dir, 1);

    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    printf("Starting Test\n");

    int dir = 1;

    uint delay = 5;

    uint dir_counter = 0;


    while(1){
        rotate_motor(a_dir, b_dir, 360, 1);
        sleep_ms(500);
        rotate_motor(a_dir, b_dir, 360, 10);
        sleep_ms(500);
        rotate_motor(a_dir, b_dir, -360, 1);
        sleep_ms(500);
    }
    // for ease of debugging
    // stop_motor(a_dir, PWM_A_GPIO, b_dir, PWM_B_GPIO);
    // reset_usb_boot(0,0);
}
