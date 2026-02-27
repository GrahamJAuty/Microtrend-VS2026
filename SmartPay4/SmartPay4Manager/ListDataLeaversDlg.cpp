/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
/**********************************************************************/
#include "..\SmartPay4ManagerBgnd\LeaversHandler.h"
/**********************************************************************/
#include "..\SmartPay4ManagerEmail\DefTextEmailAddress.h"
/**********************************************************************/
#include "..\SmartPay4Shared\GlobalState.h"
/**********************************************************************/
#include "AuditReportWrapper.h"
#include "DefEditEmailDlg.h"
#include "FindDlg.h"
#include "ListDataDatabaseReportDlg.h"
/**********************************************************************/
#include "ListDataLeaversDlg.h"
/**********************************************************************/

CSingleListContentHelpersLeavers::CSingleListContentHelpersLeavers(CStringArray& arrayYears) : CSingleListContentHelpers(),
	m_arrayYears(arrayYears)
{
}

/**********************************************************************/

int CSingleListContentHelpersLeavers::GetArraySize()
{
	return m_arrayYears.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersLeavers::PrepareLine( int nIndex, CString& strLine )
{
	strLine = "";

	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine = m_arrayYears.GetAt(nIndex);
		bResult = TRUE;
	}
	
	return bResult;
}

/**********************************************************************/

void CSingleListContentHelpersLeavers::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Leavers Year", LVCFMT_LEFT, 480 );
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataLeaversDlg::CListDataLeaversDlg( CWnd* pParent) : CListDataDlg ( pParent )
{
	m_pContentHelpers = new CSingleListContentHelpersLeavers(m_arrayYears);
	m_pDisplayHelpers = new CSingleListDisplayHelpers( m_listData, *m_pContentHelpers );

	m_pDisplayHelpers -> SetTopSingleText( "email address" );
	m_pDisplayHelpers -> SetTopPluralText( "email addresses" );
	m_pDisplayHelpers -> SetBottomSingleText( "address" );
	m_pDisplayHelpers -> SetBottomPluralText( "addresses" );

	CString strMask = Filenames.GetLeaversPathname("*.*");		// "<data path\>Pnn\Leavers\*.*"

	CFileFind FileFinder;
	BOOL bWorking = FileFinder.FindFile(strMask);
	
	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();
		if (FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() != TRUE)
		{
			continue;
		}

		CString strYear = FileFinder.GetFileTitle();

		if (strYear.GetLength() != 9)
		{
			continue;
		}

		if (::TestNumeric(strYear.Left(4) + strYear.Right(4)) == FALSE)
		{
			continue;
		}

		if (strYear.Mid(4, 1) != "-")
		{
			continue;
		}

		m_arrayYears.Add(strYear);
	}
}

/**********************************************************************/

CListDataLeaversDlg::~CListDataLeaversDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataLeaversDlg::SetTitle()
{
	SetWindowText ( "Leavers" );
}

/**********************************************************************/

void CListDataLeaversDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Reports" );
	m_button2.SetWindowText ( "&Find" );
	m_button3.ShowWindow(SW_HIDE);
	m_button4.SetWindowText ( "&Audit" );
	m_button5.ShowWindow(SW_HIDE);
	m_button6.ShowWindow ( SW_HIDE );

	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 47 );
	MoveControl( &m_button3, 368, 87 );
	MoveControl( &m_button4, 368, 147 );
	MoveControl( &m_button5, 368, 187 );
}

/**********************************************************************/

void CListDataLeaversDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataLeaversDlg::PrepareList()
{
	CListDataDlg::PrepareList();
	SelectLine(m_arrayYears.GetCount() - 1);
}
/**********************************************************************/

CString CListDataLeaversDlg::GetSelectedYear()
{
	CString strYear = "";

	int nIndex = GetSafeSelection();
	if (nIndex != -1)
	{
		strYear = m_arrayYears.GetAt(nIndex);
	}

	return strYear;
}

//**************************************************************

void CListDataLeaversDlg::HandleButton1() 
{
	CString strYear = GetSelectedYear();

	if (strYear != "")
	{
		CLeaversHandler data(strYear);						// create leavers folder\files if required

		int nYear = atoi(strYear.Right(4));

		if ((nYear >= 2000) && (nYear <= 9999))
		{
			g_GlobalState.SetLeaverYear(nYear);

			CListDataDatabaseReportDlg dlg(this);
			dlg.SetLeaversFilename(data.m_strLeaversFindListFile);

			bool bRunFind = FALSE;
			if (dlg.DoModal() == IDOK)
			{
				bRunFind = dlg.GetLeaversRunFindFlag();
			}

			if (bRunFind == TRUE)
			{
				DoFind(strYear);
			}

			g_GlobalState.ClearLeaverYear();
		}
	}
}

/**********************************************************************/

void CListDataLeaversDlg::HandleButton2() 
{
	CString strYear = GetSelectedYear();

	if (strYear != "")
	{
		DoFind(strYear);
	}
}

/**********************************************************************/

void CListDataLeaversDlg::HandleButton4()
{
	CString strYear = GetSelectedYear();

	if (strYear != "")
	{
		CLeaversHandler data(strYear);

		int nYear = atoi(strYear.Right(4));

		if ((nYear >= 2000) && (nYear <= 9999))
		{
			g_GlobalState.SetLeaverYear(nYear);
			CAuditReportWrapper Wrapper(data.m_strAuditFilename, "R127", 0, this);
			Wrapper.ShowReport();
			g_GlobalState.ClearLeaverYear();
		}
	}
}

/**********************************************************************/

void CListDataLeaversDlg::DoFind(CString strYear)
{
	CLeaversHandler data(strYear);						// create leavers folder\files if required

	int nYear = atoi(strYear.Right(4));

	if ((nYear >= 2000) && (nYear <= 9999))
	{
		g_GlobalState.SetLeaverYear(nYear);

		CFindDlg dlg(strYear, this);
		dlg.SetListFilename(data.m_strLeaversFindListFile);
		dlg.DoModal();

		CFileRecycler::SendToRecycleBin(data.m_strLeaversFindListFile);

		g_GlobalState.ClearLeaverYear();
	}
}

//**************************************************************
