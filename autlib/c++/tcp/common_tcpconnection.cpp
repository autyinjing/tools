
#include "Common/common_tcpconnection.h"
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <netinet/tcp.h>

namespace GameJayo { namespace Server {

CTCPSocket::CTCPSocket()
{
	m_socket_fd = -1;
	m_socket_type = enmSocketTypes_listen; //
	m_socket_status = enmTCPConnectionStatus_closed;

	m_recv_head = 0;
	m_recv_tail = 0;

	m_send_head = 0;
	m_send_tail = 0;

	m_4bytes = false;

	m_send_buffer_size = common_buffer_size;
	m_recv_buffer_size = common_buffer_size;
	m_send_buffer = NULL;
	m_recv_buffer = NULL;

#ifdef __MULTI_THREAD__
	pthread_mutex_init(&m_mutex, NULL);
#endif
	
}

int32_t CTCPSocket::initialize(int32_t send_buffer_size, int32_t recv_buffer_size)
{
	if (NULL == m_send_buffer || send_buffer_size != m_send_buffer_size)
	{
		m_send_buffer_size = send_buffer_size;
		if (NULL != m_send_buffer)
		{
			delete [] m_send_buffer;
			m_send_buffer = NULL;
		}
		m_send_buffer = new char [m_send_buffer_size+1];
        memset(m_send_buffer, 0, sizeof(char)*(m_send_buffer_size+1));
	}
	
	if (NULL == m_recv_buffer || recv_buffer_size != m_recv_buffer_size)
	{
		m_recv_buffer_size = recv_buffer_size;
		if (NULL != m_recv_buffer)
		{
			delete [] m_recv_buffer;
			m_recv_buffer = NULL;
		}
		m_recv_buffer = new char [m_recv_buffer_size+1];
        memset(m_recv_buffer, 0, sizeof(char)*(m_recv_buffer_size+1));
	}

	m_send_head = 0;
	m_send_tail = 0;
	m_recv_head = 0;
	m_recv_tail = 0;

	return success;
}

//
CTCPSocket::~CTCPSocket()
{
	if (0 < m_socket_fd && (enmTCPConnectionStatus_opened == m_socket_status || enmTCPConnectionStatus_connected == m_socket_status))
	{
		close_socket();
	}
	clear();
}

//close the tcp socket
void CTCPSocket::close_socket()
{
    AutoLock lock(&m_locker_close);

    if (m_socket_fd < 0)
    {
        return;
    }

    close(m_socket_fd);

    m_socket_fd = -1;
    m_socket_status = enmTCPConnectionStatus_closed;
}

int32_t CTCPSocket::get_fd()
{
	int32_t fd = -1;

//#ifdef __MULTI_THREAD__
//	pthread_mutex_lock(&m_mutex);
//#endif

	fd = m_socket_fd;


//#ifdef __MULTI_THREAD__
//	pthread_mutex_unlock(&m_mutex);
//#endif

	return fd;
}

enmTCPConnectionStatus CTCPSocket::get_status()
{
	enmTCPConnectionStatus status = enmTCPConnectionStatus_closed;

//#ifdef __MULTI_THREAD__
//	pthread_mutex_lock(&m_mutex);
//#endif

	status = m_socket_status;
	
//#ifdef __MULTI_THREAD__
//	pthread_mutex_unlock(&m_mutex);
//#endif

	return status;
}

int32_t CTCPSocket::open_as_server( uint16_t port, char* ip /*= NULL*/ )
{
	//firstly, close the socket
	if (enmTCPConnectionStatus_closed != m_socket_status && m_socket_fd > 0)
	{
		close_socket();
	}

	//open socket
	m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket_fd < 0)
	{
        printf("open socket failed, m_socket_fd < 0, with code[%d]\n", errno);
		m_socket_fd = -1;
		m_socket_status = enmTCPConnectionStatus_closed;
		return -1;
	}

	int flags = 1;
	struct linger ling = {0, 0};
	setsockopt(m_socket_fd, SOL_SOCKET, SO_KEEPALIVE, &flags, sizeof(flags));
	setsockopt(m_socket_fd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));

	setsockopt(m_socket_fd, IPPROTO_TCP, TCP_NODELAY, &flags, sizeof(flags)); //set TCP_CORK

	//


	if (0 != setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &flags, (int)sizeof(flags)))
	{
        printf("setsockopt failed with code[%d]\n", errno);
		close_socket();
		return -1;
	}

	//
	struct sockaddr_in stSocketAddr;
	memset(&stSocketAddr, 0x0, sizeof(stSocketAddr));
	stSocketAddr.sin_family = AF_INET;
	if (NULL != ip)
	{
		stSocketAddr.sin_addr.s_addr = inet_addr(ip);
	}
	else
	{
		stSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	}

	stSocketAddr.sin_port = (u_short)htons(port);
	socklen_t addrSize = socklen_t(sizeof(stSocketAddr));
	if (0 != bind(m_socket_fd, (const sockaddr*) &stSocketAddr, addrSize))
	{
        printf("bind failed with code[%d]\n", errno);
		close_socket();
		return -1;
	}

	if (0 != listen(m_socket_fd, 128))
	{
        printf("listen failed with code[%d]\n", errno);
		close_socket();
		return -1;
	}

	//设置为非阻塞
	set_non_block(m_socket_fd);

	m_socket_type = enmSocketTypes_listen; //监听socket
	m_socket_status = enmTCPConnectionStatus_opened;

	return 0;
}

//int32_t CTCPSocket::open_as_client( char* localIP /*= NULL*/ )
//{
	//firstly, close the socket
//	if (enmTCPConnectionStatus_closed != m_socket_status && m_socket_fd > 0)
//	{
//		close_socket();
//	}

	//open socket
//	m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
//	if (m_socket_fd < 0)
//	{
//		m_socket_status = enmTCPConnectionStatus_closed;
//		m_socket_fd = -1;
//		return -1;
//	}

//	if (NULL != localIP)
//	{
//		struct sockaddr_in stLocalAddress;
//		memset(&stLocalAddress, 0x0, sizeof(stLocalAddress));

//		stLocalAddress.sin_family = AF_INET;
//		stLocalAddress.sin_addr.s_addr = inet_addr(localIP);
//		socklen_t addressSize = (socklen_t)sizeof(stLocalAddress);
//		if (0 != bind(m_socket_fd, (const struct sockaddr*) &stLocalAddress, addressSize))
//		{
//			close_socket();
//			return -1;
//		}

//	}

//	m_socket_type = enmSocketTypes_comm;
//	m_socket_status = enmTCPConnectionStatus_opened;

//	m_recv_head = 0;
//	m_recv_tail = 0;

//	m_send_head = 0;
//	m_send_tail = 0;

//	return 0;
//}

int32_t CTCPSocket::open_as_client( char* localIP /*= NULL*/, u_short localPort/*=0*/ )
{
    //firstly, close the socket
    if (enmTCPConnectionStatus_closed != m_socket_status && m_socket_fd > 0)
    {
        close_socket();
    }

    //open socket
    m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket_fd < 0)
    {
        m_socket_status = enmTCPConnectionStatus_closed;
        m_socket_fd = -1;
        return -1;
    }

    //if (NULL != localIP)
    //{
	struct sockaddr_in stLocalAddress;
	memset(&stLocalAddress, 0x0, sizeof(stLocalAddress));

	stLocalAddress.sin_family = AF_INET;
	if(NULL != localIP){
		stLocalAddress.sin_addr.s_addr = inet_addr(localIP);
	} else {
		stLocalAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	if(0 != localPort)
	{
		stLocalAddress.sin_port = htons(localPort);
	}
	// 2010-5-22 add by liuqy
	int32_t iOptValue = 1;
	if (0 != setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &iOptValue, sizeof(iOptValue)))
	{
		close_socket();
		return -1;
	}
	setsockopt(m_socket_fd, IPPROTO_TCP, TCP_NODELAY, &iOptValue, sizeof(iOptValue)); //set TCP_CORK
	//////////////////////////////////////////////////////////////////////////

	socklen_t addressSize = (socklen_t)sizeof(stLocalAddress);
	if (0 != bind(m_socket_fd, (const struct sockaddr*) &stLocalAddress, addressSize))
	{
		close_socket();
		return -1;
	}

    //}

    m_socket_type = enmSocketTypes_comm;
    m_socket_status = enmTCPConnectionStatus_opened;

    m_recv_head = 0;
    m_recv_tail = 0;

    m_send_head = 0;
    m_send_tail = 0;

    return 0;
}
int32_t CTCPSocket::connect_to( char* ip, uint16_t port, bool nonblock /*= true*/ , int32_t msecond /* = 100*/)
{
	if (NULL == ip)
	{
		return -1;
	}

	//check socket type (common or listening)
	if (enmSocketTypes_comm != m_socket_type)
	{
		return -1;
	}

	if (enmTCPConnectionStatus_opened != m_socket_status || 0 > m_socket_fd)
	{
		return -1;
	}

	struct sockaddr_in stDstAddress;
	memset(&stDstAddress, 0x0, sizeof(stDstAddress));
	stDstAddress.sin_family = AF_INET;
	stDstAddress.sin_addr.s_addr = inet_addr(ip);
	stDstAddress.sin_port = htons(port);
	socklen_t sockSize = (socklen_t)sizeof(stDstAddress);

	//if (0 != connect(m_socket_fd, (const sockaddr*) &stDstAddress, sockSize))
	if (0 != connect_nonblock(m_socket_fd, &stDstAddress, sockSize, msecond))
	{
		close_socket();
		return -1;
	}

	if (nonblock)
	{
		set_non_block(m_socket_fd);
	}

	m_recv_head = 0;
	m_recv_tail = 0;

	m_send_head = 0;
	m_send_tail = 0;

	//set socket status to connected !!
	m_socket_status = enmTCPConnectionStatus_connected;
	return 0;

}

int32_t CTCPSocket::connect_to( u_long ip, uint16_t port, bool nonblock, /*true */ int32_t msecond /*100*/)
{
	//check socket type (common or listening)
	if (enmSocketTypes_comm != m_socket_type)
	{
		return -1;
	}

	if (enmTCPConnectionStatus_opened != m_socket_status || 0 > m_socket_fd)
	{
		return -1;
	}

	struct sockaddr_in stDstAddress;
	memset(&stDstAddress, 0x0, sizeof(stDstAddress));
	stDstAddress.sin_family = AF_INET;
	stDstAddress.sin_addr.s_addr = ip;
	stDstAddress.sin_port = htons(port);
	socklen_t sockSize = (socklen_t)sizeof(stDstAddress);

	//if (0 != connect(m_socket_fd, (const sockaddr*) &stDstAddress, sockSize))
	if (0 != connect_nonblock(m_socket_fd, &stDstAddress, sockSize, msecond))
	{
		close_socket();
		return -1;
	}

	if (nonblock)
	{
		set_non_block(m_socket_fd);
	}

	m_recv_head = 0;
	m_recv_tail = 0;

	m_send_head = 0;
	m_send_tail = 0;

	m_socket_status = enmTCPConnectionStatus_connected;
	return 0;
}

int32_t CTCPSocket::accept_fd( int32_t fd )
{
	if (fd < 0)
	{
		return -1;
	}

#ifdef __MULTI_THREAD__
	pthread_mutex_lock(&m_mutex);
#endif

	if (enmTCPConnectionStatus_connected == m_socket_status && m_socket_fd > 0)
	{
		close_socket();
	}

	m_socket_fd = fd;
	m_socket_status = enmTCPConnectionStatus_connected;
	m_socket_type = enmSocketTypes_comm;

	m_recv_head = 0;
	m_recv_tail = 0;

	m_send_head = 0;
	m_send_tail = 0;

	//set non block
	set_non_block(m_socket_fd);

#ifdef __MULTI_THREAD__
	pthread_mutex_unlock(&m_mutex);
#endif

	return 0;

}

int32_t CTCPSocket::add_fd_set( fd_set* fdset )
{
	if (NULL == fdset)
	{
		return -1;
	}

//#ifdef __MULTI_THREAD__
//	pthread_mutex_lock(&m_mutex);
//#endif

	if (enmTCPConnectionStatus_connected == m_socket_status &&  m_socket_fd > 0)
	{
		FD_SET(m_socket_fd, fdset);
	}
	else if (m_socket_fd > 0)
	{
		close_socket();
		return -1;
	}


//#ifdef __MULTI_THREAD__
//	pthread_mutex_unlock(&m_mutex);
//#endif
	
	return 0;
}

bool CTCPSocket::is_fd_ready( fd_set* fdset )
{
	if (NULL == fdset)
	{
		return false;
	}

	bool flag = false;

//#ifdef __MULTI_THREAD__
//	pthread_mutex_lock(&m_mutex);
//#endif

	if (enmTCPConnectionStatus_connected == m_socket_status && m_socket_fd > 0)
	{
		flag = FD_ISSET(m_socket_fd, fdset);
	}


//#ifdef __MULTI_THREAD__
//	pthread_mutex_unlock(&m_mutex);
//#endif

	return flag;
}


int32_t CTCPSocket::recv_data( void )
{
	int32_t ret = 0;

//#ifdef __MULTI_THREAD__
//	pthread_mutex_lock(&m_mutex);
//#endif

	if (enmTCPConnectionStatus_connected != m_socket_status || 0 > m_socket_fd)
	{
//#ifdef __MULTI_THREAD__
//		pthread_mutex_unlock(&m_mutex);
//#endif
		return (int32_t)enmConnErrorCode_invalid_socket;
	}

	//
	if (m_recv_tail == m_recv_head)
	{
		m_recv_head = 0;
		m_recv_tail = 0;
	}
	//
	int32_t received_byte = 0;
	
	do //循环从网络连接上接收数据
	{
		//if (m_recv_tail == (int32_t)sizeof(m_recv_buffer))
		if (m_recv_tail == m_recv_buffer_size)
		{
			//接收缓冲区的头指针不在
			if (m_recv_head > 0)
			{
				//注意: 要使用memmove而不是memcpy
				memmove(&m_recv_buffer[0], &m_recv_buffer[m_recv_head], size_t(m_recv_tail - m_recv_head));
				m_recv_tail -= m_recv_head;
				m_recv_head = 0;
			}
			else
			{
				//recv buffer was full, just stop receiving data
				ret = (int32_t)enmConnErrorCode_recv_notenouth_buffer;
				break;
			}
		}//if (m_recv_tail == (int32_t)sizeof(m_recv_buffer))

		//
		//received_byte = recv(m_socket_fd, &m_recv_buffer[m_recv_tail], size_t(sizeof(m_recv_buffer) - m_recv_tail), 0);
		received_byte = recv(m_socket_fd, &m_recv_buffer[m_recv_tail], size_t(m_recv_buffer_size - m_recv_tail), 0);
		if (received_byte > 0)
		{
			m_recv_tail += received_byte;
		}
		else if (0 == received_byte)
		{	
			//peer was closed, so just break receiving
			close_socket();
			ret = enmConnErrorCode_peer_closed;
			break;
		}
		else if (EAGAIN != errno) //received_byte < 0 && EAGAIN != errno
		{
			//must be some error!
			close_socket();
			ret = enmConnErrorCode_unknow;
			break;
		}

	} while (received_byte > 0); //


//#ifdef __MULTI_THREAD__
//	pthread_mutex_unlock(&m_mutex);
//#endif

	return ret;
}

int32_t CTCPSocket::send_remaind_data()
{
	//上次发生的剩余数据
	int32_t data_remaind_byte = 0;
	data_remaind_byte = m_send_tail - m_send_head;
	if (data_remaind_byte <= 0)
	{
		return 0;
	}

#ifdef __MULTI_THREAD__
	pthread_mutex_lock(&m_mutex);
#endif

	if (enmTCPConnectionStatus_connected != m_socket_status || m_socket_fd < 0)
	{
#ifdef __MULTI_THREAD__
		pthread_mutex_unlock(&m_mutex);
#endif
		return (int32_t)enmConnErrorCode_invalid_socket;
	}

	char* pszDataPtr = &m_send_buffer[m_send_head];
	int32_t sended_byte = 0;
	int32_t ret = 0;
	while (data_remaind_byte > 0)
	{
		sended_byte = send(m_socket_fd, pszDataPtr, (size_t)data_remaind_byte, 0);
		if (sended_byte > 0)
		{
			m_send_head += sended_byte;
			pszDataPtr += sended_byte;
			data_remaind_byte -= sended_byte;
		}
		if (sended_byte < 0 && EAGAIN != errno)
		{
			m_socket_status = enmTCPConnectionStatus_error;
			ret = (int32_t)enmConnErrorCode_unknow;
			break;
		}
		if (sended_byte < 0 )
		{
			break;
		}
	}//while (data_remaind_byte > 0)

	if (0 == data_remaind_byte)
	{
		m_send_head = 0;
		m_send_tail = 0;
	}
#ifdef __MULTI_THREAD__
	pthread_mutex_unlock(&m_mutex);
#endif

	return ret;
}

int32_t CTCPSocket::send_data( char* data, size_t size )
{
	int32_t ret = 0;
	if (NULL == data || 0 == size)
	{
		return (int32_t)enmConnErrorCode_invalid_param;
	}

#ifdef __MULTI_THREAD__
	pthread_mutex_lock(&m_mutex);
#endif

	if (enmTCPConnectionStatus_connected != m_socket_status || m_socket_fd < 0)
	{
#ifdef __MULTI_THREAD__
		pthread_mutex_unlock(&m_mutex);
#endif
		return (int32_t)enmConnErrorCode_invalid_socket;
	}

	int32_t data_remaind_byte = 0;
	data_remaind_byte = m_send_tail - m_send_head;

	//上次发生的剩余数据
	char* pszDataPtr = &m_send_buffer[m_send_head];

	int32_t sended_byte = 0;
	while (data_remaind_byte > 0)
	{
		sended_byte = send(m_socket_fd, pszDataPtr, (size_t)data_remaind_byte, 0);
		if (sended_byte > 0)
		{
			m_send_head += sended_byte;
			pszDataPtr += sended_byte;
			data_remaind_byte -= sended_byte;
		}
		if (sended_byte < 0 && EAGAIN != errno)
		{
			m_socket_status = enmTCPConnectionStatus_error;
			ret = (int32_t)enmConnErrorCode_unknow;
			break;
		}
		if (sended_byte < 0 )
		{
			//if ( send_buffer_size - m_send_tail >= (int32_t)size )
			if (m_send_buffer_size - m_send_tail >= (int32_t)size )
			{
                // 2010-11-4 add by liuqy
				//memcpy(&m_send_buffer[m_send_tail], pszDataPtr, (size_t)size);
                memcpy(&m_send_buffer[m_send_tail], data, (size_t)size);
				m_send_tail += size;
				ret = 0;  //暂且认为已经发生成功
			}
			//else if ( send_buffer_size - m_send_tail + m_send_head >= (int32_t)size)
			else if (m_send_buffer_size - m_send_tail + m_send_head >= (int32_t)size)
			{
				memmove(&m_send_buffer[0], &m_send_buffer[m_send_head], (size_t)data_remaind_byte);
				m_send_head = 0;
				m_send_tail = data_remaind_byte;
				//memcpy(&m_send_buffer[m_send_tail], pszDataPtr, (size_t)size);
                memcpy(&m_send_buffer[m_send_tail], data, (size_t)size);
				m_send_tail += size;
				ret = 0;  //暂且认为已经发生成功
			}
			else
			{
				ret = (int32_t)enmConnErrorCode_send_notenouth_buffer;
			}
			break;
		}


	}//while (data_remaind_byte > 0)

	if (0 == data_remaind_byte)
	{
		m_send_head = 0;
		m_send_tail = 0;
	}
	else
	{	
#ifdef __MULTI_THREAD__
		pthread_mutex_unlock(&m_mutex);
#endif

		return ret;
	}
	
	//
	data_remaind_byte = (int32_t)size;
	pszDataPtr = data;

	while (data_remaind_byte > 0)
	{
		sended_byte = send(m_socket_fd, pszDataPtr, (size_t)data_remaind_byte, 0);
		if (sended_byte > 0)
		{
			pszDataPtr += sended_byte;
			data_remaind_byte -= sended_byte;
		}
		else if (sended_byte < 0 && EAGAIN != errno)
		{
			m_socket_status = enmTCPConnectionStatus_error;
			ret = (int32_t)enmConnErrorCode_unknow;
			break;
		}
		else if (sended_byte < 0)
		{
			//cache the remained data
			memcpy(&m_send_buffer[m_send_tail], pszDataPtr, (size_t)data_remaind_byte);
			m_send_tail += data_remaind_byte;
			ret = 0;  //暂且认为已经发生成功
			break;
		}

	}//while (data_remaind_byte > 0)


#ifdef __MULTI_THREAD__
	pthread_mutex_unlock(&m_mutex);
#endif

	return ret;
}

//
void CTCPSocket::set_4_byte_size( bool flag )
{
	m_4bytes = flag;
}

bool CTCPSocket::is_4_byte_size( void )
{
	return m_4bytes;
}

//
int32_t CTCPSocket::get_one_code( char* data, size_t& size )
{
	if (NULL == data)
	{
		return -1;
	}

	int32_t buffer_data_size = 0;
	buffer_data_size = m_recv_tail - m_recv_head;

	//判断接收缓冲区内的数据大小
	if (buffer_data_size <= 0)
	{
		return 0;
	}

	//根据表示长度的字节数，检查数据的合法性
	if ((is_4_byte_size() && buffer_data_size < (int32_t)sizeof(int32_t)) 
		||(!is_4_byte_size() && buffer_data_size < (int32_t)sizeof(int16_t)))
	{
		//if (m_recv_tail == (int32_t)sizeof(m_recv_buffer))
		if (m_recv_tail == m_recv_buffer_size)
		{
			memcpy(&m_recv_buffer[0], &m_recv_buffer[m_recv_head], size_t(buffer_data_size));
			m_recv_head = 0;
			m_recv_tail = buffer_data_size;

		}

		return 0;
	}

	//长度字段占用4byte
	int32_t code_size = 0;
	if (is_4_byte_size())
	{
		code_size = (int32_t)ntohl((u_long) (*(int32_t*)&m_recv_buffer[m_recv_head]));
	}
	else
	{
		code_size = (int32_t)ntohs((uint16_t) (*(int16_t*)&m_recv_buffer[m_recv_head]));
	}

	if (code_size <= 0)
	{
		//update by hecb 2013-03-08
		return -2;
		//return 0;
	}

	if (code_size >= 0xffff)
	{
		//update by hecb 2013-03-08
		return -3;
	}

	//若接收缓冲区内的数据不足一个code
	if (buffer_data_size < code_size)
	{
		//并且数据已经存放到缓冲区尾了, 则移动数据到接收缓冲区头部
		//if (m_recv_tail == (int32_t)sizeof(m_recv_buffer))
		if (m_recv_tail == m_recv_buffer_size)
		{
			memmove(&m_recv_buffer[0], &m_recv_buffer[m_recv_head], size_t(buffer_data_size));
			m_recv_head = 0;
			m_recv_tail = buffer_data_size;
		}

		return 0;
	}

	//
	//最好判断一下传入参数:接收buffer的大小,目前的实现是没有判断，这就要求传入的buffer长度是足够的
	size = (size_t)code_size;
	memcpy(data, &m_recv_buffer[m_recv_head], size);

	//
	m_recv_head += code_size;
	if (m_recv_tail == m_recv_head)
	{
		m_recv_head = 0;
		m_recv_tail = 0;
	}

	return 1;
}

int32_t CTCPSocket::get_one_code_ex( char* data, size_t& size )
{
	if (NULL == data)
	{
		return -1;
	}

	int32_t buffer_data_size = 0;
	buffer_data_size = m_recv_tail - m_recv_head;

	//判断接收缓冲区内的数据大小
	if (buffer_data_size <= 0)
	{
		return 0;
	}

	//根据表示长度的字节数，检查数据的合法性
	if (buffer_data_size < 6)
	{
		//if (m_recv_tail == (int32_t)sizeof(m_recv_buffer))
		if (m_recv_tail == m_recv_buffer_size)
		{
			memcpy(&m_recv_buffer[0], &m_recv_buffer[m_recv_head], size_t(buffer_data_size));
			m_recv_head = 0;
			m_recv_tail = buffer_data_size;

		}

		return 0;
	}

	//长度字段占用6byte,取得code的长度
 	int32_t code_size = 0;
	char pkgLenBuf[6];
	memset(pkgLenBuf, 0, sizeof(pkgLenBuf));
	memcpy(pkgLenBuf, &m_recv_buffer[m_recv_head], sizeof(pkgLenBuf));
	sscanf(pkgLenBuf, "%6x", &code_size);

	//若接收缓冲区内的数据不足一个code
	if (buffer_data_size < code_size)
	{
		//移动数据到接收缓冲区头部
		//if (m_recv_tail == (int32_t)sizeof(m_recv_buffer))
		if (m_recv_tail == m_recv_buffer_size)
		{
			memmove(&m_recv_buffer[0], &m_recv_buffer[m_recv_head], size_t(buffer_data_size));
			m_recv_head = 0;
			m_recv_tail = buffer_data_size;
		}

		return 0;
	}

	//
	//最好判断一下传入参数:接收buffer的大小.
	size_t t = (size_t)(code_size + sizeof(pkgLenBuf));
	if( size < t)
	{
		return -2;	
	}
	size = t;//这意味着前6Byte中保存的长度并不包括6Byte自身
	memcpy(data, &m_recv_buffer[m_recv_head], size);

	//
	m_recv_head += size;
	if (m_recv_tail == m_recv_head)
	{
		m_recv_head = 0;
		m_recv_tail = 0;
	}

	return 1;
}

void CTCPSocket::clear()
{
	if (NULL != m_recv_buffer)
	{
		delete [] m_recv_buffer;
		m_recv_buffer = NULL;
	}
	if (NULL != m_send_buffer)
	{
		delete [] m_send_buffer;
		m_send_buffer = NULL;
	}

	m_recv_head = 0;
	m_recv_tail = 0;

	m_send_head = 0;
	m_send_tail = 0;

	m_4bytes = false;
}

int32_t CTCPSocket::moveTo(CTCPSocket &dest)
{
	if (&dest == this)
	{
		return success;
	}

	dest.clear();
	
	dest.m_socket_fd = this->m_socket_fd;
	dest.m_socket_type = this->m_socket_type;
	dest.m_socket_status = this->m_socket_status;
	dest.m_recv_head = this->m_recv_head;
	dest.m_recv_tail = this->m_recv_tail;
	dest.m_send_head = this->m_send_head;
	dest.m_send_tail = this->m_send_tail;
	dest.m_4bytes = this->m_4bytes;
	dest.m_send_buffer_size = this->m_send_buffer_size;
	dest.m_recv_buffer_size = this->m_recv_buffer_size;
	dest.m_send_buffer = this->m_send_buffer;
	dest.m_recv_buffer = this->m_recv_buffer;

	this->m_socket_fd = -1;
	this->m_socket_type = enmSocketTypes_listen;
	this->m_socket_status = enmTCPConnectionStatus_closed;
	this->m_recv_head = 0;
	this->m_recv_tail = 0;
	this->m_send_head = 0;
	this->m_send_tail = 0;
	this->m_4bytes = false;
	this->m_send_buffer_size = common_buffer_size;
	this->m_recv_buffer_size = common_buffer_size;
	this->m_send_buffer = NULL;
	this->m_recv_buffer = NULL;

	return success;
}

//////////////////////////////////////////////////////////////////////////
// CTCPConnection Imp
//////////////////////////////////////////////////////////////////////////
CTCPConnection::CTCPConnection()
{
	m_ip = 0;
	m_port = 0;
}

CTCPConnection::~CTCPConnection()
{
	//nothing
}

//int32_t CTCPConnection::initialize( int32_t entity_id, int16_t entity_type, u_long ip, uint16_t port )
int32_t CTCPConnection::initialize(uint64_t ip, uint16_t port, int32_t send_buffer_size, int32_t recv_buffer_size)
{
	//m_entity_id = entity_id;
	//m_entity_type = entity_type;
	m_ip = ip;
	m_port = port;

	m_socket.initialize(send_buffer_size, recv_buffer_size);
	
	return 0;
}

CTCPSocket& CTCPConnection::get_socket()
{
	return m_socket;
}

//int32_t CTCPConnection::get_entity_id()
//{
//	return m_entity_id;
//}

//int16_t CTCPConnection::get_entity_type()
//{
//	return m_entity_type;
//}

u_long CTCPConnection::get_ip()
{
	return m_ip;
}

uint16_t CTCPConnection::get_port()
{
	return m_port;
}

int32_t CTCPConnection::connect_to( char* localIP, bool nonblock /*= true*/, int32_t msecond /* = 100*/ )
{
	int32_t ret = m_socket.open_as_client(localIP);
	if (0 != ret)
	{
		return -1;
	}

	ret = m_socket.connect_to(m_ip, m_port, nonblock, msecond);
	if (0 != ret)
	{
		return -1;
	}

	return ret;
}

//int32_t CTCPConnection::connect_to( char* localIP/*=NULL*/, u_short localPort/*=0*/, bool nonblock /*= true*/, int32_t msecond /*= 100*/ )
/*
{
    int32_t ret = m_socket.open_as_client(localIP, localPort);
    if (0 != ret)
    {
        return -2;
    }

    ret = m_socket.connect_to(m_ip, m_port, nonblock, msecond);
    if (0 != ret)
    {
        return -3;
    }

    return ret;
}
*/

int32_t CTCPConnection::moveTo(CTCPConnection &dest)
{
	m_socket.moveTo(dest.m_socket);
	dest.m_ip = m_ip;
	dest.m_port = m_port;

	m_ip = 0;
	m_port = 0;

	return success;
}


}}//GameJayo { namespace Server {
