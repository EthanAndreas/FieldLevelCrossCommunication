#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

#include "XCom_encode.h"
#include "XCom_init.h"

#define PORT 3630

struct timer_s
{
    struct timespec start_time;
    struct timespec end_time;
};

void start_timer(struct timer_s *timer)
{
    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timer->start_time) < 0)
        exit(EXIT_FAILURE);
}

void end_timer(struct timer_s *timer)
{
    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timer->end_time) < 0)
        exit(EXIT_FAILURE);
}

long elapsed_time(struct timer_s *timer)
{
    return (timer->end_time.tv_sec - timer->start_time.tv_sec) * (long)1e9 + (timer->end_time.tv_nsec - timer->start_time.tv_nsec);
}

void print_buffer(uint8_t *buffer, size_t length)
{
    for (int i = 0; i < length; i++)
        printf("buffer : %d\n", buffer[i]);
}

void print_usage(char *prog_name)
{
    printf("Usage: %s\n", prog_name);
}

int main(int argc, char *argv[])
{
    // Check if the IP of the client is given
    if (argc != 1)
    {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    // Needed network vars
    int socketfd, len, n = 0;
    struct sockaddr_in serveraddr;

    // Needed var for the XCOM messages
    XCom_Frame_t *message;
    CHK_PTR(message = calloc(1, sizeof(XCom_Frame_t)));
    uint8_t *buffer;
    CHK_PTR(buffer = calloc(0, BUFSIZ));

    // Creating socket
    if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    // Fill struct
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);

    // Construct time synchro message to send and encode it
    XCom_Init_TS(message, XCOM_TS_TYPE_INIT, 123456789, 123);
    XCom_encode(*message, message->length, buffer);

    // Send time synchro message
    sendto(socketfd, (uint8_t *)buffer, message->length, MSG_CONFIRM, (const struct sockaddr *)&serveraddr,
           sizeof(serveraddr));

    // Construct pressure message to send and encode it
    XCom_Init_Pressure(message, XCOM_SEND_PRESSURE, 10, PRESSURE_UNIT_BAR, QUALITY_CODE_SIMULATE);
    XCom_encode(*message, message->length, buffer);

    // Send time synchro message
    sendto(socketfd, (uint8_t *)buffer, message->length, MSG_CONFIRM, (const struct sockaddr *)&serveraddr,
           sizeof(serveraddr));

    return EXIT_SUCCESS;
}