import socket
import sys
import struct
import random

HOST = "127.0.0.1"
PORT = 8082
DATA_SIZE = 100

producer_socket = socket.socket()

# producer socket settins
producer_socket.settimeout(10)
producer_socket.bind((HOST, PORT))
producer_socket.listen(1)

# file for printing generated data to
producer_file = None

while True:
    
    # file to write producer data to
    producer_file = open("ProducerOutput.txt", "w")

    # wait for consumer connection, timeout after 10 seconds
    try:
        client, address = producer_socket.accept()
        
    except Exception:
        print("Connection timed out")
        break

    print("Connected to {}".format(address))
    
    # send 100 random integers (range 1, 10000) to the consumer process
    for i in range(DATA_SIZE):
        data = random.randint(0, 10000)

        # convert data into binary data to be sent over connection
        raw_data = struct.Struct('I').pack((data))
        client.send(raw_data)

        # write the integer to a file to compare with the consumer to
        # verify correct communication
        producer_file.write("{}\n".format(data))

    # close connection with consumer after data is sent
    client.close()


