
// RemoteInjectDllDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "RemoteInjectDll.h"
#include "RemoteInjectDllDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRemoteInjectDllDlg dialog



CRemoteInjectDllDlg::CRemoteInjectDllDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REMOTEINJECTDLL_DIALOG, pParent)
	, m_strDllPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRemoteInjectDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, ID_SELECT_BTN, m_select_btn);
	DDX_Text(pDX, IDC_EDITBOX, m_strDllPath);
}

BEGIN_MESSAGE_MAP(CRemoteInjectDllDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_SELECT_BTN, &CRemoteInjectDllDlg::OnBnClickedSelectBtn)
	ON_BN_CLICKED(ID_INJECT_BTN, &CRemoteInjectDllDlg::OnBnClickedInjectBtn)
END_MESSAGE_MAP()


// CRemoteInjectDllDlg message handlers

BOOL CRemoteInjectDllDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRemoteInjectDllDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRemoteInjectDllDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRemoteInjectDllDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// �x���ļ�
void CRemoteInjectDllDlg::OnBnClickedSelectBtn()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE);
	if (IDCANCEL == dlg.DoModal()) {
		return;
	}
	m_strDllPath = dlg.GetPathName();

	// ���㽨���˿ؼ��ͱ���֮�����ϵ��
	// �����޸��˱�����ֵ����ϣ���Ի���ؼ�������ʾ����Ӧ�����޸ı��������UpdateData(FALSE)��
	UpdateData(FALSE);
}

// ע��
void CRemoteInjectDllDlg::OnBnClickedInjectBtn()
{
	// TODO: Add your control notification handler code here
	// 1.�Д�·���Л]���x���ļ���׃���Ƿ񠑿�
	// �����ϣ��֪���û��ڶԻ����е���������ʲô����Ӧ���ڷ��ʱ���ǰ����UpdateData(TRUE)
	UpdateData(TRUE);
	if (m_strDllPath.IsEmpty()) {
		MessageBox("Ո�x��dll�ļ�", "��ʾ", MB_OK);
		return;
	}
	MessageBox(m_strDllPath, "��ʾ", MB_OK);

	// 2.�����M�̙��ޣ����_�M���L������
	CString strMsg;
	HANDLE hToken;
	if (FALSE == OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken)) {
		strMsg.Format("���_�M������ʧ�����e�`�a��%d", GetLastError());
		MessageBox(strMsg, "��ʾ", MB_OK);
		return;
	}

	// �鿴�M���ؙ���Ϣ
	LUID luid;
	if (FALSE == LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {
		strMsg.Format("��ԃϵ�y�ؙ�����ʧ�����e�`�a��%d", GetLastError());
		MessageBox(strMsg, "��ʾ", MB_OK);
		return;
	}

	// �{���L�����ƴ_������
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (FALSE == AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) {
		strMsg.Format("�{���L�������ؙ�ʧ�����e�`�a��%d", GetLastError());
		MessageBox(strMsg, "��ʾ", MB_OK);
		return;
	}

	// 3.���Ҵ��ڣ��������ھ���@ȡ�M��pid
	HWND hCalc = ::FindWindow(NULL, "������");
	if (hCalc == NULL) {
		strMsg.Format("�]���ҵ�ԓ��ʹ��ڣ��e�`�a��%d", GetLastError());
		MessageBox(strMsg, "��ʾ", MB_OK);
		return;
	}

	// �@ȡ�M�̵�pid
	DWORD dwPID = 0;
	GetWindowThreadProcessId(hCalc, &dwPID);
	if (dwPID == 0) {
		strMsg.Format("��ȡ����PIDʧ��.�����룺%d", GetLastError());
		MessageBox(strMsg, "��ʾ", MB_OK);
		return;
	}
	strMsg.Format("PID��%d", dwPID);
	MessageBox(strMsg, "��ʾ", MB_OK);

	// 4. ����pid�@ȡ�M�̾��
	HANDLE hCalcProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (hCalcProcess == NULL) {
		strMsg.Format(TEXT("��ȡ���̾��ʧ��.�����룺%d"), GetLastError());
		MessageBox(strMsg, TEXT("��ʾ"), MB_OK);
		return;
	}

	// 5. ���h���M������Ո�ڴ���g
	DWORD dwSize = m_strDllPath.GetLength() + 1;
	LPVOID lpAddr = VirtualAllocEx(hCalcProcess, NULL, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (lpAddr == NULL)
	{
		strMsg.Format(TEXT("��Զ�̽����������ڴ�ʧ��.�����룺%d"), GetLastError());
		MessageBox(strMsg, TEXT("��ʾ"), MB_OK);
		return;
	}

	// 6.��dll·�������h���M��
	if (FALSE == WriteProcessMemory(hCalcProcess, lpAddr, m_strDllPath, dwSize, NULL)) {
		strMsg.Format("���h���M���Ќ���ʧ�����e�`�a��%d", GetLastError());
		MessageBox(strMsg, "��ʾ", MB_OK);
		return;
	}

	// ���Լ����M���Є���һ���Q��
	PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandle("Kernel32.dll"), "LoadLibraryA");

	// 7.���h���M�����_�Vһ���Q��
	HANDLE hThreadHandle = CreateRemoteThread(hCalcProcess, NULL, 0, pfnStartAddr, lpAddr, 0, NULL);
	if (hThreadHandle == NULL)
	{
		strMsg.Format(TEXT("��Զ�̽����д����߳�ʧ��.�����룺%d"), GetLastError());
		MessageBox(strMsg, TEXT("��ʾ"), MB_OK);
		return;
	}
	MessageBox(TEXT("��Զ�̽����д����̳߳ɹ�"), TEXT("��ʾ"), MB_OK);
}
