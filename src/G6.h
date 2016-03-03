/*
 * G6 - TCP Transfer && LB Dispenser
 * Author      : calvin
 * Email       : calvinwillliams@163.com
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

#ifndef _H_G6_
#define _H_G6_

#if ( defined __linux ) || ( defined __unix )
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <stdarg.h>
#include <limits.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#define _VSNPRINTF			vsnprintf
#define _SNPRINTF			snprintf
#define _CLOSESOCKET			close
#define _CLOSESOCKET2(_fd1_,_fd2_)	close(_fd1_),close(_fd2_);
#define _ERRNO				errno
#define _EWOULDBLOCK			EWOULDBLOCK
#define _ECONNABORTED			ECONNABORTED
#define _EINPROGRESS			EINPROGRESS
#define _ECONNRESET			ECONNRESET
#define _ENOTCONN			ENOTCONN
#define _EISCONN			EISCONN
#define _SOCKLEN_T			socklen_t
#define _GETTIMEOFDAY(_tv_)		gettimeofday(&(_tv_),NULL)
#define _LOCALTIME(_tt_,_stime_) \
	localtime_r(&(_tt_),&(_stime_));
#elif ( defined _WIN32 )
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <io.h>
#include <windows.h>
#define _VSNPRINTF			_vsnprintf
#define _SNPRINTF			_snprintf
#define _CLOSESOCKET			closesocket
#define _CLOSESOCKET2(_fd1_,_fd2_)	closesocket(_fd1_),closesocket(_fd2_);
#define _ERRNO				GetLastError()
#define _EWOULDBLOCK			WSAEWOULDBLOCK
#define _ECONNABORTED			WSAECONNABORTED
#define _EINPROGRESS			WSAEINPROGRESS
#define _ECONNRESET			WSAECONNRESET
#define _ENOTCONN			WSAENOTCONN
#define _EISCONN			WSAEISCONN
#define _SOCKLEN_T			int
#define _GETTIMEOFDAY(_tv_) \
	{ \
		SYSTEMTIME stNow ; \
		GetLocalTime( & stNow ); \
		(_tv_).tv_usec = stNow.wMilliseconds * 1000 ; \
		time( & ((_tv_).tv_sec) ); \
	}
#define _SYSTEMTIME2TIMEVAL_USEC(_syst_,_tv_) \
		(_tv_).tv_usec = (_syst_).wMilliseconds * 1000 ;
#define _SYSTEMTIME2TM(_syst_,_stime_) \
		(_stime_).tm_year = (_syst_).wYear - 1900 ; \
		(_stime_).tm_mon = (_syst_).wMonth - 1 ; \
		(_stime_).tm_mday = (_syst_).wDay ; \
		(_stime_).tm_hour = (_syst_).wHour ; \
		(_stime_).tm_min = (_syst_).wMinute ; \
		(_stime_).tm_sec = (_syst_).wSecond ;
#define _LOCALTIME(_tt_,_stime_) \
	{ \
		SYSTEMTIME	stNow ; \
		GetLocalTime( & stNow ); \
		_SYSTEMTIME2TM( stNow , (_stime_) ); \
	}
#endif

#include "LOGC.h"

#ifndef ULONG_MAX
#define ULONG_MAX 0xffffffffUL
#endif

#define FOUND				9	/* 找到 */
#define NOT_FOUND			4	/* 找不到 */

#define MATCH				1	/* 匹配 */
#define NOT_MATCH			-1	/* 不匹配 */

#define LOAD_BALANCE_ALGORITHM_G	"G"	/* 管理端口 */
#define LOAD_BALANCE_ALGORITHM_MS	"MS"	/* 主备模式 */
#define LOAD_BALANCE_ALGORITHM_RR	"RR"	/* 轮询模式 */
#define LOAD_BALANCE_ALGORITHM_LC	"LC"	/* 最少连接模式 */
#define LOAD_BALANCE_ALGORITHM_RT	"RT"	/* 最小响应时间模式 */
#define LOAD_BALANCE_ALGORITHM_RD	"RD"	/* 随机模式 */
#define LOAD_BALANCE_ALGORITHM_HS	"HS"	/* HASH模式 */

#define DEFAULT_RULES_INITCOUNT			2
#define DEFAULT_RULES_INCREASE			5

#define DEFAULT_CLIENTS_INITCOUNT_IN_ONE_RULE	2
#define DEFAULT_CLIENTS_INCREASE_IN_ONE_RULE	5
#define DEFAULT_FORWARDS_INITCOUNT_IN_ONE_RULE	2
#define DEFAULT_FORWARDS_INCREASE_IN_ONE_RULE	5
#define DEFAULT_SERVERS_INITCOUNT_IN_ONE_RULE	2
#define DEFAULT_SERVERS_INCREASE_IN_ONE_RULE	5

#define DEFAULT_FORWARD_SESSIONS_MAXCOUNT	1000	/* 缺省最大连接数量 */
#define DEFAULT_FORWARD_TRANSFER_BUFSIZE	4096	/* 缺省通讯转发缓冲区大小 */

#define RULE_ID_MAXLEN				64
#define LOAD_BALANCE_ALGORITHM_MAXLEN		2
#define IP_MAXLEN				30
#define PORT_MAXLEN				10

#define WAIT_EVENTS_COUNT			1024 /* 一次获取epoll事件数量 */

#define FORWARD_SESSION_TYPE_UNUSED		0 /* 未使用 */
#define FORWARD_SESSION_TYPE_LISTEN		1 /* 侦听会话 */
#define FORWARD_SESSION_TYPE_CLIENT		2 /* 作为客户端连接远端服务端会话 */
#define FORWARD_SESSION_TYPE_SERVER		3 /* 作为服务端接受远端连接会话 */
#define FORWARD_SESSION_TYPE_MANAGER		4 /* 作为管理服务端接受远端连接会话 */

#define FORWARD_SESSION_STATUS_UNUSED		0 /* 未使用 */
#define FORWARD_SESSION_STATUS_READY		1 /* 准备使用 */
#define FORWARD_SESSION_STATUS_LISTEN		2 /* 侦听中 */
#define FORWARD_SESSION_STATUS_CONNECTING	3 /* 非堵塞连接中 */
#define FORWARD_SESSION_STATUS_CONNECTED	4 /* 连接完成 */
#define FORWARD_SESSION_STATUS_COMMAND		5 /* 命令 */

#define IO_BUFFER_SIZE				4096 /* 输入输出缓冲区大小 */

#define DEFAULT_DISABLE_TIMEOUT			60 /* 缺省暂禁时间，单位：秒 */

#define G6_LISTEN_SOCKFDS			"G6_LISTEN_SOCKFDS"

/* 网络地址信息结构 */
struct NetAddress
{
	char			ip[ IP_MAXLEN + 1 ] ; /* ip地址 */
	union
	{
		char		port_str[ PORT_MAXLEN + 1 ] ;
		int		port_int ;
	} port ; /* 端口 */
	struct sockaddr_in	sockaddr ; /* sock地址结构 */
} ;

/* 客户端信息结构 */
struct ClientNetAddress
{
	struct NetAddress	netaddr ; /* 网络地址结构 */
	
	unsigned long		client_connection_count ; /* 客户端连接数量 */
	unsigned long		maxclients ; /* 最大客户端数量 */
} ;

/* 转发端信息结构 */
struct ForwardNetAddress
{
	struct NetAddress	netaddr ; /* 网络地址结构 */
	int			sock ; /* sock描述字 */
} ;

/* 服务端信息结构 */
struct ServerNetAddress
{
	struct NetAddress	netaddr ; /* 网络地址结构 */
	
	unsigned char		server_unable ; /* 服务端可用标志 */
	time_t			timestamp_to_enable ; /* 恢复正常秒戳 */
	
	unsigned long		server_connection_count ; /* 服务端连接数量 */
	
	time_t			rtt ; /* 最近读时间戳 */
	time_t			wtt ; /* 最近写时间戳 */
} ;

/* 转发规则结构 */
struct ForwardRule
{
	char				rule_id[ RULE_ID_MAXLEN + 1 ] ; /* 规则ID */
	char				load_balance_algorithm[ LOAD_BALANCE_ALGORITHM_MAXLEN + 1 ] ; /* 负载均衡算法 */
	
	struct ClientNetAddress		*client_addr_array ; /* 客户端地址结构 */
	unsigned long			client_addr_size ; /* 客户端规则配置最大数量 */
	unsigned long			client_addr_count ; /* 客户端规则配置数量 */
	
	struct ForwardNetAddress	*forward_addr_array ; /* 转发端地址结构 */
	unsigned long			forward_addr_size ; /* 转发端规则配置最大数量 */
	unsigned long			forward_addr_count ; /* 转发端规则配置数量 */
	
	struct ServerNetAddress		*server_addr_array ; /* 服务端地址结构 */
	unsigned long			server_addr_size ; /* 服务端规则配置最大数量 */
	unsigned long			server_addr_count ; /* 服务端规则配置数量 */
	unsigned long			selected_addr_index ; /* 当前服务端索引 */
} ;

/* 转发会话结构 */
struct ForwardSession
{
	unsigned char			status ; /* 会话状态 */
	unsigned char			type ; /* 侦听端、客户端、服务端、管理服务端 */
	
	int				sock ; /* sock描述字 */
	struct NetAddress		netaddr ; /* 网络地址结构 */
	struct ForwardRule		*p_forward_rule ; /* 转发规则 */
	unsigned long			client_index ; /* 客户端索引 */
	unsigned long			forward_index ; /* 转发端索引 */
	unsigned long			server_index ; /* 服务端索引 */
	
	char				io_buffer[ IO_BUFFER_SIZE + 1 ] ; /* 输入输出缓冲区 */
	int				io_buffer_len ; /* 输入输出缓冲区有效数据长度 */
	int				io_buffer_offsetpos ; /* 输入输出缓冲区有效数据开始偏移量 */
	
	struct ForwardSession		*p_reverse_forward_session ; /* 反向会话 */
} ;

/* 命令行参数 */
struct CommandParameter
{
	char				*config_pathfilename ; /* -f ... */
	unsigned long			forward_thread_size ; /* -t ... */
	unsigned long			forward_session_size ; /* -s ... */
	int				log_level ; /* --log-level (DEBUG|INFO|WARN|ERROR|FATAL)*/
	int				no_daemon_flag ; /* --no-daemon (1|0) */
} ;

/* 服务器环境结构 */
struct PipeFds
{
	int				fds[ 2 ] ;
} ;

extern struct ServerEnv			*g_penv ;
extern int				g_exit_flag ;

struct ServerEnv
{
	struct CommandParameter		cmd_para ; /* 命令行参数结构 */
	char				**argv ;
	
	struct ForwardNetAddress	*old_forward_addr_array ;
	unsigned int			old_forward_addr_count ;
	
	struct ForwardRule		*forward_rule_array ; /* 转发规则数组 */
	unsigned long			forward_rule_size ; /* 转发规则数组大小 */
	unsigned long			forward_rule_count ; /* 转发规则已装载数量 */
	
	pid_t				pid ; /* 子进程PID */
	struct PipeFds			accept_request_pipe ; /* 父-子进程请求命令管道 */
	struct PipeFds			accept_response_pipe ; /* 父-子进程响应命令管道 */
	int				accept_epoll_fd ; /* 侦听端口epoll池 */
	
	int				*forward_thread_index ; /* 子线程序号 */
	pthread_t			*forward_thread_tid_array ; /* 子线程TID */
	struct PipeFds			*forward_request_pipe ; /* 父-子线程请求命令管道 */
	struct PipeFds			*forward_response_pipe ; /* 父-子线程响应命令管道 */
	int				*forward_epoll_fd_array ; /* 子线程转发EPOLL池 */
	
	struct ForwardSession		*forward_session_array ; /* 转发会话数组 */
	unsigned long			forward_session_count ; /* 转发会话数组大小 */
	unsigned long			forward_session_use_offsetpos ; /* 转发会话最近使用单元偏移量 */
	
	pthread_mutex_t			mutex ; /* 临界区互斥 */
} ;

/********* util *********/

int Rand( int min, int max );
unsigned long CalcHash( char *str );
void SetReuseAddr( int sock );
void SetNonBlocking( int sock );
void SetCloseExec( int sock );
void SetCloseExec2( int sock , int sock2 );
void SetCloseExec3( int sock , int sock2 , int sock3 );
void SetCloseExec4( int sock , int sock2 , int sock3 , int sock4 );
void SetNetAddress( struct NetAddress *p_netaddr );
void GetNetAddress( struct NetAddress *p_netaddr );
int BindDaemonServer( char *pcServerName , int (* ServerMain)( void *pv ) , void *pv , int (* ControlMain)(long lControlStatus) );
int IsMatchString(char *pcMatchString, char *pcObjectString, char cMatchMuchCharacters, char cMatchOneCharacters);

/********* Envirment *********/

int InitEnvirment( struct ServerEnv *penv );
void CleanEnvirment( struct ServerEnv *penv );
int SaveListenSockets( struct ServerEnv *penv );
int LoadOldListenSockets( struct ServerEnv *penv );
int CleanOldListenSockets( struct ServerEnv *penv );
int AddListeners( struct ServerEnv *penv );
struct ForwardSession *GetForwardSessionUnused( struct ServerEnv *penv );
#define IsForwardSessionUsed(_p_forward_session_)	((_p_forward_session_)->status!=FORWARD_SESSION_STATUS_UNUSED?1:0)
void SetForwardSessionUnused( struct ServerEnv *penv , struct ForwardSession *p_forward_session );
void SetForwardSessionUnused2( struct ServerEnv *penv , struct ForwardSession *p_forward_session , struct ForwardSession *p_forward_session2 );

/********* Config *********/

int LoadConfig( struct ServerEnv *penv );
void UnloadConfig( struct ServerEnv *penv );

/********* MonitorProcess *********/

int MonitorProcess( struct ServerEnv *penv );
int _MonitorProcess( void *pv );

/********* WorkerProcess *********/

int WorkerProcess( struct ServerEnv *penv );

/********* AcceptThread *********/

void *AcceptThread( struct ServerEnv *penv );
void *_AcceptThread( void *pv );

/********* ForwardThread *********/

void *ForwardThread( struct ServerEnv *penv );
void *_ForwardThread( void *pv );

#endif

