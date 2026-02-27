//$$******************************************************************
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_ClosingBalance\SQLRepositoryClosingBalance.h"
#include "..\SmartPay4Shared\SQLTable_ClosingBalance\SQLRecordSetClosingBalance.h"
//$$******************************************************************
#include "EODRegenDlg.h"
#include "ReportCSVFile.h"
//$$******************************************************************
#include "EODListDlg.h"
//$$******************************************************************

CEODListDlg::CEODListDlg(CWnd* pParent )
	: CSSAutoShutdownDialog(AutoShutdownHelper, CEODListDlg::IDD, pParent)
{
	m_bTodayListed = FALSE;
	m_nListCurSel = -1;
	m_nConsolCurSel = 0;
	m_nShowRegenerate = SW_HIDE;
	
	// View / Execute / Reprint / Regenerate / Consolidate
	m_nEODType = nEOD_VIEW;				
	
	CSSDateRange date ( System.GetStartOfWeek(), 1, 1, 1 );
	date.GetRange ( szRANGE_THIS_WEEK, m_ctimeFrom, m_ctimeTo );

	m_strDisplayDateToday = CSSDate::GetCurrentDate().GetDate();
	m_strKeyDateToday = CSQLRowClosingBalance::MakeKeyDate(COleDateTime::GetCurrentTime());

	m_options.Read();														// read background options
}

//$$******************************************************************

CEODListDlg::~CEODListDlg()
{
}

//$$******************************************************************

void CEODListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DATERANGE, m_comboDateRange);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY1, m_buttonAltKey1);
	DDX_Control(pDX, IDC_BUTTON_ALTKEY2, m_buttonAltKey2);
	DDX_Control(pDX, IDC_BUTTON_CONFIRM, m_buttonConfirm);
	DDX_Control(pDX, IDC_BUTTON_VIEW, m_buttonView);
	DDX_Control(pDX, IDC_BUTTON_REGENERATE, m_buttonRegenerate);
	DDX_Control(pDX, IDC_BUTTON_PRINT, m_buttonPrint);
	DDX_Control(pDX, IDC_CHECK_START, m_checkStart);
	DDX_Control(pDX, IDC_STATIC_EODBOXTEXT, m_staticEODBoxText);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_ctimeFrom);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_ctimeTo);
	DDX_CBString(pDX, IDC_COMBO_DATERANGE, m_strDateRange);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CEODListDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_CHECK_START, OnToggleStart)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM, OnButtonConfirm)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_BUTTON_REGENERATE, OnButtonRegenerate)
	ON_BN_CLICKED(IDC_BUTTON_CONSOLIDATE, OnButtonConsolidate)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY1, OnButtonAltKey1)
	ON_BN_CLICKED(IDC_BUTTON_ALTKEY2, OnButtonAltKey2)
	ON_CBN_SELCHANGE(IDC_COMBO_DATERANGE, OnSelectDateRange)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDoubleClickList)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST1, OnRightDoubleClickList)
END_MESSAGE_MAP()

//$$******************************************************************

void CEODListDlg::CSSAutoShutdownPreInitDialog()
{
	m_arrayReprintDates.RemoveAll();
	m_strReprintFilename = "";
}

//$$******************************************************************

BOOL CEODListDlg::CSSAutoShutdownPostInitDialog()
{
	m_buttonAltKey1.ShowWindow(SW_HIDE);
	m_buttonAltKey2.ShowWindow(SW_HIDE);

	// Date range

	m_comboDateRange.AddString(szRANGE_THIS_WEEK);
	m_comboDateRange.AddString(szRANGE_THIS_MONTH);
	m_comboDateRange.AddString(szRANGE_LAST_WEEK);
	m_comboDateRange.AddString(szRANGE_LAST_MONTH);
	m_comboDateRange.AddString(szRANGE_LAST_3_MONTHS);
	m_comboDateRange.AddString(szRANGE_LAST_6_MONTHS);
	m_comboDateRange.AddString(szRANGE_LAST_9_MONTHS);
	m_comboDateRange.AddString(szRANGE_LAST_12_MONTHS);
	m_comboDateRange.SetCurSel(m_nConsolCurSel);

	m_listboxDate.SubclassDlgItem(IDC_LIST1, this);
	m_listboxDate.InsertColumn(0, "Date", LVCFMT_LEFT, 70);
	m_listboxDate.InsertColumn(1, "Time", LVCFMT_CENTER, 45);
	m_listboxDate.InsertColumn(2, "Purse1", LVCFMT_RIGHT, 75);
	m_listboxDate.InsertColumn(3, "Purse3", LVCFMT_RIGHT, 75);

	int nTotalRows = 0;
	m_arrayEODListInfo.RemoveAll();
	CReportEndOfDay::GetEODDateInfo(m_arrayEODListInfo, nTotalRows);

	for ( int n = 0; n < m_arrayEODListInfo.GetCount(); n++ )
	{
		CEODListInfo& EODInfo = m_arrayEODListInfo.GetAt(n);
		
		CString strLine = "";
		strLine.Format("%s\t%s\t%.2lf\t%.2lf",
			(const char*)EODInfo.m_strDisplayDate,
			(const char*)EODInfo.m_strDisplayTime,
			EODInfo.m_dPurse1,
			EODInfo.m_dPurse3);

		m_listboxDate.AddString(strLine);
	
		if ( EODInfo.m_strKeyDate == m_strKeyDateToday )
		{
			m_bTodayListed = TRUE;	
		}
	}

	int nCount = m_listboxDate.GetItemCount();

	if ( 0 == nCount)
	{
		m_buttonPrint.EnableWindow(FALSE);
	}
	else
	{
		if ( -1 == m_nListCurSel)
		{
			m_nListCurSel = nCount - 1;
		}

		m_listboxDate.SetCurSel(m_nListCurSel);
	}

	CString strMsg = "";
	bool bShowStart = FALSE;									// assume no show of confirm EOD stat

	if (m_bTodayListed == TRUE)								// see if EOD listed for today 
	{
		if (m_options.m_bEODExport == TRUE)
		{
			strMsg.Format("Next End of Day scheduled for tomorrow after %s", 
				(const char*) m_options.m_strTimeNextEODCheck);
		}
		else
		{
			strMsg = "End of Day already performed today (see above)";
		}

		m_buttonView.SetWindowText ( "Next");
	}
	else
	{
		if (m_options.m_bEODExport == TRUE)
		{
			strMsg.Format("End of Day scheduled for today after %s", 
				(const char*) m_options.m_strTimeNextEODCheck);
		}
		else
		{
			strMsg.Format ( "End of Day for today : %s", 
				(const char*) m_strDisplayDateToday );

			bShowStart = TRUE;
		}
	}

	m_staticEODBoxText.SetWindowText ( strMsg );
	m_checkStart.EnableWindow(bShowStart);

	SetDisplay();

	return TRUE;
}

//$$******************************************************************

CString CEODListDlg::GetDateString()
{
	CString strBuf = "";

	if (m_strKeyDateFrom != m_strKeyDateTo)
	{
		if ((m_strKeyDateFrom.GetLength() == 8) && (m_strKeyDateTo.GetLength() == 8))
		{
			strBuf.Format(" (%s/%s/%s - %s/%s/%s)",
				(const char*) m_strKeyDateFrom.Right(2),
				(const char*) m_strKeyDateFrom.Mid(4,2),
				(const char*) m_strKeyDateFrom.Left(4),
				(const char*) m_strKeyDateTo.Right(2),
				(const char*) m_strKeyDateTo.Mid(4,2),
				(const char*) m_strKeyDateTo.Left(4));
		}
	}
	else
	{
		if (m_strKeyDateFrom.GetLength() == 8)
		{
			strBuf.Format(" (%s/%s/%s)",
				(const char*) m_strKeyDateFrom.Right(2),
				(const char*) m_strKeyDateFrom.Mid(4, 2),
				(const char*) m_strKeyDateFrom.Left(4));
		}
	}

	return strBuf;
}

//$$******************************************************************

void CEODListDlg::OnSelectDateRange() 
{
	if ( UpdateData() == TRUE )
	{
		CSSDateRange date ( System.GetStartOfWeek(), 1, 1, 1 );
		date.GetRange ( m_strDateRange, m_ctimeFrom, m_ctimeTo );
		UpdateData(FALSE);
	}
}

//$$******************************************************************

void CEODListDlg::OnButtonAltKey1()
{
	if ( m_nShowRegenerate != SW_SHOW )
	{
		m_nShowRegenerate = SW_SHOW;
		SetDisplay();
	}
}

//$$******************************************************************

void CEODListDlg::OnButtonAltKey2()
{
	ShowNoSalesReport ( "consol" );
}

//$$******************************************************************

void CEODListDlg::ShowNoSalesReport ( const char* szKey )
{
	CString strFilename = Filenames.GetEODNoSalesFilename ( szKey );					// see if report file in EOD No sales file name
	if ( ::FileExists ( strFilename ) == TRUE )
	{
		CCSVFileReport report ( "R222", strFilename, this );
		report.Show();
	}
}

//$$******************************************************************

void CEODListDlg::OnToggleStart()	
{ 
	SetDisplay();	
}

//$$******************************************************************

void CEODListDlg::SetDisplay()
{
	bool bStart = ( m_checkStart.GetCheck() != 0 );
	m_buttonConfirm.EnableWindow(bStart);
	m_buttonView.EnableWindow(bStart);
	m_buttonRegenerate.ShowWindow(m_nShowRegenerate);
}

//$$******************************************************************

void CEODListDlg::OnButtonConfirm()
{
	CStringArray Warning;
	Warning.Add ( "End of Day Confirmation required" );
	Warning.Add ( "Current balances and cashless posting entries will be recorded." );
	Warning.Add ( "New closing balance figures will be carried forward." );

	if ( Prompter.Warning( Warning ) == IDYES )
	{
		m_nEODType = nEOD_EXECUTE;
		CloseAsOK();
	}
}

//$$******************************************************************

void CEODListDlg::OnButtonView()
{
	m_nEODType = nEOD_VIEW;

	if ((m_options.m_bEODExport == TRUE) && (m_bTodayListed == FALSE))			// see if EOD performed by background scheduler
	{
		CString strCurrentKey = "";
		CSQLRowClosingBalance RowClosing;
		RowClosing.SetKeyDate(m_strKeyDateToday);
		
		CSQLRepositoryClosingBalance RepoClosing;
		if (RepoClosing.SelectRow(RowClosing,NULL).GetSQLError() == SQLCRUD_ERR_NONE)
		{
			strCurrentKey = m_strKeyDateToday;
		}

		if ( strCurrentKey != "" )											// see if EOD been done today whilst in dialog
		{
			m_nEODType = nEOD_REPRINT;										// force reprint
			m_strReprintFilename = Filenames.GetEODReportPath ( strCurrentKey );	// report file in EOD folder
		}
	}

	CloseAsOK();
}

//$$******************************************************************

void CEODListDlg::OnButtonConsolidate()
{
	if ( UpdateData() == TRUE )
	{
		CSSDate date;
		date.SetDate ( m_ctimeFrom );	
		m_strKeyDateFrom = CSQLRowClosingBalance::MakeKeyDate(date);
		
		date.SetDate ( m_ctimeTo );	
		m_strKeyDateTo = CSQLRowClosingBalance::MakeKeyDate(date);

		m_nEODType = nEOD_CONSOLIDATE;
		m_nConsolCurSel = m_comboDateRange.GetCurSel();
		CloseAsOK();
	}
}

//$$******************************************************************

void CEODListDlg::OnButtonRegenerate()
{
	if (UpdateData() == FALSE)
	{
		return;

	}
	
	int nIndex = m_listboxDate.GetCurSel();
	if ((nIndex < 0) || (nIndex >= m_arrayEODListInfo.GetSize()))
	{
		return;
	}

	if (TRUE == m_arrayEODListInfo[nIndex].m_bArchived)
	{
		Prompter.Error("Unable to re-generate the selected report as the associated audit lines have been archived!", "End Of Day Postings");
		return;
	}
		
	CEODRegenDlg dlgRegen(this);
	if (dlgRegen.DoModal() != IDOK)
	{
		return;
	}

	m_nEODType = nEOD_REGENERATE_SINGLE;
	SetReprintDate(m_arrayEODListInfo[nIndex].m_strKeyDate);

	if (1 == dlgRegen.m_nRegenType)
	{
		nIndex++;
		for (; nIndex < m_arrayEODListInfo.GetSize(); nIndex++)
		{
			if (FALSE == m_arrayEODListInfo[nIndex].m_bArchived)
			{
				m_nEODType = nEOD_REGENERATE_ALL;
				AddReprintDate(m_arrayEODListInfo[nIndex].m_strKeyDate);
			}
			else
			{
				break;
			}
		}
	}

	CloseAsOK();
}

//$$******************************************************************

void CEODListDlg::OnDoubleClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnButtonPrint();
	*pResult = 0;
}

//$$******************************************************************

void CEODListDlg::OnButtonPrint()
{
	if ( UpdateData() == TRUE )
	{
		m_nEODType = nEOD_REPRINT;								// force reprint
		SelectEODReport();
	}
}

//$$******************************************************************

void CEODListDlg::OnRightDoubleClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nIndex = m_listboxDate.GetCurSel();

	if ((nIndex >= 0) && (nIndex < m_arrayEODListInfo.GetSize()))
	{
		CSSDate date;	
		date.SetDate(m_arrayEODListInfo[nIndex].m_strDisplayDate);
		ShowNoSalesReport(CSQLRowClosingBalance::MakeKeyDate(date));
	}
	*pResult = 0;	
}

//$$******************************************************************

void CEODListDlg::SelectEODReport()
{
	int nIndex = m_listboxDate.GetCurSel();

	if ((nIndex >= 0) && (nIndex < m_arrayEODListInfo.GetSize()))
	{
		CString strKey = m_arrayEODListInfo[nIndex].m_strKeyDate;
	
		// see if report file in EOD folder 		
		CString strFilename = Filenames.GetEODReportPath ( strKey );	
		if ( ::FileExists(strFilename) == FALSE )
		{
			if (TRUE == m_arrayEODListInfo[nIndex].m_bArchived)
			{
				Prompter.Error("Unable to re-generate the selected report as the associated audit lines have been archived!", "End Of Day Postings");
			}
			else
			{
				SetReprintDate(strKey);
				CloseAsOK();
			}
		}
		else
		{
			m_strReprintFilename = strFilename;
			CloseAsOK();
		}
	}
}

//$$******************************************************************

void CEODListDlg::SetReprintDate(CString strDate)
{
	m_arrayReprintDates.RemoveAll();
	m_arrayReprintDates.Add(strDate);
}

//$$******************************************************************

void CEODListDlg::AddReprintDate(CString strDate)
{
	m_arrayReprintDates.Add(strDate);
}

//$$******************************************************************

CString CEODListDlg::GetReprintDate(int n)
{
	CString strDate = "";
	if ((n >= 0) && (n < m_arrayReprintDates.GetSize()))
	{
		strDate = m_arrayReprintDates.GetAt(n);
	}

	return strDate;
}

//$$******************************************************************

void CEODListDlg::OnOK()
{
}

//$$******************************************************************

void CEODListDlg::CloseAsOK()
{
	m_nListCurSel = m_listboxDate.GetCurSel() ;
	EndDialog(IDOK);
}

//$$******************************************************************
