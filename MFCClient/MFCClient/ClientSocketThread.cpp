// ClientSocketThread.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCClient.h"
#include "ClientSocketThread.h"

#define PORT 21000

// CClientSocketThread

IMPLEMENT_DYNCREATE(CClientSocketThread, CWinThread)

CClientSocketThread::CClientSocketThread()
{
	

}

CClientSocketThread::~CClientSocketThread()
{
	OutputDebugString(_T("ClientSocketThread �Ҹ���\n"));
	
	//m_threadKillEvent.SetEvent();
}

BOOL CClientSocketThread::InitInstance()
{
	// TODO: ���⿡�� �� �����忡 ���� �ʱ�ȭ�� �����մϴ�.
	exit_flag=false;
	//���ϸ���� ��ü�̴�
	//�� ���� ��ü�� ���ؼ� send�Ѵ�
	m_clientsocket.SetWnd(hWnd);
	m_clientsocket.Create();
	if(m_clientsocket.Connect(_T("127.0.0.1"), PORT) == FALSE) {
		AfxMessageBox(_T("ERROR : Failed to connect Server"));
		PostQuitMessage(0);
		return FALSE;
	}
	return TRUE;

	
}

int CClientSocketThread::ExitInstance()
{
	// TODO: ���⿡�� �� �����忡 ���� ������ �����մϴ�.
	

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CClientSocketThread, CWinThread)
END_MESSAGE_MAP()


// CClientSocketThread �޽��� ó�����Դϴ�.
void CClientSocketThread::SetWnd(HWND hwnd)
{
	hWnd = hwnd;
}

void CClientSocketThread::Endthread(){
	PostQuitMessage(0);
	
}

int CClientSocketThread::Run()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	OutputDebugString(_T("Run\n"));
	
	return CWinThread::Run();
}
