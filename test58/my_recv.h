//
// Created by yang on 19-6-5.
//

#ifndef UNTITLED_MY_RECV_H
#define UNTITLED_MY_RECV_H

#define BUFSIZE 1024

void my_err(const char* err_string, int line);
int  my_recv(int conn_fd, char* data_buf, int len);

#endif //UNTITLED_MY_RECV_H

