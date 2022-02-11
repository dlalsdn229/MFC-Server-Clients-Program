// ServerSocket.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCServer.h"
#include "ServerSocket.h"
#include "SocketThread.h"

// CServerSocket

CServerSocket::CServerSocket()
{
	client_id = 0;
}

CServerSocket::~CServerSocket()
{
	OutputDebugString(_T("ServerSocket �Ҹ���\n"));
}


// CServerSocket ��� �Լ�
void CServerSocket::SetWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}



void CServerSocket::OnAccept(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	CSocket soc;
	if(Accept(soc))
	{		
		client_id++;
		
		CSocketThread *pThread = (CSocketThread *)AfxBeginThread(RUNTIME_CLASS(CSocketThread),THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		pThread->SetWnd(m_hWnd);

		if(!pThread)
		{
			soc.Close();
			AfxMessageBox(_T("ERROR : Thread could not be created."));
			return;
		}

		// ������ ������ �и��ؼ� ������ �ڵ��� �����忡�� �Ѱ��ش�
		pThread->m_hSocket = soc.Detach();
		m_ptrSocketThreadList.AddTail(pThread);

		CSendThread *sendThread = (CSendThread *)AfxBeginThread(RUNTIME_CLASS(CSendThread), THREAD_PRIORITY_NORMAL,0, CREATE_SUSPENDED);
		sendThread->SetWnd(m_hWnd);

		if(!sendThread){
			soc.Close();
			AfxMessageBox(_T("ERROR : ���� ������� ����������� send ������ ������ �����Ͽ����ϴ�."));
			return;
		}

		sendThread->m_ptrclientsocket = &pThread->m_ClientSocket;
		
		sendThread->SetClient_id(client_id);
		sendThread->batch = batch_size;
		sendThread->batch_tmp = batch_size;

		m_ptrSendThreadList.AddTail(sendThread);
		// ������ �����带 �����Ų��
		

		pThread->ResumeThread();
		sendThread->ResumeThread();

		soc.Close();
		
	}

	CSocket::OnAccept(nErrorCode);
}
