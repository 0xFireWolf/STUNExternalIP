//
//  main.c
//  STUNExternalIP
//
//  Created by FireWolf on 2017-02-24.
//  Revised by FireWolf on 2025-12-10.
//  Copyright © 2017-2025 FireWolf. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "STUNExternalIP.h"

static struct STUNServer kServers[] =
{
    {"stun.l.google.com" , 19302},
    {"stun1.l.google.com", 19302},
    {"stun2.l.google.com", 19302},
    {"stun3.l.google.com", 19302},
    {"stun4.l.google.com", 19302},
    {"stun.wtfismyip.com", 3478 },
    {"stun.bcs2005.net"  , 3478 },
};

static const size_t kServerCount = sizeof(kServers) / sizeof(struct STUNServer);

static const uint32_t kDefaultTimeout = 5;

int main(int argc, const char * argv[])
{
    for (size_t index = 0; index < kServerCount; index++)
    {
        const struct STUNServer server = kServers[index];

        struct STUNClient client = {0};
        
        const int result = getPublicIPv4Address(server, &client, kDefaultTimeout);
        
        if (result != 0)
        {
            printf("[%s:%u]: Failed. Error: %d\n",
                   server.address,
                   server.port,
                   result);
        }
        else
        {
            printf("[%s:%u]: Public IP: %u.%u.%u.%u, Port: %u\n",
                   server.address,
                   server.port,
                   client.address >> 24 & 0xFF,
                   client.address >> 16 & 0xFF,
                   client.address >> 8 & 0xFF,
                   client.address & 0xFF,
                   client.port);
        }
    }
    
    return 0;
}
