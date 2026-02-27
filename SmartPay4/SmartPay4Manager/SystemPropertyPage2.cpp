//$$******************************************************************
#include "AutoShutdownStacker.h"
#include "EmailAlertPropertySheet.h"
#include "ListDataPrinterSelectDlg.h"
#include "PCOptions.h"
#include "SetupEODTextsDlg.h"
#include "SetupFieldsDlg.h"
#include "SetupPhotoIDDlg.h"
#include "SetupUserTextsDlg.h"
#include "StandardCombos.h"
//$$******************************************************************
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
//$$******************************************************************
#include "winspool.h"
//$$******************************************************************
#include "SystemPropertyPage2.h"
//$$******************************************************************

CSystemPropertyPage2::CSystemPropertyPage2()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CSystemPropertyPage2::IDD)
{
	m_bRestartServerReqd = FALSE;
	m_strSystemDefaultPrinter = "<Use System Default>";
}

//$$******************************************************************

void CSystemPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STARTOFWEEK, m_comboStartofWeek);
	DDX_Control(pDX, IDC_COMBO_FIELDNAMES, m_comboFieldNames);
	DDX_Control(pDX, IDC_CHECK_ENABLEPHOTOID, m_checkEnablePhotoID);
	DDX_Control(pDX, IDC_BUTTON_PHOTOID, m_buttonPhotoID);
	DDX_Control(pDX, IDC_CHECK_LEAVERSDATABASE, m_checkLeavers);
	DDX_Control(pDX, IDC_CHECK_SHOWPURCHASES, m_checkPurchaseTexts);
	DDX_Control(pDX, IDC_EDIT_ENDTEXT, m_editEndText);
	/*****/
	DDX_Control(pDX, IDC_EDIT_ECRMANPATH, m_editEcrmanPath);
	/*****/
	DDX_Control(pDX, IDC_EDIT_PRINTER, m_editPrinter);
}
	
//$$******************************************************************

BEGIN_MESSAGE_MAP(CSystemPropertyPage2, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_FIELDNAMES, OnButtonFieldNames)
	ON_BN_CLICKED(IDC_CHECK_ENABLEPHOTOID, OnTogglePhotoId)
	ON_BN_CLICKED(IDC_BUTTON_PHOTOID, OnButtonPhotoId)
	ON_BN_CLICKED(IDC_BUTTON_EMAILFUNCTIONS, OnButtonEmailFunctions)
	/*****/
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	/*****/
	ON_BN_CLICKED(IDC_BUTTON_PRINTER, OnButtonPrinter)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CSystemPropertyPage2::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_comboFieldNames.SetCurSel(0);

	CStandardCombos::FillWeekdayCombo(m_comboStartofWeek, System.GetStartOfWeek() - 1);
	CStandardCombos::FillFieldNamesCombo(m_comboFieldNames, 0);

	m_checkEnablePhotoID.SetCheck(System.GetEnablePhotoIDListFlag());
	m_checkLeavers.SetCheck(System.GetKeepLeaversDatabaseFlag());
	m_checkPurchaseTexts.SetCheck(System.GetShowPurchaseTextsFlag());
	
	m_editEcrmanPath.SetWindowText(System.GetEcrmanPath());

	SetDisplay();

	CString strPrinter = PCOptionsClient.GetDefaultReportPrinter();

	if (strPrinter != "")
	{
		m_editPrinter.SetWindowText(strPrinter);
	}
	else
	{
		m_editPrinter.SetWindowText(m_strSystemDefaultPrinter);
	}

	{
		int nEndTextLines = 0;
		CString strEndText = "";

		for (int n = 1; n <= REPORT_ENDTEXT_COUNT; n++)
		{
			CString strText = System.GetReportEndText(n);
			::TrimSpacesFromString(strText, FALSE);

			if (strText != "")
			{
				while (nEndTextLines < n - 1)
				{
					strEndText += "\r\n";
					nEndTextLines++;
				}

				strEndText += strText;
				strEndText += "\r\n";
				nEndTextLines++;
			}
		}

		m_editEndText.SetWindowText(strEndText);
	}

	return TRUE;
}

//$$******************************************************************

void CSystemPropertyPage2::OnButtonFieldNames()
{
	CAutoShutdownPropSheetStacker Stacker((CSSAutoShutdownPropertySheet*)GetParent());

	switch( m_comboFieldNames.GetCurSel() )
	{
	case 0:
		{
			CSetupUserTextsDlg dlg;
			dlg.DoModal();
		}
		break;

	case 1:
		{
			CSetupEODTextsDlg dlg ( this );
			dlg.DoModal();
		}
		break;

	case 2:
		{
			CSetupAuditPeriodsDlg dlg;
			if ( dlg.DoModal() == IDOK )
				m_bRestartServerReqd = TRUE;
		}
		break;

	case 3:
		{
			CSetupFieldsDlg dlg ( Filenames.GetAlertFilename(), "Setup Alert Texts", "Alert" );
			dlg.DoModal();
		}
		break;
	}
}

//$$******************************************************************

void CSystemPropertyPage2::OnTogglePhotoId() 
{ 
	SetDisplay(); 
}

//$$******************************************************************

void CSystemPropertyPage2::SetDisplay()
{
	m_buttonPhotoID.EnableWindow(m_checkEnablePhotoID.GetCheck() != 0);
}

//$$******************************************************************

void CSystemPropertyPage2::OnButtonPhotoId()
{
	CAutoShutdownPropSheetStacker Stacker((CSSAutoShutdownPropertySheet*)GetParent());
	CSetupPhotoIDDlg dlg;
	dlg.DoModal();
}

//$$******************************************************************

void CSystemPropertyPage2::OnButtonEmailFunctions()
{
	CAutoShutdownPropSheetStacker Stacker((CSSAutoShutdownPropertySheet*)GetParent());

	CEmailAlertPropertySheet propSheet( this );

	if ( propSheet.DoModal() == IDOK )
	{
		propSheet.SaveData();
	}
}

//$$******************************************************************
//$$******************************************************************
//$$******************************************************************

void CSystemPropertyPage2::OnButtonBrowse()
{
	CAutoShutdownPropSheetStacker Stacker((CSSAutoShutdownPropertySheet*)GetParent());

	CString strPathname;
	if ( DoBrowseFolder ( strPathname, "Locate associated Ecr\\Stock Manager Pnn data folder",  NULL, this ) == TRUE )
	{
		m_editEcrmanPath.SetWindowText(strPathname);	
	}
}

//$$******************************************************************

void CSystemPropertyPage2::OnButtonTest()
{
	CString strMsg;

	CString strEcrmanPath;
	m_editEcrmanPath.GetWindowText(strEcrmanPath);

	if (::ExistSubdirectory(strEcrmanPath) == FALSE)
	{
		strMsg.Format("Unable to access folder ' %s '!",
			(const char*)strEcrmanPath);
	}
	else
	{
		CString strSysFileFolder = "";
		strSysFileFolder.Format("%s\\SysFiles",
			(const char*)strEcrmanPath);

		if (::ExistSubdirectory(strSysFileFolder) == FALSE)
		{
			strMsg = "No ' SysFiles ' folder found in specified Ecr\\Stock Manager folder!";
		}
		else
		{
			strMsg = "Path to Ecr\\Stock Manager data is valid.";
		}
	}

	Prompter.Info(strMsg, "Validation Check");
}

//$$******************************************************************

void CSystemPropertyPage2::PrinterListError( int n )
{
	CString strError = "";
	strError.Format ( "Unable to create Windows printer list.\n\n(Error code %d)", n );
	Prompter.Error( strError );
}

//$$******************************************************************

void CSystemPropertyPage2::OnButtonPrinter() 
{
	ShowPrinterList(this, m_editPrinter,m_strSystemDefaultPrinter, "Reports" );
}

//$$******************************************************************

void CSystemPropertyPage2::ShowPrinterList( CSSPropertyPage* pParentWnd, CEdit& EditPrinter, CString& strDefaultPrinter, const char* szAction ) 
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

BOOL CSystemPropertyPage2::OnKillActive()
{
	CString strEcrmanPath;
	m_editEcrmanPath.GetWindowText(strEcrmanPath);

	System.SetEnablePhotoIDListFlag(IsTicked(m_checkEnablePhotoID));
	System.SetStartOfWeek(m_comboStartofWeek.GetCurSel() + 1);
	System.SetShowPurchaseTextsFlag(IsTicked(m_checkPurchaseTexts));
	System.SetEcrmanPath(strEcrmanPath);
	System.SetKeepLeaversDatabaseFlag(IsTicked(m_checkLeavers));

	CString strPrinter = GetEditBoxText(m_editPrinter);

	if (strPrinter == m_strSystemDefaultPrinter)
	{
		PCOptionsClient.SetDefaultReportPrinter("");
	}
	else
	{
		PCOptionsClient.SetDefaultReportPrinter(strPrinter);
	}

	{
		CStringArray arrayLines;
		CString strLine = "";

		CString strEndText;
		m_editEndText.GetWindowText(strEndText);

		int nLength = strEndText.GetLength();

		for (int nPos = 0; (nPos < nLength) && (arrayLines.GetSize() < 5); nPos++)
		{
			if (nPos == nLength - 1)
			{
				strLine += strEndText.GetAt(nPos);
			}
			else
			{
				if (strEndText.Mid(nPos, 2) == "\r\n")
				{
					arrayLines.Add(strLine);
					strLine = "";
					nPos++;
				}
				else
				{
					strLine += strEndText.GetAt(nPos);
				}
			}
		}

		if (strLine != "")
		{
			arrayLines.Add(strLine);
		}

		for (int n = 1; n <= REPORT_ENDTEXT_COUNT; n++)
		{
			if (n <= arrayLines.GetSize())
			{
				System.SetReportEndText(n, arrayLines.GetAt(n - 1));
			}
			else
			{
				System.SetReportEndText(n, "");
			}
		}
	}

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

