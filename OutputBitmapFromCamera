
// TestCameraDlgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestCameraDlg.h"
#include "TestCameraDlgDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW


#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

IVideoWindow  * pVW = NULL;
IMediaControl * pMC = NULL;
IGraphBuilder * pGraph = NULL;
ICaptureGraphBuilder2 * pCapture = NULL;
IBaseFilter *pGrabberFilter = NULL;
ISampleGrabber *pGrabber = NULL;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestCameraDlgDlg 对话框




CTestCameraDlgDlg::CTestCameraDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestCameraDlgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestCameraDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREVIEW, m_PreviewPanel);
	DDX_Control(pDX, IDC_CAPTURE, m_CapturePanel);
}

BEGIN_MESSAGE_MAP(CTestCameraDlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTestCameraDlgDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTestCameraDlgDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestCameraDlgDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTestCameraDlgDlg 消息处理程序

BOOL CTestCameraDlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	HRESULT hr;
	//创建GraphBuilder和CaptureGraphBuilder Filter
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void **) & pGraph);
	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2 , NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void **) & pCapture);
	hr = pCapture->SetFiltergraph(pGraph);
	// 获得pGraph的IMediaControl,和IVideoWindow用户界面
	hr = pGraph->QueryInterface(IID_IMediaControl, (LPVOID *) & pMC);
	hr = pGraph->QueryInterface(IID_IVideoWindow, (LPVOID *) & pVW);

	

	IBaseFilter* pSrcFilter = NULL;
	IMoniker* pMoniker = NULL;
	ICreateDevEnum* pDevEnum = NULL;
	IEnumMoniker* pClassEnum = NULL;

	//创建系统设备枚举器,获得摄像头
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC,IID_ICreateDevEnum, (void **) & pDevEnum);
	hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pClassEnum, 0);
	for (unsigned int i = 0; i < 1; i++)
	{
		hr = pClassEnum->Next(1, &pMoniker, NULL);
	}
	hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void **) & pSrcFilter);
	hr = pGraph->AddFilter(pSrcFilter, L"Video Capture");

	pMoniker->Release();
	pDevEnum->Release();
	pClassEnum->Release();

	pSrcFilter->Release();
	//创建sample grabber
	hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,IID_IBaseFilter, (void**) & pGrabberFilter);
	hr = pGraph->AddFilter(pGrabberFilter, L"Sample Grabber");
	pGrabberFilter->QueryInterface(IID_ISampleGrabber, (void**)&pGrabber);

	//设置sampler grabber的媒体类型
	AM_MEDIA_TYPE mt;
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	mt.majortype = MEDIATYPE_Video;
	mt.subtype = MEDIASUBTYPE_RGB24;
	hr = pGrabber->SetMediaType(&mt);

	//设置sampler grabber的工作方式
	hr = pGrabber->SetOneShot(FALSE);
	hr = pGrabber->SetBufferSamples(TRUE);

	//通过自动连接串联Filter
	hr = pCapture->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video,pSrcFilter, pGrabberFilter, NULL);

	pSrcFilter->Release();

	//设置预览所有者和样式
	hr = pVW->put_Owner((OAHWND)m_PreviewPanel.m_hWnd);
	hr = pVW->put_WindowStyle(WS_CHILD | WS_CLIPCHILDREN);
	// Make the preview video fill our window
	RECT rc;
	m_PreviewPanel.GetWindowRect(&rc);
	pVW->SetWindowPosition(0, 0, rc.right, rc.bottom);
	// Make the video window visible, now that it is properly positioned
	hr = pVW->put_Visible(OATRUE);
	pMC->Run();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestCameraDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestCameraDlgDlg::OnPaint()
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
HCURSOR CTestCameraDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestCameraDlgDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void CTestCameraDlgDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CTestCameraDlgDlg::OnBnClickedButton1()
{
	HRESULT hr;
	//获得媒体类型
	AM_MEDIA_TYPE mt;	
	hr = pGrabber->GetConnectedMediaType(&mt);
	VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER *)mt.pbFormat;

	//从媒体类型中创建bitmapinfo结构
	BITMAPINFO BitmapInfo;  
	ZeroMemory(&BitmapInfo, sizeof(BitmapInfo));  
	CopyMemory(&BitmapInfo.bmiHeader, &(pVih->bmiHeader),sizeof(BITMAPINFOHEADER));  

	// 创建一个hbitmap对象,拷贝当前buffer到hBitmap对象的缓冲区
	void   *buffer   =   NULL;   
	HBITMAP hBitmap = CreateDIBSection(0, &BitmapInfo, DIB_RGB_COLORS, &buffer, NULL, 0);   
	long size = 0;  
	hr = pGrabber->GetCurrentBuffer(&size,NULL);  
	hr = pGrabber->GetCurrentBuffer(&size,(long *)buffer);
	//获得显示设备上下文,拷贝hbitmap到显示设备上下文
	CRect rect;
	m_CapturePanel.GetWindowRect(&rect);

	
	//HDC   hdcDest   =   ::GetDC(NULL);   
	//HDC   hdcSrc   =   CreateCompatibleDC(NULL);   
	HDC   hdcDest   =   ::GetDC(NULL);   
	HDC   hdcSrc   =   CreateCompatibleDC(NULL);   
	SelectObject(hdcSrc,   hBitmap);   
	
	SetStretchBltMode(hdcDest,COLORONCOLOR);
	StretchBlt(hdcDest,   rect.left,   rect.top,   rect.Width(),   rect.Height(),   hdcSrc,   0,   0,pVih->bmiHeader.biWidth,pVih->bmiHeader.biHeight ,  SRCCOPY); 
	//::Sleep(200);

	//////////////////////////////beg
	BITMAP bmp;
	HDC dcc;
	dcc = ::GetDC(0);
	HBITMAP hBmp;

	GetObject(hBitmap, sizeof(BITMAP), (LPBYTE)&bmp);
	int w = bmp.bmWidth;
	int h = bmp.bmHeight;
	int raw; 
	//for(int i = 0; i < w * h  - 10;i  ++)
	//{
	//	::SetPixel(dcc, i%w * 0.1, i/w * 0.1, *(COLORREF*)((BYTE*)buffer + i * 3));
	//}
	
	////////////////////////////////////end
	/////////////////////////////////////////////beg
	char cc[100];
	
	sprintf(cc, "%d, %d, %d   --",size, w, h);

	::TextOut(dcc, 10, 260, cc, 30);
	::ReleaseDC(m_hWnd, dcc);
	/////////////////////////////////////////////end

	//释放mt
	if (mt.cbFormat != 0)
	{
		CoTaskMemFree((PVOID)mt.pbFormat);
		mt.cbFormat = 0;
		mt.pbFormat = NULL;
	}
	if (mt.pUnk != NULL)
	{
		// Unecessary because pUnk should not be used, but safest.
		mt.pUnk->Release();
		mt.pUnk = NULL;
	}


}
