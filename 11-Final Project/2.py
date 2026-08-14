import socket
import ipaddress
import subprocess
import sys
from colorama import Fore, Back, Style


def validate_ip_address(ip_string):
   try:
       address = ipaddress.ip_address(ip_string)
       return 1
   except ValueError:
       print(Fore.RED + "  *** That IP address is not valid ***", Style.RESET_ALL)


# capture input from the user to define the range of ip addresses to be scanned, and verify it is a valid ip address
while True:
    x = input("Enter FIRST IP Address in range: ")
    if validate_ip_address(x):
        break

while True:
    y = input("Enter LAST IP Address in range: ")
    if validate_ip_address(y):
        break

# set the start and end ip addresses, as defined by the user input
start = ipaddress.IPv4Address(x)
end = ipaddress.IPv4Address(y)

# big giant message to let the user know the scan is starting
print("\n", Back.RED, Fore.WHITE + "====================STARTING SCAN======================", Style.RESET_ALL, "\n")

while start <= end:
    # ping the range of devices at the command line, send the output to null
    try:
        output = subprocess.check_call(["ping", "-c 1", "%s" % start], stdout=subprocess.DEVNULL)
    # defining if the try fails
    except subprocess.CalledProcessError as error:
        output = error.output
    # print a message if user does ctrl-c on keyboard
    except KeyboardInterrupt:
        print("\nProgram Halted at Keyboard")
        sys.exit()

    # if the ping returns 0, it was a success. print that a device was found
    if output == 0:
        print(Back.GREEN, Fore.WHITE + f"{start} [device found]", Style.RESET_ALL)

        # use the arp command to retrieve mac address of deteced device
        try:
            arpadd = subprocess.check_call(["arp", "%s" % start], stdout=subprocess.DEVNULL)
            # if arp returns 0, that means it found an entry, so print the info
            if arpadd == 0:
                arpadd = subprocess.check_output(["arp", "%s" % start])
                arpadd = arpadd.decode("utf-8")
                print(Fore.BLUE + f"    ╚ ARP Info: {arpadd}", end="")
        # if arp returns something other than 0, print the text to the screen
        except subprocess.CalledProcessError as error:
                arpadd = error.output
                print(Fore.BLUE + f"    ╚ ARP Info: {arpadd}")

        #scan ports on detected device
        try:
            for port in range(0,443):
                sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                socket.setdefaulttimeout(1)
                result = sock.connect_ex((str(start),port))
                if result == 0:
                    print(Fore.YELLOW + f"      ╚ Open Port: {port}", Style.RESET_ALL)
        # error when "socket" can't find the device
        except socket.error:
            print("No device connection")
        # print a message if user does ctrl-c on keyboard (the main program interrupt throws a traceback error during this phase)
        except KeyboardInterrupt:
            print("\nProgram Halted at Keyboard")
            sys.exit()
        
    # if ping returns something other than 0, it failed, so there is no device. print message to console
    else:
        print(Fore.RED + f"{start} [no connection]", Style.RESET_ALL)
    
    # add 1 to the ip address and go back to the top of the loop
    start += 1

# big giant message to let the user know the scan is done
print("\n", Back.RED, Fore.WHITE + "===================SCAN COMPLETE======================", Style.RESET_ALL, "\n")
