#pragma once

#include "ClientSocket.h"

// CSendThread

class CSendThread : public CWinThread
{
	DECLARE_DYNCREATE(CSendThread)

protected:
	CSendThread();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	

public:
	virtual ~CSendThread();

	bool exit_flag;
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	HWND m_hwnd;
	void SetWnd(HWND hwnd);
	
	//batch parameter
	int batch;
	int batch_tmp;

	int client_id;
	void SetClient_id(int id);

	CClientSocket* m_ptrclientsocket;
	

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int Run();
};


