#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"

#include <sys/signal.h>
#include <unistd.h>

int daemonize(int argc, char *argv[], struct SERVER *server) {
    
    /* Our process ID and Session ID */
    pid_t pid;
    pid_t sid;
    
    /* Log the PID */
    FILE *pFile;
            
    int result;
    
    /* for 32 bit number null terminated */
    char buffer[11];

    if (argc > 1) {

        if (result = strcmp(argv[1], "-start") == 0) {
            
            printf("\n\nStarting TheNext...\n", pid);

            /* Fork off the parent process */
            pid = fork();
            if (pid < 0) {
                exit(EXIT_FAILURE);
            }
            
            //printf("Started on pid: %d\n\n", pid);

            /* If we got a good PID, then we can exit the parent process. */
            if (pid > 0) {
                exit(EXIT_SUCCESS);
            }

            /* Change the file mode mask */
            umask(0);

            /* Open any logs here */

            /* Create a new SID for the child process */
            sid = setsid();
            if (sid < 0) {
                /* Log the failure */
                exit(EXIT_FAILURE);
            }

            /* Change the current working directory */
            if ((chdir("/")) < 0) {
                /* Log the failure */
                exit(EXIT_FAILURE);
            }

            /* Close out the standard file descriptors */
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
            
            pFile = fopen(server->SERVER_PID, "w+");
            if (pFile != NULL) {
                fprintf(pFile, "%d", sid);
                fclose(pFile);
            }

        } else if (result = strcmp(argv[1], "-stop") == 0) {
            
            printf("\n\nStopping TheNext...\n");

            pFile = fopen(server->SERVER_PID, "r");

            if (pFile != NULL) {
                
                result = fread(buffer, 1, sizeof (buffer), pFile);
                fclose(pFile);
                buffer[result] = '\0';
                pid = atoi(buffer);
                //printf("\nStopped on pid: %d\n\n", pid);
                kill(pid, SIGKILL);
                
            }
            
            exit(EXIT_SUCCESS);

        } else if (result = strcmp(argv[1], "--version") == 0) {

            printf(
                "\n%s (%s %s) %s %s\n\n",
                "Web-Server-Alpha-Build",
		__DATE__,
		__TIME__,
                "https://twitter.com/tomviglini",
		"https://github.com/tomviglini/web-server"
            );
            
            exit(EXIT_SUCCESS);

        } else {

            printf(
                "\n"
                "%-15s %s\n"
                "%-15s %s\n"
                "%-15s %s\n"
                "%-15s %s\n"
                "\n",
                "-start",
                "start the daemon",
                "-stop",
                "stop the daemon",
                "--version",
                "return the version number and build date",
                "--help",
                "display this information"
            );
            
            exit(EXIT_SUCCESS);

        }

    } else {
        
        pid = getpid();
        printf("\npid: %d\n\n", pid);
        
    }
    
    return 0;

}
