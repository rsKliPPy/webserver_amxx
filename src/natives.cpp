#include "main.h"
#include <fcntl.h>
#include <errno.h>

#if defined _WIN32
#define openfd _open
#define seekfd _lseek
#define FDMODE (_O_RDONLY | _O_SEQUENTIAL | _O_BINARY)
#else
#define openfd open
#define seekfd lseek
#define FDMODE (O_RDONLY)
#endif


#define AMXNATIVE(func) static cell AMX_NATIVE_CALL func(AMX *amx, cell *params)
#define CHECKHANDLE(handle) \
		if(handle < 0 || (size_t)handle > g_ResponseHandles.length() || g_ResponseHandles[handle]->IsFree()) \
		{ \
			MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle %i", handle); \
			return 0; \
		}


//forward callback(WSConnection: connection, WSMethod: method, const url[]);
//native WS_RegisterRequestCallback(const id[], const callback[]);
AMXNATIVE(RegisterRequestCallback)
{
	if (g_HTTPDaemon == nullptr)
	{
		return 0;
	}

	int len;
	const char *ident = MF_GetAmxString(amx, params[1], 0, &len);
	const char *callback = MF_GetAmxString(amx, params[2], 1, &len);
	int forward = MF_RegisterSPForwardByName(amx, callback, FP_CELL, FP_STRING, FP_STRING, FP_DONE);

	if (forward < 1) 
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Function %s not found.", callback);
		return -1;
	}

	g_RequestCallbacks.append(new RequestCallback(ident, forward));

	return 1;
}

//native WSResponse: WS_CreateStringResponse(const content[] = "");
AMXNATIVE(CreateStringResponse)
{
	if (g_HTTPDaemon == nullptr)
	{
		return -1;
	}

	int len;
	const char *content = MF_GetAmxString(amx, params[1], 0, &len);

	WSResponse *response = new WSStringResponse(content);

	len = g_ResponseHandles.length();
	g_ResponseHandles.append(response);

	return len;
}

//native WSResponse: WS_CreateBinaryResponse(const content[] = "", length = 0);
AMXNATIVE(CreateBinaryResponse)
{
	if (g_HTTPDaemon == nullptr)
	{
		return -1;
	}

	int len;
	cell *content = MF_GetAmxAddr(amx, params[1]);

	WSResponse *response = new WSBinaryResponse(content, params[2]);

	len = g_ResponseHandles.length();
	g_ResponseHandles.append(response);

	return len;
}

//native WSResponse: WS_CreateFileResponse(const path[]);
AMXNATIVE(CreateFileResponse)
{
	if (g_HTTPDaemon == nullptr)
	{
		return -1;
	}

	int len;
	const char *path = MF_GetAmxString(amx, params[1], 0, &len);

	int fd = openfd(MF_BuildPathname("%s", path), FDMODE);
	if (fd < 0)
	{
		MF_Log("Couldn't open file for reading, errno: %d", errno);
		return -1;
	}

	WSFileResponse *response = new WSFileResponse(seekfd(fd, 0, SEEK_END), fd);
	len = g_ResponseHandles.length();
	g_ResponseHandles.append(response);

	return len;
}

//native WS_AddHeader(WSResponse: response, const header[], const content[]);
AMXNATIVE(AddHeader)
{
	if (g_HTTPDaemon == nullptr)
	{
		return 0;
	}

	cell handle = params[1];
	CHECKHANDLE(handle);

	int len;
	const char *header = MF_GetAmxString(amx, params[2], 0, &len);
	const char *content = MF_GetAmxString(amx, params[3], 1, &len);

	return MHD_add_response_header(g_ResponseHandles[handle]->m_Response, header, content);
}

//native WS_RemoveHeader(WSResponse: response, const header[], const content[] = "");
AMXNATIVE(RemoveHeader)
{
	if (g_HTTPDaemon == nullptr)
	{
		return 0;
	}

	cell handle = params[1];
	CHECKHANDLE(handle);

	int len;
	const char *header = MF_GetAmxString(amx, params[2], 0, &len);
	const char *content = MF_GetAmxString(amx, params[3], 1, &len);
	if (*content == '\0')
	{
		return MHD_del_response_header(g_ResponseHandles[handle]->m_Response, header, content);
	}

	MHD_Response *response = g_ResponseHandles[handle]->m_Response;

	// Again thanks to Asherkin
	int success = 0;
	const char *value;
	while ((value = MHD_get_response_header(response, header)))
	{
		success = MHD_del_response_header(response, header, value) || success;
	}

	return !!success;
}

//native WS_DestroyResponse(&WSResponse: response);
AMXNATIVE(DestroyResponse)
{
	if (g_HTTPDaemon == nullptr)
	{
		return 0;
	}

	cell *handle = MF_GetAmxAddr(amx, params[1]);
	if (*handle < 0 || (size_t)*handle > g_ResponseHandles.length() || g_ResponseHandles[*handle]->IsFree())
	{
		*handle = -1;
		return 0;
	}

	g_ResponseHandles[*handle]->Clear();
	*handle = -1;
	return 1;
}

//native WS_GetClientAddress(WSConnection: connection, ip[WS_ADDRESS_LENGTH], length);
AMXNATIVE(GetClientAddress)
{
	if (g_HTTPDaemon == nullptr)
	{
		return 0;
	}

	MHD_Connection *connection = (MHD_Connection *)params[1];

	const struct sockaddr_in *addr = (struct sockaddr_in *)MHD_get_connection_info(connection, MHD_CONNECTION_INFO_CLIENT_ADDRESS)->client_addr;
	if (addr == nullptr)
	{
		return 0;
	}

	char *ip = inet_ntoa(addr->sin_addr);
	if (ip == nullptr)
	{
		return 0;
	}

	MF_SetAmxString(amx, params[2], ip, params[3]);
	return 1;
}

//native WS_QueueResponse(WSConnection: connection, WSStatusCode: status, WSResponse: response);
AMXNATIVE(QueueResponse)
{
	if (g_HTTPDaemon == nullptr)
	{
		return 0;
	}

	// Should there be some check for the connection parameter? Let's trust the user
	MHD_Connection *connection = (MHD_Connection *)params[1];
	cell statusCode = params[2];
	cell responseHandle = params[3];

	CHECKHANDLE(responseHandle);

	WSResponse *response = g_ResponseHandles[responseHandle];

	return MHD_queue_response(connection, statusCode, response->m_Response);
}

//native WS_GetArgument(WSConnection: connection, WSRequestType: type, const key[], value[], valuelength);
AMXNATIVE(GetArgument)
{
	if (g_HTTPDaemon == nullptr)
	{
		return 0;
	}

	int len;
	MHD_Connection *connection = (MHD_Connection *)params[1];
	MHD_ValueKind kind = (MHD_ValueKind)params[2];
	const char *key = MF_GetAmxString(amx, params[3], 0, &len);

	const char *value = MHD_lookup_connection_value(connection, kind, key);
	if (value == nullptr)
	{
		return 0;
	}

	MF_SetAmxString(amx, params[4], value, params[5]);
	return 1;
}


AMX_NATIVE_INFO g_NativeList[] =
{
	{"WS_RegisterRequestCallback", RegisterRequestCallback},

	{"WS_CreateStringResponse", CreateStringResponse},
	{"WS_CreateBinaryResponse", CreateBinaryResponse},
	{"WS_CreateFileResponse", CreateFileResponse},
	{"WS_QueueResponse", QueueResponse},
	{"WS_DestroyResponse", DestroyResponse},

	{"WS_AddHeader", AddHeader},
	{"WS_RemoveHeader", RemoveHeader},
	
	{"WS_GetClientAddress", GetClientAddress},
	{"WS_GetArgument", GetArgument},

	{nullptr, nullptr}
};
