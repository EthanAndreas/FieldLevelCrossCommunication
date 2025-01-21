#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

#include "XCom_decode.h"
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
    int socketfd, len = 0;
    size_t n = 0;
    struct sockaddr_in serveraddr, clientaddr;
    uint8_t buffer_ts_init[BUFSIZ];
    uint8_t buffer_recv[BUFSIZ];
    XCom_Frame_t *frame_recv;

    // Creating socket
    if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    // Fill struct
    memset(&serveraddr, 0, sizeof(serveraddr));
    memset(&clientaddr, 0, sizeof(clientaddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);

    // Bind socket
    if (bind(socketfd, (const struct sockaddr *)&serveraddr,
             sizeof(serveraddr)) < 0)
    {
        perror("Bind failed.\n");
        exit(EXIT_FAILURE);
    }

    (void)n;

    // Main loop
    while (1)
    {
        // Create the packet for calculate the latency
        XCom_Frame_t *frame_ts_init;
        memset(&buffer_ts_init, 0, BUFSIZ);
        CHK_PTR(frame_ts_init = calloc(1, sizeof(XCom_Frame_t)));
        XCom_Init_TS(frame_ts_init, XCOM_TS_TYPE_INIT, 1234, 0);
        XCom_encode(*frame_ts_init, frame_ts_init->length, buffer_ts_init);
        struct timer_s timer_latency;

        // Send 5 packets to get the mean latency with the board
        long latency[5];
        for (int i = 0; i < 5; i++)
        {
            // Reset vars
            memset(&buffer_recv, 0, BUFSIZ);
            CHK_PTR(frame_recv = calloc(1, sizeof(XCom_Frame_t)));
            n = 0;

            // Start timer
            start_timer(&timer_latency);

            // Send packet
            sendto(socketfd, (uint8_t *)buffer_ts_init, frame_ts_init->length, MSG_CONFIRM, (const struct sockaddr *)&clientaddr,
                   sizeof(clientaddr));

            // Waiting for answer
            while ((n = recvfrom(socketfd, (uint8_t *)buffer_recv, BUFSIZ,
                                 MSG_WAITALL, (struct sockaddr *)&clientaddr, (socklen_t *)&len)) < 0)
            {
                // Check if it the atempted msg
                CHK_RET(XCom_decode(buffer_recv, &n, frame_recv));
                if (frame_recv->type == XCOM_TS_TYPE_INIT)
                {
                    break;
                }
            }

            // Stop timer
            end_timer(&timer_latency);

            // Add elapsed time divides by 2 to the latency array
            latency[i] = elapsed_time(&timer_latency) / 2;
        }

        // Calculate the mean latency
        long mean_latency = (latency[0] + latency[1] + latency[2] + latency[3] + latency[4]) / 5;
    }

    return EXIT_SUCCESS;
}