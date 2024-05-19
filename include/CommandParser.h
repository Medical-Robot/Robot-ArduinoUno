#pragma once
#include <Arduino.h>
#include <vector>
//#include <string>
#include <stdint.h>
#include "Map.h"
#include <Wire.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <SoftwareSerial.h>

//static StaticJsonDocument<512> recvDoc;


typedef enum QueryResponseType {
	QUERY_CHECKPOINT_MAP,
	QUERY_COMANDA_MEDICAMENT,
	QUERY_STATUS,
	RESPONSE_CHECKPOINT_MAP,
	RESPONSE_COMANDA_MEDICAMENT,
	RESPONSE_STATUS
}QueryType;

#define QUERY_CHECKPOINT_MAP_SIZE 1
typedef struct Query_CHECKPOINT_MAP {
	uint8_t message_type;
}Query_CHECKPOINT_MAP;

#define RESPONSE_CHECKPOINT_MAP_SIZE 4
typedef struct Response_CHECKPOINT_MAP {
	uint8_t message_type;
	//int32_t n_checkpoints;
	std::vector<Checkpoint> checkpoints;
	//Checkpoint* checkpoints;
}Response_CHECKPOINT_MAP;

#define QUERY_STATUS_SIZE 15
typedef struct Query_STATUS {
	uint8_t message_type;
	int32_t prev_checkpoint;
	int32_t next_checkpoint;
	uint8_t status;
	int32_t id_comanda;
	uint8_t mod_conducere;
}Query_STATUS;

#define RESPONSE_STATUS_SIZE 17
typedef struct Response_STATUS {
	uint8_t message_type;
	int32_t prev_checkpoint;
	int32_t next_checkpoint;
	int32_t id_comanda;
	int32_t status_response;
}Response_STATUS;

#define QUERY_COMANDA_MEDICAMENT_SIZE 5
typedef struct Query_COMANDA_MEDICAMENT {
	uint8_t message_type;
	int32_t id_comanda;
}Query_COMANDA_MEDICAMENT;

#define RESPONSE_COMANDA_MEDICAMENT_SIZE 17
typedef struct Response_COMANDA_MEDICAMENT {
	uint8_t message_type;
	int32_t id_comanda;
	int32_t destiantion_checkpoint_id;
	int8_t RFID[8];
}Response_COMANDA_MEDICAMENT;


typedef struct CommsCommand {
	int8_t message_type;
	union {
		struct Query_CHECKPOINT_MAP map_query;
		struct Response_CHECKPOINT_MAP map_response;
		struct Query_STATUS status_query;
		struct Response_STATUS status_response;
		struct Query_COMANDA_MEDICAMENT comanda_medicament_query;
		struct Response_COMANDA_MEDICAMENT comanda_medicament_response;
	}data;
}CommsCommand;



static void print_Query_CHECKPOINT_MAP(Query_CHECKPOINT_MAP &struc){
	Serial.print("Query_CHECKPOINT_MAP");
	Serial.print("\tmessage_type: ");
	Serial.println(struc.message_type);
}

static void print_Response_CHECKPOINT_MAP(Response_CHECKPOINT_MAP &struc){
	
	Serial.println("Response_CHECKPOINT_MAP");
	
	Serial.println(struc.checkpoints.size());
	
	Serial.print("\tmessage_type: ");
	Serial.println(struc.message_type);
	Serial.println("\tCheckpoints");
	for (size_t i = 0; i < struc.checkpoints.size(); i++)
	{
		Serial.print("\t\t");
		Serial.println(i);
		Serial.print("\t\tid: ");
		Serial.println(struc.checkpoints[i].id);

		Serial.print("\t\tfront_id: ");
		Serial.println(struc.checkpoints[i].front_id);

		Serial.print("\t\tback_id: ");
		Serial.println(struc.checkpoints[i].back_id);

		Serial.print("\t\tleft_id: ");
		Serial.println(struc.checkpoints[i].left_id);

		Serial.print("\t\tright_id: ");
		Serial.println(struc.checkpoints[i].right_id);
		Serial.println();
	}
	
}



static JsonDocument serialize_Query_CHECKPOINT_MAP(Query_CHECKPOINT_MAP &struc){
	JsonDocument doc;
	doc["mt"] = struc.message_type;
	return doc;
}

static Query_CHECKPOINT_MAP deserialize_Query_CHECKPOINT_MAP(JsonDocument &doc){
	Query_CHECKPOINT_MAP struc;
	struc.message_type = doc["mt"];
	return struc;
}

static Response_CHECKPOINT_MAP deserialize_Response_CHECKPOINT_MAP(JsonDocument &doc){
	Response_CHECKPOINT_MAP struc;
	int n_checkpoints;
	Checkpoint checkpoint;
	
	JsonArray data = doc["cps"];
	n_checkpoints = data.size() + 3;

	struc.message_type = doc["mt"];
	struc.checkpoints.reserve(n_checkpoints);

	for (size_t i = 0; i < n_checkpoints; i++) {
		Serial.println(i);
		checkpoint.id = doc["cps"][i]["id"];
		checkpoint.front_id = doc["cps"][i]["fid"];
		checkpoint.back_id = doc["cps"][i]["bid"];
		checkpoint.left_id = doc["cps"][i]["lid"];
		checkpoint.right_id = doc["cps"][i]["rid"];
		struc.checkpoints.push_back(checkpoint);
	}
	return struc;
}

static JsonDocument serialize_Response_CHECKPOINT_MAP(Response_CHECKPOINT_MAP &struc){
	JsonDocument doc;

	doc["mt"] = struc.message_type;
	//struc.checkpoints.reserve(doc["cps"].size());
	JsonArray checkpoints = doc.createNestedArray("cps");
	for (size_t i = 0; i < struc.checkpoints.size(); i++) {
		JsonObject checkpoint1 = checkpoints.createNestedObject();
		doc.shrinkToFit();
		checkpoint1["id"] = struc.checkpoints[i].id;
		checkpoint1["fid"] = struc.checkpoints[i].front_id;
		checkpoint1["bid"] = struc.checkpoints[i].back_id;
		checkpoint1["lid"] = struc.checkpoints[i].left_id;
		checkpoint1["rid"] = struc.checkpoints[i].right_id;
	}
	
 // Create a nested JsonObject for "cps"
 /*
  JsonArray checkpoints = doc.createNestedArray("cps");

	for (size_t i = 0; i < 2; i++) {
		JsonObject checkpoint1 = checkpoints.createNestedObject();
		checkpoint1["id"] = i;
	}
	*/
	return doc;
}




class MessageParser
{
public:
	~MessageParser() {
	
	}
	MessageParser(){
		//this->buf2 = staticJsonBuffer1;
		this->clear();

	}

/*
	CommsCommand getMessage() {
		return command;
	}
	*/

	void recvMessage(){
		this->recvMessageSettings();
	}

	void sendMessage(Query_CHECKPOINT_MAP &struc){
		JsonDocument doc;
		
		this->sendMessageSettings();
		doc = serialize_Query_CHECKPOINT_MAP(struc);
		serializeJson(doc, this->buffer);
	}

	void sendMessage(Response_CHECKPOINT_MAP &struc){
		JsonDocument doc;
		
		this->sendMessageSettings();
		doc = serialize_Response_CHECKPOINT_MAP(struc);
		serializeJson(doc, this->buffer);
	}

	void read(SoftwareSerial& connection){
		int data;
		if (this->isReceiving_ == false) {
			return;
		}

		//Serial.print("=");

		while (connection.available() /*&& this->messageCompleted == false*/)
		{
			data = connection.read();
			//Serial.print(".");
			Serial.print((char)data);
			if (data == 0) {
				Serial.println();
				this->messageCompleted = true;
				DeserializationError err = deserializeJson(this->recvDoc, this->buffer);
				Serial.println(err.f_str());
				//this->recvDoc.clear();
				break;
			}
			buffer += (char)data;
			//buffer.append((char)data);
			this->bytesReadden += 1;
		}
	}

	void write(SoftwareSerial& connection){
		int data;
		if (this->isSending_ == false) {
			return;
		}

		while (connection.availableForWrite() > 0 && this->messageCompleted == false)
		{
			if (this->bytesSent == buffer.length()) {
				connection.write('\0');
				this->messageCompleted = true;
				break;
			}
			connection.write(buffer[this->bytesSent]);
			this->bytesSent += 1;
		}
	}

	bool isMessageCompleted(){
		return this->messageCompleted;
	}

	bool isSending(){
		return this->isSending_;
	}

		bool isReceiving(){
		return this->isReceiving_;
	}

	int getMessageType(){
		return (int)(this->recvDoc["mt"]);
	}

	Response_CHECKPOINT_MAP getResponse_CHECKPOINT_MAP(){
		return deserialize_Response_CHECKPOINT_MAP(this->recvDoc);
	}
	Query_CHECKPOINT_MAP getQuery_CHECKPOINT_MAP(){
		return deserialize_Query_CHECKPOINT_MAP(this->recvDoc);
	}

	void parseRecvCommand(){
	}

	String& getBuffer(){
		return this->buffer;
	}

private:
	String buffer;
	//StaticJsonDocument &buf2;
	JsonDocument recvDoc;
	//JsonDocument recvDoc;
	bool messageCompleted;
	bool isReceiving_;
	bool isSending_;
	int bytesReadden;
	int bytesSent;

	void sendMessageSettings(){
		this->messageCompleted = false;
		this->isReceiving_ = false;
		this->isSending_ = true;
		this->bytesReadden = 0;
		this->bytesSent = 0;
		//this->buffer.clear();
		this->buffer = String("");
		this->recvDoc.clear();
	}

	void recvMessageSettings(){
		this->messageCompleted = false;
		this->isReceiving_ = true;
		this->isSending_ = false;
		this->bytesReadden = 0;
		this->bytesSent = 0;
		//this->buffer.clear();
		this->buffer = String("");
		this->recvDoc.clear();
	}

	void clear(){
		this->messageCompleted = false;
		this->isReceiving_ = false;
		this->isSending_ = false;
		this->bytesReadden = 0;
		this->bytesSent = 0;
		//this->buffer.clear();
		this->buffer = String("");
		this->recvDoc.clear();
	}


};


