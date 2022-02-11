// ClientSocketThread.cpp : 구현 파일입니다.
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
	OutputDebugString(_T("ClientSocketThread 소멸자\n"));
	
	//m_threadKillEvent.SetEvent();
}

BOOL CClientSocketThread::InitInstance()
{
	// TODO: 여기에서 각 스레드에 대한 초기화를 수행합니다.
	exit_flag=false;
	//소켓만드는 본체이다
	//이 소켓 객체를 통해서 send한다
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
	// TODO: 여기에서 각 스레드에 대한 정리를 수행합니다.
	

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CClientSocketThread, CWinThread)
END_MESSAGE_MAP()


// CClientSocketThread 메시지 처리기입니다.
void CClientSocketThread::SetWnd(HWND hwnd)
{
	hWnd = hwnd;
}

void CClientSocketThread::Endthread(){
	PostQuitMessage(0);
	
}

int CClientSocketThread::Run()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	OutputDebugString(_T("Run\n"));
	
	return CWinThread::Run();
}
