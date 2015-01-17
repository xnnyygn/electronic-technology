#include "Arduino.h"

#define PAYLOAD_BUFFER_SIZE 16

class SerialCtrlServer {
  
  private:
    const int STATUS_START = 0;
    const int STATUS_SYNC_BODY = 1;
    const int STATUS_SYNC_END = 2;
    const int STATUS_PROTOCOL = 3;
    const int STATUS_PAYLOAD_LENGTH = 4;
    const int STATUS_DONE = 5;
    
    const int SYNC_BODY = 0;
    const int SYNC_END = 255;
    
    int readStatus;
    int protocolType;
    int payloadLength;
    int currentReadCount;
    byte payloadBuffer[PAYLOAD_BUFFER_SIZE];
    
  public:
  
    static const int RETURN_CODE_OK = 48;
    static const int RETURN_CODE_UNKNOWN_PROTOCOL = 49;
    
    void init();
    int read();
    bool cmdAvailable();
    int getProtocolType();
    int getPayloadLength();
    byte getPayloadByte(int index);
    void done(int returnCode);
    
};
