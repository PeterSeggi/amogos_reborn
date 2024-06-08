#ifndef PIPE_H
#define PIPE_H

/**
*@brief     Creates a unidirectional data channel.
*@param[in] pipefd pipefd[0] to be used as read end, pipefd[1] as write end.
*@return    0 on success, -1 upon failure.
*/
int pipe(int pipefd[2]);

/**
*@brief     Reads the buffer of the desired pipe.
*@param[in] fd Desired pipe read-end.
*@param[in] buffer Buffer to leave the information read.
*@param[in] amount Amount to be read.
*@return    Amount of bytes successfully read, -1 upon failure(invalid fd).
*@note      If returns 0, theres nothing left to read forever(write-end is closed and theres no content left in buffer).
*/
int read_pipe(int fd, char *buffer, uint16_t amount);

/**
*@brief     Writes the buffer of the desired pipe.
*@param[in] fd Desired pipe write-end.
*@param[in] message Message to writte onto the buffer.
*@param[in] amount Amount to be written from message.
*@return    Amount of bytes successfully written, -1 upon failure(invalid fd).
*@note      If returns 0, the read-end of the pipe is already closed.
*/
int write_pipe(int fd, char *message, uint16_t length);

/**
*@brief     Closes the desired fd of a pipe. If both are closed, deletes the corresponding pipe.
*@param[in] fd Desired file descriptor to close.
*@return    0 on success, -1 upon failure.
*/
int pclose(int fd);

#endif //PIPE_H