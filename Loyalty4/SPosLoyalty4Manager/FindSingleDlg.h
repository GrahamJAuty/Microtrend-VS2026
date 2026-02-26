#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "MyMifare.h"
#include "TerminalData.h"
//$$******************************************************************

class CFindSingleDlg : public CSSDialog
{
public:
	CFindSingleDlg( const char* szCardNo = "", CWnd* pParent = NULL);
	virtual ~CFindSingleDlg() {};
	
	virtual void MakeAbstract() = 0;

	void ClearInput();
	CString GetOutputCardNo() { return m_strOutputCardNo; }
	CString GetOutputMiFareNo() { return m_strOutputMiFareNo; }

	enum { IDD = IDD_FIND_SINGLE };
	CStatic m_staticPrompt1;
	CStatic m_staticPrompt2;
	CEdit m_editMiFareNo;
	CButton m_buttonReader;
	CButton m_buttonBrowse;
	CButton m_buttonOK;
	CButton m_buttonCancel;
	CString m_strInputCardNo;
	CString m_strInputMiFareNo;
	int m_nRecordCount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnBnClickedButtonReader();

	DECLARE_MESSAGE_MAP()

protected:
	void SetReaderMode ( bool bMode );
	bool ValidateCardNumber( const char* szCardNo, const char* szMiFareNo );
	
protected:
	CEdit* GetEditCardNo();
	CEdit* GetEditCount();

protected:
	CString m_strOutputCardNo;
	CString m_strOutputMiFareNo;

	CMyMifare m_reader;
	CTerminalData m_terminal;
	bool m_bReaderMode;

protected:
	CString m_strTitle;
	bool m_bShowBrowse;
	bool m_bShowMiFare;
	bool m_bShowCount;
};

//$$******************************************************************

class CFindSingleBrowseDlg : public CFindSingleDlg
{
public:
	CFindSingleBrowseDlg( CWnd* pParent = NULL);
	void MakeAbstract(){};
};

//$$******************************************************************

class CFindSingleHotReplaceDlg : public CFindSingleDlg
{
public:
	CFindSingleHotReplaceDlg( const char* szCardNo, CWnd* pParent = NULL);
	void MakeAbstract(){};
};

//$$******************************************************************

class CFindSingleImportDlg : public CFindSingleDlg
{
public:
	CFindSingleImportDlg( CWnd* pParent = NULL);
	void MakeAbstract(){};
};

//$$******************************************************************

class CFindSingleTransferDlg : public CFindSingleDlg
{
public:
	CFindSingleTransferDlg( const char* szCardNo, CWnd* pParent = NULL);
	void MakeAbstract(){};
};

//$$******************************************************************

class CFindSingleAddOneDlg : public CFindSingleDlg
{
public:
	CFindSingleAddOneDlg( const char* szCardNo, CWnd* pParent = NULL);
	void MakeAbstract(){};
};

//$$******************************************************************

class CFindSingleAddBatchDlg : public CFindSingleDlg
{
public:
	CFindSingleAddBatchDlg( const char* szCardNo, CWnd* pParent = NULL);
	void MakeAbstract(){};
};

//$$******************************************************************
