#include "parta.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

/* handles command-line args and runs the scheduler */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("ERROR: Missing arguments\n");
        return 1;
    }

    char *alg = argv[1];

    if (strcmp(alg, "fcfs") == 0) {
        /* need at least one burst */
        if (argc < 3) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        int plen = argc - 2;
        int *bursts = malloc(sizeof(int) * plen);
        if (bursts == NULL) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        for (int i = 0; i < plen; i++) {
            bursts[i] = atoi(argv[2 + i]);
        }

        struct pcb *procs = init_procs(bursts, plen);
        free(bursts);

        if (procs == NULL) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        printf("Using FCFS\n\n");

        for (int i = 0; i < plen; i++) {
            printf("Accepted P%d: Burst %d\n", procs[i].pid, procs[i].burst_left);
        }

        int total_time = fcfs_run(procs, plen);

        /* average wait */
        int total_wait = 0;
        for (int i = 0; i < plen; i++) {
            total_wait += procs[i].wait;
        }
        double avg_wait = (double) total_wait / (double) plen;

        printf("Average wait time: %.2f\n", avg_wait);

        free(procs);
        return 0;

    } else if (strcmp(alg, "rr") == 0) {
        /* need quantum + at least one burst */
        if (argc < 4) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        int quantum = atoi(argv[2]);
        int plen = argc - 3;
        int *bursts = malloc(sizeof(int) * plen);
        if (bursts == NULL) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        for (int i = 0; i < plen; i++) {
            bursts[i] = atoi(argv[3 + i]);
        }

        struct pcb *procs = init_procs(bursts, plen);
        free(bursts);

        if (procs == NULL) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        /* match expected format */
        printf("Using RR(%d)\n\n", quantum);

        for (int i = 0; i < plen; i++) {
            printf("Accepted P%d: Burst %d\n", procs[i].pid, procs[i].burst_left);
        }

        int total_time = rr_run(procs, plen, quantum);

        /* average wait */
        int total_wait = 0;
        for (int i = 0; i < plen; i++) {
            total_wait += procs[i].wait;
        }
        double avg_wait = (double) total_wait / (double) plen;

        printf("Average wait time: %.2f\n", avg_wait);

        free(procs);
        return 0;

    } else {
        /* bad args */
        printf("ERROR: Missing arguments\n");
        return 1;
    }
}
