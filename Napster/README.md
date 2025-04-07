# Napster
client.c: after the client starts, it should tell the server what files it has. Then the server will store this information for this client. Whenever there is a client that wants to query for a specific file, it sends a query message to the server and the server returns the addresses of clients that have this file. The connection between clients are based on TCP. The clients create a background thread to work as a server to provide file download services to other clients. Additionally, this server uses poll() to achieve concurrency among concurrent clients.

server.c: the server maintains an unsigned integer variable for each client, in which each bit indicates whether this client has this file. For example, client1 has 01.txt and 31.txt, then the bitmap corresponds to this client is “0100 0000 0000 0000 0000 0000 0000 0001” in binary format. The communication between the client and the server uses rdt3.0 on UDP, i.e., there is no permanent connection between the clients and the directory server.

REGISTER: is sent to the server to register the ip and port information. Require ACK.
UPDATE: is sent to the server to update the available file bitmap corresponds to the ip and port. Require ACK.
QUERY: is sent to the server to query the accessible p2p clients for a file.
RESPONSE: is sent to the client to provide an available ip and port.
FINISH: is sent to the client to indicate all ips and ports are returned.
GET: is sent to a p2p client to get the above queried file.
