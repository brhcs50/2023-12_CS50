# Brandon's Network Scanner
#### Video Demo:  <https://youtu.be/YoI1ZbHcSag>
#### Description:

# Summary
I created a network scanner that will find all devices on a given network, return their ARP information, and list open ports. This is done by pinging a range of IP Adresses and returning the successful pings, then running the ARP command, then executing the port scanning function on the successful pings. On each success, I save a summarized entry to a .csv file with a name entered by the user.

## import ipaddress (https://docs.python.org/3/library/ipaddress.html)
```
ipaddress.ip_address()
ipaddress.IPv4Address()
```
+ "Calculating" an IP address to be iterable is a tough task, so I searched for a python library that can do it.


## import subprocess (https://docs.python.org/3/library/subprocess.html)
```
subprocess.check_call()
subprocess.check_output()
```
+ I need to capture the output of the "ping" command so I can look for successful pings and save them.
+ I searched for "python how do i analyze a command line output?"
  - https://medium.com/@amarjeetaryan90/python-command-output-capture-a-comprehensive-tutorial-on-storing-output-as-variables-c578cb900c2

## import socket (https://docs.python.org/3/library/socket.html?highlight=socket#module-socket)
```
socket.socket()
socket.AF_INET
socket.SOCK_STREAM
```
+ It would be nice to search for open ports on detected devices.
+ I did a search for "finding network ports with python."
+ I used code from the simple port scanner I made before starting this project.

## import colorama (https://www.geeksforgeeks.org/print-colors-python-terminal/)
```
Fore.RED
Back.GREEN
Style.RESET_ALL
```
+ I ran a scan of my entire home network to see what a larger output would look like (**192.168.1.1 to 192.168.1.254**).
+ It would be nice to have color-coded output to help visualize success, failure, and other results.
  - I did a serch for "printing colors in python terminal."
+ I also looked for UTF "box" or "line" symbols that would organize the ARP and Open Port printouts to the IP Address more clearly.

## The ARP command
```
arp [-v] [-i if] [-H type] -a [hostname]
```
+ It would be nice to get the MAC Address of a detected device, and I can do this with ARP command.
+ I'm not doing anything special with this info, so the default command without options works fine.

## The PING command
```
ping -c 2 [ip address]
```
+ I limited the ping attempts to 2, in order to reduce the wait time for feedback.
+ The first ping might be missed, but the second one usually works.
+ The default timeout for a ping is 1 second, so I did not adjust this.

## Validating User Input
+ I need to make sure a user inputs a valid IP Address, or the ipaddress library will throw Traceback errors.
+ I did a search to see if the ipaddress library has a function to do this, but there is none.
+ Technically, IP Addresses are binary octets, so it would be possible to write code to validate this, but if it is already done, it lets me focus on the big picture.
+ I did a search for "validate an ip address in python."
  - https://www.abstractapi.com/guides/validate-ip-address-python
```
import ipaddress
def validate_ip_address(ip_string):
   try:
       ip_object = ipaddress.ip_address(ip_string)
       print("The IP address '{ip_object}' is valid.")
   except ValueError:
       print("The IP address '{ip_string}' is not valid")

validate_ip_address("127.0.0.1")
```
+ I modified this code to ```return 1``` for a successful validation, and removed the print statements.
+ I put this in a ```while True:``` loop to verify the first and last IP Addresses entered by the user.
+ I then followed a smiilar method for validating filename input, where a search returned the "pathvalidate" library. I created a function using the same logic with this module as with the IP Address.
```
import pathvalidate
def validate_filename(filename):
    try:
        pathvalidate.validate_filename(filename)
        return 1
    except pathvalidate.ValidationError:
       print(Fore.RED + "  *** That filename is not valid ***", Style.RESET_ALL)
```

# Problems
+ When running the "ping" command and getting no reply, traceback occurred: "```returned non-zero exit status 2```"
+ I searched for "subprocess.check_output returned non-zero exit status 2."
  - https://stackoverflow.com/questions/29824461/python-subprocess-calledprocesserror-returned-non-zero-exit-status-2
```
"A CalledProcessError will be raised if any non-zero exit code is returned by your called process. 
If that is okay in your python code, you can except the CalledProcessError and get any information from within 
its attributes especially the output attribute. (Look up this error in the python docs for more information.)"

    try:, except subprocess.CalledProcessError as error:
```
+ I then thought, if a successful ping returns 0, all i have to do is check the return value and print "success" for a 0, or "fail" for anything else.
+ I can direct the ping command output to /dev/null and just print the results for clarity.
  - In python this is ```"stdout=subprocess.DEVNULL"``` within the function to direct the output away from the screen.

# Final Thoughts
I initially wanted to make a webpage where a user could input an IP range and run a scan with real-time feedback in a window. The website would save the output to a SQL database, and display as a table on the website. The user could then retrieve the output at a later time if needed. This would have incorporated the user login system and databae from the Finance PSet.

However, I work full time and the scope of my project proved too large to complete in the timeframe I had. So I scaled back and decided to focus on the core functionality first, and build the website if I had time. 

I viewed the network scanner portion as the focus of my project, not the web interface. If I had viewed my project as the whole experience, I would have used a library to handle the network and port scanning, and then focused my effort on designing a nice web interface.

I quite enjoy working on a project like this that relates to my interest in computer networks, so I will likely continue developing ths idea after the course has finished.