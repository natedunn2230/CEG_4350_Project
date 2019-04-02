import socket
import struct

HOST = "127.0.0.1"
PORT = 8082

# consumer socket settings
consumer_socket = socket.socket()
consumer_socket.connect((HOST, PORT))

# packer is used to unpack binary data into tuple
binary_packer = struct.Struct('I')

# file to write consumer data to
f = open("ConsumerOutput.txt", "w")

while True:
    # recieve data in chunks. Each chunk is the size of 
    # the packed integer tuple
    raw_data = consumer_socket.recv(binary_packer.size)

    # if there is no data on next data chunk consumption,
    # break connection loop
    if not raw_data:
        break

    # extract the integer from the upacked tuple
    (data ,) = binary_packer.unpack(raw_data)
    
    # write value to file to compare with producer to verify 
    # correct communication
    f.write("{}\n".format(data))
