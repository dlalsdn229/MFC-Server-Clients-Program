
// MFCClientDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MFCClient.h"
#include "MFCClientDlg.h"


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


// CMFCClientDlg ��ȭ ����




CMFCClientDlg::CMFCClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	*cThread = NULL;
	*sendThread = NULL;
}

void CMFCClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_msg_list2);
	DDX_Control(pDX, IDC_LIST1, m_msg_list);
	DDX_Control(pDX, IDC_CLIENT_LIST, m_client_list);
	DDX_Control(pDX, IDC_ERROR_LIST, m_error_list);
}

BEGIN_MESSAGE_MAP(CMFCClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
ON_BN_CLICKED(IDC_CREATE_BTN, &CMFCClientDlg::OnBnClickedCreateBtn)
ON_MESSAGE(WM_CLIENT_RECV, &CMFCClientDlg::OnClientRecv)
ON_MESSAGE(WM_CLIENT_CLOSE, &CMFCClientDlg::OnClientClose)
ON_BN_CLICKED(IDC_SUSPEND_BTN, &CMFCClientDlg::OnBnClickedSuspendBtn)
ON_BN_CLICKED(IDC_RESTART_BTN, &CMFCClientDlg::OnBnClickedRestartBtn)
ON_WM_CLOSE()

ON_BN_CLICKED(IDC_BUTTON_apply1, &CMFCClientDlg::OnBnClickedButtonapply1)
ON_BN_CLICKED(IDC_BUTTON2, &CMFCClientDlg::OnBnClickedButton2)
ON_BN_CLICKED(IDC_FileDlg, &CMFCClientDlg::OnBnClickedFiledlg)
END_MESSAGE_MAP()


// CMFCClientDlg �޽��� ó����

BOOL CMFCClientDlg::OnInitDialog()
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
	
	//�⺻�� �ʱ�ȭ
	batch_size = 0;
	print_period = 200;
	lines=10000;
	thread_period=2000;
	dir = _T("C:\\Users\\22009601\\Desktop\\savetxt\\Client");
	filename = _T("Client");
	EndSaveThread=false;

	if(g_Lock == NULL) {
		g_Lock = (LPCRITICAL_SECTION) malloc(sizeof(CRITICAL_SECTION));
		InitializeCriticalSection(g_Lock);
	}

	//edit ���� ���� �ʱ�ȭ
	m_batch_edit = (CEdit *) GetDlgItem(IDC_EDIT_batch);
	m_batchV_edit = (CEdit *) GetDlgItem(IDC_EDIT_batchV);
	m_period_edit = (CEdit *) GetDlgItem(IDC_EDIT_period);
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


	//���� ��ũ������ ���� �� ����
	p_savethread = NULL;
	p_savethread = AfxBeginThread(SaveFileThread, this);
	if(p_savethread == NULL){
		AfxMessageBox(_T("���� Work������ Error"));
	}

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CMFCClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCClientDlg::OnPaint()
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
HCURSOR CMFCClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


afx_msg LRESULT CMFCClientDlg::OnClientRecv(WPARAM wParam, LPARAM lParam)
{
	
	LPCTSTR lpszStr = (LPCTSTR)lParam;

	CString recv_msg = lpszStr;
	
	CString recv_msgs[100]={_T("")};
	CString resToken;
	int curPos = 0;
	int i=0;
	resToken = recv_msg.Tokenize(_T(":"),curPos);
	//[0]:client_id / [1]:br_flag / [2]TextField / [3] seq,ack_num / [4] : SCSA_num
	while(resToken != ""){
		recv_msgs[i] = resToken;
		resToken = recv_msg.Tokenize(_T(":"),curPos);
		i++;
	}	

	CString str;
	
	CClientSocket *Client = (CClientSocket *)wParam;
	POSITION pos = m_ptrServerSocketList.Find(Client);
	CClientSocket *pServer = (CClientSocket *)m_ptrServerSocketList.GetAt(pos);
	
	int k =1;
	int j =0;	
	
	while(recv_msgs[k]!=_T("")){
		k = 5*j + 1;
		j++;
		if(recv_msgs[k+3] == _T("2000")){
			
			str = recv_msgs[k-1] +_T(":") +recv_msgs[k]+ _T(":This is Client ACK:") + recv_msgs[k+2] +  _T(":1001:");
			if(pServer != NULL){
				EnterCriticalSection(g_Lock);
					pServer->Send(str, lstrlen(str)*2);
				LeaveCriticalSection(g_Lock);
			}

			CString a = recv_msgs[k-1] +_T(":") + recv_msgs[k]+_T(":") + recv_msgs[k+1]+_T(":")+ recv_msgs[k+2]+_T(":") + recv_msgs[k+3]+_T(":");
			
			EnterCriticalSection(g_Lock);
				q.push(a);
			LeaveCriticalSection(g_Lock);

			if(_ttoi(recv_msgs[k+2]) % print_period==0){
				EnterCriticalSection(g_Lock);
					m_msg_list.InsertString(-1,a);
					m_msg_list.SetCurSel(m_msg_list.GetCount()-1);
				LeaveCriticalSection(g_Lock);
			}

			//seq �˻� �ڵ�			
			pServer->setpostseq(pServer->postseq + 1);
			int seq;
			CString e;
			seq=pServer->getpostseq();
			//seq ������
			if(seq != _ttoi(recv_msgs[k+2])){
				int recv = _ttoi(recv_msgs[k+2]);
				e.Format(_T("Client%d SEQ ERROR seq=%d, recv_msgs=%d"),_ttoi(recv_msgs[k-1]),seq, recv);
				

				EnterCriticalSection(g_Lock);
					err_q.push(e);
					m_error_list.InsertString(-1,e);
					m_error_list.SetCurSel(m_error_list.GetCount()-1);
				LeaveCriticalSection(g_Lock);
				//seq����
				pServer->setpostseq(_ttoi(recv_msgs[k+2]));
			}
		}

		else if(recv_msgs[k+3]==_T("2001")){

			CString a = recv_msgs[k-1] +_T(":") + recv_msgs[k]+_T(":") + recv_msgs[k+1]+_T(":")+ recv_msgs[k+2]+_T(":") + recv_msgs[k+3]+_T(":");
			
			EnterCriticalSection(g_Lock);
				q.push(a);
			LeaveCriticalSection(g_Lock);
			
			if(_ttoi(recv_msgs[k+2]) % print_period==0){
				EnterCriticalSection(g_Lock);
					m_msg_list2.InsertString(-1,a);
					m_msg_list2.SetCurSel(m_msg_list2.GetCount()-1);
				LeaveCriticalSection(g_Lock);
			}

			//ack �˻� �ڵ�
			pServer->setpostack(pServer->postack + 1);
			int ack;
			CString e;
			ack=pServer->getpostack();
			//ack ������
			if(ack != _ttoi(recv_msgs[k+2])){
				e.Format(_T("Client%d : ACK ERROR ack=%d, recv_msgs=%d"),_ttoi(recv_msgs[k-1]),ack, _ttoi(recv_msgs[k+2]) );
				EnterCriticalSection(g_Lock);
					err_q.push(e);
					m_error_list.InsertString(-1,e);
					m_error_list.SetCurSel(m_error_list.GetCount()-1);
				LeaveCriticalSection(g_Lock);

				//ack����
				pServer->setpostack(_ttoi(recv_msgs[k+2]));
			}

		}		
		//2000, 2001 ���޾��� ��츦 ó������ �ʾƵ� ���� �˻����� ���� ���� ���� ��.
		
	}
	
	return 0;
}

//Create Client Button
void CMFCClientDlg::OnBnClickedCreateBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//ù��° Ŭ���̾�Ʈ�� 0
	int client_num = m_client_list.GetCount();
	
	//Ŭ���̾�Ʈ ���� ������ ����
	//CClientSocketThread
	cThread[client_num] = (CClientSocketThread *)AfxBeginThread(RUNTIME_CLASS(CClientSocketThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	
	if(!cThread[client_num])
	{
		AfxMessageBox(_T("ERROR : Thread could not be created."));
		return;
	}
	

	cThread[client_num]->SetWnd(m_hWnd);
	//Dlg�� m_clientsocket �����ʹ� cThread������ �ּҿ��� clientsocketThread�� ���ǵ� clientsocket ��ü�� ����Ų��.
	m_ClientSocket = &cThread[client_num]->m_clientsocket;
	//manage for serversocket
	m_ptrServerSocketList.AddTail(&cThread[client_num]->m_clientsocket);

	//��� ���ϰ� �Բ� ��������� ���� �������̴�.
	sendThread[client_num] = (CSendThread *)AfxBeginThread(RUNTIME_CLASS(CSendThread), THREAD_PRIORITY_NORMAL, 0 , CREATE_SUSPENDED);
	if(!sendThread[client_num])
	{
		AfxMessageBox(_T("ERROR : sendThread could not be created."));
		return;
	}
	sendThread[client_num]->SetWnd(m_hWnd);
	sendThread[client_num]->Setclient_id(client_num);
	//sendTHread���� send�� �ϱ����� Ŭ���̾�Ʈ ������ �ʿ��ϴ�
	//�� ������ ������ ������� clientsocketthread ���� clientsocket ��ü�̴�.
	//���� sendThread�� m_ptrclientsocket �����ʹ� clientsocketthread�� m_clientsocket ��ü�� ����Ų��.
	sendThread[client_num]->m_ptrclientsocket = &cThread[client_num]->m_clientsocket;
	
	m_ptrSendThreadList.AddTail(sendThread[client_num]);
	

	
	CString str;
	str.Format(_T("Client %d"),client_num+1);
	
	m_client_list.InsertString(client_num,str);

	sendThread[client_num]->batch = batch_size;
	sendThread[client_num]->batch_tmp = batch_size;

	//�� ������ ��� ������ ������� �����Ѵ�.
	cThread[client_num]->ResumeThread();
	sendThread[client_num]->ResumeThread();

}

void CMFCClientDlg::OnBnClickedSuspendBtn()
{
	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	POSITION pos = m_ptrSendThreadList.GetHeadPosition();
	
	while(pos !=NULL){
		CSendThread *p = (CSendThread *)m_ptrSendThreadList.GetNext(pos);
		if(*p !=NULL){
			p->SuspendThread();
			
		}		
	}
	
}

void CMFCClientDlg::OnBnClickedRestartBtn()
{
	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	POSITION pos = m_ptrSendThreadList.GetHeadPosition();

	while(pos !=NULL){
		CSendThread *p = (CSendThread *)m_ptrSendThreadList.GetNext(pos);
		if(*p !=NULL){
			p->ResumeThread();
		}	
	}
	
}

afx_msg LRESULT CMFCClientDlg::OnClientClose(WPARAM wParam, LPARAM lParam){
	
	
	p_savethread->m_bAutoDelete=false;
	savethread_handle = p_savethread->m_hThread;
	EndSaveThread = true;

	return 0;
}


void CMFCClientDlg::OnClose()
{ 
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	POSITION pos = m_ptrSendThreadList.GetHeadPosition();
	while(pos !=NULL){
		CSendThread *p = (CSendThread *)m_ptrSendThreadList.GetNext(pos);
		if(*p !=NULL){
			p->exit_flag=true;
			OutputDebugString(_T("exit_flag=true\n"));
			p->m_ptrclientsocket->OnClose(0);			
		}		
	}
	
	for(int i =0;i<m_ptrServerSocketList.GetCount();i++){
		
			OutputDebugString(_T("cThread \n"));
			cThread[i]->PostThreadMessage(WM_QUIT,0,0);		
	}
	
	delete(p_savethread);
	Wait(1000);
	free(g_Lock);

	CDialog::OnClose();
}

void CMFCClientDlg::Wait(DWORD dwMillisecond)

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

UINT CMFCClientDlg::SaveFileThread(LPVOID pParam){

	CMFCClientDlg* pDlg = (CMFCClientDlg*) AfxGetApp()->m_pMainWnd;
	CMFCClientDlg* fir = (CMFCClientDlg*) pParam;

	int c=0;
	while(1){		

		//������ ���� �ֱ�
		Sleep(pDlg->thread_period);
		//out
		if(pDlg->EndSaveThread){
			
			break;
		}
		//�������
		if(pDlg->q.size() < pDlg->lines){
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
		num.Format(_T("%d"),c);
		//���� ��� �� ���� �̸�,����
		CString filepath = folderpath + pDlg->filename+_T("_")+ num +_T(".txt");
		c++;
		//���� ���¿���
		if(!file.Open(filepath, CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate, &e)){
			e.ReportError();			
		}

		//���� ���� ����
		else{			
			//lines ��ŭ ���� ����
			for(int i =0;i<pDlg->lines;i++){					
				CString data;	
				//ũ��Ƽ�� ���� ����
				EnterCriticalSection(g_Lock);
					data = pDlg->q.front();
					pDlg->q.pop();
				LeaveCriticalSection(g_Lock);
				//ũ��Ƽ�� ���� ��
				file.WriteString(data + _T("\n"));
			}		

			//����ð�
			CString currentDate;
			CTime time = CTime::GetCurrentTime();
			currentDate.Format(_T("%d-%d-%d %d:%d:%d"),
				 time.GetYear(), time.GetMonth(), time.GetDay(),
				 time.GetHour(), time.GetMinute(), time.GetSecond());
			//���Ͽ� �Ķ���� ���� �� ��¥ ���
			CString info;
			info.Format(_T("\nParameter Information : lines=%d , thread_period=%d (ms)\ndate=%s"),pDlg->lines,pDlg->thread_period,currentDate);
			file.WriteString(info);
			//���� �ݱ�
			file.Close();
		}
		

		//���� �α� ����
		CStdioFile err_file;
		CFileException ex;
		CreateDirectory(folderpath + _T("\\ErrorLog"),NULL);
		CString filepath2 = folderpath + _T("ErrorLog\\")+pDlg->filename +_T("_ErrorLog.txt");
		if(!pDlg->err_q.empty())
		{
			if(!err_file.Open(filepath2, CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate, &ex))
			{
				ex.ReportError();
			}
			else
			{	err_file.SeekToEnd();
				EnterCriticalSection(g_Lock);
					int errqsize = pDlg->err_q.size();
				LeaveCriticalSection(g_Lock);
				for(int i =0;i<errqsize;i++)
				{					
					CString data;	
					//ũ��Ƽ�� ���� ����
					EnterCriticalSection(g_Lock);
						data = pDlg->err_q.front();
						pDlg->err_q.pop();
					LeaveCriticalSection(g_Lock);
					//ũ��Ƽ�� ���� ��
					
					err_file.WriteString(data + _T("\n"));
				}
				err_file.Close();
			}
		}
	}

	return 0;
}
void CMFCClientDlg::OnBnClickedButtonapply1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString get_editext = _T("");
	CString info=_T("");
	
	//batch����
	m_batch_edit->GetWindowTextW(get_editext);	
	
	if(m_ptrServerSocketList.GetCount()!= 0)
	{
		info += _T("* epoch�� ������ ���� �߿� ������ �� �����ϴ�.\n\n");
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
				
				POSITION pos = m_ptrSendThreadList.GetHeadPosition();

				while(pos !=NULL){
					CSendThread *p = (CSendThread *)m_ptrSendThreadList.GetNext(pos);
					if(*p !=NULL){
						p->batch = batch_size;
					}	
				}

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


//����
void CMFCClientDlg::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if(m_ptrServerSocketList.GetCount() != 0 ){
		AfxMessageBox(_T("������ �����߿��� ������ �� �����ϴ�."));
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

void CMFCClientDlg::OnBnClickedFiledlg()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(m_ptrServerSocketList.GetCount() != 0 ){
		AfxMessageBox(_T("������ �����߿��� ������ �� �����ϴ�."));
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
