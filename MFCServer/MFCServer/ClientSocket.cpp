// ClientSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCServer.h"
#include "ClientSocket.h"


// CClientSocket

CClientSocket::CClientSocket()
{
	postack=0;
	postseq=0;
}

CClientSocket::~CClientSocket()
{
	OutputDebugString(_T("Clientsocket 소멸자\n"));
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
	postseq =seq;
}

int CClientSocket::getpostseq()
{
	return postseq;
}

void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CString strTmp = _T(""), strIPAddress = _T("");
	UINT uPortNumber = 0;
	TCHAR strBuffer[16384];
	ZeroMemory(strBuffer, sizeof(strBuffer));

	GetPeerName(strIPAddress, uPortNumber);
	if(Receive(strBuffer, sizeof(strBuffer)) > 0) { // 전달된 데이터(문자열)가 있을 경우
		
		
		strTmp.Format(_T("%s"),strBuffer);
		//CString size = _T("");
		//size.Format(_T("%d\n"),strTmp.GetLength());
		//OutputDebugString(size);
		

	}
	//Main Window에 Send
	SendMessage(m_hWnd,WM_CLIENT_MSG_RECV,(WPARAM)this,(LPARAM)((LPCTSTR)strTmp));
	

	CSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	SendMessage(m_hWnd, WM_CLIENT_CLOSE, 0, (LPARAM)this);
	CSocket::OnClose(nErrorCode);
}
