STUN External IP
================
##Description
A lightweight STUN client written in C to get the external IPv4 address.
##Usage
The API provided in this library is pretty straightforward. 
```
int getPublicIPAddress(struct STUNServer server, char* address);
```
This function requires a valid STUN server and a non-null buffer to store the external IP address.
It returns `0` on success or a negative number on error.
```
struct STUNServer
{
    char* address;
    
    unsigned short port;
};
```
A `STUNServer` contains the `address` and `port` number. `address` can be either an IP address or a domain name.
Check the example code in `main.c`.
####Notes
Not all STUN servers use the XOR mapped address. So this function will return `-5` if it cannot find the XOR mapped address in the response.
##Compilation
`cd` to the `STUNExternalIP` directory and type `make` to compile. Type `make clean` to clean up.
##Tested Environments
I have tested the code on macOS Sierra, Ubuntu 14.04 LTS and OpenSUSE.
Windows is probably not supported now because of the headers for socket.
##Reference
Check the [RFC5386](https://tools.ietf.org/html/rfc5389) for further information.
##License
MIT License
