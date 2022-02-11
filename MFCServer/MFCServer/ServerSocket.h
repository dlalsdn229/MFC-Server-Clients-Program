#pragma once

// CServerSocket 명령 대상입니다.

#include "ClientSocket.h"
#include "SendThread.h"
#include "SocketThread.h"

#define WM_ACCEPT_SOCKET WM_USER+1

class CServerSocket : public CSocket
{
public:
	CServerSocket();
	virtual ~CServerSocket();
	
	int client_id;
	CPtrList m_ptrSocketThreadList;
	CPtrList m_ptrSendThreadList;

	int batch_size;

	//CSocketThread *soc_th_arr[20];

	//CSendThread *send_th_arr[20];

	//CSendThread *sendThread;

	void SetWnd(HWND hWnd);
	HWND m_hWnd;
		

	virtual void OnAccept(int nErrorCode);
};


