import ipaddress
import subprocess

x = input("Enter FIRST IP Address in range: ")
y = input("Enter LAST IP Address in range: ")

print("\n====================STARTING SCAN======================\n")

start = ipaddress.IPv4Address(x)
end = ipaddress.IPv4Address(y)

while start <= end:
    try:
        output = subprocess.check_call(["ping", "-c 1", "%s" % start], stdout=subprocess.DEVNULL)
    except subprocess.CalledProcessError as error:
        output = error.output

    if output == 0:
        arpadd = subprocess.check_output(["arp", "%s" % start])
        arpadd = arpadd.decode("utf-8")
        print(f"{start} [device found]")
        print(f"    ARP Info: {arpadd}")
    else:
        print(f"{start} [no connection]")
    
    start += 1
