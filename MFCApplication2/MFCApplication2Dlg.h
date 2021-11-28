
// MFCApplication2Dlg.h : ヘッダー ファイル
//

#pragma once
#include <atlstr.h>
#include "RDSAPI_2/RDS_API2.h"		// RDS API include



// CMFCApplication2Dlg ダイアログ
class CMFCApplication2Dlg : public CDialogEx
{
// コンストラクション
public:
	CMFCApplication2Dlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

	CString m_strXPosition;
	CString m_strYPosition;
	CString m_strZPosition;

	CString m_strA_angle;
	CString m_strB_angle;
	CString m_strC_angle;
	CString m_strD_angle;
	CString m_strE_angle;
	CString m_strF_angle;
	CString m_strG_angle;

	CString m_strXPosition_tool;
	CString m_strYPosition_tool;
	CString m_strZPosition_tool;

	CString	m_strEvent;
	CString	m_strArmStatus;
	CString m_strProbe;

	BOOL		m_bButton1State;
	BOOL		m_bButton2State;
	BOOL		m_bButton3State;
	BOOL		m_bServerAvailable;
	BOOL		m_bEventSyncd;
	
	//afx_msg void OnTimer(UINT nIDEvent);

// 実装
protected:
	HICON m_hIcon;

	RDS_ADD_VARIABLES
	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangestrxposition();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
