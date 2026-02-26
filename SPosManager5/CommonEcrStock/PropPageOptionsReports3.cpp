/**********************************************************************/
#include "..\SPosStockManager5\SiteCSVArray.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
/**********************************************************************/
#include "ListDataPrinterSelectDlg.h"
#include "LocationSetCSVArray.h"
#include "PCOptions.h"
#include "ReportpointCSVArray.h"
#include "SysInfo.h"
/**********************************************************************/
#include "winspool.h"
/**********************************************************************/
#include "PropPageOptionsReports3.h"
/**********************************************************************/

CPropPageOptionsReports3::CPropPageOptionsReports3() : CSSPropertyPage(CPropPageOptionsReports3::IDD)
{
	//{{AFX_DATA_INIT(CPropPageOptionsReports3)
	//}}AFX_DATA_INIT
	m_strSystemDefaultPrinterClient = "<Use System Default>";
	m_strSystemDefaultPrinterHost = "<Use System Default>";
}

/**********************************************************************/

CPropPageOptionsReports3::~CPropPageOptionsReports3()
{
}

/**********************************************************************/

void CPropPageOptionsReports3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageOptionsReports3)
	
	DDX_Control(pDX, IDC_COMBO_SYSTEM_DPQTY, m_comboQuantity);
	DDX_Control(pDX, IDC_COMBO_SYSTEM_DPVALUE, m_comboValue);
	DDX_Control(pDX, IDC_COMBO_SORT, m_comboSort);
	DDX_Control(pDX, IDC_COMBO_SALESTAX, m_comboSalesTaxType);
	
	DDX_Control(pDX, IDC_CHECK_REPORTPOINTS, m_checkReportpoints);
	DDX_Control(pDX, IDC_CHECK_LOCATIONSETS, m_checkLocationSets);
	DDX_Control(pDX, IDC_STATIC_SITES, m_staticSites);
	DDX_Control(pDX, IDC_CHECK_SITES, m_checkSites);
	DDX_Control(pDX, IDC_CHECK_REGION, m_checkRegion);
	DDX_Control(pDX, IDC_CHECK_HIDEPLUZERO, m_checkHidePluZeros);
	DDX_Control(pDX, IDC_CHECK_HIDESTOCKZERO, m_checkHideStockZeros);
	DDX_Control(pDX, IDC_CHECK_CASHRSPTEXT, m_checkCashRSPText);
	DDX_Control(pDX, IDC_CHECK_CASHRSPPENNY, m_checkCashRSPDiscountPenny);

	DDX_Control(pDX, IDC_STATIC_PRINTERBOX, m_staticPrinterBox);
	DDX_Control(pDX, IDC_EDIT_PRINTER_CLIENT, m_editPrinterClient);	
	DDX_Control(pDX, IDC_EDIT_PRINTER_HOST, m_editPrinterHost);

	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageOptionsReports3, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageOptionsReports3)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE( IDC_COMBO_SALESTAX, OnSelectTaxType )
	ON_BN_CLICKED(IDC_BUTTON_PRINTER_CLIENT, OnButtonPrinterClient)
	ON_BN_CLICKED(IDC_BUTTON_PRINTER_HOST, OnButtonPrinterHost)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageOptionsReports3::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	SetOptionTicks();

	m_comboQuantity.AddString("0.");
	m_comboQuantity.AddString("0.0");
	m_comboQuantity.AddString("0.00");
	m_comboQuantity.AddString("0.000");

	m_comboValue.AddString("0.");
	m_comboValue.AddString("0.0");
	m_comboValue.AddString("0.00");
	m_comboValue.AddString("0.000");

	m_comboSort.AddString("Sorted by Database");
	m_comboSort.AddString("Sorted by Database then by Name");
	m_comboSort.AddString("Sorted by Name");

	m_comboSalesTaxType.AddString("VAT");
	m_comboSalesTaxType.AddString("GST");
	m_comboSalesTaxType.AddString("Tax");

#ifndef STOCKMAN_SYSTEM
	m_checkHideStockZeros.ShowWindow(SW_HIDE);
	m_staticSites.ShowWindow(SW_HIDE);
	m_checkSites.ShowWindow(SW_HIDE);
#endif

	// Create tooltip control
	m_ToolTip.Create(this, TTS_ALWAYSTIP);
	m_ToolTip.SetMaxTipWidth(500);
	m_ToolTip.Activate(TRUE);

	// Add tooltips for static controls
	CWnd* pPrinterClient = GetDlgItem(IDC_STATIC_PRINTERCLIENT);
	CWnd* pPrinterHost = GetDlgItem(IDC_STATIC_PRINTERHOST);
	CWnd* pEditPrinterHost = GetDlgItem(IDC_EDIT_PRINTER_HOST);
	CWnd* pEditPrinterClient = GetDlgItem(IDC_EDIT_PRINTER_CLIENT);

	{
		CString strTip = "";
		strTip += "Client usually refers to the PC that you are sat in front of. This may be a standalone ";
		strTip += "device or the Client for an RDP connection. If you are running over RDP in admin mode ";
		strTip += "then Client refers to the RDP Host PC, but will only be used for print jobs that are ";
		strTip += "initiated manually by direct use of Ecr Manager or Stock Manager on the Host PC, or ";		
		strTip += "by subsequent RDP use in admin mode. ";
		
		if (pPrinterClient)
		{
			m_ToolTip.AddTool(pPrinterClient, strTip);
		}

		if (pEditPrinterClient)
		{
			m_ToolTip.AddTool(pEditPrinterClient, strTip);
		}
	}
	
	{
		CString strTip = "";
		strTip += "Host refers to the PC on which the application is actually running. This may be a ";
		strTip += "standalone device or the Host for an RDP connection. Host settings are used when ";
		strTip += "printer jobs are initiated automatically by the PosTray module. ";

		if (pPrinterHost)
		{
			m_ToolTip.AddTool(pPrinterHost, strTip);
		}

		if (pEditPrinterHost)
		{
			m_ToolTip.AddTool(pEditPrinterHost, strTip);
		}
	}

	SetRecordControls();
	return TRUE;
}

/**********************************************************************/

BOOL CPropPageOptionsReports3::PreTranslateMessage(MSG* pMsg)
{
	if (m_ToolTip.GetSafeHwnd())
	{
		m_ToolTip.RelayEvent(pMsg);
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

/**********************************************************************/

void CPropPageOptionsReports3::OnSelectTaxType()
{
	int nTemp = EcrmanOptions.GetReportsSalesTaxType();
	EcrmanOptions.SetReportsSalesTaxType( m_comboSalesTaxType.GetCurSel() );
	EcrmanOptions.SetReportsSalesTaxType( nTemp );
}

/**********************************************************************/

BOOL CPropPageOptionsReports3::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	bResult &= SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageOptionsReports3::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetOptionTicks();
	return bResult;
}

/**********************************************************************/

void CPropPageOptionsReports3::SetOptionTicks()
{
	m_checkHidePluZeros.SetCheck( EcrmanOptions.GetReportsTrimPluZerosFlag() );
	m_checkReportpoints.SetCheck( ( EcrmanOptions.GetReportsReportpointFlag() ) || ( dbReportpoint.GetSize() != 0 ) );
	m_checkLocationSets.SetCheck( ( EcrmanOptions.GetReportsLocationSetFlag() ) || ( dbLocationSet.GetSize() != 0 ) );
	m_checkRegion.SetCheck ( EcrmanOptions.GetReportsRegionDateRangeFlag() );
	m_checkCashRSPText.SetCheck( EcrmanOptions.GetReportsCashRSPItemTextFlag() );
	m_checkCashRSPDiscountPenny.SetCheck( EcrmanOptions.GetReportsCashRSPDiscountPennyFlag() );
#ifdef STOCKMAN_SYSTEM
	m_checkSites.SetCheck ( ( StockOptions.GetReportsSiteFlag() ) || ( dbSite.GetSize() != 0 ) );
	m_checkHideStockZeros.SetCheck( StockOptions.GetReportsTrimStockZerosFlag() );
#endif

#ifndef STOCKMAN_SYSTEM

	ShowAndEnableWindow( &m_staticSites, FALSE );
	ShowAndEnableWindow( &m_checkSites, FALSE );
	ShowAndEnableWindow( &m_checkHideStockZeros, FALSE );
	
#else

	ShowAndEnableWindow( &m_staticSites, TRUE );
	ShowAndEnableWindow( &m_checkSites, TRUE );
	
#endif
}

/**********************************************************************/

bool CPropPageOptionsReports3::SaveRecord()
{
	int nQty = m_comboQuantity.GetCurSel();
	if ( nQty < 0 || nQty > 3 ) nQty = 0;
	SysInfo.SetDPQty ( nQty );
	
	int nVal = m_comboValue.GetCurSel();
	if ( nVal < 0 || nVal > 3 ) nVal = 0;
	SysInfo.SetDPValue ( nVal );
	
	int nSort = m_comboSort.GetCurSel();
	if ( nSort < 0 || nSort > 2 ) nSort = 0;
	EcrmanOptions.SetReportsLocationSortType( nSort );

	int nTaxType = m_comboSalesTaxType.GetCurSel();
	if ( nTaxType < 0 || nTaxType > 2 ) nTaxType = 0;
	EcrmanOptions.SetReportsSalesTaxType( nTaxType );

	EcrmanOptions.SetReportsCashRSPItemTextFlag( IsTicked( m_checkCashRSPText ) );
	EcrmanOptions.SetReportsCashRSPDiscountPennyFlag( IsTicked( m_checkCashRSPDiscountPenny ) );
	EcrmanOptions.SetReportsReportpointFlag( IsTicked ( m_checkReportpoints ) );
	EcrmanOptions.SetReportsLocationSetFlag( IsTicked ( m_checkLocationSets ) );
	EcrmanOptions.SetReportsTrimPluZerosFlag( IsTicked ( m_checkHidePluZeros) );
	EcrmanOptions.SetReportsRegionDateRangeFlag( IsTicked( m_checkRegion ) );
	
#ifdef STOCKMAN_SYSTEM
	StockOptions.SetReportsSiteFlag ( IsTicked ( m_checkSites ) );
	StockOptions.SetReportsTrimStockZerosFlag (IsTicked ( m_checkHideStockZeros ) );
#endif

	{
		CString strPrinter = GetEditBoxText(m_editPrinterClient);

		if (strPrinter == m_strSystemDefaultPrinterClient)
		{
			PCOptionsClient.SetDefaultReportPrinter("");
		}
		else
		{
			PCOptionsClient.SetDefaultReportPrinter(strPrinter);
		}
	}

	{
		CString strPrinter = GetEditBoxText(m_editPrinterHost);

		if (strPrinter == m_strSystemDefaultPrinterHost)
		{
			PCOptionsHost.SetDefaultReportPrinter("");
		}
		else
		{
			PCOptionsHost.SetDefaultReportPrinter(strPrinter);
		}
	}

	return TRUE;
}

/**********************************************************************/

void CPropPageOptionsReports3::SetRecordControls()
{
	int nQty = SysInfo.GetDPQty();
	if ( nQty < 0 || nQty > 3 ) nQty = 0;
	m_comboQuantity.SetCurSel ( nQty );
	
	int nVal = SysInfo.GetDPValue();
	if ( nVal < 0 || nVal > 3 ) nVal = 0;
	m_comboValue.SetCurSel ( nVal );
	
	int nSort = EcrmanOptions.GetReportsLocationSortType();
	if ( nSort < 0 || nSort > 2 ) nSort = 0;
	m_comboSort.SetCurSel(nSort);

	int nTaxType = EcrmanOptions.GetReportsSalesTaxType();
	if ( nTaxType < 0 || nTaxType > 2 ) nTaxType = 0;
	m_comboSalesTaxType.SetCurSel(nTaxType);

	m_checkReportpoints.EnableWindow ( dbReportpoint.GetSize() == 0 );
	m_checkLocationSets.EnableWindow ( dbLocationSet.GetSize() == 0 );

#ifdef STOCKMAN_SYSTEM
	m_checkSites.EnableWindow ( dbSite.GetSize() == 0 );
#endif STOCKMAN_SYSTEM

	if ( SysInfo.IsEcrLiteSystem() == TRUE )
	{
		m_checkRegion.ShowWindow( SW_HIDE );
	}

	int nPCIdHost = Sysset.GetPCIdHost();
	int nPCIdClient = Sysset.GetPCIdClient();

	if ( nPCIdHost == nPCIdClient )
	{
		m_staticPrinterBox.SetWindowText("Default Printer (Client and Host refer to this PC)");
	}
	else
	{
		m_staticPrinterBox.SetWindowText("Default Printer (Client and Host are Separate PCs)");
	}

	{
		CString strPrinter = PCOptionsClient.GetDefaultReportPrinter();

		if (strPrinter != "")
		{
			m_editPrinterClient.SetWindowText(strPrinter);
		}
		else
		{
			m_editPrinterClient.SetWindowText(m_strSystemDefaultPrinterClient);
		}
	}

	{
		CString strPrinter = PCOptionsHost.GetDefaultReportPrinter();

		if (strPrinter != "")
		{
			m_editPrinterHost.SetWindowText(strPrinter);
		}
		else
		{
			m_editPrinterHost.SetWindowText(m_strSystemDefaultPrinterHost);
		}
	}
}

/**********************************************************************/

void CPropPageOptionsReports3::PrinterListError( int n )
{
	CString strError;
	strError.Format ( "Unable to create Windows printer list.\n\n(Error code %d)", n );
	Prompter.Error( strError );
}

/**********************************************************************/

void CPropPageOptionsReports3::OnButtonPrinterClient() 
{
	ShowPrinterList(this, m_editPrinterClient,m_strSystemDefaultPrinterClient, "Reports" );
}

/**********************************************************************/

void CPropPageOptionsReports3::OnButtonPrinterHost()
{
	ShowPrinterList(this, m_editPrinterHost, m_strSystemDefaultPrinterHost, "Reports");
}

/**********************************************************************/

void CPropPageOptionsReports3::ShowPrinterList( CSSPropertyPage* pParentWnd, CEdit& EditPrinter, CString& strDefaultPrinter, const char* szAction ) 
{
	PRINTER_INFO_2* pListOfPrinters;
    DWORD dwPrinterCount = 0;
    DWORD dwBufferSize = 0;
    DWORD dwEnumLevel = 2;
    
    EnumPrinters( PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS, NULL, dwEnumLevel, NULL, 0, &dwBufferSize, &dwPrinterCount );
	
	if ( dwBufferSize <= 0 )
	{
		PrinterListError(1);
		return;
	}

    if ( ( pListOfPrinters = (PRINTER_INFO_2*) malloc( dwBufferSize ) ) == 0 )
	{
		PrinterListError(2);
		return;
	}
   
    if (!EnumPrinters(PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS , NULL, dwEnumLevel, (LPBYTE)pListOfPrinters, dwBufferSize, &dwBufferSize, &dwPrinterCount))
    {
        free( pListOfPrinters );
		PrinterListError(3);
        return;
    }

	if ( dwPrinterCount <= 0 )
	{ 
		free( pListOfPrinters );
		PrinterListError(4);
        return;
	}
    
    CReportConsolidationArray<CSortedStringItem> arrayPrinters;

    for ( int i = 0; i < (int) dwPrinterCount; i++ )
    {	
		CSortedStringItem item;
		item.m_strItem = pListOfPrinters[i].pPrinterName;
		arrayPrinters.Consolidate(item);
	}

	free( pListOfPrinters );

	CString strTitle = "";
	strTitle += "Select Default Printer for ";
	strTitle += szAction;

	CListDataPrinterSelectDlg dlgSelect( strTitle, arrayPrinters, pParentWnd );

	CSortedStringItem item;
	item.m_strItem = pParentWnd -> GetEditBoxText( EditPrinter );
	
	if ( item.m_strItem == strDefaultPrinter )
	{
		dlgSelect.m_nSelection = 0;
	}
	if ( arrayPrinters.Find( item, dlgSelect.m_nSelection ) == FALSE )
	{
		dlgSelect.m_nSelection = 0;
	}
	else
	{
		dlgSelect.m_nSelection++;
	}

	//ADD TO START OF LIST, IGNORING SORTING
	item.m_strItem = strDefaultPrinter;
	arrayPrinters.InsertAt( 0, item );

	if ( dlgSelect.DoModal() == IDOK )
	{
		if ( ( dlgSelect.m_nSelection >= 0 ) && ( dlgSelect.m_nSelection < arrayPrinters.GetSize() ) )
		{
			CSortedStringItem item;
		
			if ( 0 == dlgSelect.m_nSelection )
			{
				item.m_strItem = strDefaultPrinter;
			}
			else
			{
				arrayPrinters.GetAt( dlgSelect.m_nSelection, item );
			}

			EditPrinter.SetWindowText( item.m_strItem );
		}
	}
}

/**********************************************************************/


