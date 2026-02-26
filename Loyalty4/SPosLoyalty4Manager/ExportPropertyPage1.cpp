//$$******************************************************************
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\ReportConsolidationArray.h"
#include "..\SPosLoyalty4Shared\SQLNamespaces.h"
#include "..\SPosLoyalty4Shared\UserTexts.h"
//$$******************************************************************
#include "HeaderWarnDlg.h"
#include "LoyaltyManager.h"
//$$******************************************************************
#include "ExportPropertyPage1.h"
//$$******************************************************************

CExportPropertyPage1::CExportPropertyPage1() : CSSPropertyPage(CExportPropertyPage1::IDD)
{
	m_strHeader = "";
	m_bSelectAll = FALSE;
	m_bLockBasicFields = FALSE;
	SetTranslateTable();
}

//$$******************************************************************

CExportPropertyPage1::~CExportPropertyPage1()
{
}

//$$******************************************************************

void CExportPropertyPage1::SetPointer ( CDbExportHandler* pDbExportHandler )
{
	m_pDbExportHandler	= pDbExportHandler;
}

//$$******************************************************************

void CExportPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_editFilename);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC_STATIC_FILENAME, m_staticFilename);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_buttonSelect);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CExportPropertyPage1, CPropertyPage)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CExportPropertyPage1::OnInitDialog()
{
	m_strHeader = m_pDbExportHandler->m_strHeader;

	CPropertyPage::OnInitDialog();

	m_editFilename.SetWindowText(m_pDbExportHandler->m_strFilename);

	CString strDatabaseHeader = "";
	if (System.GetEnablePurse2Flag() == TRUE)
	{
		strDatabaseHeader = LoyaltyHeaderHelpers.GetHeaderFull();
	}
	else
	{
		strDatabaseHeader = LoyaltyHeaderHelpers.GetHeaderNoPurse2();
	}
	
	CCSV csvHeader(strDatabaseHeader);

	for (int n = 0; n < csvHeader.GetSize(); n++)
	{
		CString str = csvHeader.GetString(n);
		str.MakeUpper();
		if (str == "CTS")
		{
			csvHeader.RemoveAt(n);
			break;
		}
	}

	csvHeader.Add(Account::TotalSpendToDate.Label);
	csvHeader.Add(Account::DOBNextAge.Label);			// age next birthday
	csvHeader.Add(Account::DOBDay.Label);				// Next birthday Day of week 

	if (m_pDbExportHandler->m_strLabel.Left(3) == "DEL")	// delete export function
	{
		csvHeader.Add(Account::DeletedDate.Label);		// add special export delete header fields
		csvHeader.Add(Account::DeletedTime.Label);
	}

	bool bLockFirstItem = (m_pDbExportHandler->m_nExportFlag == nDBEXPORTFIELDS_DATA);

	m_listFields.SubclassDlgItem(IDC_LIST_EXPORT, this);
	m_listFields.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listFields.InsertColumn(0, "", LVCFMT_LEFT, 300);
	m_listFields.SetLockItemZeroFlag(bLockFirstItem || m_bLockBasicFields);

	m_bSelectAll = TRUE;

	{
		CReportConsolidationArray<CSortedStringItem> arrayAvailableFields;

		for (int nPos = bLockFirstItem ? 1 : 0; nPos < csvHeader.GetSize(); nPos++)
		{
			CString strField = csvHeader.GetString(nPos);

			if (strField.GetLength() > 0)
			{
				bool bAllowField = TRUE;

				if (TRUE == m_bLockBasicFields)
				{
					CString strTest = strField;
					strTest.MakeUpper();
					bAllowField = ((strTest != "CARDNO" && strTest != "GROUPNO"));
				}

				if (TRUE == bAllowField)
				{
					CSortedStringItem item;
					item.m_strItem = strField;
					arrayAvailableFields.Consolidate(item);
				}
			}
		}

		if (TRUE == bLockFirstItem)
		{
			CSSListTaggedSelectItem itemList;
			itemList.m_strText = GetDisplayText(csvHeader.GetString(0));
			itemList.SetTagState(1);
			m_listFields.AddItemToList(itemList);
		}
		else if (TRUE == m_bLockBasicFields)
		{
			CSSListTaggedSelectItem itemList;
			itemList.m_strText = "SortByField,CardNo,GroupNo";
			itemList.SetTagState(1);
			m_listFields.AddItemToList(itemList);
		}

		CCSV csvTickedFields(m_strHeader);

		for (int nPos = 0; nPos < csvTickedFields.GetSize(); nPos++)
		{
			CSortedStringItem itemCheck;
			itemCheck.m_strItem = csvTickedFields.GetString(nPos);
			LoyaltyHeaderHelpers.UpdateLegacyField(itemCheck.m_strItem);

			int nFindPos;
			if (arrayAvailableFields.Find(itemCheck, nFindPos) == TRUE)
			{
				CSSListTaggedSelectItem itemList;
				itemList.m_strText = GetDisplayText(itemCheck.m_strItem);
				itemList.SetTagState(1);
				m_listFields.AddItemToList(itemList);
				arrayAvailableFields.RemoveAt(nFindPos);
			}
		}

		for (int nPos = 0; nPos < csvHeader.GetSize(); nPos++)
		{
			CString strField = csvHeader.GetString(nPos);

			if (strField.GetLength() > 0)
			{
				CSortedStringItem itemCheck;
				itemCheck.m_strItem = strField;

				int nFindPos;
				if (arrayAvailableFields.Find(itemCheck, nFindPos) == TRUE)
				{
					CSSListTaggedSelectItem itemList;
					itemList.m_strText = GetDisplayText(itemCheck.m_strItem);
					itemList.SetTagState(0);
					m_listFields.AddItemToList(itemList);
					arrayAvailableFields.RemoveAt(nFindPos);
					m_bSelectAll = FALSE;
				}
			}
		}
	}

	SetSelectButtonText();

	// no filename if not export database
	if (m_pDbExportHandler->m_nExportFlag != nDBEXPORTFIELDS_DATA)	
	{
		m_buttonBrowse.ShowWindow(SW_HIDE);

		if (m_pDbExportHandler->IsCustomReport() == TRUE)
		{
			m_staticFilename.SetWindowText("Report name");
		}
		else
		{
			m_editFilename.ShowWindow(SW_HIDE);
			m_staticFilename.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		CString strFilename = GetEditBoxText(m_editFilename);
		if (strFilename == "")
		{
			m_editFilename.SetFocus();
			return FALSE;
		}
	}

	return TRUE;
}

//$$******************************************************************

void CExportPropertyPage1::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST_EXPORT )  
   {
	   m_listFields.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

//$$******************************************************************

BOOL CExportPropertyPage1::OnKillActive()
{
	CreateHeaderLine();
	if (CreateHeaderLine() == 0 && m_pDbExportHandler->m_nExportFlag == nDBEXPORTFIELDS_DATA)
	{
		MessageBeep(MB_ICONEXCLAMATION);
		Prompter.Error("No database fields have been selected", "Field selection required");
		return FALSE;
	}

	CString strFilename = GetEditBoxText(m_editFilename);

	if (strFilename.GetLength() != 0)					// check have filename
	{
		CSSFile file;
		if (file.Open(strFilename, "rb") == TRUE)	// file may not be found
		{
			CString strOldHeader = "";
			file.ReadString(strOldHeader);
			file.Close();

			if (strOldHeader.GetLength() > 0 && strOldHeader != m_strHeader)
			{
				if (m_pDbExportHandler->m_nExportFlag == nDBEXPORTFIELDS_DATA)		// if report doesn't matter if its changed
				{
					MessageBeep(MB_ICONEXCLAMATION);
					
					CHeaderWarnDlg dlg(this);
					dlg.SetInfo1Text("");
					dlg.SetInfo2Text("");
					dlg.SetHeader1(strOldHeader);
					dlg.SetHeader2(m_strHeader);
					dlg.SetHeader1Description("Old Header");
					dlg.SetHeader2Description("New Header");
					dlg.SetAllowCancelFlag(TRUE);

					if (dlg.DoModal() != IDOK)
					{
						return FALSE;
					}
				}

				::SendToRecycleBin(strFilename);		// delete existing file
			}
		}
	}

	m_pDbExportHandler->m_strFilename = strFilename;
	m_pDbExportHandler->m_strHeader = m_strHeader;

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

int CExportPropertyPage1::CreateHeaderLine()
{
	int nStartPos = (m_bLockBasicFields) ? 1 : 0;

	CCSV csv;
	for ( int nPos = nStartPos; nPos < m_listFields.GetItemCount(); nPos++ )
	{
		if ( m_listFields.GetItemTagState( nPos ) == 1 )
		{
			CSSListTaggedSelectItem item;
			m_listFields.GetListItem( nPos, item );
			csv.Add ( GetHeaderText ( item.m_strText ) );
		}
	}

	m_strHeader = csv.GetLine();
	return csv.GetSize();
}

//$$******************************************************************

void CExportPropertyPage1::OnButtonDown() 
{
	int nSel = m_listFields.GetCurSel();

	int nMin = 0;
	if ((m_pDbExportHandler->m_nExportFlag == nDBEXPORTFIELDS_DATA) || (TRUE == m_bLockBasicFields))
	{
		nMin = 1;
	}

	if ( ( nSel >= nMin ) && ( nSel < m_listFields.GetItemCount() - 1 ) )
	{
		m_listFields.SwapItems( nSel, nSel + 1 );
	}
}

//**********************************************************************

void CExportPropertyPage1::OnButtonUp() 
{
	int nMin = 1;
	if ((m_pDbExportHandler->m_nExportFlag == nDBEXPORTFIELDS_DATA) || (TRUE == m_bLockBasicFields))
	{
		nMin = 2;
	}

	int nSel = m_listFields.GetCurSel();
	if ( ( nSel >= nMin ) && ( nSel < m_listFields.GetItemCount() ) )
	{
		m_listFields.SwapItems( nSel, nSel - 1 );
	}
}

//$$******************************************************************

void CExportPropertyPage1::OnButtonSelect() 
{
	m_bSelectAll = !m_bSelectAll;

	if (TRUE == m_bSelectAll)
	{
		m_listFields.SelectAll();
	}
	else
	{
		m_listFields.SelectNone();
	}

	SetSelectButtonText();
	UpdateData ( FALSE );
}

//$$******************************************************************

void CExportPropertyPage1::SetSelectButtonText() 
{
	CString strText = ( m_bSelectAll == FALSE ) ? "Select &All" : "&Clear";
	m_buttonSelect.SetWindowText ( strText );
}

//$$******************************************************************

void CExportPropertyPage1::OnButtonBrowse()
{
	CString strFileFilters = m_pDbExportHandler->m_strFileFilters;
	strFileFilters += "|All Files (*.*)|*.*||";

	CFileDialog dlg ( FALSE, m_pDbExportHandler->m_strDefExtension, NULL, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY|OFN_NOCHANGEDIR, strFileFilters, this );
	dlg.m_ofn.lpstrTitle = "Select Export filename";

	CString strPath = "";
	CString strFilename = GetEditBoxText(m_editFilename);
	::StripFilename ( strFilename, strPath );
	if (strPath == "")
	{
		strPath = ".";
	}

	dlg.m_ofn.lpstrInitialDir = strPath;

	if ( dlg.DoModal() == IDOK )
	{
		strFilename = dlg.GetPathName();
		m_editFilename.SetWindowTextA(strFilename);
	}
}

//$$******************************************************************

void CExportPropertyPage1::CreateDisplayText ( const char* szHeaderText, const char* szUserText )
{
	CString strUserText = szUserText;
	if ( strUserText.GetLength() > 0 )
	{
		if ( strUserText != szHeaderText )
		{
			m_arrayFieldIn.Add ( szHeaderText );

			CString strDisplayText = "";
			strDisplayText.Format ( "%s  { %s }", szHeaderText, szUserText );
			m_arrayFieldOut.Add ( strDisplayText );
		}
	}
}

//$$******************************************************************

void CExportPropertyPage1::SetTranslateTable()
{
	CUserTexts texts;
	texts.Read();

	CreateDisplayText(Account::Address1.Label, texts.GetText(LOYALTYFIELD_ADDRESS1, ""));
	CreateDisplayText(Account::Address2.Label, texts.GetText(LOYALTYFIELD_ADDRESS2, ""));
	CreateDisplayText(Account::Address3.Label, texts.GetText(LOYALTYFIELD_ADDRESS3, ""));
	CreateDisplayText(Account::Address4.Label, texts.GetText(LOYALTYFIELD_ADDRESS4, ""));
	CreateDisplayText(Account::Address5.Label, texts.GetText(LOYALTYFIELD_ADDRESS5, ""));

	CreateDisplayText(Account::Phone1.Label, texts.GetText(LOYALTYFIELD_PHONE1, ""));
	CreateDisplayText(Account::Phone2.Label, texts.GetText(LOYALTYFIELD_PHONE2, ""));

	CreateDisplayText(Account::Info1.Label, texts.GetText(LOYALTYFIELD_INFO1, ""));
	CreateDisplayText(Account::Info2.Label, texts.GetText(LOYALTYFIELD_INFO2, ""));
	CreateDisplayText(Account::Info3.Label, texts.GetText(LOYALTYFIELD_INFO3, ""));
	CreateDisplayText(Account::Info4.Label, texts.GetText(LOYALTYFIELD_INFO4, ""));

	CreateDisplayText(Account::DOB.Label, texts.GetText(LOYALTYFIELD_DOB, ""));
	CreateDisplayText(Account::ExternalRef.Label, texts.GetText(LOYALTYFIELD_EXTERNALREF, ""));
}

//$$******************************************************************

const char* CExportPropertyPage1::GetDisplayText ( const char* szHeaderText )
{
	for ( int i = 0 ; i < m_arrayFieldIn.GetSize() ; i++ )
	{
		if (lstrcmp(szHeaderText, m_arrayFieldIn.GetAt(i)) == 0)
		{
			return m_arrayFieldOut.GetAt(i);
		}
	}

	return szHeaderText;
}

//$$******************************************************************

const char* CExportPropertyPage1::GetHeaderText ( const char* szDisplayName )
{
	for ( int i = 0 ; i < m_arrayFieldOut.GetSize() ; i++ )
	{
		if (lstrcmp(szDisplayName, m_arrayFieldOut.GetAt(i)) == 0)
		{
			return m_arrayFieldIn.GetAt(i);
		}
	}

	return szDisplayName;
}

//$$******************************************************************
