#pragma once

// CClientSocket 명령 대상입니다.

#define WM_CLIENT_MSG_RECV WM_USER+2
#define WM_CLIENT_CLOSE WM_USER+3

class CClientSocket : public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();

	void SetWnd(HWND hWnd);
	HWND m_hWnd;

	CWinThread *m_pThread;
	int postack;
	int postseq;
	void setpostack(int ack);
	void setpostseq(int seq);
	int getpostack();
	int getpostseq();
	
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};