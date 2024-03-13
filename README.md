# Uselessly UDP Chat

This applications tries to implement a server and client for a chat application.

The chat protocol will be implemented using udp beyond any common sense.

## Message Protocol 

A Message is always initiated by the client.

Following message types are needed:
- connect
- send message
- get messages

message structure:

```
enum messageType{
    connect,
    send,
    read
};
struct message{
    messageType type;
    void* paylod;
    unsigned int payloadLength;
};
```

payload might be any other data structure defined by messageType;

