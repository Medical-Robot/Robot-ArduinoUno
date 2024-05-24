#pragma once
#include <vector>
#include <stdint.h>
#include "Map.h"
#include "netconv.h"
#include <Wire.h>


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
	uint8_t query_response_type;
}Query_CHECKPOINT_MAP;

#define RESPONSE_CHECKPOINT_MAP_SIZE 4
typedef struct Response_CHECKPOINT_MAP {
	uint8_t query_response_type;
	int32_t n_checkpoints;
	Checkpoint* checkpoints;
}Response_CHECKPOINT_MAP;

#define QUERY_STATUS_SIZE 15
typedef struct Query_STATUS {
	uint8_t query_response_type;
	int32_t prev_checkpoint;
	int32_t next_checkpoint;
	uint8_t status;
	int32_t id_comanda;
	uint8_t mod_conducere;
}Query_STATUS;

#define RESPONSE_STATUS_SIZE 17
typedef struct Response_STATUS {
	uint8_t query_response_type;
	int32_t prev_checkpoint;
	int32_t next_checkpoint;
	int32_t id_comanda;
	int32_t status_response;
}Response_STATUS;

#define QUERY_COMANDA_MEDICAMENT_SIZE 5
typedef struct Query_COMANDA_MEDICAMENT {
	uint8_t query_response_type;
	int32_t id_comanda;
}Query_COMANDA_MEDICAMENT;

#define RESPONSE_COMANDA_MEDICAMENT_SIZE 17
typedef struct Response_COMANDA_MEDICAMENT {
	uint8_t query_response_type;
	int32_t id_comanda;
	int32_t destiantion_checkpoint_id;
	int8_t RFID[8];
}Response_COMANDA_MEDICAMENT;


static Checkpoint hton_Checkpoint(Checkpoint& struc) {
	Checkpoint result;
	result = struc;
	result.id = htonl(struc.id);
	result.left_id = htonl(struc.left_id);
	result.right_id = htonl(struc.right_id);
	result.front_id = htonl(struc.front_id);
	result.back_id = htonl(struc.back_id);
	return result;
}



typedef struct CommsCommand {
	int8_t query_response_type;
	int32_t size;
	union {
		struct Query_CHECKPOINT_MAP map_query;
		struct Response_CHECKPOINT_MAP map_response;
		struct Query_STATUS status_query;
		struct Response_STATUS status_response;
		struct Query_COMANDA_MEDICAMENT comanda_medicament_query;
		struct Response_COMANDA_MEDICAMENT comanda_medicament_response;
	}data;
}CommsCommand;

static Checkpoint ntohl_Checkpoint(Checkpoint& struc) {
	Checkpoint result;
	result = struc;
	result.id = ntohl(struc.id);
	result.left_id = ntohl(struc.left_id);
	result.right_id = ntohl(struc.right_id);
	result.front_id = ntohl(struc.front_id);
	result.back_id = ntohl(struc.back_id);
	return result;
}

static Query_CHECKPOINT_MAP hton_Query_CHECKPOINT_MAP(Query_CHECKPOINT_MAP& struc) {
	Query_CHECKPOINT_MAP result;
	result = struc;
	return result;
}

static Query_CHECKPOINT_MAP ntohl_Query_CHECKPOINT_MAP(Query_CHECKPOINT_MAP& struc) {
	Query_CHECKPOINT_MAP result;
	result = struc;
	return result;
}

static Response_CHECKPOINT_MAP hton_Response_CHECKPOINT_MAP(Response_CHECKPOINT_MAP& struc) {
	Response_CHECKPOINT_MAP result;
	result = struc;
	result.n_checkpoints = htonl(struc.n_checkpoints);
	for (size_t i = 0; i < struc.n_checkpoints; i++) {
		result.checkpoints[i] = hton_Checkpoint(result.checkpoints[i]);
	}
	return result;
}

static Response_CHECKPOINT_MAP ntohl_Response_CHECKPOINT_MAP(Response_CHECKPOINT_MAP& struc) {
	Response_CHECKPOINT_MAP result;
	result = struc;
	result.n_checkpoints = ntohl(struc.n_checkpoints);
	for (size_t i = 0; i < result.n_checkpoints; i++) {
		result.checkpoints[i] = ntohl_Checkpoint(result.checkpoints[i]);
	}
	return result;
}

static Query_STATUS hton_Query_STATUS(Query_STATUS& struc) {
	Query_STATUS result;
	result = struc;
	result.prev_checkpoint = htonl(struc.prev_checkpoint);
	result.next_checkpoint = htonl(struc.next_checkpoint);
	result.id_comanda = htonl(struc.id_comanda);
	return result;
}

static Query_STATUS ntohl_Query_STATUS(Query_STATUS& struc) {
	Query_STATUS result;
	result = struc;
	result.prev_checkpoint = ntohl(struc.prev_checkpoint);
	result.next_checkpoint = ntohl(struc.next_checkpoint);
	result.id_comanda = ntohl(struc.id_comanda);
	return result;
}

static Response_STATUS hton_Response_STATUS(Response_STATUS& struc) {
	Response_STATUS result;
	result = struc;
	result.prev_checkpoint = htonl(struc.prev_checkpoint);
	result.next_checkpoint = htonl(struc.next_checkpoint);
	result.id_comanda = htonl(struc.id_comanda);
	return result;
}

static Response_STATUS ntohl_Response_STATUS(Response_STATUS& struc) {
	Response_STATUS result;
	result = struc;
	result.prev_checkpoint = ntohl(struc.prev_checkpoint);
	result.next_checkpoint = ntohl(struc.next_checkpoint);
	result.id_comanda = ntohl(struc.id_comanda);
	return result;
}

static Query_COMANDA_MEDICAMENT hton_Query_COMANDA_MEDICAMENT(Query_COMANDA_MEDICAMENT& struc) {
	Query_COMANDA_MEDICAMENT result;
	result = struc;
	result.id_comanda = htonl(struc.id_comanda);
	return result;
}

static Query_COMANDA_MEDICAMENT ntohl_Query_COMANDA_MEDICAMENT(Query_COMANDA_MEDICAMENT& struc) {
	Query_COMANDA_MEDICAMENT result;
	result = struc;
	result.id_comanda = ntohl(struc.id_comanda);
	return result;
}

static Response_COMANDA_MEDICAMENT hton_Response_COMANDA_MEDICAMENT(Response_COMANDA_MEDICAMENT& struc) {
	Response_COMANDA_MEDICAMENT result;
	result = struc;
	result.id_comanda = htonl(struc.id_comanda);
	result.destiantion_checkpoint_id = htonl(struc.destiantion_checkpoint_id);
	for (size_t i = 0; i < sizeof(result.RFID); i++) {
		result.RFID[i] = struc.RFID[sizeof(result.RFID) - i];
	}
	return result;
}

static Response_COMANDA_MEDICAMENT ntohl_Response_COMANDA_MEDICAMENT(Response_COMANDA_MEDICAMENT& struc) {
	Response_COMANDA_MEDICAMENT result;
	result = struc;
	result.id_comanda = ntohl(struc.id_comanda);
	result.destiantion_checkpoint_id = ntohl(struc.destiantion_checkpoint_id);
	for (size_t i = 0; i < sizeof(result.RFID); i++) {
		result.RFID[i] = struc.RFID[sizeof(result.RFID) - i];
	}
	return result;
}

class CommandParser
{
public:
	~CommandParser() {
	}
	CommandParser() {

	}


	CommsCommand getCommand() {
		return command;
	}

	void read(TwoWire& connection){
		int data;
		if (connection.available() <= 0) {
			return;
		}
		
		if(this->bytesReadden <= 0 && connection.available()){
			data = connection.read();
			this->command.query_response_type = (int8_t)data;
		}

		if(this->startedReceiving > 0 && this->startedReceiving < 5 && connection.available()){
			while (connection.available() && this->bytesReadden < 5)
			{
				data = connection.read();
				((char*)(&(this->command.size)))[this->bytesReadden - 1] = (char)data;

				this->bytesReadden += 1;
			}
			if (this->bytesReadden >= 5) {
				this->command.size = ntohl(this->command.size);
				this->dataPtr = new char[this->command.size];
			}
		}

		while (connection.available() && (this->bytesReadden - 5) < this->command.size)
		{
			data = connection.read();
			((char*)(this->dataPtr))[this->bytesReadden-5] = (char)data;

			this->bytesReadden += 1;
		}

		if ((this->bytesReadden - 5) >= this->command.size && this->commandCompleted == false) {
			this->commandCompleted = true;
			this->parseRecvCommand();
		}
	}

	typedef enum QueryResponseType {
	QUERY_CHECKPOINT_MAP,
	QUERY_COMANDA_MEDICAMENT,
	QUERY_STATUS,
	RESPONSE_CHECKPOINT_MAP,
	RESPONSE_COMANDA_MEDICAMENT,
	RESPONSE_STATUS
}QueryType;

	void parseRecvCommand(){
		switch (this->command.query_response_type)
		{
		case QueryResponseType::RESPONSE_COMANDA_MEDICAMENT:
			this->command.data.comanda_medicament_response = ntohl_Response_COMANDA_MEDICAMENT(*((Response_COMANDA_MEDICAMENT*)(this->dataPtr)));
			delete this->dataPtr;
			break;
		
		case QueryResponseType::RESPONSE_STATUS:
			this->command.data.status_response = ntohl_Response_STATUS(*((Response_STATUS*)(this->dataPtr)));
			delete this->dataPtr;
			break;

		case QueryResponseType::RESPONSE_CHECKPOINT_MAP:
			delete this->dataPtr;
			break;
		
		default:
			break;
		}
	}

private:
	void* dataPtr;
	bool startedReceiving;
	bool commandCompleted;
	bool isReceiving;
	bool isSending;
	int bytesReadden;
	int bytesSent;
	int commandSize;
	CommsCommand command;


};


