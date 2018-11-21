#ifndef __COMMON_TCPCONNECTION_H__
#define __COMMON_TCPCONNECTION_H__

#include "common_socket.h"
#include "lock.h"

namespace GameJayo { namespace Server {


//some const variables
enum 
{
	//recv_buffer_size = 0x200000,
	//send_buffer_size = 0x400000,	//�ö���Ϊserver֮���socket buffer���ȣ�ע���ҪҪ��֤����server��ͨѶ��������
	common_buffer_size	= 0x200000,	//2M��С������socket buffer�����С
	//common_buffer_size	= 0x2000000,//32M��С�������ݴ���socket buffer�����С
	large_buffer_size	= 0x400000,	//4M��С�������ݴ���socket buffer�����С
	sparse_buffer_size	= 0x010000, //64K��С��ϡ��С���ݴ���socket buffer�����С������logic<=>connect����������
};

//connection status
enum enmTCPConnectionStatus
{
	enmTCPConnectionStatus_closed = 0,
	enmTCPConnectionStatus_opened,
	enmTCPConnectionStatus_connected,
	enmTCPConnectionStatus_error,
};

enum enmConnErrorCode
{
	enmConnErrorCode_success					= 0,
	enmConnErrorCode_invalid_socket				= -1,
	enmConnErrorCode_recv_notenouth_buffer		= -2,
	enmConnErrorCode_peer_closed				= -3,
	enmConnErrorCode_send_notenouth_buffer		= -4,
	enmConnErrorCode_invalid_param				= -5,
	enmConnErrorCode_unknow						= -6,

};


//socket type
enum enmSocketTypes
{
	enmSocketTypes_listen = 0,
	enmSocketTypes_comm = 1,
};

//��TCPЭ���Socket�ķ�װ
//��Ҫ�ӿ�:
//1.��������. �ṩ����Ӧ�ò�Ľ��ջ�����
//2.��������. �ṩ����Ӧ�ò��һ�η���δ����������ݵĻ���
//3.���������˿�(�����������˼���)
//4.��������(���ӵ������ķ�������)
class CTCPSocket
{
public:
	CTCPSocket();
	~CTCPSocket();

	int32_t initialize(int32_t send_buffer_size, int32_t recv_buffer_size);
	void close_socket();
	
	int32_t get_fd();
	enmTCPConnectionStatus get_status();

	//open socket as  a server or client
	int32_t open_as_server(uint16_t port, char* ip = NULL);
	//int32_t open_as_client(char* localIP = NULL);
    int32_t open_as_client(char* localIP = NULL, u_short localPort=0);

	//
	int32_t connect_to(char* ip, uint16_t port, bool nonblock = true, int32_t msecond = 100);
	//u_long ip : �����ֽ���ĵ�ַ
	int32_t connect_to(u_long ip, uint16_t port, bool nonblock = true, int32_t msecond = 100);

	//
	int32_t accept_fd(int32_t fd);

	//
	int32_t add_fd_set(fd_set* fdset);

	//
	bool is_fd_ready(fd_set* fdset);

	//receive data from socket stream, and store recv buffer, wait for processing
	//
	int32_t recv_data(void);

	//send remaind data to remote peer
	int32_t send_remaind_data();

	//send data to remote peer
	int32_t send_data(char* data, size_t size);

	//get one completed code from receive buffer
	//return value: >0 -- success, <= 0  -- some error
	//size: Ӧ�ô���data�Ĵ�С,�����õ���ʵ�����ݴ�С��Ŀǰsizeû�д���data�Ĵ�С�����������ݵ�ʵ�ʴ�С��
	int32_t get_one_code(char* data, size_t& size);
	
	/**
	@brief ���յ������ݰ�ǰ6���ֽڱ�ʾ������
	*/
	int32_t get_one_code_ex(char* data, size_t& size);

	//
	void set_4_byte_size(bool flag);

	//
	bool is_4_byte_size(void);

private:
	//prohibit copy constructor & operator '='
	CTCPSocket(const CTCPSocket& original);
	CTCPSocket& operator =(CTCPSocket& rvalue);

public:
	int32_t  moveTo(CTCPSocket& dest);
	void 	 clear();

protected:
	enmSocketTypes m_socket_type;
	int32_t m_socket_fd;

	enmTCPConnectionStatus m_socket_status;
    Locker m_locker_close;

	//��ʶ�Ƿ���4�ֽڱ�ʾ���ȣ�true :yes������no
	//��ʹ��int32_t��ʾ����,ʵ�ʳ��Ȳ��ᳬ��int16_t�ķ�Χ
	//������ʹ��4���ֽڻ���2���ֽڱ�ʾ���ȣ�����Ӧ��Э������ģ����Ӱ��
	//�԰�ʵ�ʳ��ȵ�ʶ��
	bool m_4bytes;

	//receiving buffer
	int32_t m_recv_head;
	int32_t m_recv_tail;
	//char m_recv_buffer[recv_buffer_size+1]; //�ر�ע�⣺����ѭ��������
	char* m_recv_buffer;
	int32_t m_recv_buffer_size;

	//sending buffer
	int32_t m_send_head;
	int32_t m_send_tail;
	//char m_send_buffer[send_buffer_size+1];//�ر�ע�⣺����ѭ��������
	char* m_send_buffer;
	int32_t m_send_buffer_size;

#ifdef __MULTI_THREAD__
	pthread_mutex_t m_mutex;			//the mutual exclusive variable
#endif

private:
};

class CTCPConnection
{
public:
	CTCPConnection();
	~CTCPConnection();
private:
	CTCPConnection(const CTCPConnection& original);
	CTCPConnection& operator = (const CTCPConnection& original);

public:
	//
	//int32_t initialize(int32_t entity_id, int16_t entity_type, u_long ip, uint16_t port);
	int32_t initialize(uint64_t ip, uint16_t port, int32_t send_buffer_size = common_buffer_size, int32_t recv_buffer_size = common_buffer_size);
	//
	CTCPSocket& get_socket();
	//
	//int32_t get_entity_id();
	//
	//int16_t get_entity_type();
	//
	u_long get_ip();
	//
	uint16_t get_port();
	//
	int32_t connect_to(char* localIP = NULL, bool nonblock = true, int32_t msecond = 100);
    //int32_t connect_to(char* localIP = NULL, u_short localPort = 0, bool nonblock = true, int32_t msecond = 100);

protected:
	CTCPSocket m_socket;

	u_long m_ip;
	uint16_t m_port;

public:
	int32_t  moveTo(CTCPConnection& dest);
};

}}//GameJayo { namespace Server {



#endif //__COMMON_TCPCONNECTION_H__

