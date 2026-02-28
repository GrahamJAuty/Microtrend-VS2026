/**********************************************************************/
#include "..\SPosLoyalty4Shared\SQLTable_EcrmanDatabase\SQLRepositoryEcrmanDatabase.h"
/**********************************************************************/
#include "..\SPosLoyalty4Shared\ServerData.h"
/**********************************************************************/
#include "ListDataSchemeNamesDlg.h"
#include "ReportCSVFile.h"
/**********************************************************************/
#include "ListDataDatabaseTestDlg.h"
/**********************************************************************/

CSingleListContentHelpersDatabaseTest::CSingleListContentHelpersDatabaseTest(CStringArray& arrayDbNames,bool bSchemeNames) : CSingleListContentHelpers(),
	m_arrayDbNames(arrayDbNames)
{
	m_bSchemeNames = bSchemeNames;
}

/**********************************************************************/

bool CSingleListContentHelpersDatabaseTest::PrepareLine(int nIndex, CString& strLine)
{
	if (TRUE == m_bSchemeNames)
	{
		strLine = "Loyalty Scheme Names are global";
	}
	else if ((nIndex >= 0) && (nIndex < m_arrayDbNames.GetSize()) && (nIndex < System.GetMaxEcrmanDatabases()))
	{
		strLine.Format( "%3.3d\t%s",
			nIndex + 1, 
			(const char*) m_arrayDbNames.GetAt(nIndex));
	}
	else
	{
		strLine.Format("%3.3d\tDatabase %d",
			nIndex + 1,
			nIndex + 1);
	}
	
	return TRUE;
}

/**********************************************************************/

void CSingleListContentHelpersDatabaseTest::DeleteDatabaseEntry( int nIndex )
{
}

/**********************************************************************/

void CSingleListContentHelpersDatabaseTest::FilterSelectedItemArray( CReportConsolidationArray<CSortedIntItem>& arraySelected )
{
	m_bPreventDeletion = FALSE;
}

/**********************************************************************/

void CSingleListContentHelpersDatabaseTest::SetColumns( CSSListCtrlVirtual& list, CWnd* pParent )
{
	list.SubclassDlgItem ( IDC_LIST, pParent );

	if (FALSE == m_bSchemeNames)
	{
		list.InsertColumn(0, "DbNo", LVCFMT_LEFT, 80);
		list.InsertColumn(1, "Name", LVCFMT_LEFT, 400);
	}
	else
	{
		list.InsertColumn(0, "", LVCFMT_LEFT, 480);
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CListDataDatabaseTestDlg::CListDataDatabaseTestDlg( CString strDatabaseRootFolder, CWnd* pParent) : CListDataDlg(pParent)
{
	m_strDatabaseRootFolder = strDatabaseRootFolder;

	if (m_strDatabaseRootFolder != "")
	{
		m_strTitle = "Test Database Import Links";
		CSQLRepositoryEcrmanDatabase repoDb;
		repoDb.GetDatabaseNameList(m_arrayDbNames, NULL);
	}
	else
	{
		m_strTitle = "Edit Loyalty Scheme Names";	
	}

	m_pContentHelpers = new CSingleListContentHelpersDatabaseTest(m_arrayDbNames, m_strDatabaseRootFolder == "");
	m_pDisplayHelpers = new CSingleListDisplayHelpers(m_listData, *m_pContentHelpers);
	
	m_pDisplayHelpers->SetTopSingleText("database");
	m_pDisplayHelpers->SetTopPluralText("databases");
	m_pDisplayHelpers->SetBottomSingleText("database");
	m_pDisplayHelpers->SetBottomPluralText("databases");
}

/**********************************************************************/

CListDataDatabaseTestDlg::~CListDataDatabaseTestDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CListDataDatabaseTestDlg::SetTitle()
{
	SetWindowText ( m_strTitle );
}

/**********************************************************************/

void CListDataDatabaseTestDlg::SetButtonTexts()
{
	if (m_strDatabaseRootFolder != "" )
	{
		m_button1.SetWindowText("Plu");
		m_button2.SetWindowText("Dept");
		m_button3.SetWindowText("Stamp\nOffer");
		m_button4.SetWindowText("Loyalty\nScheme");
		m_button5.ShowWindow(SW_HIDE);
		m_button6.ShowWindow(SW_HIDE);
	}
	else
	{
		m_button1.SetWindowText("Scheme\nNames");
		m_button2.ShowWindow(SW_HIDE);
		m_button3.ShowWindow(SW_HIDE);
		m_button4.ShowWindow(SW_HIDE);
		m_button5.ShowWindow(SW_HIDE);
		m_button6.ShowWindow(SW_HIDE);
	}

	MoveControl(&m_button1, 368, 7);
	MoveControl(&m_button2, 368, 47);
	MoveControl(&m_button3, 368, 87);
	MoveControl(&m_button4, 368, 127);
	MoveControl(&m_button5, 368, 187);
	MoveControl(&m_button6, 368, 187);

	ShowAndEnableWindow(&m_buttonOK, FALSE);
	m_buttonCancel.SetWindowText("Close");
}

/**********************************************************************/

void CListDataDatabaseTestDlg::SetColumns()
{
	m_pContentHelpers -> SetColumns( m_listData, this );
}

/**********************************************************************/

void CListDataDatabaseTestDlg::PrepareList()
{
	CListDataDlg::PrepareList();
	SelectLine(0);
}

/**********************************************************************/

void CListDataDatabaseTestDlg::HandleButton1()
{
	if (m_strDatabaseRootFolder == "")
	{
		CListDataSchemeNamesDlg dlg(this);
		dlg.DoModal();
	}
	else
	{
		int nDbNo = GetSafeSelection() + 1;

		if ((nDbNo >= 1) && (nDbNo <= PLU_DBNO_COUNT))
		{
			HandleTestButton(Server.GetEcrManagerImportPluFilename(nDbNo), "plupts.csv", "Plu", "R127");
		}
	}
}

/**********************************************************************/

void CListDataDatabaseTestDlg::HandleButton2()
{
	if (m_strDatabaseRootFolder != "")
	{
		int nDbNo = GetSafeSelection() + 1;

		if ((nDbNo >= 1) && (nDbNo <= PLU_DBNO_COUNT))
		{
			HandleTestButton(Server.GetEcrManagerImportDeptFilename(nDbNo), "loydept.csv", "Department", "R131");
		}
	}
}

/**********************************************************************/

void CListDataDatabaseTestDlg::HandleButton3()
{
	if (m_strDatabaseRootFolder != "")
	{
		int nDbNo = GetSafeSelection() + 1;

		if ((nDbNo >= 1) && (nDbNo <= PLU_DBNO_COUNT))
		{
			HandleTestButton(Server.GetEcrManagerImportStampOfferFilename(), "stampoffer.csv", "Stamp Offer", "R128");
		}
	}
}

/**********************************************************************/

void CListDataDatabaseTestDlg::HandleButton4()
{
	if (m_strDatabaseRootFolder != "")
	{
		HandleTestButton(Server.GetEcrManagerImportLoyaltySchemeFilename(), "loyscheme.csv", "Loyalty Scheme", "R134");
	}
}

/**********************************************************************/

void CListDataDatabaseTestDlg::HandleTestButton( CString strFilepath, CString strFilename, CString strText, CString strReportType)
{
	CString strMsg = "";
	int nType = 0;

	CString strTest1 = strFilepath;
	CString strTest2 = strFilename;
	strTest1.MakeLower();
	strTest2.MakeLower();

	if (strTest1.Right(strTest2.GetLength()) != strTest2)
	{
		strMsg.Format("Unexpected file name %s",
			(const char*)strFilename);
	}
	else
	{
		CString strImportFolder = strFilepath.Left(strFilepath.GetLength() - strFilename.GetLength());

		if (::ExistSubdirectory(strImportFolder) == FALSE)
		{
			strMsg.Format("Path to import folder ' %s 'is invalid!",
				(const char*)strImportFolder);
		}
		else
		{
			if (::ValidateFolderAccess(strImportFolder) == FALSE)
			{
				strMsg.Format("Unable to write to folder ' %s '!",
					(const char*)strImportFolder);
			}
			else
			{
				strMsg = "Path to import folder is valid.";
				if (SolutionGlobalFunctions::FileExists(strFilepath) == TRUE)
				{
					strMsg += "\n\n";
					strMsg += strText;
					strMsg += " update file found, would you like to view it?";
					nType = MB_ICONQUESTION | MB_YESNO;
				}
				else
				{
					strMsg += "\n\nNo outstanding ";
					strMsg += strText;
					strMsg += " updates found.";
				}
			}
		}
	}

	if (MessageBox(strMsg, "Validation Check", nType) == IDYES)
	{
		CCSVFileReport report(strReportType, strFilepath, this);
		report.Show(strText + " Update File");
	}
}

/**********************************************************************/

bool CListDataDatabaseTestDlg::HandleOK()
{
	return TRUE;
}

/**********************************************************************/

bool CListDataDatabaseTestDlg::HandleCancel()
{
	return TRUE;
}

/**********************************************************************/

