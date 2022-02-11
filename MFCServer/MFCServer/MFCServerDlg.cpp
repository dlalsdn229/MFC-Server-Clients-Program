
// MFCServerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MFCServer.h"
#include "MFCServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PORT 21000

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

LPCRITICAL_SECTION g_Lock=NULL;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CMFCServerDlg ��ȭ ����




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


// CMFCServerDlg �޽��� ó����

BOOL CMFCServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	
	ClientCloseflag = false;
	isConnected = false;
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(435);
	//�⺻�� �ʱ�ȭ
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

	//Listen ���� �ʱ�ȭ
	m_pServerSocket = new CServerSocket;
	m_pServerSocket->SetWnd(this->m_hWnd);

	m_pServerSocket->batch_size = batch_size;

	//���� Listen�ϱ�
	m_pServerSocket->Create(PORT);
	m_pServerSocket->Listen();
	

	//edit ���� ���� �ʱ�ȭ
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


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMFCServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CMFCServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
} 

afx_msg LRESULT CMFCServerDlg::OnAcceptSocket(WPARAM wParam, LPARAM lParam)
{

	isConnected = true;
	//���� ��ũ������ ���� �� ����
	ClientCloseflag = true;

	if(p_savethread == NULL){
		p_savethread = AfxBeginThread(SaveFileThread, this);
	}
	else{
		p_savethread->ResumeThread();
	}
	
	if(p_savethread == NULL){
		AfxMessageBox(_T("���� Work������ Error"));
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

//�̰� ���Ǵ�
afx_msg LRESULT CMFCServerDlg::OnClientMsgRecv(WPARAM wParam, LPARAM lParam)
{	
	CClientSocket *Client = (CClientSocket *)wParam;

	//LPCTSTR : Long Pointer Constant T_STRing
	LPCTSTR lpszStr = (LPCTSTR)lParam;
	//USES_CONVERSION;
	CString recv_msg = lpszStr;	

	//���⼭ ���� �޽����� ':' ������ �����Ͽ� ���
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

		//client seq�̸�
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
			//seq ������
			if(seq != _ttoi(recv_msgs[k+2])){
				e.Format(_T("Client%d SEQ ERROR seq=%d, recv_msgs=%d"), _ttoi(recv_msgs[k-1]),seq, _ttoi(recv_msgs[k+2]) );
				EnterCriticalSection(g_Lock);
					m_error_list.InsertString(-1,e);
					m_error_list.SetCurSel(m_error_list.GetCount()-1);
				LeaveCriticalSection(g_Lock);
				//seq����
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
			//ack �˻� �ڵ�
			pClient->setpostack(pClient->postack+1);
			int ack;
			CString e;
			ack=pClient->getpostack();
			//ack ������
			if(ack != _ttoi(recv_msgs[k+2])){
				e.Format(_T("Client%d ACK ERROR ack=%d, recv_msgs=%d"), _ttoi(recv_msgs[k-1]),ack, _ttoi(recv_msgs[k+2]) );
				EnterCriticalSection(g_Lock);
					m_error_list.InsertString(-1,e);
					m_error_list.SetCurSel(m_error_list.GetCount()-1);
				LeaveCriticalSection(g_Lock);
				//ack����
				pClient->setpostack(_ttoi(recv_msgs[k+2]));
			}
		}
	}
	
	return 0;
}


//���� �� �߻��ϴ� �޽���
//ClientSocket cpp �� Onclose���� �޽��� ��
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
				//q�� �����ϴ� ������ ����
				q.pop();
			}			
		}	
	}
	isConnected = false;
		
	return 0;
}

void CMFCServerDlg::OnBnClickedSuspendBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	

	CString get_editext = _T("");
	CString info=_T("");
	
	//batch����
	m_batch_edit->GetWindowTextW(get_editext);	
	
	if(m_ptrClientSocketList.GetCount()!= 0)
	{
		info += _T("* epoch�� Ŭ���̾�Ʈ ���� �߿� ������ �� �����ϴ�.\n\n");
		m_batch_edit->SetWindowTextW(_T(""));	
	}
	else{
		if(get_editext != _T("") ){
			if(_ttoi(get_editext)<0){
				info += _T("* epoch�� ������ �� �� �����ϴ�.\n\n");
				m_batch_edit->SetWindowTextW(_T(""));	
			}
			
			else{
				batch_size = _ttoi(get_editext);
				if(batch_size == 0){
					info += _T("o epoch ������ �����Ǿ����ϴ�.\n");
				}
				m_batchV_edit->SetWindowTextW(get_editext);
				m_pServerSocket->batch_size = batch_size;
				info += _T("o epoch�� ����Ǿ����ϴ�.\n\n");
				m_batch_edit->SetWindowTextW(_T(""));
			}
		}
	}
	
	
	//��� �ֱ� ����
	m_period_edit->GetWindowTextW(get_editext);
	
	if(get_editext != _T("")){
		if(_ttoi(get_editext) < 10){
			print_period = 10;
			info += _T("* Lock ������ ���� �ּ� ���尪 10���� �����˴ϴ�.\n\n");
			m_periodV_edit->SetWindowTextW(_T("10"));
			m_period_edit->SetWindowTextW(_T(""));
		}
		else{
			print_period = _ttoi(get_editext);
			m_periodV_edit->SetWindowTextW(get_editext);
			info += _T("o ��� �ֱⰡ ����Ǿ����ϴ�.\n\n");
			m_period_edit->SetWindowTextW(_T(""));
		}
	}

	
	AfxMessageBox(info);
	
		

}

UINT CMFCServerDlg::SaveFileThread(LPVOID pParam){

	CMFCServerDlg* pDlg = (CMFCServerDlg*) AfxGetApp()->m_pMainWnd;
	CMFCServerDlg* fir = (CMFCServerDlg*) pParam;
	while(1){		

		//������ ���� �ֱ�
		Sleep(pDlg->thread_period);
		//out
		if(pDlg->EndSaveThread){			
			break;
		}
		//�������
		EnterCriticalSection(g_Lock);
			int qsize = pDlg->q.size();
		LeaveCriticalSection(g_Lock);
		
		if(qsize < pDlg->lines){
			continue;
		}
		
		//���� ���� ��ü
		CStdioFile file;
		CFileException e;
		CFileFind finder;
		
		//����� ���� ��ġ
		CString folderpath = pDlg->dir+_T("\\");
		
		//���� ����
		CreateDirectory(folderpath,NULL);
		
		//���� ���� �ؽ�Ʈ ���� ���� ī��Ʈ
		int count=0;	
		BOOL bworking = finder.FindFile(folderpath + _T("*.txt"));
		while(bworking){
			bworking = finder.FindNextFile();
			if(finder.IsDots())
				continue;
			count++;
		}
		finder.Close();
		
		//�����̸� ���� ���� ��ȣ
		CString num;
		num.Format(_T("%d"),count);
		//���� ��� �� ���� �̸�,����
		CString filepath = folderpath + pDlg->filename+ num +_T(".txt");
		
		//���� ���¿���
		if(!file.Open(filepath, CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate, &e)){
			e.ReportError();			
		}

		//���� ���� ����
		else{
			//ũ��Ƽ�� ���� ����
			
				//lines ��ŭ ���� ����
				for(int i =0;i<pDlg->lines;i++){					
					CString data;	
					EnterCriticalSection(g_Lock);
						data = pDlg->q.front();
						pDlg->q.pop();
					LeaveCriticalSection(g_Lock);
					file.WriteString(data + _T("\n"));
				}
			
			//ũ��Ƽ�� ���� ��
		}
		//����ð�
		CString currentDate;
		CTime time = CTime::GetCurrentTime();
		currentDate.Format(_T("%d-%d-%d %d:%d:%d"),
             time.GetYear(), time.GetMonth(), time.GetDay(),
             time.GetHour(), time.GetMinute(), time.GetSecond());
		//���Ͽ� �Ķ���� ���� �� ��¥ ���
		CString info;
		info.Format(_T("\nParmeter Setting Information : lines=%d , thread_period=%d (ms)\ndate=%s"),pDlg->lines,pDlg->thread_period,currentDate);
		file.WriteString(info);
		//���� �ݱ�
		file.Close();
	}
	return 0;

}

//���� ������ ���� ���� ��ư
void CMFCServerDlg::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if(m_ptrClientSocketList.GetCount() != 0 ){
		AfxMessageBox(_T("Ŭ���̾�Ʈ�� �����߿��� ������ �� �����ϴ�."));
		return;
	}
	else{
		CString info=_T("");
		CString get_editext;
		//���� �̸� ����
		
		m_name_edit->GetWindowTextW(get_editext);
		if(get_editext != _T("")){
			m_nameV_edit->SetWindowTextW(get_editext);
			filename = get_editext;
			info+=_T("o ���� �̸��� ����Ǿ����ϴ�.\n\n");
			m_name_edit->SetWindowTextW(_T(""));
		}

		//���� ���� ����
		m_line_edit->GetWindowTextW(get_editext);
		if(get_editext != _T("")){
			if(_ttoi(get_editext) < 2000 || _ttoi(get_editext) > 100000){
				info += _T("* ���� ������ ������ 2000 ~ 100000 �Դϴ�.\n** ���� ������ ũ�� ������Ͽ� ���� �ۼ��� ������ �߻��� �� �ֽ��ϴ�.\n\n");
				m_line_edit->SetWindowTextW(_T(""));
			}
			else{
				m_lineV_edit->SetWindowTextW(get_editext);
				lines = _ttoi(get_editext);
				info += _T("o ���� ������ ����Ǿ����ϴ�.\n* 50000���� ����\n** ���� ������ ũ�� ���Ͽ� ���� Error Log�� �߻��� �� �ֽ��ϴ�.\n\n");
				m_line_edit->SetWindowTextW(_T(""));
			}		
		}
		//������ �����ֱ� ����
		m_sleep_edit->GetWindowTextW(get_editext);
		if(get_editext != _T("")){
			if(_ttoi(get_editext) < 1000){
				info += _T("* ���� ������ ���� �ֱ��� �ּҰ��� 1000(ms)�Դϴ�.\n\n");
				m_line_edit->SetWindowTextW(_T(""));
			}
			else{
				m_sleepV_edit->SetWindowTextW(get_editext);
				thread_period = _ttoi(get_editext);
				info += _T("o ���� ������ ���� �ֱⰡ ����Ǿ����ϴ�.\n\n");
				m_sleep_edit->SetWindowTextW(_T(""));
			}	
		}
		if(info != _T("")){
			AfxMessageBox(info);
		}
	}
	
	
}


//���� ���̾�α�
void CMFCServerDlg::OnBnClickedFiledlg()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(m_ptrClientSocketList.GetCount() != 0 ){
		AfxMessageBox(_T("Ŭ���̾�Ʈ�� �����߿��� ������ �� �����ϴ�."));
		return;
	}
	BROWSEINFO BrInfo;
	TCHAR szBuffer[1024];                                      // ������� ���� 

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 1024);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = _T("������ ����� ������ �����ϼ���");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);               // ���ϰ�� �о����

    // ��θ� ������ ����� ���, Edit Control �� �� ����
	CString str;
	str.Format(_T("%s"), szBuffer);
	m_dirV_edit->SetWindowTextW(str);
	
	dir = str;
}
