//
//  main.c
//  ALSReadout
//
//  Created by Isaac Overacker on 10/16/12.
//  Copyright (c) 2012 Isaac Overacker. All rights reserved.
//

#include <stdio.h>
#include <IOKit/IOKitLib.h>

int main(int argc, const char * argv[])
{
    io_service_t service = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching("AppleLMUController"));

    io_connect_t port = 0;
    IOServiceOpen(service, mach_task_self(), 0, &port);

    uint32_t outputs = 2;
    uint64_t values[outputs];

    while (true)
    {
        sleep(1);

        // Output values will be on the interval [0, 67092480]
        IOConnectCallMethod(port, 0, nil, 0, nil, 0, values, &outputs, nil, 0);

        printf("%lld, %lld\n", values[0], values[1]);

    }
        
    return 0;
}

