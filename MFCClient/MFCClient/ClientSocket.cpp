// ClientSocket.cpp : 구현 파일입니다.
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
	OutputDebugString(_T("ClientSocket 소멸자\n"));
}


// CClientSocket 멤버 함수
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	TCHAR szBuffer[16384];
	ZeroMemory(szBuffer, sizeof(szBuffer));
	
	//재요청 단계


	//일반 송수신 단계	
	if(Receive(szBuffer, sizeof(szBuffer)) > 0)
	{ 		
		//OutputDebugString(szBuffer);
		
		SendMessage(m_hWnd, WM_CLIENT_RECV,(WPARAM)this,(LPARAM)szBuffer);	
	}
	
	CSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	ShutDown();

	CSocket::OnClose(nErrorCode);
}
