#define PORT 8080
#define MSG_SIZE 400
#define PACKET_SIZE 5000
#define LOCALHOST "127.0.0.1"
#define ACKNOWLEDGEMENT "ACK\n"
#define TIMEOUT 3

typedef struct
{
    int sequence;
    int acknowledgement;
    char data[PACKET_SIZE];
} FRAME;


