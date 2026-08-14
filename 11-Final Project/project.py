import socket
import ipaddress
import subprocess
import pathvalidate
import sys
from colorama import Fore, Back, Style


def validate_ip_address(address):
    # validating that the ip address a user enters is real
    try:
       ipaddress.ip_address(address)
       return 1
    except ValueError:
       print(Fore.RED + "  *** That IP address is not valid ***", Style.RESET_ALL)


def validate_filename(filename):
    # validating that the filename a user enters does not contain invalid characters
    try:
        pathvalidate.validate_filename(filename)
        return 1
    except pathvalidate.ValidationError:
       print(Fore.RED + "  *** That filename is not valid ***", Style.RESET_ALL)


def pingscan(address, filename):
    # ping the range of devices at the command line, send the output to null
    try:
        subprocess.check_call(["ping", "-c 1", "%s" % address], stdout=subprocess.DEVNULL)
        # write the ip of the device on successful ping to file
        filename.write(str(address))
        filename.write(",")
        return 1
    # defining if the try fails
    except subprocess.CalledProcessError as error:
        output = error.output
        return 0
    # print a message if user does ctrl-c on keyboard
    except KeyboardInterrupt:
        print(Fore.RED + "\n  *** Program Halted at Keyboard ***", Style.RESET_ALL)
        filename.close()
        sys.exit()


def arpscan(address, filename):
    # use the arp command to retrieve mac address of deteced device
    try:
        arpadd = subprocess.check_call(["arp", "%s" % address], stdout=subprocess.DEVNULL)
        # if arp returns 0, that means it found an entry, so print the info
        if arpadd == 0:
            arpadd = subprocess.check_output(["arp", "%s" % address])
            arpadd = arpadd.decode("utf-8")
            print(Fore.BLUE + f"    ╚ ARP Info: {arpadd}", end="")
    # if arp returns something other than 0, print the text to the screen, this isn't necessarily a bad result
    except subprocess.CalledProcessError as error:
            arpadd = error.output
            print(Fore.BLUE + f"    ╚ ARP Info: {arpadd}")
    # print a message if user does ctrl-c on keyboard (otherwise a main program interrupt throws a traceback error during this phase)
    except KeyboardInterrupt:
        print(Fore.RED + "\n  *** Program Halted at Keyboard ***", Style.RESET_ALL)
        filename.close()
        sys.exit()
    # write the arp result to file
    arpadd = arpadd.replace(" (", ",").replace(str(address), "").replace(") ", "").replace("at ", "").replace(" on ", ",")
    filename.write(str(arpadd).strip())
    filename.write(",")

def portscan(address, filename):
    ports = []
    # scan ports on detected device to see if any are open
    try:
        for port in range(0,445):
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            socket.setdefaulttimeout(1)
            result = sock.connect_ex((str(address),port))
            # a return of 0 is a success, so print a message with the port number
            if result == 0:
                print(Fore.YELLOW + f"      ╚ Open Port: {port}", Style.RESET_ALL)
                ports.append(port)
                # write the open port info of the device to file
                filename.write(str(port))
                filename.write("; ")
    # error when "socket" module can't find the device
    except socket.error:
        print("No device connection")
    # print a message if user does ctrl-c on keyboard (otherwise a main program interrupt throws a traceback error during this phase)
    except KeyboardInterrupt:
        print(Fore.RED + "\n  *** Program Halted at Keyboard ***", Style.RESET_ALL)
        filename.close()
        sys.exit()
    # write a new line to file
    filename.write("\n")

def main():
    # capture input from the user to define the range of ip addresses to be scanned, and verify it is a valid ip address
    while True:
        x = input("Enter FIRST IP Address in range: ")
        if validate_ip_address(x):
            break

    while True:
        y = input("Enter LAST IP Address in range: ")
        if validate_ip_address(y):
            break

    # capture a filename from the user, and verify it doesn't contain invalid characters
    while True:
        filein = input("Enter File Name: ")
        if validate_filename(filein):
            break
    
    # set the start and end ip addresses, as defined by the user input
    start = ipaddress.IPv4Address(x)
    ends = ipaddress.IPv4Address(y)
    # add the .csv extension to the filename the user entered, set mode to "append"
    file = open(filein + ".csv", "a")

    # big giant message to let the user know the scan is starting
    print("\n", Back.RED, Fore.WHITE + "====================STARTING SCAN======================", Style.RESET_ALL, "\n")

    while start <= ends:
        # if the ping returns 0, it was a success. print that a device was found
        if pingscan(start, file):
            print(Back.GREEN, Fore.WHITE + f"{start} [device found]", Style.RESET_ALL)
            # use the arp command to retrieve mac address of detected device
            arpscan(start, file)
            # scan ports on detected device
            portscan(start, file)

        # if ping returns something other than 0, it failed, so there is no device. print message to console
        else:
            print(Fore.RED + f"{start} [no connection]", Style.RESET_ALL)
        
        # add 1 to the ip address and go back to the top of the loop
        start += 1

    # big giant message to let the user know the scan is done
    print("\n", Back.RED, Fore.WHITE + "====================SCAN COMPLETE======================", Style.RESET_ALL, "\n")

    file.close()

    return 0


main()
