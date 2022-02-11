
// MFCServerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MFCServer.h"
#include "MFCServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PORT 21000

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

LPCRITICAL_SECTION g_Lock=NULL;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMFCServerDlg 대화 상자




CMFCServerDlg::CMFCServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	
}

void CMFCServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_LIST1, m_client_list);
	DDX_Control(pDX, IDC_ERROR_LIST, m_error_list);
	DDX_Control(pDX, IDC_LIST2, m_msg_list);
	DDX_Control(pDX, IDC_LIST3, m_msg_list2);	
}

BEGIN_MESSAGE_MAP(CMFCServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_ACCEPT_SOCKET, &CMFCServerDlg::OnAcceptSocket)
	ON_MESSAGE(WM_CLIENT_MSG_RECV, &CMFCServerDlg::OnClientMsgRecv)
	ON_MESSAGE(WM_CLIENT_CLOSE, &CMFCServerDlg::OnClientClose)
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_SUSPEND_BTN, &CMFCServerDlg::OnBnClickedSuspendBtn)
	ON_BN_CLICKED(IDC_RESUME_BTN, &CMFCServerDlg::OnBnClickedResumeBtn)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CMFCServerDlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCServerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_FileDlg, &CMFCServerDlg::OnBnClickedFiledlg)
END_MESSAGE_MAP()


// CMFCServerDlg 메시지 처리기

BOOL CMFCServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	
	ClientCloseflag = false;
	isConnected = false;
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(435);
	//기본값 초기화
	batch_size = 0;
	print_period = 200;
	lines=10000;
	thread_period=2000;
	dir = _T("C:\\Users\\22009601\\Desktop\\savetxt\\Server");
	filename = _T("Server_");
	EndSaveThread=false;

	if(g_Lock == NULL) {
		g_Lock = (LPCRITICAL_SECTION) malloc(sizeof(CRITICAL_SECTION));
		InitializeCriticalSection(g_Lock);
	}

	//Listen 소켓 초기화
	m_pServerSocket = new CServerSocket;
	m_pServerSocket->SetWnd(this->m_hWnd);

	m_pServerSocket->batch_size = batch_size;

	//소켓 Listen하기
	m_pServerSocket->Create(PORT);
	m_pServerSocket->Listen();
	

	//edit 관련 내용 초기화
	m_batch_edit = (CEdit *) GetDlgItem(IDC_EDIT_batch);
	m_batchV_edit = (CEdit *) GetDlgItem(IDC_EDIT_batchV);
	m_period_edit = (CEdit *) GetDlgItem(IDC_EDIT_PERIOD);
	m_periodV_edit = (CEdit *) GetDlgItem(IDC_EDIT_periodV);
	m_line_edit= (CEdit *) GetDlgItem(IDC_EDIT_line);
	m_lineV_edit = (CEdit *) GetDlgItem(IDC_EDIT_lineV);
	
	m_dirV_edit = (CEdit *) GetDlgItem(IDC_EDIT_dirV);
	m_name_edit = (CEdit *) GetDlgItem(IDC_EDIT_name);
	m_nameV_edit = (CEdit *) GetDlgItem(IDC_EDIT_nameV);
	m_sleep_edit = (CEdit *) GetDlgItem(IDC_EDIT_sleep);
	m_sleepV_edit = (CEdit *) GetDlgItem(IDC_EDIT_sleepV);
	
	m_batchV_edit->SetWindowTextW(_T("0"));
	m_periodV_edit->SetWindowTextW(_T("200"));
	m_lineV_edit->SetWindowTextW(_T("10000"));
	m_dirV_edit->SetWindowTextW(dir);
	m_nameV_edit->SetWindowTextW(filename);
	m_sleepV_edit->SetWindowTextW(_T("2000"));

	
	p_savethread = NULL;


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
} 

afx_msg LRESULT CMFCServerDlg::OnAcceptSocket(WPARAM wParam, LPARAM lParam)
{

	isConnected = true;
	//저장 워크스레드 생성 및 실행
	ClientCloseflag = true;

	if(p_savethread == NULL){
		p_savethread = AfxBeginThread(SaveFileThread, this);
	}
	else{
		p_savethread->ResumeThread();
	}
	
	if(p_savethread == NULL){
		AfxMessageBox(_T("저장 Work스레드 Error"));
	}
	//

	
	CString str;
	m_pClientSocket = (CClientSocket*) lParam;
	m_ptrClientSocketList.AddTail(m_pClientSocket);
	

	str.Format(_T("Client (%d)"), (int)(m_pClientSocket));
	m_client_list.InsertString(-1,str);

	m_pClientSocket = NULL;
	delete m_pClientSocket;

	return 0;
}

//이거 정의는
afx_msg LRESULT CMFCServerDlg::OnClientMsgRecv(WPARAM wParam, LPARAM lParam)
{	
	CClientSocket *Client = (CClientSocket *)wParam;

	//LPCTSTR : Long Pointer Constant T_STRing
	LPCTSTR lpszStr = (LPCTSTR)lParam;
	//USES_CONVERSION;
	CString recv_msg = lpszStr;	

	//여기서 받은 메시지를 ':' 단위로 분해하여 사용
	CString recv_msgs[100]={_T("")};
	CString resToken;
	int curPos = 0;
	int i=0;
	resToken = recv_msg.Tokenize(_T(":"),curPos);
	// [0]:client_id / [1]:SCSA_num / [2]br_flag / [3] seqack_num / [4] : TextField
	while(resToken != ""){
		recv_msgs[i] = resToken;
		resToken = recv_msg.Tokenize(_T(":"),curPos);
		i++;
	}

	CString str;
	
	//POSITION head_pos = m_ptrClientSocketList.GetHeadPosition();
	//CClientSocket *pClienthead = (CClientSocket *)m_ptrClientSocketList.GetNext(head_pos);

	POSITION pos = m_ptrClientSocketList.Find(Client);
	CClientSocket *pClient = (CClientSocket *)m_ptrClientSocketList.GetAt(pos);

	int k =1;
	int j =0;
	//BOOL br_flag;

	//[k-1]client_id [k]flag [k+1]text [k+2]num [k+3]SCSA
	while(recv_msgs[k]!=_T("")){
		k = 5*j + 1;
		j++;

		//client seq이면
		if(recv_msgs[k+3] == _T("1000")){
			//broadcasting

			//
			str = recv_msgs[k-1] +_T(":") +recv_msgs[k]+ _T(":This is Server ACK:")+ recv_msgs[k+2] + _T(":2001:"); 
			
			if(pClient != NULL){
				EnterCriticalSection(g_Lock);
					pClient->Send(str, lstrlen(str)*2);
				LeaveCriticalSection(g_Lock);
			}

			CString a = recv_msgs[k-1] +_T(":") + recv_msgs[k]+_T(":") + recv_msgs[k+1]+_T(":")+ recv_msgs[k+2]+_T(":") + recv_msgs[k+3]+_T(":");

			EnterCriticalSection(g_Lock);
				q.push(a);
			LeaveCriticalSection(g_Lock);

			if(_ttoi(recv_msgs[k+2]) % print_period == 0){
				EnterCriticalSection(g_Lock);
					m_msg_list.InsertString(-1,a);
					m_msg_list.SetCurSel(m_msg_list.GetCount()-1);
				LeaveCriticalSection(g_Lock);
			}

			//check_seq
			pClient->setpostseq(pClient->postseq+1);
			int seq;
			CString e;
			seq=pClient->getpostseq();
			//seq 오류시
			if(seq != _ttoi(recv_msgs[k+2])){
				e.Format(_T("Client%d SEQ ERROR seq=%d, recv_msgs=%d"), _ttoi(recv_msgs[k-1]),seq, _ttoi(recv_msgs[k+2]) );
				EnterCriticalSection(g_Lock);
					m_error_list.InsertString(-1,e);
					m_error_list.SetCurSel(m_error_list.GetCount()-1);
				LeaveCriticalSection(g_Lock);
				//seq보정
				pClient->setpostseq(_ttoi(recv_msgs[k+2]));
			}

		}
		else if(recv_msgs[k+3]==_T("1001")){
			CString a = recv_msgs[k-1] +_T(":") + recv_msgs[k]+_T(":") + recv_msgs[k+1]+_T(":")+ recv_msgs[k+2]+_T(":") + recv_msgs[k+3]+_T(":");

			EnterCriticalSection(g_Lock);
				q.push(a);
			LeaveCriticalSection(g_Lock);

			if(_ttoi(recv_msgs[k+2]) % print_period == 0){
				EnterCriticalSection(g_Lock);
					m_msg_list2.InsertString(-1,a);
					m_msg_list2.SetCurSel(m_msg_list2.GetCount()-1);
				LeaveCriticalSection(g_Lock);
			}
			

			//check_ack
			//ack 검사 코드
			pClient->setpostack(pClient->postack+1);
			int ack;
			CString e;
			ack=pClient->getpostack();
			//ack 오류시
			if(ack != _ttoi(recv_msgs[k+2])){
				e.Format(_T("Client%d ACK ERROR ack=%d, recv_msgs=%d"), _ttoi(recv_msgs[k-1]),ack, _ttoi(recv_msgs[k+2]) );
				EnterCriticalSection(g_Lock);
					m_error_list.InsertString(-1,e);
					m_error_list.SetCurSel(m_error_list.GetCount()-1);
				LeaveCriticalSection(g_Lock);
				//ack보정
				pClient->setpostack(_ttoi(recv_msgs[k+2]));
			}
		}
	}
	
	return 0;
}


//닫을 때 발생하는 메시지
//ClientSocket cpp 의 Onclose에서 메시지 옴
afx_msg LRESULT CMFCServerDlg::OnClientClose(WPARAM wParam, LPARAM lParam)
{
	if(ClientCloseflag){
		ClientCloseflag = false;
		CString str;
		UINT idx=0;

		POSITION pos = m_pServerSocket->m_ptrSocketThreadList.GetHeadPosition();

		while(pos != NULL){
			CSocketThread *p = (CSocketThread *)m_pServerSocket->m_ptrSocketThreadList.GetNext(pos);
			if(*p !=NULL){
				p->PostThreadMessage(WM_QUIT,0,0);			
			}
		}
		m_pServerSocket->m_ptrSocketThreadList.RemoveAll();


		POSITION pos2 = m_pServerSocket->m_ptrSendThreadList.GetHeadPosition();

		while(pos2 != NULL){
			
			CSendThread *p = (CSendThread *)m_pServerSocket->m_ptrSendThreadList.GetNext(pos2);
			if(*p != NULL){
				
				p->exit_flag=true;				
			}			
		}		
		m_pServerSocket->m_ptrSendThreadList.RemoveAll();

		if(p_savethread != NULL){
			m_client_list.ResetContent();
			m_ptrClientSocketList.RemoveAll();
			m_error_list.ResetContent();
			m_msg_list.ResetContent();     
			m_msg_list2.ResetContent();
			
			p_savethread->SuspendThread();
			
			m_pServerSocket->client_id = 0;

			while(!q.empty())
			{
				//q를 공유하는 스레드 없음
				q.pop();
			}			
		}	
	}
	isConnected = false;
		
	return 0;
}

void CMFCServerDlg::OnBnClickedSuspendBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	POSITION pos = m_pServerSocket->m_ptrSendThreadList.GetHeadPosition();

	while(pos != NULL){
		CSendThread *p = (CSendThread *)m_pServerSocket->m_ptrSendThreadList.GetNext(pos);
		if(*p != NULL){
			p->SuspendThread();
		}
	}
}

void CMFCServerDlg::OnBnClickedResumeBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	POSITION pos = m_pServerSocket->m_ptrSendThreadList.GetHeadPosition();

	while(pos != NULL){
		CSendThread *p = (CSendThread *)m_pServerSocket->m_ptrSendThreadList.GetNext(pos);
		if(*p != NULL){
			p->ResumeThread();
		}
	}
}

void CMFCServerDlg::OnClose()
{
	if(isConnected)
		OnClientClose(0, 0);
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//m_pServerSocket->Close();
	delete m_pServerSocket;
	delete p_savethread;
	Wait(1000);
	free(g_Lock);

	CDialog::OnClose();
}

void CMFCServerDlg::Wait(DWORD dwMillisecond)

{
    MSG msg;
    DWORD t0, t1, diff; 

    t0 = GetTickCount();

    while(TRUE){
        t1 = GetTickCount();

        if( t0 <= t1 )
			diff = t1 - t0;
        else	
			diff = 0xFFFFFFFF - t0 + t1;

        if( diff > dwMillisecond )
			break;

         while( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Sleep(1);
    }
}

void CMFCServerDlg::OnBnClickedButtonApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	

	CString get_editext = _T("");
	CString info=_T("");
	
	//batch설정
	m_batch_edit->GetWindowTextW(get_editext);	
	
	if(m_ptrClientSocketList.GetCount()!= 0)
	{
		info += _T("* epoch는 클라이언트 연결 중에 변경할 수 없습니다.\n\n");
		m_batch_edit->SetWindowTextW(_T(""));	
	}
	else{
		if(get_editext != _T("") ){
			if(_ttoi(get_editext)<0){
				info += _T("* epoch는 음수가 될 수 없습니다.\n\n");
				m_batch_edit->SetWindowTextW(_T(""));	
			}
			
			else{
				batch_size = _ttoi(get_editext);
				if(batch_size == 0){
					info += _T("o epoch 제한이 해제되었습니다.\n");
				}
				m_batchV_edit->SetWindowTextW(get_editext);
				m_pServerSocket->batch_size = batch_size;
				info += _T("o epoch가 적용되었습니다.\n\n");
				m_batch_edit->SetWindowTextW(_T(""));
			}
		}
	}
	
	
	//출력 주기 설정
	m_period_edit->GetWindowTextW(get_editext);
	
	if(get_editext != _T("")){
		if(_ttoi(get_editext) < 10){
			print_period = 10;
			info += _T("* Lock 방지를 위해 최소 권장값 10으로 설정됩니다.\n\n");
			m_periodV_edit->SetWindowTextW(_T("10"));
			m_period_edit->SetWindowTextW(_T(""));
		}
		else{
			print_period = _ttoi(get_editext);
			m_periodV_edit->SetWindowTextW(get_editext);
			info += _T("o 출력 주기가 적용되었습니다.\n\n");
			m_period_edit->SetWindowTextW(_T(""));
		}
	}

	
	AfxMessageBox(info);
	
		

}

UINT CMFCServerDlg::SaveFileThread(LPVOID pParam){

	CMFCServerDlg* pDlg = (CMFCServerDlg*) AfxGetApp()->m_pMainWnd;
	CMFCServerDlg* fir = (CMFCServerDlg*) pParam;
	while(1){		

		//스레드 동작 주기
		Sleep(pDlg->thread_period);
		//out
		if(pDlg->EndSaveThread){			
			break;
		}
		//저장단위
		EnterCriticalSection(g_Lock);
			int qsize = pDlg->q.size();
		LeaveCriticalSection(g_Lock);
		
		if(qsize < pDlg->lines){
			continue;
		}
		
		//파일 관련 객체
		CStdioFile file;
		CFileException e;
		CFileFind finder;
		
		//저장될 폴더 위치
		CString folderpath = pDlg->dir+_T("\\");
		
		//폴더 생성
		CreateDirectory(folderpath,NULL);
		
		//폴더 안의 텍스트 파일 개수 카운트
		int count=0;	
		BOOL bworking = finder.FindFile(folderpath + _T("*.txt"));
		while(bworking){
			bworking = finder.FindNextFile();
			if(finder.IsDots())
				continue;
			count++;
		}
		finder.Close();
		
		//파일이름 옆에 파일 번호
		CString num;
		num.Format(_T("%d"),count);
		//파일 경로 및 파일 이름,형식
		CString filepath = folderpath + pDlg->filename+ num +_T(".txt");
		
		//파일 오픈에러
		if(!file.Open(filepath, CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate, &e)){
			e.ReportError();			
		}

		//파일 정상 오픈
		else{
			//크리티컬 섹션 시작
			
				//lines 만큼 파일 쓰기
				for(int i =0;i<pDlg->lines;i++){					
					CString data;	
					EnterCriticalSection(g_Lock);
						data = pDlg->q.front();
						pDlg->q.pop();
					LeaveCriticalSection(g_Lock);
					file.WriteString(data + _T("\n"));
				}
			
			//크리티컬 섹션 끝
		}
		//현재시간
		CString currentDate;
		CTime time = CTime::GetCurrentTime();
		currentDate.Format(_T("%d-%d-%d %d:%d:%d"),
             time.GetYear(), time.GetMonth(), time.GetDay(),
             time.GetHour(), time.GetMinute(), time.GetSecond());
		//파일에 파라미터 정보 및 날짜 기록
		CString info;
		info.Format(_T("\nParmeter Setting Information : lines=%d , thread_period=%d (ms)\ndate=%s"),pDlg->lines,pDlg->thread_period,currentDate);
		file.WriteString(info);
		//파일 닫기
		file.Close();
	}
	return 0;

}

//저장 스레드 제어 적용 버튼
void CMFCServerDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_ptrClientSocketList.GetCount() != 0 ){
		AfxMessageBox(_T("클라이언트와 연결중에는 변경할 수 없습니다."));
		return;
	}
	else{
		CString info=_T("");
		CString get_editext;
		//파일 이름 설정
		
		m_name_edit->GetWindowTextW(get_editext);
		if(get_editext != _T("")){
			m_nameV_edit->SetWindowTextW(get_editext);
			filename = get_editext;
			info+=_T("o 파일 이름이 적용되었습니다.\n\n");
			m_name_edit->SetWindowTextW(_T(""));
		}

		//저장 단위 설정
		m_line_edit->GetWindowTextW(get_editext);
		if(get_editext != _T("")){
			if(_ttoi(get_editext) < 2000 || _ttoi(get_editext) > 100000){
				info += _T("* 저장 단위의 범위는 2000 ~ 100000 입니다.\n** 저장 단위가 크면 저장부하에 따라 송수신 오류가 발생할 수 있습니다.\n\n");
				m_line_edit->SetWindowTextW(_T(""));
			}
			else{
				m_lineV_edit->SetWindowTextW(get_editext);
				lines = _ttoi(get_editext);
				info += _T("o 저장 단위가 적용되었습니다.\n* 50000이하 권장\n** 저장 단위가 크면 부하에 따라 Error Log가 발생할 수 있습니다.\n\n");
				m_line_edit->SetWindowTextW(_T(""));
			}		
		}
		//스레드 동작주기 설정
		m_sleep_edit->GetWindowTextW(get_editext);
		if(get_editext != _T("")){
			if(_ttoi(get_editext) < 1000){
				info += _T("* 저장 스레드 동작 주기의 최소값은 1000(ms)입니다.\n\n");
				m_line_edit->SetWindowTextW(_T(""));
			}
			else{
				m_sleepV_edit->SetWindowTextW(get_editext);
				thread_period = _ttoi(get_editext);
				info += _T("o 저장 스레드 동작 주기가 적용되었습니다.\n\n");
				m_sleep_edit->SetWindowTextW(_T(""));
			}	
		}
		if(info != _T("")){
			AfxMessageBox(info);
		}
	}
	
	
}


//파일 다이얼로그
void CMFCServerDlg::OnBnClickedFiledlg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_ptrClientSocketList.GetCount() != 0 ){
		AfxMessageBox(_T("클라이언트와 연결중에는 변경할 수 없습니다."));
		return;
	}
	BROWSEINFO BrInfo;
	TCHAR szBuffer[1024];                                      // 경로저장 버퍼 

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 1024);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = _T("파일이 저장될 폴더를 선택하세요");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);               // 파일경로 읽어오기

    // 경로를 가져와 사용할 경우, Edit Control 에 값 저장
	CString str;
	str.Format(_T("%s"), szBuffer);
	m_dirV_edit->SetWindowTextW(str);
	
	dir = str;
}
