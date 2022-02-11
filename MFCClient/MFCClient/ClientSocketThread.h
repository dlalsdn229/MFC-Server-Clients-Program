#pragma once

#include "ClientSocket.h"
//#include<afxmt.h>

// CClientSocketThread

class CClientSocketThread : public CWinThread
{
	DECLARE_DYNCREATE(CClientSocketThread)

protected:
	CClientSocketThread();           // 동적 만들기에 사용되는 protected 생성자입니다.
	

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual ~CClientSocketThread();
	CClientSocket m_clientsocket;
	
	bool exit_flag;
	void Endthread();
	HWND hWnd;
	void SetWnd(HWND hwnd);
	
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual int Run();
};


