# STUN External IP

## Description

A lightweight STUN client written in C to get the external IPv4 address and port number.

## Usage

The API provided by this library is pretty straightforward. 

```c++
/// Represents a STUN server
struct STUNServer
{
    /// The server address (which can be a host name of an IP address)
    const char* address;

    /// The server port in host endian
    uint16_t port;
};

/// Represents a STUN client
struct STUNClient
{
    /// The client IPv4 address in host endian (4 octets)
    uint32_t address;

    /// The client port in host endian
    uint16_t port;
};

///
/// Get the external IPv4 address
///
/// @param server A STUN server
/// @param client A non-null STUN client that stores the IPv4 address and port number on return
/// @param timeout Specify the timeout in seconds waiting for the response message
/// @return `kReturnSuccess` on success;
///         `kReturnSocketError` if failed to create the socket;
///         `kReturnSocketError` if failed to set the reception timeout for the socket;
///         `kReturnSocketError` if failed to bind the socket;
///         `kReturnResolverError` if failed to resolve the address of the given STUN server;
///         `kReturnSendError` if failed to send the request message to the given STUN server;
///         `kReturnReceiveError` if failed to read the response message from the socket;
///         `kReturnReceiveError` if failed to parse the response message returned by the given STUN server.
///
int getPublicIPv4Address(struct STUNServer server, struct STUNClient* client, uint32_t timeout);
```

> [!CAUTION]
> Not all STUN servers use the XOR mapped address. This function returns `kReturnReceiveError` if it cannot find the XOR mapped address in the response message.

## Compilation

A `CMakeLists.txt` file is provided.

```bash
cd /path/to/STUNExternalIP
cmake -S . -B build
cmake --build build
```

## Tested Environments

I have tested the code on macOS Sierra, Ubuntu 14.04 LTS and OpenSUSE.
Windows is probably not supported now because of the headers for socket.

## Reference

Check the [RFC5386](https://tools.ietf.org/html/rfc5389) for further information.

## Updates

### Dec 10, 2025
I wrote this tiny client in late 2016, and it seems people are still interested in this repository in 2025. 
As such, I have revised the implementation to remove unnecessary heap allocations and add support for retrieving the external port number.
Note that the code is mainly for demonstration purposes. 
If you plan to use this in real-world application, please consider using a more robust random number generator for the identifier in the request message.

## License

MIT License
