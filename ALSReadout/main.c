#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <IOKit/IOKitLib.h>

static void handleCtrlC(int sig, siginfo_t* siginfo, void* context)
{
    exit(0);
}

int main(int argc, const char * argv[])
{
    struct sigaction action;
    memset(&action, '\0', sizeof(action));
    action.sa_sigaction = &handleCtrlC;
    action.sa_flags = SA_SIGINFO;
    if (sigaction(SIGINT, &action, NULL) < 0)
    {
        perror("sigaction");
        return 1;
    }

    io_service_t service = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching("AppleLMUController"));

    io_connect_t port = 0;
    IOServiceOpen(service, mach_task_self(), 0, &port);

    uint32_t outputs = 2;
    uint64_t values[outputs];

    uint64_t lastReading;
    time_t timeOfLastChange;
    struct tm* localTime;

    while (1)
    {
        // Output values will be on the interval [0, 67092480]
        IOConnectCallMethod(port, 0, nil, 0, nil, 0, values, &outputs, nil, 0);

        if (values[0] != lastReading)
        {
            lastReading = values[0];
            timeOfLastChange = time(NULL);
            localTime = localtime(&timeOfLastChange);
            
            fprintf(stdout, "%s%lld\n", asctime(localTime), lastReading);
        }

        sleep(1);
    }
        
    return 0;
}

