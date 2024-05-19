#include <StandardCplusplus.h>
#include <Arduino.h>
#include <Wire.h>
#include "CommandParser.h"

#define SLAVE_ADDRESS 9

MessageParser parser;
Map checkpointMap;
Response_CHECKPOINT_MAP resp;
Query_CHECKPOINT_MAP query;

void setMap(){
  Checkpoint checkPoint;

	checkPoint.id = 1;
	checkPoint.front_id = 2;
	checkPoint.back_id = 0;
	checkPoint.left_id = 0;
	checkPoint.right_id = 0;
	checkpointMap.addCheckPoint(checkPoint);

	checkPoint.id = 2;
	checkPoint.front_id = 3;
	checkPoint.back_id = 1;
	checkPoint.left_id = 7;
	checkPoint.right_id = 0;
	checkpointMap.addCheckPoint(checkPoint);

	checkPoint.id = 3;
	checkPoint.front_id = 8;
	checkPoint.back_id = 2;
	checkPoint.left_id = 0;
	checkPoint.right_id = 5;
	checkpointMap.addCheckPoint(checkPoint);

	checkPoint.id = 5;
	checkPoint.front_id = 0;
	checkPoint.back_id = 0;
	checkPoint.left_id = 3;
	checkPoint.right_id = 0;
	checkpointMap.addCheckPoint(checkPoint);

	checkPoint.id = 7;
	checkPoint.front_id = 0;
	checkPoint.back_id = 0;
	checkPoint.left_id = 0;
	checkPoint.right_id = 2;
	checkpointMap.addCheckPoint(checkPoint);

	checkPoint.id = 8;
	checkPoint.front_id = 0;
	checkPoint.back_id = 3;
	checkPoint.left_id = 0;
	checkPoint.right_id = 0;
	checkpointMap.addCheckPoint(checkPoint);

	checkpointMap.setPreviousCheckPoint(1);
	checkpointMap.setNextCheckPoint(2);

  //checkpointPath = checkpointMap.findPath(8);
}

void setup()
{
  Wire.begin(SLAVE_ADDRESS);
  //Wire.onReceive(receiveEvent);
  Serial.begin(9600);
      while (!Serial){
      delay(100);
    }
  
  Serial.println("Arduino Started");
  query.message_type = QueryResponseType::QUERY_CHECKPOINT_MAP;
  resp.message_type = QueryResponseType::RESPONSE_CHECKPOINT_MAP;
  resp.checkpoints = checkpointMap.getCheckPoints();
  parser.sendMessage(resp);
  delay(100);
}

void loop(){
  if (parser.isMessageCompleted() && parser.isSending()) {
    Serial.println("Message sent!");
    parser.sendMessage(resp);
  }
  else if (parser.isMessageCompleted() && parser.isReceiving()) {
    if (parser.getMessageType() == QueryResponseType::RESPONSE_CHECKPOINT_MAP) {
      resp = parser.getResponse_CHECKPOINT_MAP();
      print_Response_CHECKPOINT_MAP(resp);
    }
    else if (parser.getMessageType() == QueryResponseType::QUERY_CHECKPOINT_MAP) {
      query = parser.getQuery_CHECKPOINT_MAP();
      print_Query_CHECKPOINT_MAP(query);
    }
    parser.recvMessage();
  }

  if (parser.isReceiving()) {
    parser.read(Wire);
  }
  else if (parser.isSending()) {
    parser.write(Wire);
  }
    

}