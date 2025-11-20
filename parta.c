#include "parta.h"
#include <stdlib.h>
#include <stdio.h>

/* makes an array of pcbs from the burst list */
struct pcb* init_procs(int* bursts, int blen) {
    if (blen <= 0 || bursts == NULL) {
        return NULL;
    }

    struct pcb* procs = malloc(sizeof(struct pcb) * blen);
    if (procs == NULL) {
        return NULL;
    }

    for (int i = 0; i < blen; i++) {
        procs[i].pid = i;
        procs[i].burst_left = bursts[i];
        procs[i].wait = 0;
    }

    return procs;
}

/* optional print helper (not used in tests) */
void printall(struct pcb* procs, int plen) {
    // you can print however you want
}

/* runs one process for a set amount and adds wait time to others */
void run_proc(struct pcb* procs, int plen, int current, int amount) {
    if (procs == NULL || plen <= 0 || amount <= 0) {
        return;
    }
    if (current < 0 || current >= plen) {
        return;
    }

    /* reduce current process burst */
    int new_burst = procs[current].burst_left - amount;
    if (new_burst < 0) {
        new_burst = 0;
    }
    procs[current].burst_left = new_burst;

    /* everyone else waits */
    for (int i = 0; i < plen; i++) {
        if (i == current) {
            continue;
        }
        if (procs[i].burst_left > 0) {
            procs[i].wait += amount;
        }
    }
}

/* simple fcfs scheduling */
int fcfs_run(struct pcb* procs, int plen) {
    if (procs == NULL || plen <= 0) {
        return -1;
    }

    int time = 0;

    for (int i = 0; i < plen; i++) {
        while (procs[i].burst_left > 0) {
            int amount = procs[i].burst_left;  /* run to completion */
            run_proc(procs, plen, i, amount);
            time += amount;
        }
    }

    return time;
}

/* picks next runnable process in rr order */
int rr_next(int current, struct pcb* procs, int plen) {
    if (procs == NULL || plen <= 0) {
        return -1;
    }

    int start;
    if (current < 0 || current >= plen) {
        start = 0;
    } else {
        start = (current + 1) % plen;
    }

    int i = start;
    do {
        if (procs[i].burst_left > 0) {
            return i;
        }
        i = (i + 1) % plen;
    } while (i != start);

    /* none left */
    return -1;
}

/* round-robin scheduler */
int rr_run(struct pcb* procs, int plen, int quantum) {
    if (procs == NULL || plen <= 0 || quantum <= 0) {
        return -1;
    }

    int time = 0;
    int current = -1;

    while (1) {
        current = rr_next(current, procs, plen);
        if (current == -1) {
            break; /* all done */
        }

        int remaining = procs[current].burst_left;
        if (remaining <= 0) {
            continue;
        }

        int amount = (remaining < quantum) ? remaining : quantum;

        run_proc(procs, plen, current, amount);
        time += amount;
    }

    return time;
}
