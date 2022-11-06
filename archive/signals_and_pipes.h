#ifndef TEST_H
#define TEST_H
#include <sys/types.h>
// function declarations
void kill_child(int sig);                  // Handle signal SIGQUIT
void process_messages_of_parent(int sig);  // Handle signal SIGUSR1
void process_messages_of_child(int sig);   // Handle signal SIGUSR2
void send_message_to_pipe(char* buffer, size_t buffer_size);
int receive_message_from_pipe();
void poke_parent();
void poke_child(int cpid);

/* Used signals and their corresponding meanings.
 * SIGQUIT: parent -> child. Kill the child.
 * SIGUSR1: chlid -> parent. Parent has a new message.
 * SIGUSR2: parent -> child. Child has a new message.
 */

#define MAX_CONNECTIONS (5)
#define MAX_BUF_SIZE (200)
#define R_END (0)
#define W_END (1)
#define PREPARE_TO_READ(pipe_fd_local) (close(pipe_fd_local[W_END]))
#define PREPARE_TO_WRITE(pipe_fd_local) (close(pipe_fd_local[R_END]))
#define HAS_MESSAGE_FOR_PARENT (SIGINT)
#define HAS_MESSAGE_FOR_CHILD (SIGUSR2)
#endif  // TEST_H