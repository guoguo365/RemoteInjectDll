
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


// 選擇文件
void CRemoteInjectDllDlg::OnBnClickedSelectBtn()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE);
	if (IDCANCEL == dlg.DoModal()) {
		return;
	}
	m_strDllPath = dlg.GetPathName();

	// 当你建立了控件和变量之间的联系后：
	// 当你修改了变量的值，而希望对话框控件更新显示，就应该在修改变量后调用UpdateData(FALSE)；
	UpdateData(FALSE);
}

// 注入
void CRemoteInjectDllDlg::OnBnClickedInjectBtn()
{
	// TODO: Add your control notification handler code here
	// 1.判斷路徑有沒有選擇文件，變量是否爲空
	// 如果你希望知道用户在对话框中到底输入了什么，就应该在访问变量前调用UpdateData(TRUE)
	UpdateData(TRUE);
	if (m_strDllPath.IsEmpty()) {
		MessageBox("請選擇dll文件", "提示", MB_OK);
		return;
	}
	MessageBox(m_strDllPath, "提示", MB_OK);

	// 2.提升進程權限，打開進程訪問令牌
	CString strMsg;
	HANDLE hToken;
	if (FALSE == OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken)) {
		strMsg.Format("打開進程令牌失敗，錯誤碼：%d", GetLastError());
		MessageBox(strMsg, "提示", MB_OK);
		return;
	}

	// 查看進程特權信息
	LUID luid;
	if (FALSE == LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {
		strMsg.Format("查詢系統特權屬性失敗，錯誤碼：%d", GetLastError());
		MessageBox(strMsg, "提示", MB_OK);
		return;
	}

	// 調節訪問令牌確權屬性
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (FALSE == AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) {
		strMsg.Format("調節訪問令牌特權失敗，錯誤碼：%d", GetLastError());
		MessageBox(strMsg, "提示", MB_OK);
		return;
	}

	// 3.查找窗口，根據窗口句柄獲取進程pid
	HWND hCalc = ::FindWindow(NULL, "计算器");
	if (hCalc == NULL) {
		strMsg.Format("沒有找到該類型窗口，錯誤碼：%d", GetLastError());
		MessageBox(strMsg, "提示", MB_OK);
		return;
	}

	// 獲取進程的pid
	DWORD dwPID = 0;
	GetWindowThreadProcessId(hCalc, &dwPID);
	if (dwPID == 0) {
		strMsg.Format("获取窗口PID失败.错误码：%d", GetLastError());
		MessageBox(strMsg, "提示", MB_OK);
		return;
	}
	strMsg.Format("PID：%d", dwPID);
	MessageBox(strMsg, "提示", MB_OK);

	// 4. 根據pid獲取進程句柄
	HANDLE hCalcProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (hCalcProcess == NULL) {
		strMsg.Format(TEXT("获取进程句柄失败.错误码：%d"), GetLastError());
		MessageBox(strMsg, TEXT("提示"), MB_OK);
		return;
	}

	// 5. 在遠程進程中申請内存空間
	DWORD dwSize = m_strDllPath.GetLength() + 1;
	LPVOID lpAddr = VirtualAllocEx(hCalcProcess, NULL, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (lpAddr == NULL)
	{
		strMsg.Format(TEXT("在远程进程下申请内存失败.错误码：%d"), GetLastError());
		MessageBox(strMsg, TEXT("提示"), MB_OK);
		return;
	}

	// 6.將dll路徑寫入遠程進程
	if (FALSE == WriteProcessMemory(hCalcProcess, lpAddr, m_strDllPath, dwSize, NULL)) {
		strMsg.Format("在遠程進程中寫入失敗，錯誤碼：%d", GetLastError());
		MessageBox(strMsg, "提示", MB_OK);
		return;
	}

	// 在自己的進程中創建一個綫程
	PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandle("Kernel32.dll"), "LoadLibraryA");

	// 7.在遠程進程中開闢一個綫程
	HANDLE hThreadHandle = CreateRemoteThread(hCalcProcess, NULL, 0, pfnStartAddr, lpAddr, 0, NULL);
	if (hThreadHandle == NULL)
	{
		strMsg.Format(TEXT("在远程进程中创建线程失败.错误码：%d"), GetLastError());
		MessageBox(strMsg, TEXT("提示"), MB_OK);
		return;
	}
	MessageBox(TEXT("在远程进程中创建线程成功"), TEXT("提示"), MB_OK);
}
