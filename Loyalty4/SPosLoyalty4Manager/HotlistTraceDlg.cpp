//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
//*******************************************************************
#include "LoyaltyManager.h"
#include "ColumnHandler.h"
#include "EditLoyaltyRecord.h"
#include "EditAccountPropertySheet.h"
//*******************************************************************
#include "HotlistTraceDlg.h"
//*******************************************************************
static const char* szNOTFOUND = "no record";
static const char* szLOSTLINK = "lost link";
//*******************************************************************

CHotlistTraceDlg::CHotlistTraceDlg( __int64 nUserID, CWnd* pParent /*=NULL*/)
	: CDialog(CHotlistTraceDlg::IDD, pParent)
{
	m_nRequestedUserID = nUserID;
}

//*******************************************************************

CHotlistTraceDlg::~CHotlistTraceDlg()
{
}

//*******************************************************************

void CHotlistTraceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TRACE, m_list);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CHotlistTraceDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TRACE, OnDoubleClickList)
END_MESSAGE_MAP()

//*******************************************************************
static const int nColumnCount = 4;
//*******************************************************************

BOOL CHotlistTraceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
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
	m_list.InsertColumn ( 1, Account::UserID.Label, LVCFMT_LEFT, column.GetWidth(1) );
	m_list.InsertColumn ( 2, "Hotlisted", LVCFMT_LEFT, column.GetWidth(2) );
	m_list.InsertColumn ( 3, "", LVCFMT_LEFT, column.GetWidth(3) );

	__int64 nCurrentCardNo = m_nRequestedUserID;

	CSQLRowAccountFull RowAccount;
	RowAccount.SetUserID(nCurrentCardNo);
	CSQLRepositoryAccount repoAccount;
	repoAccount.SelectRow(RowAccount,NULL);

	int nLine = 0;
	while (RowAccount.HavePreviousUserID() == TRUE )
	{
		RowAccount.SetUserID(RowAccount.GetPreviousUserID());

		{
			if (repoAccount.SelectRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				__int64 nUserID = RowAccount.GetUserID();
				m_arrayUserIDs.Add(nUserID);
				m_list.AddString(CreateDisplayLine(++nLine, nUserID, szNOTFOUND));
				break;
			}
		}

		if (RowAccount.HaveNextUserID() == FALSE )
		{
			__int64 nUserID = RowAccount.GetNextUserID();
			m_arrayUserIDs.Add(nUserID);
			m_list.AddString ( CreateDisplayLine ( ++nLine, nUserID, szLOSTLINK ) );	// lost link to next record
			break;
		}
	}

	{
		__int64 nUserID = RowAccount.GetUserID();
		m_arrayUserIDs.Add(nUserID);
		m_list.AddString(CreateDisplayLine(++nLine, nUserID, RowAccount.GetHotlistDate(), RowAccount.GetHotlistTime()));
	}

	if (m_nRequestedUserID == RowAccount.GetUserID())	
	{
		m_list.SetCurSel(nLine - 1);
	}

	while (RowAccount.HaveNextUserID() == TRUE )
	{
		RowAccount.SetUserID(RowAccount.GetNextUserID());
		__int64 nUserID = RowAccount.GetUserID();

		{
			if (repoAccount.SelectRow(RowAccount, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
			{
				m_arrayUserIDs.Add(nUserID);
				m_list.AddString(CreateDisplayLine(++nLine, nUserID, szNOTFOUND));
				break;
			}
		}

		if (RowAccount.IsCardHotlisted() == TRUE)
		{
			m_arrayUserIDs.Add(nUserID);
			m_list.AddString(CreateDisplayLine(++nLine, nUserID, RowAccount.GetHotlistDate(), RowAccount.GetHotlistTime()));
		}
		else
		{
			m_arrayUserIDs.Add(nUserID);
			m_list.AddString(CreateDisplayLine(++nLine, nUserID));
		}

		if (m_nRequestedUserID == nUserID)
		{
			m_list.SetCurSel(nLine - 1);
		}
	}
	
	return TRUE;
}

//*******************************************************************

CString CHotlistTraceDlg::CreateDisplayLine ( int nLine, __int64 nUserID, const char* szDate, const char* szTime )
{
	CString strLine = "";
	strLine.Format ( "%d\t%s\t%s\t%s", 
		nLine,
		(const char*) System.FormatCardNo(SimpleFormatInt64Value(nUserID)),
		szDate,  
		szTime );

	return strLine;
}

//*******************************************************************

void CHotlistTraceDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nIndex = m_list.GetCurSel();
	if ((nIndex >= 0) && (nIndex < m_arrayUserIDs.GetSize()))
	{
		__int64 nUserID = m_arrayUserIDs.GetAt(nIndex);
		CEditLoyaltyRecord EditRecord;
		EditRecord.EditRecord(ACCOUNTEDIT_HOTLIST, nUserID, this);
	}

	*pResult = 0;
}

//*******************************************************************

void CHotlistTraceDlg::OnOK()
{
	if ( UpdateData() == TRUE )
	{
		int nIndex = m_list.GetCurSel();
		if ((nIndex >= 0) && (nIndex < m_arrayUserIDs.GetSize()))
		{
			m_nRequestedUserID = m_arrayUserIDs.GetAt(nIndex);
		}
		
		CColumnHandler column;
		column.Save ( "TRACE", 0, nColumnCount, m_list );
		CDialog::OnOK();
	}
}

//*******************************************************************

