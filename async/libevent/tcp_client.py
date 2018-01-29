import socket
import sys

def start_tcp_client(ip, port):
    #server port and ip
    server_ip = ip
    server_port = port

    tcp_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        tcp_client.connect((server_ip, server_port))
    except socket.error:
        print "fail to setup socket connection"

    #tcp_client.sendall("echo message")
    print 'reading...............'
    print tcp_client.recv(1024)
    tcp_client.close()

if __name__ == '__main__':
    start_tcp_client('127.0.0.1', 9995)
