
// MFCApplication2Dlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication2.h"
#include "MFCApplication2Dlg.h"
#include "afxdialogex.h"
#include <string>
#include <Windows.h>
#include <fstream>
#include <iomanip>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

// Register windows messages that will permit to communicate with RDS driver
RDS_ADD_PRIVATE_MESSAGES

// RDS_ON_* macros define dialog box event handlers. Be sure to indicate in parameters the correct class names.
RDS_ON_COPY_DATA(CMFCApplication2Dlg, CDialog);
RDS_ON_SERVER_AVAILABLE(CMFCApplication2Dlg, RDS_REMOTE_MOUSE_OFF);
RDS_ON_REMOVE_FROM_SERVER(CMFCApplication2Dlg)


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define DEF_EXT  _T("*.csv")
#define FILTER   _T("CSV(*.csv)|*.csv")

using std::fixed;
using std::setprecision;

unsigned long iter = 0;
bool      run_flag = FALSE;
const int size_x = 10000000;
const int size_y = 11;
long double Memo[size_x][size_y];

// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
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


// CMFCApplication2Dlg ダイアログ



CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION2_DIALOG, pParent)
	, m_strXPosition_(_T(""))
	, m_strYPosition(_T(""))
	, m_strZPosition(_T(""))
	, m_strA_angle(_T(""))
	, m_strB_angle(_T(""))
	, m_strC_angle(_T(""))
	, m_strD_angle(_T(""))
	, m_strE_angle(_T(""))
	, m_strF_angle(_T(""))
	, m_strG_angle(_T(""))
	, m_bButton2State(FALSE)
{	
	// Initialize RDS variables
	// Use RDS_INITIALIZE_EXT is you've got an authentication key provided by Romer
	RDS_INITIALIZE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strXPosition_);
	DDX_Text(pDX, IDC_EDIT2, m_strYPosition);
	DDX_Text(pDX, IDC_EDIT3, m_strZPosition);
	DDX_Text(pDX, IDC_EDIT4, m_strA_angle);
	DDX_Text(pDX, IDC_EDIT5, m_strB_angle);
	DDX_Text(pDX, IDC_EDIT6, m_strC_angle);
	DDX_Text(pDX, IDC_EDIT7, m_strD_angle);
	DDX_Text(pDX, IDC_EDIT8, m_strE_angle);
	DDX_Text(pDX, IDC_EDIT9, m_strF_angle);
	DDX_Text(pDX, IDC_EDIT10, m_strG_angle);
	DDX_Check(pDX, IDC_CHECK1, m_bButton2State);
	//DDX_Control(pDX, IDC_BUTTON1, OnBnClickedButton1);
}

BEGIN_MESSAGE_MAP(CMFCApplication2Dlg, CDialogEx)
	// Declare RDS messages handlers
	RDS_MSGS
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication2Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication2Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication2Dlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CMFCApplication2Dlg メッセージ ハンドラー

BOOL CMFCApplication2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
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

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CMFCApplication2Dlg::OnDestroy()
{
	RDS_REMOVE_FROM_SERVER
	CDialog::OnDestroy();
}

void CMFCApplication2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CMFCApplication2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


// Button1 イベントハンドラ
void CMFCApplication2Dlg::OnBnClickedButton1()
{
	CFileDialog dlg(FALSE, DEF_EXT, NULL, 0, FILTER);
	if (dlg.DoModal() == IDOK)
	{	
		// output csv
		CString pathName = dlg.GetPathName();

		// chk file exist
		int retval = PathFileExists(pathName);
		if (retval == 1) {
			if (MessageBox(_T("上書きしますか?"), _T("Sure?"), MB_YESNO) == IDNO) {
				return;
			}
		}

		// ファイル出力用のオブジェクトを作成
		std::ofstream data_file(pathName);
		for (int i = 0; i < iter; i++) {
			for (int j = 0; j < size_y; j++) {
				data_file << fixed << setprecision(15) << Memo[i][j] << ",";
			}
			data_file << "\n";
		}
		data_file << std::endl;
		data_file.close();
	}
	else
	{
	}
}

// Button2 イベントハンドラ
void CMFCApplication2Dlg::OnBnClickedButton2()
{
	for (int i = 0; i < iter; i++) {
		for (int j = 0; j < size_y; j++) {
			Memo[i][j] = 0;
		}
	}
	iter = 0;
}

// Button3 イベントハンドラ
void CMFCApplication2Dlg::OnBnClickedButton3()
{
	run_flag = TRUE;
}


// Button4 イベントハンドラ
void CMFCApplication2Dlg::OnBnClickedButton4()
{
	run_flag = FALSE;
}


//////////////////////////////////////////////////////////////////////////////////////
//
//  Write RDS Events
//
//////////////////////////////////////////////////////////////////////////////////////

// RDS_VALID_DATA is automatically called when program receives a "valid" RDS event
void CMFCApplication2Dlg::RDS_SYNCED_DATA(sRDSPosition& p)
{	
	m_strXPosition_.Format(_T("%12.4f"), p.ProbeXYZ[RDS_X]);
	m_strYPosition.Format(_T("%12.4f"),p.ProbeXYZ[RDS_Y]);
	m_strZPosition.Format(_T("%12.4f"), p.ProbeXYZ[RDS_Z]);

	m_strA_angle.Format(_T("%12.6f"), p.Angles[0]);
	m_strB_angle.Format(_T("%12.6f"), p.Angles[1]);
	m_strC_angle.Format(_T("%12.6f"), p.Angles[2]);
	m_strD_angle.Format(_T("%12.6f"), p.Angles[3]);
	m_strE_angle.Format(_T("%12.6f"), p.Angles[4]);
	m_strF_angle.Format(_T("%12.6f"), p.Angles[5]);
	m_strG_angle.Format(_T("%12.6f"), p.Angles[6]);

	m_bButton2State = p.ButtonsStates[1];

	// data update
	
	// data update
	if (run_flag) {
		Memo[iter][0] = p.ButtonsStates[1];
		Memo[iter][1] = p.ProbeXYZ[RDS_X];
		Memo[iter][2] = p.ProbeXYZ[RDS_Y];
		Memo[iter][3] = p.ProbeXYZ[RDS_Z];
		Memo[iter][4] = p.Angles[0];
		Memo[iter][5] = p.Angles[1];
		Memo[iter][6] = p.Angles[2];
		Memo[iter][7] = p.Angles[3];
		Memo[iter][8] = p.Angles[4];
		Memo[iter][9] = p.Angles[5];
		Memo[iter][10] = p.Angles[6];
		iter++;
	}

	UpdateData(FALSE);
	Sleep(10);
}


// RDS_INVALID_DATA is automatically called when program receives an "invalid" RDS event
void CMFCApplication2Dlg::RDS_UNSYNCED_DATA(sRDSPosition& p)
{
	m_strXPosition_.Format(_T("%12.4f"), p.ProbeXYZ[RDS_X]);
	m_strYPosition.Format(_T("%12.4f"), p.ProbeXYZ[RDS_Y]);
	m_strZPosition.Format(_T("%12.4f"), p.ProbeXYZ[RDS_Z]);

	m_strA_angle.Format(_T("%12.6f"), p.Angles[0]);
	m_strB_angle.Format(_T("%12.6f"), p.Angles[1]);
	m_strC_angle.Format(_T("%12.6f"), p.Angles[2]);
	m_strD_angle.Format(_T("%12.6f"), p.Angles[3]);
	m_strE_angle.Format(_T("%12.6f"), p.Angles[4]);
	m_strF_angle.Format(_T("%12.6f"), p.Angles[5]);
	m_strG_angle.Format(_T("%12.6f"), p.Angles[6]);

	m_bButton2State = p.ButtonsStates[1];

	// data update
	if (run_flag) {
		Memo[iter][0] = p.ButtonsStates[1];
		Memo[iter][1] = p.ProbeXYZ[RDS_X];
		Memo[iter][2] = p.ProbeXYZ[RDS_Y];
		Memo[iter][3] = p.ProbeXYZ[RDS_Z];
		Memo[iter][4] = p.Angles[0];
		Memo[iter][5] = p.Angles[1];
		Memo[iter][6] = p.Angles[2];
		Memo[iter][7] = p.Angles[3];
		Memo[iter][8] = p.Angles[4];
		Memo[iter][9] = p.Angles[5];
		Memo[iter][10] = p.Angles[6];
		iter++;
	}

	UpdateData(FALSE);
	Sleep(10);
}

// RDS_SERVER_UP is called when application is connected to service
void CMFCApplication2Dlg::RDS_SERVER_UP(void)
{
	MessageBox(L"Server is up !");
}

// RDS_SERVER_DOWN is called when application is disconnected from service
void CMFCApplication2Dlg::RDS_SERVER_DOWN(void)
{
	MessageBox(L"Server is down !");
}

// RDS_PROBE_DATA is automatically called when probe changes
void CMFCApplication2Dlg::RDS_PROBE_DATA(sRDSProbe& probe)
{
}

// RDS_ARM_DATA is called every 500ms and gives interesting information about connection with the arm
void CMFCApplication2Dlg::RDS_ARM_DATA(sRDSArm& arm)
{
}


// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CMFCApplication2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication2Dlg::OnEnChangestrxposition()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
}
