// ServerSocket.cpp : 구현 파일입니다.
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
	OutputDebugString(_T("ServerSocket 소멸자\n"));
}


// CServerSocket 멤버 함수
void CServerSocket::SetWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}



void CServerSocket::OnAccept(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
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

		// 접속한 소켓을 분리해서 소켓의 핸들을 스레드에게 넘겨준다
		pThread->m_hSocket = soc.Detach();
		m_ptrSocketThreadList.AddTail(pThread);

		CSendThread *sendThread = (CSendThread *)AfxBeginThread(RUNTIME_CLASS(CSendThread), THREAD_PRIORITY_NORMAL,0, CREATE_SUSPENDED);
		sendThread->SetWnd(m_hWnd);

		if(!sendThread){
			soc.Close();
			AfxMessageBox(_T("ERROR : 소켓 쓰레드는 만들어졌으나 send 쓰레드 생성에 실패하였습니다."));
			return;
		}

		sendThread->m_ptrclientsocket = &pThread->m_ClientSocket;
		
		sendThread->SetClient_id(client_id);
		sendThread->batch = batch_size;
		sendThread->batch_tmp = batch_size;

		m_ptrSendThreadList.AddTail(sendThread);
		// 생성한 스레드를 실행시킨다
		

		pThread->ResumeThread();
		sendThread->ResumeThread();

		soc.Close();
		
	}

	CSocket::OnAccept(nErrorCode);
}
