#pragma once

#include "ClientSocket.h"
#include "afxmt.h"

// CSendThread

class CSendThread : public CWinThread
{
	DECLARE_DYNCREATE(CSendThread)

protected:
	CSendThread();           // 동적 만들기에 사용되는 protected 생성자입니다.

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual ~CSendThread();
	int client_id;
	bool exit_flag;

	int batch;
	int batch_tmp;
	
	//clientsocketThread의 clientsocket 인스턴스를 가리키는 포인터
	CClientSocket *m_ptrclientsocket;
	HWND m_hwnd;
	void SetWnd(HWND hwnd);
	void Setclient_id(int id);

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};


