/*
* Brian R Taylor
* brian.taylor@bolderflight.com
* 
* Copyright (c) 2020 Bolder Flight Systems
*/

#include "mpu9250/mpu9250.h"
#include "mcu_hil_defs/mcu_hil_defs.h"

/* Mpu9250 object using SPI */
sensors::Mpu9250 mpu9250(&MPU9250_SPI, MPU9250_SPI_CS);
/* Data acquisition ISR */
void imu_isr() {
  /* Check if data read */
  if (mpu9250.Read()) {
    /* Print data */
    Imu imu = mpu9250.imu();
    Mag mag = mpu9250.mag();
    Temperature t = mpu9250.die_temperature();
    Serial.print(imu.accel.x_mps2());
    Serial.print("\t");
    Serial.print(imu.accel.y_mps2());
    Serial.print("\t");
    Serial.print(imu.accel.z_mps2());
    Serial.print("\t");
    Serial.print(imu.gyro.x_radps());
    Serial.print("\t");
    Serial.print(imu.gyro.y_radps());
    Serial.print("\t");
    Serial.print(imu.gyro.z_radps());
    Serial.print("\t");
    Serial.print(mag.x_ut());
    Serial.print("\t");
    Serial.print(mag.y_ut());
    Serial.print("\t");
    Serial.print(mag.z_ut());
    Serial.print("\t");
    Serial.print(t.c());
    Serial.print("\n");
  }
}

int main() {
  /* Serial to display data */
  Serial.begin(115200);
  while(!Serial) {}
  /* Start communicating with MPU-9250 */
  bool status = mpu9250.Begin();
  if (!status) {
    Serial.println("ERROR: unable to communicate with MPU-9250");
    while(1) {}
  }
  /* Set sample rate divider for 50 Hz */
  status = mpu9250.sample_rate_divider(19);
  if (!status) {
    Serial.println("ERROR: unable to setup sample rate divider");
    while(1) {}
  }
  /* Enable the data ready interrupt */
  status = mpu9250.EnableDrdyInt();
  if (!status) {
    Serial.println("ERROR: unable to set data ready interrupt");
    while(1) {}
  }
  /* Setup callback for data ready interrupt */
  mpu9250.DrdyCallback(MPU9250_SPI_INT, imu_isr);
  while (1) {}
}

