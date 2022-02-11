#pragma once

#include "ClientSocket.h"  //ClientSocket ��� �߰�

#define WM_ACCEPT_SOCKET WM_USER+1  //ServerSocket�� �ִ� �޽��� ������� �̵�

// CSocketThread

class CSocketThread : public CWinThread
{
	DECLARE_DYNCREATE(CSocketThread)

protected:
	CSocketThread();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CSocketThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	CClientSocket m_ClientSocket;
	SOCKET m_hSocket;

	HWND m_hWnd;
	void SetWnd(HWND hWnd);

protected:
	DECLARE_MESSAGE_MAP()
};


