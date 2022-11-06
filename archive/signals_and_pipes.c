#include "signals_and_pipes.h"

#include <fcntl.h>     // fcntl()
#include <signal.h>    // signals
#include <stdio.h>     // printf()
#include <stdlib.h>    // exit()
#include <string.h>    // sprintf(), strlen()
#include <sys/wait.h>  // wait()
#include <unistd.h>    // fodrk(), getpid(), pipe()
// Reference: https://www.geeksforgeeks.org/signals-c-language/

// Global variables
int pipe_fd[2];
char buf[MAX_BUF_SIZE];
int child_process_ids[MAX_CONNECTIONS];

int main() {
  int pid;
  if (pipe(pipe_fd)) {
    fprintf(stderr, "Pipe creation failed.");
    return 10;
  }

  if (fcntl(pipe_fd[R_END], F_SETFL, O_NONBLOCK) < 0) {
    fprintf(stderr, "Pipe setting of non-blocking failed.\n");
    return 11;
  }

  // Create a child process
  pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(12);
  }

  if (pid == 0) {
    // Child's work

    // Set up signal handlers
    signal(SIGQUIT, kill_child);
    signal(HAS_MESSAGE_FOR_CHILD, process_messages_of_child);

    // Wait for the setup process of the parent
    sleep(2);

    sprintf(buf, "Aloha from child!\n");
    buf[strlen(buf)] = '\0';
    send_message_to_pipe(buf, strlen(buf));
    poke_parent();

    // Sleep until an event occurs
    sleep(15);

  } else {
    // Parent's work
    child_process_ids[0] = pid;

    // Set up signal handlers
    signal(HAS_MESSAGE_FOR_PARENT, process_messages_of_parent);

    // Wait for the message from child
    sleep(10);
  }
  return 0;
}

void kill_child(int sig) {
  printf("CHILD: I am dying! QAQ\n");
  sig = sig;  // Ignore the signal and make compiler happy
  exit(0);
}

void process_messages_of_child(int sig) {
  printf("CHILD: I have a message!\n");
  fflush(stdout);
  signal(sig, process_messages_of_child);  // reset the signal
  // PREPARE_TO_READ(pipe_fd);
  while (receive_message_from_pipe() > 0) {
    // Blocking-read message from the parent.
    // until it returns 0 (EOF) or -1 (error)
    buf[MAX_BUF_SIZE - 1] = '\0';
    printf("CHILD: message received: %s", buf);
  }
}

void process_messages_of_parent(int sig) {
  printf("PARENT: I have a message!\n");
  signal(sig, process_messages_of_parent);  // reset the signal
  while (receive_message_from_pipe() > 0) {
    // Blocking-read message from the parent.
    // until it returns 0 (EOF) or -1 (error)
    buf[MAX_BUF_SIZE - 1] = '\0';
    printf("PARENT: message received: %s", buf);
  }

  printf("PARENT: test\n");

  // Send signal to the child
  sprintf(buf, "Aloha from parent!\n");
  buf[strlen(buf)] = '\0';
  printf("PARENT: Message is ready.\n");
  send_message_to_pipe(buf, strlen(buf));
  poke_child(child_process_ids[0]);
  printf("PARENT: Message is sent.\n");

  // Kill the child
  sleep(10);
  printf("PARENT: SIGQUIT sent.\n");
  kill(child_process_ids[0], SIGQUIT);

  // Wait for the dead body of the child
  int status;
  int cpid = wait(&status);
  printf("PARENT: Child process (pid=%d) terminated with status %d.\n", cpid,
         status);
}

void send_message_to_pipe(char* buffer, size_t buffer_size) {
  // close(pipe_fd[R_END]);  // close the reading end of the pipe
  write(pipe_fd[W_END], buffer, buffer_size);
  // PREPARE_TO_READ(pipe_fd);
}

int receive_message_from_pipe() {
  return read(pipe_fd[R_END], &buf, sizeof(buf));
}

void poke_parent() {
  int ppid = getpid();
  printf("CHILD: parent %d is poked.\n", ppid);
  kill(0, HAS_MESSAGE_FOR_PARENT);
  // if (kill(ppid, HAS_MESSAGE_FOR_PARENT) < 0) {
  //   printf("CHILD: failed to poke parent %d.\n", ppid);
  // }
}

void poke_child(int cpid) {
  kill(cpid, HAS_MESSAGE_FOR_CHILD);
  printf("PARENT: child %d is poked.\n", cpid);
}