import SocketServer
import sqlite3

class MyTCPHandler(SocketServer.BaseRequestHandler):
    """
    The RequestHandler class for our server.

    It is instantiated once per connection to the server, and must
    override the handle() method to implement communication to the
    client.
    """

    def handle(self):
        # self.request is the TCP socket connected to the client
        self.data = self.request.recv(1024).strip()
        print "ClientIP:", self.client_address[0]
        print "Rx: ", self.data

        cx = sqlite3.connect("d:/myprog/Arduino/lm35-esp8266-client/test.db")
        cu = cx.cursor()
        cu.execute("SELECT status FROM led")
        row=cu.fetchone()
        x=row[0]
        cx.close()
        s="#LED:OFF#"
        if x==0:
            s="#LED:OFF#"
            print "Tx: ", s
        else:
            s="#LED:ON#"
            print "Tx: ", s
        self.request.sendall( s )

if __name__ == "__main__":
    HOST, PORT = "192.168.199.174", 9800

    # Create the server, binding to localhost on port 9999
    server = SocketServer.TCPServer((HOST, PORT), MyTCPHandler)
    print
    print "Started: %s   Port: %d" % ( HOST, PORT )
    print "Server: ", server
    print

    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    server.serve_forever()
