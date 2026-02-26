//$$******************************************************************
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
//$$******************************************************************
#include "ListDataPrinterSelectDlg.h"
#include "PCOptions.h"
#include "SetupAlertTextsDlg.h"
#include "SetupAuditPeriodsDlg.h"
#include "SetupPhotoIDDlg.h"
#include "SetupUserTextsDlg.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "winspool.h"
//$$******************************************************************
#include "SystemPropertyPage4.h"
//$$******************************************************************

CSystemPropertyPage4::CSystemPropertyPage4() : CSSPropertyPage(CSystemPropertyPage4::IDD)
{
	m_strSystemDefaultPrinter = "<Use System Default>";
}

//$$******************************************************************

CSystemPropertyPage4::~CSystemPropertyPage4()
{
}

//$$******************************************************************

void CSystemPropertyPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GENERAL, m_staticGeneral);
	DDX_Control(pDX, IDC_COMBO_STARTOFWEEK, m_comboStartOfWeek);
	DDX_Control(pDX, IDC_BUTTON_PHOTOID, m_buttonPhotoID);
	DDX_Control(pDX, IDC_CHECK_ENABLEPHOTOID, m_checkEnablePhotoID);
	DDX_Control(pDX, IDC_CHECK_SHOWPURCHASES, m_checkShowPurchases);
	DDX_Control(pDX, IDC_COMBO_FIELDNAMES, m_comboSetFieldNames);
	DDX_Control(pDX, IDC_EDIT_PRINTER_REPORT, m_editPrinterReport);
	DDX_Control(pDX, IDC_STATIC_BARCODE1, m_staticBarcode1);
	DDX_Control(pDX, IDC_STATIC_BARCODE2, m_staticBarcode2);
	DDX_Control(pDX, IDC_EDIT_PRINTER_BARCODE, m_editPrinterBarcode);
	DDX_Control(pDX, IDC_BUTTON_PRINTER_BARCODE, m_buttonPrinterBarcode);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CSystemPropertyPage4, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_PHOTOID, OnButtonPhotoID)
	ON_BN_CLICKED(IDC_CHECK_ENABLEPHOTOID, OnTogglePhotoID)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_PRINTER_REPORT, OnButtonPrinterReport)
	ON_BN_CLICKED(IDC_BUTTON_PRINTER_BARCODE, OnButtonPrinterBarcode)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CSystemPropertyPage4::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_checkEnablePhotoID.SetCheck(System.GetEnablePhotoIDListFlag());
	m_checkShowPurchases.SetCheck(System.GetShowAuditPurchasesFlag());

	CStandardCombos::FillStartOfWeekCombo(m_comboStartOfWeek, System.GetStartOfWeek() - 1);
	CStandardCombos::FillFieldNamesCombo(m_comboSetFieldNames, 0);

	SetDisplay();

	{
		CString strPrinter = PCOptionsClient.GetDefaultReportPrinter();

		if (strPrinter != "")
		{
			m_editPrinterReport.SetWindowText(strPrinter);
		}
		else
		{
			m_editPrinterReport.SetWindowText(m_strSystemDefaultPrinter);
		}
	}

	{
		CString strPrinter = PCOptionsClient.GetDefaultBarcodePrinter();

		if (strPrinter != "")
		{
			m_editPrinterBarcode.SetWindowText(strPrinter);
		}
		else
		{
			m_editPrinterBarcode.SetWindowText(m_strSystemDefaultPrinter);
		}
	}

	return TRUE;
}

//$$******************************************************************

BOOL CSystemPropertyPage4::OnKillActive()
{
	System.SetStartOfWeek(m_comboStartOfWeek.GetCurSel() + 1);
	System.SetEnablePhotoIDListFlag(IsTicked(m_checkEnablePhotoID));
	System.SetShowAuditPurchasesFlag(IsTicked(m_checkShowPurchases));

	{
		CString strPrinter = GetEditBoxText(m_editPrinterReport);

		if (strPrinter == m_strSystemDefaultPrinter)
		{
			PCOptionsClient.SetDefaultReportPrinter("");
		}
		else
		{
			PCOptionsClient.SetDefaultReportPrinter(strPrinter);
		}
	}

	{
		CString strPrinter = GetEditBoxText(m_editPrinterBarcode);

		if (strPrinter == m_strSystemDefaultPrinter)
		{
			PCOptionsClient.SetDefaultBarcodePrinter("");
		}
		else
		{
			PCOptionsClient	.SetDefaultBarcodePrinter(strPrinter);
		}
	}

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************
void CSystemPropertyPage4::OnTogglePhotoID()	{ SetDisplay(); }
//$$******************************************************************

void CSystemPropertyPage4::SetDisplay()
{
	if (System.HaveBarman() == FALSE)
	{
		m_staticBarcode1.ShowWindow(SW_HIDE);
		m_staticBarcode2.ShowWindow(SW_HIDE);
		m_editPrinterBarcode.ShowWindow(SW_HIDE);
		m_buttonPrinterBarcode.ShowWindow(SW_HIDE);
		m_checkEnablePhotoID.ShowWindow(SW_HIDE);
		m_buttonPhotoID.ShowWindow(SW_HIDE);
		ResizeControl(&m_staticGeneral, 466, 256);
	}
	else
	{
		m_buttonPhotoID.EnableWindow(IsTicked(m_checkEnablePhotoID));
	}
}

//$$******************************************************************

void CSystemPropertyPage4::OnButtonPhotoID()
{
	CSetupPhotoIDDlg dlg{};
	dlg.DoModal();
}

//$$******************************************************************

void CSystemPropertyPage4::OnButtonSet()
{
	switch( m_comboSetFieldNames.GetCurSel())
	{ 
	case 0:
		{
			CSetupUserTextsDlg dlg;
			dlg.DoModal();
		}
		break;

	case 1:
		{
			CSetupAuditPeriodsDlg dlg;
			dlg.DoModal();
		}
		break;

	case 2:
		{
			CSetupAlertTextsDlg dlg;
			dlg.DoModal();
		}
		break;
	}
}

//$$******************************************************************

void CSystemPropertyPage4::PrinterListError( int n )
{
	CString strError;
	strError.Format ( "Unable to create Windows printer list.\n\n(Error code %d)", n );
	Prompter.Error( strError );
}

//$$******************************************************************

void CSystemPropertyPage4::OnButtonPrinterReport() 
{
	ShowPrinterList(this, m_editPrinterReport,m_strSystemDefaultPrinter, "Reports" );
}

//$$******************************************************************

void CSystemPropertyPage4::OnButtonPrinterBarcode()
{
	ShowPrinterList(this, m_editPrinterBarcode, m_strSystemDefaultPrinter, "Photo IDs");
}

//$$******************************************************************

void CSystemPropertyPage4::ShowPrinterList( CSSPropertyPage* pParentWnd, CEdit& EditPrinter, CString& strDefaultPrinter, const char* szAction ) 
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

//$$******************************************************************
