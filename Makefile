Server
      compile: g++ server.cpp -o server 
      run: ./server port_no

Client
      compile: g++ client.cpp -o cli 
      run: ./cli server_ip_addr port_no

The uplink and downlink is calculated by transmitting the contents of a file from client to server and server to client respectively.  

Run the executables server and cli.
Start the server first.Pass the port nuber to be used as a command line argument. Once it starts it will remain idle until client wants to establish a connection. Start the client and pass the port number and the server ip address which is the ip address of the machine on which the server is running. 
