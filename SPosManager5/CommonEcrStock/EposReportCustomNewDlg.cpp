/**********************************************************************/
#include "SysInfo.h"
/**********************************************************************/
#include "EposReportCustomNewDlg.h"
/**********************************************************************/

CEposReportCustomNewDlg::CEposReportCustomNewDlg(CWnd* pParent /*=NULL*/)
	: CSSDialog(CEposReportCustomNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEposReportCustomNewDlg)
	//}}AFX_DATA_INIT
	m_nSel = 0;
}

/**********************************************************************/

void CEposReportCustomNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEposReportCustomNewDlg)
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CEposReportCustomNewDlg, CDialog)
	//{{AFX_MSG_MAP(CEposReportCustomNewDlg)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CEposReportCustomNewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_listReports.SubclassDlgItem ( IDC_LIST, this );
	m_listReports.InsertColumn ( 0, "", LVCFMT_LEFT, 200 );
	m_listReports.ModifyStyle( LVS_OWNERDRAWFIXED, 0, 0 );
	m_listReports.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	FillListBox();

	m_listReports.SetCurSel(0);
	m_listReports.SetFocus();
	m_listReports.Invalidate();
	
	return FALSE;  
}

/**********************************************************************/

void CEposReportCustomNewDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )  
   {
	   m_listReports.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

void CEposReportCustomNewDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOK();
	*pResult = 0;
}

/**********************************************************************/

void CEposReportCustomNewDlg::FillListBox()
{
	m_listReports.DeleteAllItems();

	CStringArray arrayReports;

	if (SysInfo.IsEcrLiteSystem() == FALSE)
	{
		arrayReports.Add("Hourly");
		arrayReports.Add("Hourly Group");
		arrayReports.Add("Hourly Department");
		arrayReports.Add("Covers Hourly");
		arrayReports.Add("Covers Hourly Group");
		arrayReports.Add("Covers Hourly Department");
		arrayReports.Add("Plu by Price Band");
		arrayReports.Add("General");
		arrayReports.Add("Deposits");
		arrayReports.Add("Mix Match Usage");
		arrayReports.Add("Discount Detail");
		arrayReports.Add("Payment Detail");

		{
			CString strType = "";
			switch (SysInfo.GetLoyaltyOrSmartPayType())
			{
			case LOYALTY_TYPE_LOY_V3:
			case LOYALTY_TYPE_LOY_V4:
				strType = "Loyalty";
				break;

			case LOYALTY_TYPE_SMP_V2:
			case LOYALTY_TYPE_SMP_V4:
				strType = "SmartPay";
				break;
			}

			if (strType != "")
			{
				arrayReports.Add(strType + " Reconciliation");
			}
		}

		arrayReports.Add("Transaction Log");
	}
	else
	{
		arrayReports.Add("Hourly");
		arrayReports.Add("Transaction Log");
	}

	for (int n = 0; n < arrayReports.GetSize(); n++)
	{
		m_listReports.InsertItem(n, "");
		m_listReports.SetItem(n, 0, LVIF_TEXT, arrayReports.GetAt(n), 0, 0, 0, NULL);
	}
}

/**********************************************************************/

void CEposReportCustomNewDlg::OnOK() 
{
	m_nSel = m_listReports.GetCurSel();

	int nMaxSel = 1;

	if (SysInfo.IsEcrLiteSystem() == FALSE)
	{
		switch (SysInfo.GetLoyaltyOrSmartPayType())
		{
		case LOYALTY_TYPE_LOY_V3:
		case LOYALTY_TYPE_LOY_V4:
		case LOYALTY_TYPE_SMP_V2:
		case LOYALTY_TYPE_SMP_V4:
			nMaxSel = 13;
			break;

		default:
			nMaxSel = 12;
			break;
		}
	}

	if ((m_nSel < 0) || (m_nSel > nMaxSel))
	{
		m_nSel = 0;
	}

	EndDialog ( IDOK );
}

/**********************************************************************/

void CEposReportCustomNewDlg::OnCancel() 
{
	EndDialog ( IDCANCEL );
}

/**********************************************************************/
