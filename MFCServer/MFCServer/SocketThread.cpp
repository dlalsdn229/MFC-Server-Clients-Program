// SocketThread.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCServer.h"
#include "SocketThread.h"


// CSocketThread

IMPLEMENT_DYNCREATE(CSocketThread, CWinThread)

CSocketThread::CSocketThread()
{
}

CSocketThread::~CSocketThread()
{
	OutputDebugString(_T("SocketThread 소멸자\n"));
}

//CreateThread()시 실행
BOOL CSocketThread::InitInstance()
{
	// TODO: 여기에서 각 스레드에 대한 초기화를 수행합니다.
	m_ClientSocket.Attach(m_hSocket);
	m_ClientSocket.m_pThread = this;
	m_ClientSocket.SetWnd(m_hWnd);

	//ServerSocket에서 활용하던 Sendmessage 이곳으로 이동
	SendMessage(m_hWnd, WM_ACCEPT_SOCKET, 0, (LPARAM)&m_ClientSocket);

	return TRUE;
}

int CSocketThread::ExitInstance()
{
	// TODO: 여기에서 각 스레드에 대한 정리를 수행합니다.
	return CWinThread::ExitInstance();
}

void CSocketThread::SetWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}


BEGIN_MESSAGE_MAP(CSocketThread, CWinThread)
END_MESSAGE_MAP()


// CSocketThread 메시지 처리기입니다.

