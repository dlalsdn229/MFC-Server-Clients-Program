// SendThread.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCServer.h"
#include "SendThread.h"


// CSendThread

IMPLEMENT_DYNCREATE(CSendThread, CWinThread)

CSendThread::CSendThread()
{
	exit_flag =false;
	batch = 1000;
}

CSendThread::~CSendThread()
{
	OutputDebugString(_T("SendThread 소멸자\n"));
}

BOOL CSendThread::InitInstance()
{
	// TODO: 여기에서 각 스레드에 대한 초기화를 수행합니다.
	
	return TRUE;
}

int CSendThread::ExitInstance()
{
	// TODO: 여기에서 각 스레드에 대한 정리를 수행합니다.
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSendThread, CWinThread)
END_MESSAGE_MAP()


// CSendThread 메시지 처리기입니다.

void CSendThread::SetWnd(HWND hwnd){
	m_hwnd = hwnd;
}
void CSendThread::SetClient_id(int id){
	client_id = id;
}
int CSendThread::Run()
{	int seq=1;
	
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	Sleep(500);

	while(true){
		try{
			if(exit_flag){
				return 0;
			}
			
			if(batch != 0){
				if(seq-1 >= batch){					
					batch += batch_tmp;
					this->SuspendThread();
				}
			}
			CString str = _T("");
			//client_id:SCSA:br_flag:(seq/ack)_num:TEXT			
			str.Format(_T("%d:0:This is Server SEQ 01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789:%d:2000:"),client_id,seq);	
			seq++;

			m_ptrclientsocket->Send((LPVOID)(LPCTSTR)str,str.GetLength() *2);
			
		}
		catch(CException* e){
			e->ReportError();
		}

		Sleep(1);
	}
	return CWinThread::Run();
}
