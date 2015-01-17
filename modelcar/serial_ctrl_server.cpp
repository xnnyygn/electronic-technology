#include "Arduino.h"
#include "serial_ctrl_server.h"

void SerialCtrlServer::init() {
  readStatus = STATUS_START;
  protocolType = -1;
  payloadLength = -1;
  currentReadCount = 0;
  
  // clear payload buffer
  for(int i = 0; i < PAYLOAD_BUFFER_SIZE; i++) {
    payloadBuffer[i] = 0;
  }
}

void SerialCtrlServer::done(int returnCode) {
  Serial.write(returnCode);
  
  init();
}

int SerialCtrlServer::read() {
  int nBytes = Serial.available();
  if(readStatus == STATUS_DONE || nBytes == 0) {
    return 0;
  }
  
  for(int i = 0; i < nBytes; i++) {
    int incomingByte = Serial.read();
    if(readStatus == STATUS_START && incomingByte == SYNC_BODY) {
      readStatus = STATUS_SYNC_BODY;
    }else if(readStatus == STATUS_SYNC_BODY && incomingByte == SYNC_END) {
      readStatus = STATUS_SYNC_END;
    }else if(readStatus == STATUS_SYNC_END) {
      protocolType = incomingByte;
      readStatus = STATUS_PROTOCOL;
    }else if(readStatus == STATUS_PROTOCOL) {
      payloadLength = incomingByte;
      readStatus = STATUS_PAYLOAD_LENGTH;
    }else if(readStatus == STATUS_PAYLOAD_LENGTH) {
      if(currentReadCount > PAYLOAD_BUFFER_SIZE) {
        readStatus = STATUS_DONE;
      }else {
        if(currentReadCount < payloadLength) {
          payloadBuffer[currentReadCount++] = incomingByte;
        }
        if(currentReadCount == payloadLength) {
          readStatus = STATUS_DONE;
        }
      }
    }
  }
  return nBytes;
}

bool SerialCtrlServer::cmdAvailable() {
  return readStatus == STATUS_DONE;
}

int SerialCtrlServer::getProtocolType() {
  return protocolType;
}

int SerialCtrlServer::getPayloadLength() {
  return payloadLength;
}

byte SerialCtrlServer::getPayloadByte(int index) {
  return index >= payloadLength ? 0 : payloadBuffer[index];
}
