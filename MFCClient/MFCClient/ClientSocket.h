#pragma once

#include<vector>

using namespace std;

// CClientSocket 명령 대상입니다.

#define WM_CLIENT_RECV WM_USER+1
#define WM_CLIENT_CLOSE WM_USER+3

class CClientSocket : public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();
	void SetWnd(HWND hwnd);
	HWND m_hWnd;


	int postack;
	int postseq;

	void setpostack(int ack);
	void setpostseq(int seq);
	int getpostack();
	int getpostseq();
	

	bool index_errorflag;
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


