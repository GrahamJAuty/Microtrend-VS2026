/**********************************************************************/
#include "CommsBatchDlg.h"
#include "DatabaseCSVArray.h"
#include "DownloadDateDlg.h"
#include "KeyboardMap.h"
#include "ListDataDlg.h"
#include "NetworkCSVArray.h"
#include "Password.h"
#include "SelectMultipleLocationDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "EditRatesDlg.h"
/**********************************************************************/

CEditRatesDlg::CEditRatesDlg( CWnd* pParent)
	: CSSDialog(CEditRatesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditRatesDlg)
	//}}AFX_DATA_INIT
	m_IDEditRate[0] = IDC_EDIT_RATE1;
	m_IDEditRate[1] = IDC_EDIT_RATE2;
	m_IDEditRate[2] = IDC_EDIT_RATE3;
	m_IDEditRate[3] = IDC_EDIT_RATE4;
	m_IDEditRate[4] = IDC_EDIT_RATE5;
	m_IDEditRate[5] = IDC_EDIT_RATE6;
	m_IDEditRate[6] = IDC_EDIT_RATE7;
	m_IDEditRate[7] = IDC_EDIT_RATE8;
	m_IDEditRate[8] = IDC_EDIT_RATE9;
	m_IDEditRate[9] = IDC_EDIT_RATE10;
	m_IDEditRate[10] = IDC_EDIT_RATE11;
	m_IDEditRate[11] = IDC_EDIT_RATE12;

	m_IDEditText[0] = IDC_EDIT_TEXT1;
	m_IDEditText[1] = IDC_EDIT_TEXT2;
	m_IDEditText[2] = IDC_EDIT_TEXT3;
	m_IDEditText[3] = IDC_EDIT_TEXT4;
	m_IDEditText[4] = IDC_EDIT_TEXT5;
	m_IDEditText[5] = IDC_EDIT_TEXT6;
	m_IDEditText[6] = IDC_EDIT_TEXT7;
	m_IDEditText[7] = IDC_EDIT_TEXT8;
	m_IDEditText[8] = IDC_EDIT_TEXT9;
	m_IDEditText[9] = IDC_EDIT_TEXT10;
	m_IDEditText[10] = IDC_EDIT_TEXT11;
	m_IDEditText[11] = IDC_EDIT_TEXT12;
	
	for ( int nTaxRate = 1; nTaxRate <= MAX_TAX_BANDS; nTaxRate++ )
	{
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo( nTaxRate, TaxRateInfo );
		m_dRate[nTaxRate - 1] = TaxRateInfo.m_dTaxRate;
	}

	m_strType = "Tax Rate";
	m_nMaxLen = 8;
	m_nDPQty = 4;

	m_bWarnCancelAfterDownload = FALSE;
}

/**********************************************************************/

void CEditRatesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditRatesDlg)
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_BUTTON_DOWNLOAD, m_buttonDownload);

	for ( int n = 0; n < MAX_TAX_BANDS; n++ )
	{
		DDX_Control(pDX, m_IDEditText[n], m_editText[n] );
		DDX_Text(pDX, m_IDEditRate[n], m_dRate[n] );
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CEditRatesDlg, CDialog)
	//{{AFX_MSG_MAP(CEditRatesDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, OnButtonDownload)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CEditRatesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (PasswordArray.GetEnable(PasswordTicks::CommsDownloads) == FALSE)
	{
		m_buttonDownload.ShowWindow(SW_HIDE);
	}

	CString strTitle = EcrmanOptions.GetTaxString("%T Rates ");
	strTitle += dbDatabase.GetDatabaseTitle(DataManager.GetActiveDbIdx(), EcrmanOptions.GetGlobalTaxFlag());
	SetWindowText(strTitle);

	for (int n = 0; n < MAX_TAX_BANDS; n++)
	{
		SubclassEdit(m_IDEditRate[n], SS_NUM_DP, 7, "%.4f");

		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo(n + 1, TaxRateInfo);
		m_editText[n].LimitText(20);
		m_editText[n].SetWindowText(TaxRateInfo.m_strReportText);
	}

	return TRUE;
}

/**********************************************************************/

void CEditRatesDlg::OnButtonDownload()
{
	if (UpdateData(TRUE) == FALSE)
	{
		return;
	}

	CTaxArray TaxArrayBuffer;

	for (int nTaxRate = 1; nTaxRate <= MAX_TAX_BANDS; nTaxRate++)
	{
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo(nTaxRate, TaxRateInfo);
		TaxArrayBuffer.SetTaxRateInfo(nTaxRate, TaxRateInfo);
	}

	for (int nTaxRate = 1; nTaxRate <= MAX_TAX_BANDS; nTaxRate++)
	{
		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo(nTaxRate, TaxRateInfo);
		TaxRateInfo.m_dTaxRate = m_dRate[nTaxRate - 1];
		DataManager.TaxRates.SetTaxRateInfo(nTaxRate, TaxRateInfo);
	}

	int nDbIdx;
	if (EcrmanOptions.GetGlobalTaxFlag())
	{
		nDbIdx = -1;
	}
	else
	{
		nDbIdx = DataManager.GetActiveDbIdx();
	}

	bool bBlocked = FALSE;
	CArray<int, int> arrayLocIdx;
	dbNetwork.GetDatabaseCommsLocArray(nDbIdx, arrayLocIdx, bBlocked);

	if (arrayLocIdx.GetSize() == 0)
	{
		if (FALSE == bBlocked)
			Prompter.Error("There are no locations available for\ndownload on this database.");
		else
			Prompter.Error("You are not authorised to download files\nfrom this database.");

		return;
	}

	HandleDownload(nDbIdx, arrayLocIdx);

	for (int nTaxRate = 1; nTaxRate <= MAX_TAX_BANDS; nTaxRate++)
	{
		CTaxRateInfo TaxRateInfo;
		TaxArrayBuffer.GetTaxRateInfo(nTaxRate, TaxRateInfo);
		DataManager.TaxRates.SetTaxRateInfo(nTaxRate, TaxRateInfo);
	}
}

/**********************************************************************/

void CEditRatesDlg::HandleDownload( int nDbIdx, CArray<int,int>& arrayLocIdx )
{
	if ( CSelectMultipleLocationDlg::FilterLocListForDownload( arrayLocIdx, EcrmanOptions.GetTaxString( "Download %T Rates" ), this ) == TRUE )
	{
		CEposTaskDatabase TaskDatabase;
		TaskDatabase.CreateSimpleTask( DN_TAX, arrayLocIdx );

		CString strDate = "";
		
		{
			CEposTaskHeader TaskHeader;
			TaskDatabase.GetEditableTaskHeader( TaskHeader );
			
			TaskHeader.SetName( EcrmanOptions.GetTaxString ( "Download %T Rates" ) );

			CString strTitle;
			strTitle.Format ( "Epos Import Date (%s)", TaskHeader.GetName() );

			CDownloadDateDlg dlgDate( strTitle, this );
			if ( dlgDate.RunIfNeeded() == FALSE )
				return;

			if ( dlgDate.m_bImmediate == TRUE )
				TaskHeader.SetImportType( IMPORT_IMMEDIATE );
			else
			{
				TaskHeader.SetImportType( IMPORT_SPECIFIED );
				strDate = dlgDate.m_strDate;
			}

			TaskDatabase.SetEditableTaskHeader( TaskHeader );
		}

		CKeyboardMap KeyboardMap;
		CCommsBatchDlg dlgComms ( TaskDatabase, strDate, TRUE, KeyboardMap, -1, 0, 0, this );
		dlgComms.DoModal();
		m_bWarnCancelAfterDownload = TRUE;
	}
}

/**********************************************************************/

void CEditRatesDlg::OnCancel() 
{
	if (CListDataDlg::CheckBeforeCancel(m_bWarnCancelAfterDownload, TRUE) == TRUE)
	{
		EndDialog(IDOK);
	}
}

/**********************************************************************/

void CEditRatesDlg::OnOK()
{
	if (UpdateData(TRUE) == FALSE)
	{
		return;
	}

	for (int nTaxRate = 1; nTaxRate <= MAX_TAX_BANDS; nTaxRate++)
	{
		CString strText;
		m_editText[nTaxRate - 1].GetWindowText(strText);

		CTaxRateInfo TaxRateInfo;
		TaxRateInfo.m_dTaxRate = m_dRate[nTaxRate - 1];
		TaxRateInfo.m_strReportText = strText;
		DataManager.TaxRates.SetTaxRateInfo(nTaxRate, TaxRateInfo);
	}

	EndDialog(IDOK);
}

/**********************************************************************/
