import hypermedia.net.*;


UDP udp;

void setupUDP() {

  udp = new UDP( this, 15801 );
  //udp.log( true );     // <-- printout the connection activity
  udp.listen( true );
}


// change image change to python
void sendImgChange() {

  String message  = "NewImage"; // the message to send
  String ip       = "localhost";  // the remote IP address
  int port        = 15800;    // the destination port
  // send the message
  udp.send( message, ip, port );
}
