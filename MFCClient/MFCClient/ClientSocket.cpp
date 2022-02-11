// ClientSocket.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCClient.h"
#include "ClientSocket.h"

// CClientSocket

CClientSocket::CClientSocket()
{
	postack = 0;
	postseq=0;
	index_errorflag = false;
}

CClientSocket::~CClientSocket()
{
	OutputDebugString(_T("ClientSocket �Ҹ���\n"));
}


// CClientSocket ��� �Լ�
void CClientSocket::SetWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

void CClientSocket::setpostack(int ack){
	postack = ack;
}
int CClientSocket::getpostack(){
	return postack;
}

void CClientSocket::setpostseq(int seq){
	postseq = seq;
}

int CClientSocket::getpostseq(){
	return postseq;
}



void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	TCHAR szBuffer[16384];
	ZeroMemory(szBuffer, sizeof(szBuffer));
	
	//���û �ܰ�


	//�Ϲ� �ۼ��� �ܰ�	
	if(Receive(szBuffer, sizeof(szBuffer)) > 0)
	{ 		
		//OutputDebugString(szBuffer);
		
		SendMessage(m_hWnd, WM_CLIENT_RECV,(WPARAM)this,(LPARAM)szBuffer);	
	}
	
	CSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	ShutDown();

	CSocket::OnClose(nErrorCode);
}
