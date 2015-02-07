char ipString[24];

TCPServer server = TCPServer(23);
TCPClient client;

void setup() {
    publishIpAddress();

    server.begin();
}

void loop() {
    // TODO: Read sensors
    // TODO: Read input
    // TODO: Adjust motors

    if (client.connected()) {
        // echo all available bytes back to the client
        while (client.available()) {
            char input = client.read();
            switch(input) {
                case 'w':
                    server.write("Forward");
                    break;
                case 'a':
                    server.write("Left");
                    break;
                case 's':
                    server.write("Right");
                    break;
                case 'd':
                    server.write("Backward");
                    break;
                case 'q':
                    server.write("Disconnect");
                    client.stop();
                default:
                    break;
            }

            /*delay(100);*/
        }
    } else {
        // if no client is yet connected, check for a new connection
        client = server.available();
    }
}

// Make local IP address available via web api at
// https://api.spark.io/v1/devices/<device_id>/ipAddress\?access_token\=<access_token>
void publishIpAddress() {
    IPAddress myIp = WiFi.localIP();
    sprintf(ipString, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
    Spark.variable("ipAddress", ipString, STRING);
}
