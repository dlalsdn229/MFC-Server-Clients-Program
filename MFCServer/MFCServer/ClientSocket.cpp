// ClientSocket.cpp : ���� �����Դϴ�.
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
	OutputDebugString(_T("Clientsocket �Ҹ���\n"));
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
	postseq =seq;
}

int CClientSocket::getpostseq()
{
	return postseq;
}

void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CString strTmp = _T(""), strIPAddress = _T("");
	UINT uPortNumber = 0;
	TCHAR strBuffer[16384];
	ZeroMemory(strBuffer, sizeof(strBuffer));

	GetPeerName(strIPAddress, uPortNumber);
	if(Receive(strBuffer, sizeof(strBuffer)) > 0) { // ���޵� ������(���ڿ�)�� ���� ���
		
		
		strTmp.Format(_T("%s"),strBuffer);
		//CString size = _T("");
		//size.Format(_T("%d\n"),strTmp.GetLength());
		//OutputDebugString(size);
		

	}
	//Main Window�� Send
	SendMessage(m_hWnd,WM_CLIENT_MSG_RECV,(WPARAM)this,(LPARAM)((LPCTSTR)strTmp));
	

	CSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	SendMessage(m_hWnd, WM_CLIENT_CLOSE, 0, (LPARAM)this);
	CSocket::OnClose(nErrorCode);
}
