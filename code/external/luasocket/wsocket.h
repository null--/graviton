#ifndef WSOCKET_H
#define WSOCKET_H
/*=========================================================================*\
* Socket compatibilization module for Win32
* LuaSocket toolkit
*
* RCS ID: $Id: wsocket.h,v 1.4 2005/10/07 04:40:59 diego Exp $
\*=========================================================================*/

/*=========================================================================*\
* WinSock include files
\*=========================================================================*/
#include <winsock.h>

/*-------------------------------------------------------------------------*\
* Wait for readable/writable/connected socket with timeout
\*-------------------------------------------------------------------------*/
#define WAITFD_R        1
#define WAITFD_W        2
#define WAITFD_E        4
#define WAITFD_C        (WAITFD_E|WAITFD_W)

typedef int socklen_t;
typedef SOCKET t_socket;
typedef t_socket *p_socket;

#define SOCKET_INVALID (INVALID_SOCKET)

#endif /* WSOCKET_H */
