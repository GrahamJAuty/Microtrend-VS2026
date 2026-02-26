/**********************************************************************/
#include "resource.h"
/**********************************************************************/
#include "WorkingDlg.h"
/**********************************************************************/

CWorkingDlg::CWorkingDlg(const char* szCaption, bool bShowCancelButton)
{
    m_strCaptionText = szCaption;
    m_bCancel = FALSE;
    m_bParentDisabled = FALSE;
    m_bShowCancelButton = bShowCancelButton;
    m_nLastPercent = -1;
    m_nRecordsChecked = 0;
}

/**********************************************************************/

CWorkingDlg::~CWorkingDlg()
{
    if (m_hWnd != NULL)
    {
        DestroyWindow();
    }
}

/**********************************************************************/

BOOL CWorkingDlg::DestroyWindow()
{
    ReEnableParent();
    return CDialog::DestroyWindow();
}

/**********************************************************************/

void CWorkingDlg::ReEnableParent()
{
    if (m_bParentDisabled && (m_pParentWnd != NULL))
    {
        m_pParentWnd->EnableWindow(TRUE);
    }
    m_bParentDisabled=FALSE;
}

/**********************************************************************/

BOOL CWorkingDlg::Create(CWnd *pParent)
{
    // Get the true parent of the dialog
    m_pParentWnd = CWnd::GetSafeOwner(pParent);

    // m_bParentDisabled is used to re-enable the parent window
    // when the dialog is destroyed. So we don't want to set
    // it to TRUE unless the parent was already enabled.

    if ((m_pParentWnd != NULL) && m_pParentWnd->IsWindowEnabled())
    {
        m_pParentWnd->EnableWindow(FALSE);
        m_bParentDisabled = TRUE;
    }

    if (!CDialog::Create(CWorkingDlg::IDD, pParent))
    {
        ReEnableParent();
        return FALSE;
    }

    if (FALSE == m_bShowCancelButton)
    {
        m_buttonCancel.EnableWindow(FALSE);
        m_buttonCancel.ShowWindow(SW_HIDE);
    }

    return TRUE;
}

/**********************************************************************/

void CWorkingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDCANCEL, m_buttonCancel);
    DDX_Control(pDX, IDC_STATIC_CAPTION1, m_staticCaption1);
    DDX_Control(pDX, IDC_STATIC_CAPTION2, m_staticCaption2);
}

/**********************************************************************/
BEGIN_MESSAGE_MAP(CWorkingDlg, CDialog)
END_MESSAGE_MAP()
/**********************************************************************/

void CWorkingDlg::SetCaption1(const char* szMessage)
{
    PumpMessages();
    m_staticCaption1.SetWindowText(szMessage);
    m_staticCaption2.SetWindowText("");
}

/**********************************************************************/

void CWorkingDlg::SetCaption2(const char* szMessage)
{
    PumpMessages();
    m_staticCaption2.SetWindowText(szMessage);
    m_nLastPercent = -1;
}

/**********************************************************************/

bool CWorkingDlg::SetCaption2Message(const char* szMsg, bool bCheckCancel)
{
    PumpMessages();
    m_nLastPercent = -1;

    m_staticCaption2.SetWindowText(szMsg);

    if (TRUE == bCheckCancel)
    {
        return CheckCancelButton();
    }
    else
    {
        return FALSE;
    }
}

/**********************************************************************/

bool CWorkingDlg::IncrementRecordsChecked(int nMod, bool bCheckCancel)
{
    m_nRecordsChecked++;
    
    bool bResult = FALSE;
    bool bTryProgress = TRUE;

    if (nMod > 1)
    {
        if ((m_nRecordsChecked % nMod) != 0)
        {
            bTryProgress = FALSE;
        }
    }

    if (TRUE == bTryProgress)
    {
        bResult = SetCaption2RecordsChecked(m_nRecordsChecked, bCheckCancel);
        UpdateWindow();
    }

    return bResult;
}

/**********************************************************************/

bool CWorkingDlg::SetCaption2RecordsChecked(int nRecords, bool bCheckCancel)
{
    PumpMessages();
    m_nLastPercent = -1;

    CString strRecords = "";
    strRecords.Format("%d record", nRecords);

    if (nRecords != 1)
    {
        strRecords += "s";
    }

    strRecords += " checked";
    m_staticCaption2.SetWindowText(strRecords);

    if (TRUE == bCheckCancel)
    {
        return CheckCancelButton();
    }
    else
    {
        return FALSE;
    }
}

/**********************************************************************/

bool CWorkingDlg::SetCaption2RecordsOfTotal(__int64 nDone, __int64 nTotal, bool bCheckCancel)
{
    PumpMessages();
    m_nLastPercent = -1;

    CString strRecords = "";
    strRecords.Format("%I64d of %I64d done", nDone, nTotal);

    m_staticCaption2.SetWindowText(strRecords);

    if (TRUE == bCheckCancel)
    {
        return CheckCancelButton();
    }
    else
    {
        return FALSE;
    }
}

/**********************************************************************/

bool CWorkingDlg::SetCaption2PercentDone(int nPercent, bool bCheckCancel)
{
    PumpMessages();

    if (nPercent != m_nLastPercent)
    {
        CString strRecords = "";
        strRecords.Format("%d%% done", nPercent);
        m_staticCaption2.SetWindowText(strRecords);
        m_nLastPercent = nPercent;
    }

    if (TRUE == bCheckCancel)
    {
        return CheckCancelButton();
    }
    else
    {
        return FALSE;
    }
}

/**********************************************************************/

bool CWorkingDlg::SetCaption2RecordsOfUpToTotal(__int64 nDone, __int64 nTotal, bool bCheckCancel)
{
    PumpMessages();

    CString strRecords = "";
    strRecords.Format("%I64d of up to %I64d done", nDone, nTotal);

    m_staticCaption2.SetWindowText(strRecords);

    if (TRUE == bCheckCancel)
    {
        return CheckCancelButton();
    }
    else
    {
        return FALSE;
    }
}

/**********************************************************************/

void CWorkingDlg::OnCancel()
{
    m_bCancel = TRUE;
}

/**********************************************************************/

void CWorkingDlg::PumpMessages()
{
    ASSERT(m_hWnd != NULL);

    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (!IsDialogMessage(&msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

/**********************************************************************/

bool CWorkingDlg::CheckCancelButton()
{
    // Process all pending messages
    PumpMessages();

    // Reset m_bCancel to FALSE so that
    // CheckCancelButton returns FALSE until the user
    // clicks Cancel again. This will allow you to call
    // CheckCancelButton and still continue the operation.
    // If m_bCancel stayed TRUE, then the next call to
    // CheckCancelButton would always return TRUE

    bool bResult = m_bCancel;
    m_bCancel = FALSE;

    return bResult;
}

/**********************************************************************/

BOOL CWorkingDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_staticCaption1.SetWindowText(m_strCaptionText);
    m_staticCaption2.SetWindowText("");
    return TRUE;
}

/**********************************************************************/


