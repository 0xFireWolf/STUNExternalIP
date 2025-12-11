//
//  STUNExternalIP.c
//  STUNExternalIP
//
//  Created by FireWolf on 2016-11-16.
//  Revised by FireWolf on 2017-02-24.
//  Revised by FireWolf on 2025-12-10.
//
//  Copyright © 2016-2025 FireWolf. All rights reserved.
//

#include "STUNExternalIP.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>

// MARK: === PRIVATE DATA STRUCTURE ===

/// RFC 5389 Section 6 STUN Message Structure
enum STUNMessageType
{
    kSTUNMessageTypeBindingRequest = 0x0001,
    kSTUNMessageTypeBindingResponse = 0x0101,
};

/// RFC 5389 Section 6 STUN Message Structure
static const uint32_t kSTUNMessageCookie = 0x2112A442;

/// RFC 5389 Section 6 STUN Message Structure
struct STUNMessageHeader
{
    /// [BE] Message Type (Binding Request / Response)
    uint16_t type;
    
    /// [BE] Payload length of this message
    uint16_t length;
    
    /// [BE] Magic Cookie
    uint32_t cookie;
    
    /// [BE] Unique Transaction ID
    uint32_t identifier[3];
};

/// RFC 5389 Section 15 STUN Attributes
enum STUNAttributeType
{
    kSTUNAttributeTypeXORMappedAddress = 0x0020,
};

/// RFC 5389 Section 15 STUN Attributes
struct STUNAttribute
{
    /// [BE] Attribute Type
    uint16_t type;
    
    /// [BE] Payload length of this attribute
    uint16_t length;

    /// The payload content
    uint8_t value[0];
};

#define IPv4_ADDRESS_FAMILY 0x01;
#define IPv6_ADDRESS_FAMILY 0x02;

/// RFC 5389 Section 15.2 XOR-MAPPED-ADDRESS
struct STUNXORMappedIPv4Address
{
    /// [BE] Reserved, Zeroed
    uint8_t reserved;

    /// [BE] The address family
    uint8_t family;

    /// [BE] The external port
    uint16_t port;

    /// [BE] The external IPv4 address
    uint32_t address;
};

///
/// Resolve the address of a STUN server
///
/// @param address The address of a STUN server
/// @param result The IPv4 address of the STUN server of interest
/// @return `true` on success, `false` otherwise.
///
static bool resolveSTUNServerAddress(const char* address, struct in_addr* result)
{
    // A list of resolved IPv4 addresses
    struct addrinfo* results = NULL;

    // Initialize the hints
    const struct addrinfo hints = { .ai_family = AF_INET, .ai_socktype = SOCK_STREAM };

    // Guard: Resolve the server address
    if (getaddrinfo(address, NULL, &hints, &results) != 0)
    {
        return false;
    }

    // Guard: Retrieve the result
    if (results == NULL)
    {
        return false;
    }

    // Read the first socket address in the list
    *result = ((struct sockaddr_in*) results->ai_addr)->sin_addr;

    // Release the linked list
    freeaddrinfo(results);

    return true;
}

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
int getPublicIPv4Address(struct STUNServer server, struct STUNClient* client, uint32_t timeout)
{
    // Guard: Create a UDP socket
    const int socketd = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketd < 0)
    {
        return kReturnSocketError;
    }

    // Guard: Set the reception timeout for the socket
    const struct timeval tv = {timeout, 0};

    if (setsockopt(socketd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval)) != 0)
    {
        close(socketd);

        return kReturnSocketError;
    }
    
    // Initialize the local socket address
    struct sockaddr_in localAddress =
    {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(0)
    };

    // Guard: Bind the socket
    if (bind(socketd, (struct sockaddr*) &localAddress, sizeof(struct sockaddr_in)) < 0)
    {
        close(socketd);
        
        return kReturnSocketError;
    }

    // Initialize the remote socket address
    struct sockaddr_in remoteAddress =
    {
        .sin_family = AF_INET,
        .sin_port = htons(server.port)
    };

    // Guard: Resolve the STUN server address
    if (!resolveSTUNServerAddress(server.address, &remoteAddress.sin_addr))
    {
        close(socketd);

        return kReturnResolverError;
    }
    
    // Construct a STUN binding request
    srand((uint32_t) time(NULL));

    struct STUNMessageHeader request = {
        .type = htons(kSTUNMessageTypeBindingRequest),
        .length = htons(0),
        .cookie = htonl(kSTUNMessageCookie),
        .identifier[0] = rand(),
        .identifier[1] = rand(),
        .identifier[2] = rand(),
    };
    
    // Guard: Send the request
    if (sendto(socketd, &request, sizeof(struct STUNMessageHeader), 0, (struct sockaddr*) &remoteAddress, sizeof(struct sockaddr_in)) != sizeof(struct STUNMessageHeader))
    {
        close(socketd);
        
        return kReturnSendError;
    }
    
    // Guard: Read the response
    uint8_t buffer[512] = {0};
    
    const ssize_t length = read(socketd, buffer, sizeof(buffer));
    
    if (length < 0)
    {
        close(socketd);
        
        return kReturnReceiveError;
    }

    // Guard: Validate the type in the response message header
    const struct STUNMessageHeader* response = (struct STUNMessageHeader*) buffer;

    if (response->type != htons(kSTUNMessageTypeBindingResponse))
    {
        close(socketd);

        return kReturnReceiveError;
    }

    // Guard: Validate the identifier in the response message header
    if (memcmp(request.identifier, response->identifier, sizeof(request.identifier)) != 0)
    {
        close(socketd);

        return kReturnReceiveError;
    }

    // Parse the response message content
    uint8_t* current = buffer + sizeof(struct STUNMessageHeader);

    while (current < buffer + length)
    {
        // The response message contains a collection of attributes (TLVs)
        const struct STUNAttribute* attribute = (struct STUNAttribute*) current;

        current += sizeof(struct STUNAttribute) + ntohs(attribute->length);

        // Guard: Look for the attribute that contains an XOR-mapped address
        if (attribute->type != ntohs(kSTUNAttributeTypeXORMappedAddress))
        {
            continue;
        }

        // Found the attribute that contains an XOR-mapped address
        const struct STUNXORMappedIPv4Address* content = (struct STUNXORMappedIPv4Address*) attribute->value;

        client->address = ntohl(content->address) ^ kSTUNMessageCookie;

        client->port = ntohs(content->port) ^ (kSTUNMessageCookie >> 16);

        close(socketd);

        return kReturnSuccess;
    }
    
    close(socketd);
    
    return kReturnReceiveError;
}

