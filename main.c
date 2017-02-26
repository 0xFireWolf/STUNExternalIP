//
//  main.c
//  STUNExternalIP
//
//  Created by FireWolf on 2017-02-24.
//  Copyright © 2017 FireWolf. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "STUNExternalIP.h"

int main(int argc, const char * argv[])
{
    struct STUNServer servers[14] = {
        {"stun.l.google.com" , 19302}, {"stun.l.google.com" , 19305},
        {"stun1.l.google.com", 19302}, {"stun1.l.google.com", 19305},
        {"stun2.l.google.com", 19302}, {"stun2.l.google.com", 19305},
        {"stun3.l.google.com", 19302}, {"stun3.l.google.com", 19305},
        {"stun4.l.google.com", 19302}, {"stun4.l.google.com", 19305},
        {"stun.wtfismyip.com", 3478 }, {"stun.bcs2005.net"  , 3478 },
        {"numb.viagenie.ca"  , 3478 }, {"173.194.202.127"   , 19302}};
    
    for (int index = 0; index < 14; index++)
    {
        char* address = malloc(sizeof(char) * 100);
        
        bzero(address, 100);
        
        struct STUNServer server = servers[index];
        
        int retVal = getPublicIPAddress(server, address);
        
        if (retVal != 0)
        {
            printf("%s: Failed. Error: %d\n", server.address, retVal);
        }
        else
        {
            printf("%s: Public IP: %s\n", server.address, address);
        }
        
        free(address);
    }
    
    return 0;
}
