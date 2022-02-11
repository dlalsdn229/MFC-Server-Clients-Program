
// MFCServerDlg.h : ��� ����
//

#pragma once
#include "ServerSocket.h"
#include <queue>
using namespace std;

// CMFCServerDlg ��ȭ ����
class CMFCServerDlg : public CDialog
{
// �����Դϴ�.
public:
	CMFCServerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MFCSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CClientSocket *m_pClientSocket;
	CServerSocket *m_pServerSocket;
	CWinThread *p_savethread;
	HANDLE savethread_handle;
public:
	int print_period;
	int batch_size;
	int lines;
	int thread_period;
	CString dir;
	CString filename;
	bool EndSaveThread;
	bool ClientCloseflag;
	bool isConnected;

	CEdit *m_batch_edit;
	CEdit *m_period_edit;
	CEdit *m_batchV_edit;
	CEdit *m_periodV_edit;
	CEdit *m_line_edit;
	CEdit *m_lineV_edit;
	CEdit *m_dirV_edit;
	CEdit *m_name_edit;
	CEdit *m_nameV_edit;
	CEdit *m_sleep_edit;
	CEdit *m_sleepV_edit;


	CListBox m_error_list;
	CListBox m_client_list;     //IDC_LIST_CLIENT	
	CListBox m_msg_list;      //IDC_LIST_MSG
	CListBox m_msg_list2;
	CPtrList m_ptrClientSocketList; //For manage Client Sockets
	
	void Wait(DWORD dwMillisecond);
	static UINT SaveFileThread(LPVOID pParam);
	queue<CString> q;
protected:
	afx_msg LRESULT OnAcceptSocket(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientMsgRecv(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientClose(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedSuspendBtn();
	afx_msg void OnBnClickedResumeBtn();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedFiledlg();
};
