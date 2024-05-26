#include <Arduino.h>
#include <SteeringController.h>
#include "strtod_.h"

#define SERIAL_PORT Serial

#define MOTORS_LEFT_IN1_PIN1 2
#define MOTORS_LEFT_IN2_PIN2 5
#define MOTORS_RIGHT_IN3_PIN1 4
#define MOTORS_RIGHT_IN4_PIN2 6


String serialInputBuffer;
SteeringController steeringController(255.0f, 0.0f, -255.0f);
float MaxSpeed = 0.0f;
float LeftSideSpeedRatio = 0.0f;
float RightSideSpeedRatio = 0.0f;

void setup()
{
  serialInputBuffer.reserve(128);
  SERIAL_PORT.begin(9600);
  while (!SERIAL_PORT) {
    delay(100);
  }
  
  pinMode(MOTORS_LEFT_IN1_PIN1, OUTPUT);
  pinMode(MOTORS_LEFT_IN2_PIN2, OUTPUT);
  pinMode(MOTORS_RIGHT_IN3_PIN1, OUTPUT);
  pinMode(MOTORS_RIGHT_IN4_PIN2, OUTPUT);

  steeringController.attach(MOTORS_LEFT_IN1_PIN1, MOTORS_LEFT_IN2_PIN2, MOTORS_RIGHT_IN3_PIN1, MOTORS_RIGHT_IN4_PIN2);
  steeringController.write(0.0f, 0.0f, 0.0f);
}



bool readRecordFromSerial(HardwareSerial& serialPort, String recordTermintor, String &record){
  static String inputBuffer;
  static bool terminatorFound = false;
  size_t temp_size_t;

  char tempChar, lastTerminatorCharacter;

  if (serialPort.available() <= 0) {
    //record = std::vector<char>();
    return false;
  }

  if (terminatorFound == true && inputBuffer.length() > 0) {
    terminatorFound = false;
    inputBuffer = String("");
  }
  

  lastTerminatorCharacter = recordTermintor.charAt(recordTermintor.length()-1);

  while (serialPort.available() > 0){
    tempChar = (char)serialPort.read();
    inputBuffer += tempChar;
    if (tempChar == lastTerminatorCharacter && inputBuffer.length() >= (size_t)recordTermintor.length()) {
      if (memcmp((const void*)recordTermintor.c_str(), (const void*)(inputBuffer.begin() + (size_t)((int)inputBuffer.length() - (int)recordTermintor.length())), (size_t)recordTermintor.length()) == 0) {
        terminatorFound = true;
        break;
      }
    }
  }

  if (terminatorFound) {

    if (inputBuffer.length() >= (size_t)recordTermintor.length()) {
      temp_size_t = (inputBuffer.length() - (size_t)recordTermintor.length());
      //inputBuffer.erase(inputBuffer.begin() + (inputBuffer.length() - (size_t)recordTermintor.length()), inputBuffer.end());
      inputBuffer.remove(temp_size_t, inputBuffer.length() - temp_size_t);
    }
    else {
      // If vector has less than n number of elements,
      // then delete all elements
      inputBuffer = String("");
      //inputBuffer.clear();
    //record = std::vector<char>();
    return false;
    }
    // parse the inputBuffer and load the new global variables
    record = inputBuffer;
    record += '\0';
    //record.push_back('\0');
    terminatorFound = false;
    //inputBuffer.clear();
    inputBuffer = String("");
    return true;
  }
  //record = std::vector<char>();
  record = String("");
  return false;
}


/*==============================================================================*/
/*
* str:
* C-string beginning with the representation of a floating-point number.
* endptr:
* Reference to an already allocated object of type char*, whose value is set by the function to the next character in str after the numerical value.
* This parameter can also be a null pointer, in which case it is not used.
*/

float parseNextFloat(char* str, size_t strSize, char variableTerminator, char** endptr, int* success) {
	char* nextTerminator;
	char* pEnd;
	float result = 0;

	nextTerminator = (char*)memchr(str, (int)variableTerminator, strSize);
	if (str == nextTerminator && strSize == 1) {
		if (endptr) {
			*endptr = nextTerminator;
		}
		if (success) {
			*success = 0;
		}

		return 0.0f;
	}
	if (nextTerminator) {
		*nextTerminator = '\0';
	}
	else {
		nextTerminator = str + strSize;
	}
	
	result = (float)strtod_(str, &pEnd);  
  

	if (pEnd != nextTerminator) {
		// handle incomplete parse
		pEnd += 1;
		*success = 0;
		if (endptr) {
			*endptr = pEnd;
		}
	}
	else {
		pEnd += 1;
		*success = 1;
		if (endptr) {
			*endptr = pEnd;
		}
	}
	return result;
}

/*==============================================================================*/


// 0 = success, 1 = error
int parseAndSetGlobalVariables(String& rawData, char variableTerminator = ';') {
	char* pEnd;
	int resultSuccess;
  float temp_float;

	pEnd = rawData.begin();
	temp_float = parseNextFloat(pEnd, (rawData.length() + rawData.begin()) - pEnd, variableTerminator, &pEnd, &resultSuccess);
//  if (resultSuccess == 0) {
 //   return 1;
 // }
  MaxSpeed = temp_float;

  temp_float = parseNextFloat(pEnd, (rawData.length() + rawData.begin()) - pEnd, variableTerminator, &pEnd, &resultSuccess);
  //if (resultSuccess == 0) {
  //  return 1;
  //}
  LeftSideSpeedRatio = temp_float;

  temp_float = parseNextFloat(pEnd, (rawData.length() + rawData.begin()) - pEnd, variableTerminator, &pEnd, &resultSuccess);
  //if (resultSuccess == 0) {
  //  return 1;
 // }
  RightSideSpeedRatio = temp_float;

  return 0;
}

void printGlobalVariables(HardwareSerial& serialPort){
  serialPort.print(MaxSpeed);
  serialPort.print(';');
  serialPort.print(LeftSideSpeedRatio);
  serialPort.print(';');
  serialPort.print(RightSideSpeedRatio);
  serialPort.print("\r\n");
}


void loop()
{
  if(readRecordFromSerial(SERIAL_PORT, String("\r\n"), serialInputBuffer)){
    //SERIAL_PORT.print(String("Input: "));
    //SERIAL_PORT.println(serialInputBuffer);
    if (parseAndSetGlobalVariables(serialInputBuffer, ';') == 0) {
      //printGlobalVariables(SERIAL_PORT);
    }
    else{
      //SERIAL_PORT.println(String("Parsing error"));
    }
    
    
    serialInputBuffer = String("");
    steeringController.write(MaxSpeed, LeftSideSpeedRatio, RightSideSpeedRatio);
  }
  delay(1);
}