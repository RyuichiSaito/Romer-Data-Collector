
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
#include <numeric>
#include <wchar.h>

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

int data_num = 0;

unsigned long iter = 0;
unsigned long cnt = 0;

bool      run_flag = FALSE;
const int size_x = 2000000;
const int size_xcur = 10000;
const int size_y = 11;
long double Memo[size_x][size_y];
long double Data[size_x][(size_y - 1)*2];
long double Cur[size_xcur][size_y];


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
	, m_strXPosition(_T(""))
	, m_strYPosition(_T(""))
	, m_strZPosition(_T(""))
	, m_strA_angle(_T(""))
	, m_strB_angle(_T(""))
	, m_strC_angle(_T(""))
	, m_strD_angle(_T(""))
	, m_strE_angle(_T(""))
	, m_strF_angle(_T(""))
	, m_strG_angle(_T(""))
	, m_strEvent(_T(""))
	, m_strArmStatus(_T(""))
	, m_strProbe(_T(""))
	, m_bButton2State(FALSE)
	, m_bServerAvailable(FALSE)
	, m_bEventSyncd(FALSE)
	, m_strXPosition_tool(_T("0"))
	, m_strYPosition_tool(_T("0"))
	, m_strZPosition_tool(_T("0"))
{	
	// Initialize RDS variables
	// Use RDS_INITIALIZE_EXT is you've got an authentication key provided by Romer
	RDS_INITIALIZE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strXPosition);
	DDX_Text(pDX, IDC_EDIT2, m_strYPosition);
	DDX_Text(pDX, IDC_EDIT3, m_strZPosition);
	DDX_Text(pDX, IDC_EDIT4, m_strA_angle);
	DDX_Text(pDX, IDC_EDIT5, m_strB_angle);
	DDX_Text(pDX, IDC_EDIT6, m_strC_angle);
	DDX_Text(pDX, IDC_EDIT7, m_strD_angle);
	DDX_Text(pDX, IDC_EDIT8, m_strE_angle);
	DDX_Text(pDX, IDC_EDIT9, m_strF_angle);
	DDX_Text(pDX, IDC_EDIT10, m_strG_angle);
	DDX_Text(pDX, IDC_EDIT11, m_strXPosition_tool);
	DDX_Text(pDX, IDC_EDIT12, m_strYPosition_tool);
	DDX_Text(pDX, IDC_EDIT13, m_strZPosition_tool);
	DDX_Check(pDX, IDC_CHECK1, m_bButton2State);
	DDX_Check(pDX, IDC_CHECK2, m_bServerAvailable);
	DDX_Check(pDX, IDC_CHECK3, m_bEventSyncd);

	DDX_Text(pDX, IDC_EDIT_Event, m_strEvent);
	DDX_Text(pDX, IDC_EDIT_ArmStatus, m_strArmStatus);
	DDX_Text(pDX, IDC_EDIT_Probe, m_strProbe);
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
		CStringA pathName_(pathName.GetBuffer(0));
		std::string fullpath = pathName_.GetBuffer(0);

		int ext_i = fullpath.find_last_of(".");
		std::string filename = fullpath.substr(0, ext_i);
		filename += "_describe.csv";


		// chk file exist
		int retval = PathFileExists(pathName);
		if (retval == 1) {
			if (MessageBox(_T("上書きしますか?"), _T("Sure?"), MB_YESNO) == IDNO) {
				return;
			}
		}

		// ファイル出力用のオブジェクトを作成
		std::ofstream data_file(pathName);
		data_file << "Button Status, X,Y,Z,A1,A2,A3,A4,A5,A6,A7,X tool,Y tool,Z tool";
		for (int i = 0; i < iter; i++) {
			for (int j = 0; j < size_y; j++) {
				data_file << fixed << setprecision(15) << Memo[i][j] << ",";
				data_file << m_strXPosition_tool << "," << m_strYPosition_tool << "," << m_strZPosition_tool << ",";
			}
			data_file << "\n";
		}
		data_file << std::endl;
		data_file.close();


		// ファイル出力用のオブジェクトを作成
		std::ofstream data_file_2(filename);
		data_file_2 << "X mean,Y mean,Z mean,A1 mean,A2 mean,A3 mean,A4 mean,A5 mean,A6 mean,A7 mean,";
		data_file_2 << "X std,Y std,Z std,A1 std,A2 std,A3 std,A4 std,A5 std,A6 std,A7 std,X tool,Y tool,Z tool";
		data_file_2 << "\n";

		for (int i = 0; i < data_num; i++) {
			for (int j = 0; j < (size_y-1)*2; j++) {
				data_file_2 << fixed << setprecision(15) << Data[i][j] << ",";
				data_file_2 << m_strXPosition_tool << "," << m_strYPosition_tool << "," << m_strZPosition_tool << ",";
			}
			data_file_2 << "\n";
		}
		data_file_2 << std::endl;
		data_file_2.close();
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

	// reset cur
	for (int i = 0; 0 < i < cnt + 1; i++) {
		for (int j = 0; j < 10; j++) {
			Cur[i][j] = 0;
		}
	}
	cnt = 0;
}

// Button3 イベントハンドラ 
void CMFCApplication2Dlg::OnBnClickedButton3()
{
	run_flag = TRUE;
}


// Button4 イベントハンドラ (Stop Button)
void CMFCApplication2Dlg::OnBnClickedButton4()
{
	run_flag = FALSE;


	// data describe
	

	for (int i = 0; i < iter-1; i++) {
		if (iter == 0) {
			break;
		}
		if (Memo[i][0] && Memo[i + 1][0]) {
			// data store
			for (int j = 0; j < 10; j++) {
				Cur[cnt][j] = Memo[i][j + 1];
			}
			cnt++;
			// データが途切れた時
		}
		else if (cnt == 0) {
			continue;
		}
		else {
			// 統計量の計算
			for (int j = 0; j < 10; j++) {
				double ave = 0.0, var = 0.0;
				for (int ii = 0; ii < cnt; ii++) {
					ave += Cur[ii][j];
					var += Cur[ii][j] * Cur[ii][j];
				}
				ave /= cnt;
				var = var / cnt - ave * ave;
				Data[data_num][j] = ave;
				Data[data_num][j + 10] = var;
			}
			data_num++;


			// reset cur
			for (int ii = 0; ii < cnt+1; ii++) {
				for (int j = 0; j < 10; j++) {
					Cur[ii][j] = 0;
				}
			}
			cnt = 0;
		}
			
	}

}


//////////////////////////////////////////////////////////////////////////////////////
//
//  Write RDS Events
//
//////////////////////////////////////////////////////////////////////////////////////

// RDS_VALID_DATA is automatically called when program receives a "valid" RDS event
void CMFCApplication2Dlg::RDS_SYNCED_DATA(sRDSPosition& p)
{	
	m_strXPosition.Format(_T("%12.4f"), p.ProbeXYZ[RDS_X]);
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

	m_bEventSyncd = TRUE;
	UpdateData(FALSE);
	Sleep(10);
}


// RDS_INVALID_DATA is automatically called when program receives an "invalid" RDS event
void CMFCApplication2Dlg::RDS_UNSYNCED_DATA(sRDSPosition& p)
{
	m_strXPosition.Format(_T("%12.4f"), p.ProbeXYZ[RDS_X]);
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

	m_bEventSyncd = FALSE;
	UpdateData(FALSE);
	Sleep(10);
}


// Refresh link status
void CMFCApplication2Dlg::OnTimer(UINT nIDEvent)
{
	m_bServerAvailable = m_bServerFound;
	UpdateData(FALSE);

	CDialog::OnTimer(nIDEvent);
}


// RDS_SERVER_UP is called when application is connected to service
void CMFCApplication2Dlg::RDS_SERVER_UP(void)
{
	MessageBox(L"Server is up !");
	//m_bServerAvailable = TRUE;
}

// RDS_SERVER_DOWN is called when application is disconnected from service
void CMFCApplication2Dlg::RDS_SERVER_DOWN(void)
{
	MessageBox(L"Server is down !");
	//m_bServerAvailable = FALSE;
}

// RDS_PROBE_DATA is automatically called when probe changes
void CMFCApplication2Dlg::RDS_PROBE_DATA(sRDSProbe& probe)
{
	m_strEvent = "RDS_PROBE_DATA event";

	m_strProbe.Format(_T("Index=%ld, Name=%ls, Type=%ls"),probe.Index, probe.Name, probe.TypeName);

	UpdateData(FALSE);
}

// RDS_ARM_DATA is called every 500ms and gives interesting information about connection with the arm
void CMFCApplication2Dlg::RDS_ARM_DATA(sRDSArm& arm)
{
	m_strEvent = "RDS_ARM_DATA event";

	m_strArmStatus = arm.ArmStatusText;

	UpdateData(FALSE);
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
