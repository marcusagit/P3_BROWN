#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

// default i2c bus address
static int addr = 0x68;

#define I2C_PORT i2c0
#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5

static void mpu6050_reset() {
    // Two byte reset. First byte register, second byte data
    // There are a load more options to set up the device in different ways that could be added here
    uint8_t buf[] = {0x6B, 0x80};
    i2c_write_blocking(I2C_PORT, addr, buf, 2, false);
}

static void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp) {
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.

    uint8_t buffer[6];

    // Start reading acceleration registers from register 0x3B for 6 bytes
    uint8_t val = 0x3B;
    i2c_write_blocking(I2C_PORT, addr, &val, 1, true); // true to keep master control of bus
    i2c_read_blocking(I2C_PORT, addr, buffer, 6, false);

    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    // Now gyro data from reg 0x43 for 6 bytes
    // The register is auto incrementing on each read
    val = 0x43;
    i2c_write_blocking(I2C_PORT, addr, &val, 1, true);
    i2c_read_blocking(I2C_PORT, addr, buffer, 6, false);  // False - finished with bus

    for (int i = 0; i < 3; i++) {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);;
    }

    // Now temperature from reg 0x41 for 2 bytes
    // The register is auto incrementing on each read
    val = 0x41;
    i2c_write_blocking(I2C_PORT, addr, &val, 1, true);
    i2c_read_blocking(I2C_PORT, addr, buffer, 2, false);  // False - finished with bus

    *temp = buffer[0] << 8 | buffer[1];
}

int mpu6050_who_am_i(){
    sleep_ms(500);
    uint8_t reg = 0x75;
    uint8_t chipID = 0;
    i2c_write_blocking(I2C_PORT, addr, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, addr, &chipID, 1, false);

    return chipID == 0x68;
}


void mpu6050_i2c_init(i2c_inst_t *i2c_port, uint8_t gpio_scl, uint8_t gpio_sda){
    i2c_init(i2c_port, 400 * 1000);
    gpio_set_function(gpio_scl, GPIO_FUNC_I2C);
    gpio_set_function(gpio_sda, GPIO_FUNC_I2C);
    gpio_pull_up(gpio_scl);
    gpio_pull_up(gpio_sda);
}
void mpu6050_wake(){
    uint8_t buf[] = {0x6B, 0x00};
    i2c_write_blocking(I2C_PORT, addr, buf, 2, false);
}

void mpu6050_sleep(){
    uint8_t buf[] = {0x6B, 0x40};
    i2c_write_blocking(I2C_PORT, addr, buf, 2, false);
}

void mpu6050_print_raw_values(){
    int16_t acceleration[3], gyro[3], temp;
    mpu6050_read_raw(acceleration, gyro, &temp);

    // These are the raw numbers from the chip, so will need tweaking to be really useful.
    // See the datasheet for more information
    printf("Acc. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1], acceleration[2]);
    printf("Gyro. X = %d, Y = %d, Z = %d\n", gyro[0], gyro[1], gyro[2]);
    // Temperature is simple so use the datasheet calculation to get deg C.
    // Note this is chip temperature.
    printf("Temp. = %f\n", (temp / 340.0) + 36.53);
}

int mpu6050_init() {
    printf("Hello, MPU6050!\n");


    printf("initializing i2c and gpio pins...\n");
    mpu6050_i2c_init(I2C_PORT, I2C_SCL_PIN, I2C_SDA_PIN);
    sleep_ms(100);

    if(!mpu6050_who_am_i()){
        while(1){
            printf("Who Am I failed!\n");
            sleep_ms(500);
        }
    }

    printf("reseting mpu6050 to defaults...\n");
    mpu6050_reset();
    sleep_ms(100);

    printf("waking mpu6050 from sleep...\n");
    mpu6050_wake();
    sleep_ms(100);
    return 1;
}
