#pragma once
#include <SteeringController.h>
#include <LineSensors.h>

static void takeRight(float speed, SteeringController& steeringController, LineSensors& lineSensor, float blackThreshold){
    do {
        steeringController.write(speed, 1.0f, -1.0f);
        lineSensor.read();
    } while (lineSensor.getMaxValue().y > blackThreshold);

    do {
        steeringController.write(speed, 1.0f, 1.0f);
        lineSensor.read();
    } while (lineSensor.getMaxValue().y < blackThreshold);
}


static void takeLeft(float speed, SteeringController& steeringController, LineSensors& lineSensor, float blackThreshold){
    do {
        steeringController.write(speed, -1.0f, 1.0f);
        lineSensor.read();
    } while (lineSensor.getMaxValue().y > blackThreshold);

    do {
        steeringController.write(speed, 1.0f, 1.0f);
        lineSensor.read();
    } while (lineSensor.getMaxValue().y < blackThreshold);
}
