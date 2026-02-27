//*******************************************************************
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//*******************************************************************
#include "ColumnHandler.h"
#include "EditRecord.h"
//*******************************************************************
#include "HotlistTraceDlg.h"
//*******************************************************************
static const char* szNOTFOUND = "no record";
static const char* szLOSTLINK = "lost link";
//*******************************************************************

CHotlistTraceDlg::CHotlistTraceDlg(CSQLRowAccountFull& RowAccount, CWnd* pParent /*=NULL*/)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CHotlistTraceDlg::IDD, pParent)
	,m_RowAccount(RowAccount)
{
	// default to current highlighted record
	m_strRequestedCardNo = m_RowAccount.GetUserIDString();			
}

//*******************************************************************

void CHotlistTraceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TRACE, m_list);
}
//*******************************************************************

BEGIN_MESSAGE_MAP(CHotlistTraceDlg, CSSAutoShutdownDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TRACE, OnNMDblclkListTrace)
END_MESSAGE_MAP()

//*******************************************************************
static const int nColumnCount = 4;
//*******************************************************************

BOOL CHotlistTraceDlg::CSSAutoShutdownPostInitDialog()
{
	CColumnHandler column;
	if ( column.Read (  "TRACE", nColumnCount ) == FALSE )
	{
		column.SetWidth ( 0, 50 );						// default values
		column.SetWidth ( 1, 65 );
		column.SetWidth ( 2, 80 );
		column.SetWidth ( 3, 40 );
	}

	m_list.SubclassDlgItem ( IDC_LIST1, this );
	m_list.InsertColumn ( 0, "", LVCFMT_LEFT, column.GetWidth(0) );
	m_list.InsertColumn ( 1, "UserID", LVCFMT_LEFT, column.GetWidth(1) );
	m_list.InsertColumn ( 2, "Hotlisted", LVCFMT_LEFT, column.GetWidth(2) );
	m_list.InsertColumn ( 3, "", LVCFMT_LEFT, column.GetWidth(3) );

	__int64 nCurrentUserID = m_RowAccount.GetUserID();

	int nLine = 0;
	while ( m_RowAccount.HavePreviousUserID() == TRUE )
	{
		__int64 nPreviousUserID = m_RowAccount.GetPreviousUserID();		// get previous card no

		m_RowAccount.SetUserID(nPreviousUserID);

		CSQLRepositoryAccount RepoAccount;
		if (RepoAccount.SelectRow(m_RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			m_list.AddString ( CreateDisplayLine ( ++nLine, m_RowAccount.GetUserIDString(), szNOTFOUND ) );	// first in list record does not exist
			m_RowAccount.SetUserID(nCurrentUserID);
			break;
		}

		if ( m_RowAccount.HaveNextUserID() == FALSE )
		{
			m_list.AddString ( CreateDisplayLine ( ++nLine, m_RowAccount.GetUserIDString(), szLOSTLINK ) );	// lost link to next record
			break;
		}

		nCurrentUserID = m_RowAccount.GetUserID();
	}

	m_list.AddString ( CreateDisplayLine ( ++nLine, m_RowAccount.GetUserIDString(), m_RowAccount.GetAlertDate(), m_RowAccount.GetAlertTime() ) ); 

	if (m_strRequestedCardNo == m_RowAccount.GetUserIDString())			// see if its the highlighted record
	{
		m_list.SetCurSel(nLine - 1);
	}

	while ( m_RowAccount.HaveNextUserID() == TRUE )
	{
		__int64 nCurrUserID = m_RowAccount.GetUserID();
		__int64 nNextUserID = m_RowAccount.GetNextUserID();

		m_RowAccount.SetUserID(nNextUserID);

		CSQLRepositoryAccount RepoAccount;
		if ( RepoAccount.SelectRow(m_RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
		{
			m_list.AddString ( CreateDisplayLine ( ++nLine, m_RowAccount.GetUserIDString(), szNOTFOUND ) );
			m_RowAccount.SetUserID(nCurrUserID);
			break;
		}

		if (m_RowAccount.IsAlertSet() == TRUE)
		{
			m_list.AddString(CreateDisplayLine(++nLine, m_RowAccount.GetUserIDString(), m_RowAccount.GetAlertDate(), m_RowAccount.GetAlertTime()));
		}
		else
		{
			m_list.AddString(CreateDisplayLine(++nLine, m_RowAccount.GetUserIDString()));
		}

		if (m_strRequestedCardNo == m_RowAccount.GetUserIDString())
		{
			m_list.SetCurSel(nLine - 1);
		}
	}

	return TRUE;
}

//*******************************************************************

CString CHotlistTraceDlg::CreateDisplayLine ( int nLine, const char* szCardNo, const char* szDate, const char* szTime )
{
	CString strLine = "";
	strLine.Format ( "%d\t%s\t%s\t%s", nLine, System.FormatCardNo(szCardNo), szDate,  szTime );
	return strLine;
}

//*******************************************************************

void CHotlistTraceDlg::OnNMDblclkListTrace(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nIndex = 0;
	if ((nIndex = m_list.GetCurSel()) >= 0)
	{
		CString strCardNo = m_list.GetItemText(nIndex, 1);
		if (strCardNo == System.GetBlankCardNo())
		{
			return;
		}

		CEditRecord record("", this);
		record.Edit(_atoi64(strCardNo));
	}

	*pResult = 0;
}

//*******************************************************************

void CHotlistTraceDlg::OnOK()
{
	if ( UpdateData() == TRUE )
	{
		int nIndex;
		if ((nIndex = m_list.GetCurSel()) >= 0)
		{
			m_strRequestedCardNo = m_list.GetItemText(nIndex, 1);
		}

		CColumnHandler column;
		column.Save ( "TRACE", 0, nColumnCount, m_list );
		EndDialog(IDOK);
	}
}

//*******************************************************************

