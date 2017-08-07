
#include <giga-engine.h>

NetworkSession::NetworkSession() {
    sessionID = 0;
    socket = 0;
    player = 0;
    memset(&info, 0, sizeof(NetworkConnectionInfo));
}

NetworkSession::~NetworkSession() {
    if(socket) {
        delete socket;
    }
}

void NetworkSession::Write(NetworkMessage* msg) {
    // Get the message envelope
    NetworkMessage::NetworkEnvelope* env = msg->GetEnvelope();
    
    // Set session ID
    env->session = sessionID;
    
    // Create payload
    msg->OnSend();
    
    // Get our payload and message and write to socket
    int count = 0;
    unsigned char* message = msg->GetPayload(count);
    
    int written = socket->Write(message, count);
    assert(written == count); // Assert = message not sent in full
    free(message);
}
