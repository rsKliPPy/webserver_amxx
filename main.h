#ifndef _MAIN_H_INCLUDED
#define _MAIN_H_INCLUDED

#include <sys/types.h>
#ifndef _WIN32
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif
#include <microhttpd.h>

#include "amxxsdk/amxxmodule.h"
#include <amtl/am-vector.h>
#include "WSResponse.h"
#include "RequestCallback.h"


// We only care for IPv4
#define WS_ADDRESS_LENGTH 16


extern struct MHD_Daemon *g_HTTPDaemon;
extern AMX_NATIVE_INFO g_NativeList[];
extern ke::Vector<WSResponse *> g_ResponseHandles;
extern ke::Vector<RequestCallback *> g_RequestCallbacks;

// Damn long prototype
int MHDConnectionCallback(void *cls, struct MHD_Connection *connection,
	const char *url, const char *method,
	const char *version, const char *upload_data,
	size_t *upload_data_size, void **con_cls);

#endif