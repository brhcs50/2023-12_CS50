import ipaddress
import subprocess

x = input("Enter FIRST IP Address in range: ")
marty = ipaddress.IPv4Address(x)

# 3 pings, "-q" will only report summary if needed
output = subprocess.check_output(["ping", "-c 1", "%s" % marty])
output = output.decode("utf-8")
print(output)