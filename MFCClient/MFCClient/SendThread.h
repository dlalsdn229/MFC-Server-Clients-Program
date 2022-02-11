#pragma once

#include "ClientSocket.h"
#include "afxmt.h"

// CSendThread

class CSendThread : public CWinThread
{
	DECLARE_DYNCREATE(CSendThread)

protected:
	CSendThread();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual ~CSendThread();
	int client_id;
	bool exit_flag;

	int batch;
	int batch_tmp;
	
	//clientsocketThread�� clientsocket �ν��Ͻ��� ����Ű�� ������
	CClientSocket *m_ptrclientsocket;
	HWND m_hwnd;
	void SetWnd(HWND hwnd);
	void Setclient_id(int id);

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};


