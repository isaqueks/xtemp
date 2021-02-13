#include "cpu_temp.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int get_cpu_temp() {
    FILE *fp;
    char stdout_output[512];

    /* Open the command for reading. */
    fp = popen("sensors | grep Package", "r");
    if (fp == NULL) {
        printf("Failed to run sensors.\n" );
        exit(1);
    }

    /*
        Something like:
        Package id 0:  +74.0°C  (high = +86.0°C, crit = +100.0°C)
    */
    char buf[16];
    int buf_i = 0;

    int temperature = -1;

    while (fgets(stdout_output, sizeof(stdout_output), fp) != NULL) {

        char* plus_sign = strchr(stdout_output, '+');
        char* celsius_deg = strchr(stdout_output, '.');

        while((int)(++plus_sign) < (int)celsius_deg) {
            buf[buf_i++] = *(plus_sign);
        }
        buf[buf_i] = 0x00;

        // printf("%s\n", buf);
        sscanf(buf, "%d", &temperature);
    }

    /* close */
    pclose(fp);

    return temperature;
}