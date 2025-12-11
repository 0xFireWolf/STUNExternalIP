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
    /// The server address
    const char* address;

    /// The server port in host endian
    uint16_t port;
};

///
/// Get the external IPv4 address
///
/// @param server A valid STUN server
/// @param address A non-null buffer to store the public IPv4 address
/// @return 0 on success.
/// @warning This function returns
///          -1 if failed to bind the socket;
///          -2 if failed to resolve the given STUN server;
///          -3 if failed to send the STUN request;
///          -4 if failed to read from the socket (and timed out; default = 5s);
///          -5 if failed to get the external address.
///
int getPublicIPAddress(struct STUNServer server, char* address);

#endif /* STUNExternalIP_h */
