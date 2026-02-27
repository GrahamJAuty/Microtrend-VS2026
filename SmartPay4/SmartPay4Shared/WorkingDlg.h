#pragma once
/**********************************************************************/

//******************************************************************
#ifndef SYSTEMTYPE_MANAGER
//*****************************************************************

class CWorkingDlg
{
public:
	CWorkingDlg(const char*szCaption, bool bShowCancelButton = TRUE){}

public:
	BOOL Create(CWnd *pParent=NULL){ return TRUE; }
	void DestroyWindow(){}

public:
    CString GetCaption1() { return ""; }
    void SetCaption1(const char* szMessage, bool bClearCaption2 = TRUE) {}
    void SetCaption2(const char* szMessage) {}
    bool SetCaption2Message(const char* szMessage, bool bCheckCancel = TRUE) { return FALSE; }
    bool SetCaption2RecordsChecked(int nRecords, bool bCheckCancel = TRUE) { return FALSE; }
    bool SetCaption2PercentDone(int nRecords, bool bCheckCancel = TRUE) { return FALSE; }
    bool SetCaption2RecordsOfTotal(__int64 nDone, __int64 nTotal, bool bCheckCancel = TRUE) { return FALSE; }
    bool SetCaption2RecordsOfUpToTotal(__int64 nDone, __int64 nTotal, bool bCheckCancel = TRUE) { return FALSE; }
    bool IncrementRecordsChecked(int nMod, bool bCheckCancel) { return FALSE; }

public:
	bool CheckCancelButton(){ return FALSE; }
	void SetStatus( const char*sz ){}
};

//******************************************************************
#else 
//******************************************************************

#include "..\SmartPay4Manager\resource.h"

class CWorkingDlg : public CDialog
{
public:
    CWorkingDlg(const char* szCaption, bool bShowCancelButton = TRUE);   // standard constructor
    ~CWorkingDlg();

public:
    virtual BOOL DestroyWindow();
    BOOL Create(CWnd* pParent = NULL);
    bool CheckCancelButton();

public:
    CString GetCaption1() { return m_strCaptionText; }
    void SetCaption1(const char* szMessage, bool bClearCaption2 = TRUE);
    void SetCaption2(const char* szMessage);
    bool SetCaption2Message(const char* szMessage, bool bCheckCancel = TRUE);
    bool SetCaption2RecordsChecked(int nRecords, bool bCheckCancel = TRUE);
    bool SetCaption2PercentDone(int nRecords, bool bCheckCancel = TRUE);
    bool SetCaption2RecordsOfTotal(__int64 nDone, __int64 nTotal, bool bCheckCancel = TRUE);
    bool SetCaption2RecordsOfUpToTotal(__int64 nDone, __int64 nTotal, bool bCheckCancel = TRUE);
    bool IncrementRecordsChecked(int nMod, bool bCheckCancel);

    // Dialog Data
    enum { IDD = IDD_WORKING };
    CButton	m_buttonCancel;
    CStatic m_staticCaption1;
    CStatic m_staticCaption2;

    // Overrides
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnCancel();
    virtual void OnOK() {};
    virtual BOOL OnInitDialog();

protected:
    bool m_bCancel;
    bool m_bParentDisabled;

    void ReEnableParent();
    void PumpMessages();

    DECLARE_MESSAGE_MAP()

private:
    CString m_strCaptionText;
    bool m_bShowCancelButton;
    int m_nRecordsChecked;
};

//******************************************************************
#endif 
//******************************************************************
