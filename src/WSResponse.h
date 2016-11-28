#ifndef _WSRESPONSE_H
#define _WSRESPONSE_H

#include <sys/types.h>
#ifndef _WIN32
#include <sys/select.h>
#include <sys/socket.h>
#else
#include <winsock2.h>
#endif
#include <microhttpd.h>

#include <stdio.h>

#include <amtl/am-string.h>



class WSResponse {
private:
	bool m_Free;

public:
	MHD_Response *m_Response;

public:
	enum ResponseType {
		StringResponse,
		BinaryResponse,
		FileResponse
	};

	virtual ResponseType GetType() const = 0;
	virtual ~WSResponse() {
		Clear();
	}

	bool IsFree() {
		return m_Free;
	}

	void Clear() {
		m_Free = true;
		MHD_destroy_response(m_Response);
		Free();
	}

private:
	WSResponse(const WSResponse&);
	WSResponse& operator=(const WSResponse&);

protected:
	WSResponse() {
		m_Free = false;
		m_Response = nullptr;
	}

	virtual void Free() { }
};



class WSStringResponse : public WSResponse {
private:
	ke::AString m_Content;

public:
	WSStringResponse(const char *content) {
		m_Content = content;
		m_Response = MHD_create_response_from_buffer(strlen(content), (void *)content, MHD_RESPMEM_PERSISTENT);
	}

	ResponseType GetType() const override {
		return StringResponse;
	}
};



class WSBinaryResponse : public WSResponse {
private:
	cell *m_Content;

public:
	WSBinaryResponse(cell *content, cell length) {
		m_Content = new cell[length];
		memcpy(m_Content, content, length * sizeof(cell));
	}

	ResponseType GetType() const override {
		return BinaryResponse;
	}

	void Free() override {
		delete[] m_Content;
	}
};



class WSFileResponse : public WSResponse {
public:
	WSFileResponse(size_t size, int fd) {
		m_Response = MHD_create_response_from_fd(size, fd);
	}

	ResponseType GetType() const override {
		return FileResponse;
	}
};

#endif