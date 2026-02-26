#pragma once
/**********************************************************************/
#include "..\SPOSLoyalty4Manager\resource.h"
/**********************************************************************/

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
    void SetCaption1(const char* szMessage);
    void SetCaption2(const char* szMessage);
    bool SetCaption2Message(const char* szMessage, bool bCheckCancel = TRUE);
    bool SetCaption2RecordsChecked(int nRecords, bool bCheckCancel = TRUE);
    bool SetCaption2PercentDone(int nRecords, bool bCheckCancel = TRUE);
    bool SetCaption2RecordsOfTotal(__int64 nDone, __int64 nTotal, bool bCheckCancel = TRUE);
    bool SetCaption2RecordsOfUpToTotal(__int64 nDone, __int64 nTotal, bool bCheckCancel = TRUE);
    bool IncrementRecordsChecked(int nMod, bool bCheckCancel = TRUE);

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
    int m_nLastPercent;
};

//******************************************************************

