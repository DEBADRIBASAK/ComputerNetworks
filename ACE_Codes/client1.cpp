#include "ace/SOCK_Connector.h"
#include "ace/INET_Addr.h"
#define SIZE_BUF 128
#define NO_ITERATIONS 5
class Client{
public:
Client(char *hostname, int port):remote_addr_(port,hostname)
{
data_buf_="Hello from Client";
}
//Uses a connector component `connector_’ to connect to a
//remote machine and pass the connection into a stream
//component client_stream_
int connect_to_server()
{
// Initiate blocking connection with server.
ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting connect to %s:%d\n",
remote_addr_.get_host_name(),remote_addr_.get_port_number()));
if (connector_.connect (client_stream_, remote_addr_) == -1)
ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) %p\n","connection failed"),-1);
else
ACE_DEBUG ((LM_DEBUG,"(%P|%t) connected to %s\n",
remote_addr_.get_host_name ()));
return 0;
}
//Uses a stream component to send data to the remote host.
int send_to_server()
{
// Send data to server
for(int i=0;i<NO_ITERATIONS; i++){
if (client_stream_.send_n (data_buf_, ACE_OS::strlen(data_buf_)+1, 0) == -1){
ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) %p\n","send_n"),0);
break;
}
}
//Close down the connection
close();
}
//Close down the connection properly.
int close()
{
if (client_stream_.close () == -1)
ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) %p\n","close"),-1);
else
return 0;
}
private:
ACE_SOCK_Stream client_stream_;
ACE_INET_Addr remote_addr_;
ACE_SOCK_Connector connector_;
char *data_buf_;
};
int main (int argc, char *argv[])
{
if(argc<3){
ACE_DEBUG((LM_DEBUG,”Usage %s <hostname> <port_number>\n”, argv[0]));
ACE_OS::exit(1);
}
Client client(argv[1],ACE_OS::atoi(argv[2]));
client.connect_to_server();
client.send_to_server();
}