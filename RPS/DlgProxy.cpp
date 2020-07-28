
// DlgProxy.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "RPS.h"
#include "DlgProxy.h"
#include "RPSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRPSDlgAutoProxy

IMPLEMENT_DYNCREATE(CRPSDlgAutoProxy, CCmdTarget)

CRPSDlgAutoProxy::CRPSDlgAutoProxy()
{
	EnableAutomation();

	// 为使应用程序在自动化对象处于活动状态时一直保持
	//	运行，构造函数调用 AfxOleLockApp。
	AfxOleLockApp();

	// 通过应用程序的主窗口指针
	//  来访问对话框。  设置代理的内部指针
	//  指向对话框，并设置对话框的后向指针指向
	//  该代理。
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CRPSDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CRPSDlg)))
		{
			m_pDialog = reinterpret_cast<CRPSDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CRPSDlgAutoProxy::~CRPSDlgAutoProxy()
{
	// 为了在用 OLE 自动化创建所有对象后终止应用程序，
	//	析构函数调用 AfxOleUnlockApp。
	//  除了做其他事情外，这还将销毁主对话框
	if (m_pDialog != nullptr)
		m_pDialog->m_pAutoProxy = nullptr;
	AfxOleUnlockApp();
}

void CRPSDlgAutoProxy::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CRPSDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CRPSDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// 注意: 我们添加了对 IID_IRPS 的支持来支持类型安全绑定
//  以支持来自 VBA 的类型安全绑定。  此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {42a5e17a-3e00-4e1a-a0ef-eef54bbfe211}
static const IID IID_IRPS =
{0x42a5e17a,0x3e00,0x4e1a,{0xa0,0xef,0xee,0xf5,0x4b,0xbf,0xe2,0x11}};

BEGIN_INTERFACE_MAP(CRPSDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CRPSDlgAutoProxy, IID_IRPS, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 宏是在此项目的 pch.h 中定义的
// {85742e78-9a4e-4cbd-86ae-64efe816eb97}
IMPLEMENT_OLECREATE2(CRPSDlgAutoProxy, "RPS.Application", 0x85742e78,0x9a4e,0x4cbd,0x86,0xae,0x64,0xef,0xe8,0x16,0xeb,0x97)


// CRPSDlgAutoProxy 消息处理程序
