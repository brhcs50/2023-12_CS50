import sys
import socket


host = input("Enter IP Address of Host to Scan: ")


try:
    for port in range(0,445):
        skt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        socket.setdefaulttimeout(1)
        result = skt.connect_ex((host,port))
        if result == 0:
            print(f"Port {port} is open")

except KeyboardInterrupt:
    print("\nProgram Cancelled by User")
    sys.exit()
