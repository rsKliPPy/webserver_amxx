#include "main.h"


struct MHD_Daemon *g_HTTPDaemon = nullptr;
MHD_Response *g_NotFoundResponse = nullptr;
ke::Vector<WSResponse *> g_ResponseHandles;
ke::Vector<RequestCallback *> g_RequestCallbacks;

const char * const g_NotFoundPage = " \
	<html><head><title>404 Not Found</title></head> \
	<body> \
	<span style=\"font-size: 32px;\">404 Not Found</span><br/> \
	<span style=\"font-size: 24px;\">The requested page was not found on this server</span> \
	</body></html>";


void OnAmxxDetach(void) 
{
	if (g_HTTPDaemon != nullptr)
	{
		MHD_stop_daemon(g_HTTPDaemon);
	}

	g_ResponseHandles.clear();
	g_RequestCallbacks.clear();

	RETURN_META(MRES_IGNORED);
}

void OnAmxxAttach(void) 
{
	MF_AddNatives(g_NativeList);

	const char *szIp = CVAR_GET_STRING("net_address");
	const char *szPort;
	uint16_t iPort;

	if ((szPort = strstr(szIp, ":")) != nullptr && (szPort + 1) != '\0')
	{
		iPort = atoi(szPort + 1);
	}
	else 
	{
		MF_Log("HTTP Daemon failed to start. net_address cvar contains no port.");
		return;
	}

	g_HTTPDaemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, iPort, NULL, NULL, &MHDConnectionCallback, NULL, MHD_OPTION_END);

	if (g_HTTPDaemon == nullptr)
	{
		MF_Log("HTTP Daemon failed to start. MHD_start_daemon() returned nullptr.");
		return;
	}

	g_NotFoundResponse = MHD_create_response_from_buffer(strlen(g_NotFoundPage), (void * const)g_NotFoundPage, MHD_RESPMEM_PERSISTENT);

	MF_Log("Successfully started HTTP Daemon on port %i", iPort);

	RETURN_META(MRES_IGNORED);
}

void OnStartFrame(void)
{
	
}


int MHDConnectionCallback(void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls) {
	char *ident;
	const char *start = url + 1;
	const char *end;

	if (*start == '\0')
	{
		return MHD_queue_response(connection, 404, g_NotFoundResponse);
	}

	if ((end = strstr(start, "/")) != nullptr)
	{
		ident = new char[(end - start) + 1];
		memcpy(ident, start, end - start);
		ident[end - start] = '\0';
	}
	else
	{
		return MHD_queue_response(connection, 404, g_NotFoundResponse);
	}

	int ret;
	for (size_t i = 0; i < g_RequestCallbacks.length(); i++)
	{
		RequestCallback *rc = g_RequestCallbacks[i];
		
		if (FStrEq(rc->m_Ident.chars(), ident))
		{
			ret = MF_ExecuteForward(rc->m_Forward, connection, method, end);
			if (ret == -1) 
			{
				ret = MHD_queue_response(connection, 404, g_NotFoundResponse);
			}

			delete[] ident;
			return ret;
		}
	}

	delete[] ident;
	return MHD_queue_response(connection, 404, g_NotFoundResponse);
}

extern "C" void __cxa_pure_virtual()
{

}
