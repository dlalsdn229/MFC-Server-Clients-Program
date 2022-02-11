// SocketThread.cpp : ���� �����Դϴ�.
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
	OutputDebugString(_T("SocketThread �Ҹ���\n"));
}

//CreateThread()�� ����
BOOL CSocketThread::InitInstance()
{
	// TODO: ���⿡�� �� �����忡 ���� �ʱ�ȭ�� �����մϴ�.
	m_ClientSocket.Attach(m_hSocket);
	m_ClientSocket.m_pThread = this;
	m_ClientSocket.SetWnd(m_hWnd);

	//ServerSocket���� Ȱ���ϴ� Sendmessage �̰����� �̵�
	SendMessage(m_hWnd, WM_ACCEPT_SOCKET, 0, (LPARAM)&m_ClientSocket);

	return TRUE;
}

int CSocketThread::ExitInstance()
{
	// TODO: ���⿡�� �� �����忡 ���� ������ �����մϴ�.
	return CWinThread::ExitInstance();
}

void CSocketThread::SetWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}


BEGIN_MESSAGE_MAP(CSocketThread, CWinThread)
END_MESSAGE_MAP()


// CSocketThread �޽��� ó�����Դϴ�.

