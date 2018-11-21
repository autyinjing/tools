/* ***********************************************************************

  > File Name: tcpconnection.h
  > Author: Aut
  > Mail: autyinjing@126.com 
  > Created Time: 2018年10月10日 星期三 19时24分37秒

 ********************************************************************** */

#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

class CTCPSocket
{
private:
    //套接字描述符
    int32_t m_socket_fd_;

    //接收缓冲区
    char *m_recv_buf_;
    int32_t m_recv_buf_size_;
    int32_t m_recv_head_;
    int32_t m_recv_tail_;

    //发送缓冲区
    char *m_send_buf_;
    int32_t m_send_buf_size_;
    int32_t m_send_head_;
    int32_t m_send_tail_;

public:
    CTCPSocket();
    virtual ~CTCPSocket();
};

#endif  //TCPCONNECTION_H
