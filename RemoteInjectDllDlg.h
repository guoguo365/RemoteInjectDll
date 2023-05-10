
// RemoteInjectDllDlg.h : header file
//

#pragma once


// CRemoteInjectDllDlg dialog
class CRemoteInjectDllDlg : public CDialogEx
{
// Construction
public:
	CRemoteInjectDllDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REMOTEINJECTDLL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_select_btn;
	CString m_strDllPath;
	afx_msg void OnBnClickedSelectBtn();
	afx_msg void OnBnClickedInjectBtn();
};
