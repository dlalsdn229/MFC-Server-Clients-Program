
// MFCClientDlg.h : ��� ����
//

#pragma once

#include "ClientSocket.h"
#include "ClientSocketThread.h"
#include "SendThread.h"
#include <queue>
using namespace std;

// CMFCClientDlg ��ȭ ����
class CMFCClientDlg : public CDialog
{
// �����Դϴ�.
public:
	CMFCClientDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MFCCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

	
// �����Դϴ�.
private:
	CClientSocket *m_ClientSocket;
	CWinThread *p_savethread;
	HANDLE savethread_handle;
	
protected:
	HICON m_hIcon;
	afx_msg LRESULT OnClientRecv(WPARAM wParam, LPARAM lParam);

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int print_period;
	int batch_size;
	int lines;
	int thread_period;
	CString dir;
	CString filename;
	bool EndSaveThread;

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


	CListBox m_msg_list;	//seq
	CListBox m_client_list;
	CListBox m_msg_list2;	//ack
	CListBox m_error_list;
	CPtrList m_ptrServerSocketList;
	CPtrList m_ptrSendThreadList;

	CClientSocketThread *cThread[20];
	CSendThread *sendThread[20];
	
	static UINT SaveFileThread(LPVOID pParam);
	void Wait(DWORD dwMillisecond);
	queue<CString> q;
	queue<CString> err_q;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCreateBtn();
	afx_msg LRESULT OnClientClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedSuspendBtn();
	afx_msg void OnBnClickedRestartBtn();
	afx_msg void OnClose();
	afx_msg void OnEnChangeEdit9();
	afx_msg void OnBnClickedButtonapply1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedFiledlg();
};
