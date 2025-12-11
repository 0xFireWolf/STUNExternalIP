//
//  STUNExternalIP.h
//  STUNExternalIP
//
//  Created by FireWolf on 2016-11-16.
//  Revised by FireWolf on 2017-02-24.
//
//  Copyright © 2017 FireWolf. All rights reserved.
//

#ifndef STUNExternalIP_h
#define STUNExternalIP_h

#include <stdint.h>

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

/// Enumerates all possible return codes
enum ReturnCode: int
{
    kReturnSuccess = 0,
    kReturnSocketError = -1,
    kReturnResolverError = -2,
    kReturnSendError = -3,
    kReturnReceiveError = -4,
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

#endif /* STUNExternalIP_h */
