//$$******************************************************************
#include "..\SmartPay4Shared\PasswordData.h"
//$$******************************************************************
#include "..\SmartPay4ManagerEmail\DefTextEmailAddress.h"
//$$******************************************************************
#include "DatabaseReport.h"
#include "DbExportEditor.h"
#include "DefEditEmailDlg.h"
//$$******************************************************************
#include "ListDataDatabaseReportDlg.h"
//$$******************************************************************

CSingleListContentHelpersDatabaseReport::CSingleListContentHelpersDatabaseReport(CArray<CDatabaseReportInfo, CDatabaseReportInfo>& arrayReportInfo) : CSingleListContentHelpers(),
	m_arrayReportInfo( arrayReportInfo )
{
}

//$$******************************************************************

int CSingleListContentHelpersDatabaseReport::GetArraySize()
{
	return m_arrayReportInfo.GetSize();
}

//$$******************************************************************

bool CSingleListContentHelpersDatabaseReport::PrepareLine( int nIndex, CString& strLine )
{
	bool bResult = FALSE;
	if ( ( nIndex >= 0 ) && ( nIndex < GetArraySize() ) )
	{
		strLine = m_arrayReportInfo[nIndex].m_strName;
		bResult = TRUE;
	}
	
	return bResult;
}

//$$******************************************************************

void CSingleListContentHelpersDatabaseReport::DeleteDatabaseEntry( int nIndex )
{
	if ( ( nIndex >= 0 ) && ( nIndex < m_arrayReportInfo.GetSize() ) )
	{
		m_arrayReportInfo.RemoveAt( nIndex );
		m_bDeletionEffect = TRUE;
	}
}

//$$******************************************************************

void CSingleListContentHelpersDatabaseReport::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );
	list.InsertColumn ( 0, "Report Name", LVCFMT_LEFT, 480 );
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

CListDataDatabaseReportDlg::CListDataDatabaseReportDlg(CWnd* pParent) : CListDataDlg ( pParent )
{
	m_strReportName = "";
	m_strReportLabel = "";
	m_nDefaultOption = 0;
	m_bEditReqd = FALSE;

	m_pContentHelpers = new CSingleListContentHelpersDatabaseReport( m_arrayReportInfo);
	m_pDisplayHelpers = new CSingleListDisplayHelpers(m_listData, *m_pContentHelpers);

	m_pDisplayHelpers->SetTopSingleText("database report");
	m_pDisplayHelpers->SetTopPluralText("database reports");
	m_pDisplayHelpers->SetBottomSingleText("report");
	m_pDisplayHelpers->SetBottomPluralText("reports");

	m_strLeaversFilename = "";
	m_bLeaversRunFind = FALSE;
}

//$$******************************************************************

CListDataDatabaseReportDlg::~CListDataDatabaseReportDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

//$$******************************************************************

void CListDataDatabaseReportDlg::CSSAutoShutdownPreInitDialog()
{
	m_export.GetCustomListArray(m_arrayReportInfo);
}	

//$$******************************************************************

void CListDataDatabaseReportDlg::SetTitle()
{
	SetWindowText ( "Database Reports" );
}

//$$******************************************************************

void CListDataDatabaseReportDlg::SetButtonTexts()
{
	m_button1.SetWindowText ( "&Add" );
	m_button2.SetWindowText ( "&Edit" );
	m_button3.SetWindowText ( "&Delete" );
	m_button4.SetWindowText ( "Move &Up" );
	m_button5.SetWindowText ( "Move D&own");
	m_button6.SetWindowText ( "&Show Report");
	
	MoveControl( &m_button1, 368, 7 );
	MoveControl( &m_button2, 368, 42 );
	MoveControl( &m_button3, 368, 77 );
	MoveControl( &m_button4, 368, 122 );
	MoveControl( &m_button5, 368, 157 );
	MoveControl( &m_button6, 368, 202 );
}

//$$******************************************************************

void CListDataDatabaseReportDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

//$$******************************************************************

void CListDataDatabaseReportDlg::PrepareList()
{
	CListDataDlg::PrepareList();

	m_bEditReqd = FALSE;

	if (Passwords.IsEditRecordAllowed() == FALSE)
	{
		MoveControl(&m_button6, 368, 7);
		SetDefID(IDC_BUTTON6);

		ShowAndEnableWindow(&m_button1, FALSE);
		ShowAndEnableWindow(&m_button2, FALSE);
		ShowAndEnableWindow(&m_button3, FALSE);
		ShowAndEnableWindow(&m_button4, FALSE);
		ShowAndEnableWindow(&m_button5, FALSE);
	}

	SelectLine(m_nDefaultOption);
}

//$$******************************************************************

void CListDataDatabaseReportDlg::HandleButton1()
{
	BOOL bLabelInUse[100];
	for (int n = 1; n <= 99; n++)
	{
		bLabelInUse[n] = FALSE;
	}

	for (int n = 0; n < m_arrayReportInfo.GetSize(); n++)
	{
		CString strLabel = m_arrayReportInfo[n].m_strLabel;

		if ((strLabel.Left(2) == "UR") && (::TestNumeric(strLabel.Right(2)) == TRUE))
		{
			int x = atoi(strLabel.Right(2));
			if ((x >= 1) && (x <= 99))
			{
				bLabelInUse[x] = TRUE;
			}
		}
	}

	CString strNewLabel = "";

	for (int n = 1; n <= 99; n++)
	{
		if (bLabelInUse[n] == FALSE)
		{
			strNewLabel.Format("UR%2.2d", n);
			break;
		}
	}

	if (strNewLabel == "")
	{
		Prompter.Error("All custom report slots are already in use");
	}
	else
	{
		CDatabaseReportInfo info;
		info.m_strLabel = strNewLabel;
		m_export.MakeDefaultReportInfo(info);

		int nPos = GetSafeSelection() + 1;
		m_arrayReportInfo.InsertAt(nPos, info);
		m_listData.SetItemCountEx(GetArraySize());
		m_listData.Invalidate();
		SelectLine(nPos);

		HandleButton2();

		m_WarnCancelAfterChange.SetFlag(TRUE);
	}
}

//$$******************************************************************

void CListDataDatabaseReportDlg::HandleButton2()
{
	int nSel = GetSafeSelection();

	if ((nSel >= 0) && (nSel < m_arrayReportInfo.GetSize()))
	{
		CDbExportEditor editor(this);

		CDatabaseReportInfo info = m_arrayReportInfo.GetAt(nSel);
		
		if (editor.EditExportLabelFromReportInfo(info) == TRUE)
		{
			m_arrayReportInfo.SetAt(nSel, info);
			m_listData.Invalidate();
			m_WarnCancelAfterChange.SetFlag(TRUE);
		}
	}
}

//$$******************************************************************

void CListDataDatabaseReportDlg::HandleButton3()
{
	m_pDisplayHelpers -> HandleDelete( this );
	m_WarnCancelAfterChange.SetFlag( m_pContentHelpers -> GetDeletionEffectFlag() );
}

//$$******************************************************************

void CListDataDatabaseReportDlg::HandleButton4()
{
	int nSel = GetSafeSelection();

	if ((nSel > 0) && (nSel < m_arrayReportInfo.GetSize()))
	{
		CDatabaseReportInfo temp1 = m_arrayReportInfo.GetAt(nSel);
		CDatabaseReportInfo temp2 = m_arrayReportInfo.GetAt(nSel-1);
		m_arrayReportInfo.SetAt(nSel - 1, temp1);
		m_arrayReportInfo.SetAt(nSel, temp2);
		SelectLine(nSel - 1);
	}
}

//$$******************************************************************

void CListDataDatabaseReportDlg::HandleButton5()
{
	int nSel = GetSafeSelection();

	if ((nSel >= 0) && (nSel < m_arrayReportInfo.GetSize() - 1 ))
	{
		CDatabaseReportInfo temp1 = m_arrayReportInfo.GetAt(nSel);
		CDatabaseReportInfo temp2 = m_arrayReportInfo.GetAt(nSel + 1);
		m_arrayReportInfo.SetAt(nSel + 1, temp1);
		m_arrayReportInfo.SetAt(nSel, temp2);
		SelectLine(nSel + 1);
	}
}

//$$******************************************************************

void CListDataDatabaseReportDlg::HandleButton6()
{
	int nSel = GetSafeSelection();

	if ((nSel >= 0) && (nSel < m_arrayReportInfo.GetSize() ) )
	{
		CDatabaseReportInfo info = m_arrayReportInfo.GetAt(nSel);
		CDatabaseReport report(info.m_strLabel, this);
		report.Show(info);

		if ( m_strLeaversFilename != "" )
		{
			CString strFilename = report.m_strSaveAsFilename;
			if (report.m_strSaveAsFilename.CompareNoCase( m_strLeaversFilename) == 0)
			{
				m_bLeaversRunFind = TRUE;
				HandleOK();
			}
		}
	}
}

//$$******************************************************************

int CListDataDatabaseReportDlg::GetLabel()
{
	int nSel = GetSafeSelection();

	if ((nSel >= 0) && (nSel < m_arrayReportInfo.GetSize()))
	{
		m_strReportLabel = m_arrayReportInfo[nSel].m_strLabel;
	}
	else
	{
		m_strReportLabel = "";
	}

	return nSel;
}

//$$******************************************************************

bool CListDataDatabaseReportDlg::HandleOK()
{
	bool bResult = CListDataDlg::HandleOK();
	m_export.MergeCustomReports(m_arrayReportInfo);
	return bResult;
}

//$$******************************************************************
