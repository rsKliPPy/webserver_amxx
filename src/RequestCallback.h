#ifndef _REQUESTCALLBACK_H
#define _REQUESTCALLBACK_H

#include <amtl/am-string.h>

struct RequestCallback {
public:
	int m_Forward;
	ke::AString m_Ident;

	RequestCallback(const char *ident, int forward) {
		m_Ident = ident;
		m_Forward = forward;
	}

	~RequestCallback() {
		MF_UnregisterSPForward(m_Forward);
	}
};

#endif