#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#define MSGSIZE 100

void
cmd_exec(int pfd, char *command)
{
    pid_t child;
    int p[2], to_parent, from_child;
    int child_status;
    char buffer[1024];
    ssize_t nread;

    pipe(p);
    from_child = p[0];
    to_parent = p[1];

    child = fork();
    if (child == 0) {
        /* child */
        /* close parent end of pipe */
        close(from_child);
        /* close unnecessary file descriptors */
        close(STDIN_FILENO);
        /* redirect standard output & error to pipe */
        dup2(STDOUT_FILENO, to_parent);
        dup2(STDERR_FILENO, to_parent);
        /* exec or die */
        execlp(command, command, NULL);
        //execve(command[0], &command[0], NULL);
        exit(EXIT_SUCCESS);
    }

    /* parent */
    /* close child end of pipe */
    close(to_parent);

    /* read output from child until EOF */
    while ((nread=read(from_child, &buffer[0], sizeof(buffer))) > 0) {
        write(pfd, &buffer[0], nread);
    }
    buffer[0] = '\n';
    write(pfd, &buffer[0], 1);
    close(from_child);
}
 
void parent_read(int p[])
{
    int nread;
    char buf[MSGSIZE];
 
    // write link
    close(p[1]);
 
    while (1) {
 
        // read call if return -1 then pipe is
        // empty because of fcntl
        nread = read(p[0], buf, MSGSIZE);
        switch (nread) {
        case -1:
 
            // case -1 means pipe is empty and errono
            // set EAGAIN
            if (errno == EAGAIN) {
                printf("(pipe empty)\n");
                sleep(1);
                break;
            }
 
            else {
                perror("read");
                exit(4);
            }
 
        // case 0 means all bytes are read and EOF(end of conv.)
        case 0:
            printf("End of conversation\n");
 
            // read link
            close(p[0]);
 
            exit(0);
        default:
 
            // text read
            // by default return no. of bytes
            // which read call read at that time
            //printf("MESG {\"%s\"}\n", buf);
            break;
        }
    }
}
void child_write(int p[] )
{
    int i;
    char *command1 = "lss";
    char *command2 = "lsd";
    printf("zz %s\n", command1);
    cmd_exec(p[1], command1);
    cmd_exec(p[1], command2);
 
    // read link
    close(p[0]);
}



int main()
{
    int p[2], i;
 
    // error checking for pipe
    if (pipe(p) < 0)
        exit(1);
 
    // error checking for fcntl
    if (fcntl(p[0], F_SETFL, O_NONBLOCK) < 0)
        exit(2);
 
    // continued
    switch (fork()) {
 
    // error
    case -1:
        exit(3);
 
    // 0 for child process
    case 0:
        child_write(p);
        break;
 
    default:
        parent_read(p);
        break;
    }
    return 0;
}
