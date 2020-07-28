
// RPSDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "RPS.h"
#include "RPSDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"

#define WM_SETTEXT (WM_USER+1)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CRPSDlg 对话框


IMPLEMENT_DYNAMIC(CRPSDlg, CDialogEx);

CRPSDlg::CRPSDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RPS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = nullptr;
}

CRPSDlg::~CRPSDlg()
{
	// 如果该对话框有自动化代理，则
	//  此对话框的返回指针为 null，所以它知道
	//  此代理知道该对话框已被删除。
	if (m_pAutoProxy != nullptr)
		m_pAutoProxy->m_pDialog = nullptr;
}

void CRPSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRPSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_BN_CLICKED(IDC_BUTTON1, &CRPSDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CRPSDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CRPSDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CRPSDlg 消息处理程序

int cntplayer = 0, cntcomputer = 0;
int ch;
struct text {
	int nID;
	CString str;
};

BOOL CRPSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	srand((unsigned)time(NULL));

	text str_init;
	str_init.nID = IDC_EDIT2;
	str_init.str = L"电脑已准备就绪！\r\n请输入你的选择\r\n";
	SendMessage(WM_SETTEXT, (WPARAM)&str_init, 0);

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRPSDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRPSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRPSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 当用户关闭 UI 时，如果控制器仍保持着它的某个
//  对象，则自动化服务器不应退出。  这些
//  消息处理程序确保如下情形: 如果代理仍在使用，
//  则将隐藏 UI；但是在关闭对话框时，
//  对话框仍然会保留在那里。

void CRPSDlg::OnClose()
{
	if (CanExit())
		CDialogEx::OnClose();
}

void CRPSDlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void CRPSDlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

BOOL CRPSDlg::CanExit()
{
	// 如果代理对象仍保留在那里，则自动化
	//  控制器仍会保持此应用程序。
	//  使对话框保留在那里，但将其 UI 隐藏起来。
	if (m_pAutoProxy != nullptr)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}



DWORD WINAPI setMain(void* param) {
	CRPSDlg* p_Dlg = (CRPSDlg*)param;
	text str;
	str.nID = IDC_EDIT2;
	switch (ch) {
	case 1: {
		str.str = L"你选择了石头\r\n";
		p_Dlg->SendMessage(WM_SETTEXT, (WPARAM)&str, 0);
		break;
	}
	case 2: {
		str.str = L"你选择了剪刀\r\n";
		p_Dlg->SendMessage(WM_SETTEXT, (WPARAM)&str, 0);
		break;
	}
	case 3: {
		str.str = L"你选择了布\r\n";
		p_Dlg->SendMessage(WM_SETTEXT, (WPARAM)&str, 0);
		break;
	}
	}
	Sleep(200);
	str.str = L"电脑沉思中...\r\n";
	p_Dlg->SendMessage(WM_SETTEXT, (WPARAM)&str, 0);
	srand((unsigned)time(NULL));
	int ch_computer = int(rand()) % 3;
	if (ch_computer == 0)ch_computer = 3;
	Sleep(800);
	switch (ch_computer) {
	case 1: {
		str.str = L"电脑选择了石头\r\n";
		p_Dlg->SendMessage(WM_SETTEXT, (WPARAM)&str, 0);
		break;
	}
	case 2: {
		str.str = L"电脑选择了剪刀\r\n";
		p_Dlg->SendMessage(WM_SETTEXT, (WPARAM)&str, 0);
		break;
	}
	case 3: {
		str.str = L"电脑选择了布\r\n";
		p_Dlg->SendMessage(WM_SETTEXT, (WPARAM)&str, 0);
		break;
	}
	}
	Sleep(500);
	if (ch == ch_computer) {
		str.str = L"平局\r\n";
		p_Dlg->SendMessage(WM_SETTEXT, (WPARAM)&str, 0);
	}
	else if (ch_computer - ch == 1 || ch_computer - ch == -2) {
		str.str = L"你赢了！\r\n";
		p_Dlg->SendMessage(WM_SETTEXT, (WPARAM)&str, 0);
		cntplayer++;
		CString score;
		score.Format(L"你的得分：%d", cntplayer);
		p_Dlg->SetDlgItemTextW(IDC_STATIC2, score);
	}
	else {
		str.str = L"你输了！\r\n";
		p_Dlg->SendMessage(WM_SETTEXT, (WPARAM)&str, 0);
		cntcomputer++;
		CString score;
		score.Format(L"电脑得分：%d", cntcomputer);
		p_Dlg->SetDlgItemTextW(IDC_STATIC3, score);
	}
	p_Dlg->GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	p_Dlg->GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	p_Dlg->GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	return 0;
}

LRESULT CRPSDlg::OnSetText(WPARAM wParam, LPARAM lParam)
{
	text* param = (text*)wParam;
	CEdit* pEdit = (CEdit*)GetDlgItem(param->nID);
	int nLength = pEdit->GetWindowTextLength();

	//选定当前文本的末端
	pEdit->SetSel(nLength, nLength);
	//追加文本
	pEdit->ReplaceSel(param->str);
	return LRESULT();
}

void CRPSDlg::OnBnClickedButton1()//石头
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	ch = 1;
	CreateThread(NULL, 0, setMain, this, 0, 0);
}


void CRPSDlg::OnBnClickedButton2()//剪刀
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	ch = 2;
	CreateThread(NULL, 0, setMain, this, 0, 0);
}


void CRPSDlg::OnBnClickedButton3()//布
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	ch = 3;
	CreateThread(NULL, 0, setMain, this, 0, 0);
}


void CAboutDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(0, NULL, _T("https://www.luogu.com.cn/team/27887"), NULL, NULL, SW_NORMAL);
}
