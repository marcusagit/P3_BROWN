#include <stdio.h>
// #include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/bootrom.h"


const static int max_revolutions_per_second = 10;


// Step angle = 1.80/360
// Steps per revolution = 200.0
const int steps_per_rev = 200;

// full step phase:
// 1: A+ B+
// 2: A- B+
// 3: A- B-
// 4: A+ B-
const uint a_step[] = {
    1, 0, 0, 1
};
const uint b_step[] = {
    1, 1, 0, 0
};


typedef struct Stepper {
    int current_step;
    int delay_ms;
    int gpio_a_pwm;
    int gpio_a_dir;
    int gpio_b_pwm;
    int gpio_b_dir;
} Stepper;

Stepper stepper_default(){
    Stepper s = {0};
    s.current_step = 0;
    s.delay_ms = 1;
    s.gpio_a_pwm = 2;
    s.gpio_a_dir = 3;
    s.gpio_b_pwm = 4;
    s.gpio_b_dir = 5;
    return s;
}


void stepper_stop(Stepper *s){
    gpio_put(s->gpio_a_pwm, 0);
    gpio_put(s->gpio_b_pwm, 0);
}

// TODO: control delay/step speed (1ms is the minimum delay) 

void stepper_step_no_delay(Stepper *s, int dir){
    gpio_put(s->gpio_a_pwm, 1);
    gpio_put(s->gpio_b_pwm, 1);

    if(dir > 0){
        s->current_step += 1;
        s->current_step = s->current_step % 4;
        gpio_put(s->gpio_a_dir, a_step[s->current_step]);
        gpio_put(s->gpio_b_dir, b_step[s->current_step]);
    } else {
        s->current_step -= 1;
        if(s->current_step < 0){ s->current_step = 3; }
        gpio_put(s->gpio_a_dir, a_step[s->current_step]);
        gpio_put(s->gpio_b_dir, b_step[s->current_step]);
    }
}

void stepper_step(Stepper *s, int dir){
    stepper_step_no_delay(s, dir);
    sleep_ms(1);
}

void stepper_rotate(Stepper *s, int deg){
    if (deg == 0){ return; }
    int steps = steps_per_rev/(360/deg);
    uint usteps = steps;
    if (steps < 0){
        usteps = -steps;
    }
    for(size_t i = 0; i < usteps; i++){
        stepper_step(s, steps);
    }
    // NOTE: stops the buzz....
    gpio_put(s->gpio_a_pwm, 0);
    gpio_put(s->gpio_b_pwm, 0);
}

void stepper_init(Stepper *s){
    gpio_init(s->gpio_a_pwm);
    gpio_set_dir(s->gpio_a_pwm, GPIO_OUT);
    gpio_put(s->gpio_a_pwm, 0);

    gpio_init(s->gpio_b_pwm);
    gpio_set_dir(s->gpio_b_pwm, GPIO_OUT);
    gpio_put(s->gpio_b_pwm, 0);

    gpio_init(s->gpio_a_dir);
    gpio_set_dir(s->gpio_a_dir, GPIO_OUT);
    gpio_put(s->gpio_a_dir, 0);

    gpio_init(s->gpio_b_dir);
    gpio_set_dir(s->gpio_b_dir, GPIO_OUT);
    gpio_put(s->gpio_b_dir, 0);

}
