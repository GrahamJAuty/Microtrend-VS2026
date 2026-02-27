/**********************************************************************/
#include "SmartPay4Manager.h"
/**********************************************************************/
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\SmartPayHeaderHelpers.h"
#include "..\SmartPay4Shared\SystemData.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
/**********************************************************************/
#include "HeaderWarnDlg.h"
/**********************************************************************/
#include "ExportPropertyPage1.h"
/**********************************************************************/

CExportPropertyPage1::CExportPropertyPage1()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CExportPropertyPage1::IDD)
{
	m_strHeader = "";
	m_bSelectAll = FALSE;
	SetTranslateTable();
}

/**********************************************************************/

CExportPropertyPage1::~CExportPropertyPage1()
{
}

/**********************************************************************/

void CExportPropertyPage1::SetPointer(CDbExportHandler* pDbExportHandler )
{
	m_pDbExportHandler = pDbExportHandler;
}

/**********************************************************************/

void CExportPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FILENAME, m_staticFilename);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_editFilename);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_buttonSelect);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CExportPropertyPage1, CSSAutoShutdownPropertyPage)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CExportPropertyPage1::OnInitDialog()
{
	m_strHeader = m_pDbExportHandler->m_strHeader;

	CPropertyPage::OnInitDialog();

	m_editFilename.SetWindowText(m_pDbExportHandler->m_strFilename);

	CCSV csvHeader{};

	{
		CString strHeader = "";
		if (nINTERFACE_BIOMETRIC == System.GetInterfaceType())
		{
			strHeader = SmartPayHeaderHelpers.GetHeaderBiometric();
		}
		else
		{
			strHeader = SmartPayHeaderHelpers.GetHeaderFull();
		}
		csvHeader.ParseLine(strHeader);
	}

	if (nDBEXPORTFIELDS_DBASE == m_pDbExportHandler->m_nExportFlag)
	{
		csvHeader.InsertAt(0,"EmptyField");
	}

	//GROUP IS ALWAYS INCLUDED IN DATABASE REPORT MODE
	{
		bool bTest = FALSE;
		bTest |= (nDBEXPORTFIELDS_DBASE == m_pDbExportHandler->m_nExportFlag);
		bTest |= (System.GetAllowGroupShiftDateRangeFlag() == FALSE);

		if (TRUE == bTest)
		{
			for (int n = 0; n < csvHeader.GetSize(); n++)
			{
				CString strField = csvHeader.GetString(n);

				if (nDBEXPORTFIELDS_DBASE == m_pDbExportHandler->m_nExportFlag)
				{
					if (
						(strField == Account::GroupNo.Label) ||
						(strField == Account::GroupShiftStartDate.Label) ||
						(strField == Account::GroupShiftEndDate.Label) ||
						(strField == Account::GroupShiftFutureGroup.Label))
					{
						csvHeader.RemoveAt(n--);
						continue;
					}
				}

				if (System.GetAllowGroupShiftDateRangeFlag() == FALSE)
				{
					if (strField == Account::GroupShiftStatus.Label)
					{
						csvHeader.RemoveAt(n--);
					}
				}
			}
		}
	}
	
	csvHeader.Add(PINNumber::PINNumber.Label);
	csvHeader.Add(Account::TotalSpendToDate.Label);
	csvHeader.Add(Account::DOBNextAge.Label);	// age next birthday
	csvHeader.Add(Account::DOBDay.Label);		// Next birthday Day of week 
	csvHeader.Add(Account::AlertText.Label);		// Alert text
	csvHeader.Add(Account::AllergyText.Label);	// Allergy text

	if (m_pDbExportHandler->m_strLabel.Left(3) == "DEL")	// delete export function
	{
		csvHeader.Add(Account::DeletedDate.Label);		// add special export delete header fields
		csvHeader.Add(Account::DeletedTime.Label);
	}

	bool bLockFirstItem = (m_pDbExportHandler->m_nExportFlag == nDBEXPORTFIELDS_DATA);

	m_listFields.SubclassDlgItem(IDC_LIST_EXPORT, this);
	m_listFields.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listFields.InsertColumn(0, "", LVCFMT_LEFT, 200);
	m_listFields.SetLockItemZeroFlag(bLockFirstItem);

	m_bSelectAll = TRUE;

	{
		CReportConsolidationArray<CSortedStringItem> arrayAvailableFields;

		for (int nPos = bLockFirstItem ? 1 : 0; nPos < csvHeader.GetSize(); nPos++)
		{
			CString strField = csvHeader.GetString(nPos);

			if (strField.GetLength() > 0)
			{
				CSortedStringItem item;
				item.m_strItem = strField;
				arrayAvailableFields.Consolidate(item);
			}
		}

		if (TRUE == bLockFirstItem)
		{
			CSSListTaggedSelectItem itemList;
			itemList.m_strText = GetDisplayText(csvHeader.GetString(0));
			itemList.SetTagState(1);
			m_listFields.AddItemToList(itemList);
		}

		CCSV csvTickedFields(m_strHeader);

		for (int nPos = 0; nPos < csvTickedFields.GetSize(); nPos++)
		{
			CSortedStringItem itemCheck;
			itemCheck.m_strItem = csvTickedFields.GetString(nPos);

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

	if (m_pDbExportHandler->m_nExportFlag != nDBEXPORTFIELDS_DATA)	// no filename if not export database
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
	else if (m_pDbExportHandler->m_strFilename == "")
	{
		m_editFilename.SetFocus();
		return FALSE;
	}

	return TRUE;
}

/**********************************************************************/

void CExportPropertyPage1::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST_EXPORT )  
   {
	   m_listFields.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

BOOL CExportPropertyPage1::OnKillActive()
{
	if ((CreateHeaderLine() == 0) && (m_pDbExportHandler->m_nExportFlag == nDBEXPORTFIELDS_DATA))
	{
		MessageBeep(MB_ICONEXCLAMATION);
		Prompter.Error("No database fields have been selected", "Field selection required");
		return FALSE;
	}

	CString strFilename = GetEditBoxText(m_editFilename);

	if (m_pDbExportHandler->m_strLabel != "DEL2")				// epos delete file has no header
	{
		if (strFilename.GetLength() != 0)					// check have filename
		{
			CSSFile file;
			if (file.Open(strFilename, "rb") == TRUE)	// file may not be found
			{
				CString strOldHeader;
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

					CFileRecycler::SendToRecycleBin(strFilename);		// delete existing file
				}
			}
		}
	}

	m_pDbExportHandler->m_strFilename = strFilename;
	m_pDbExportHandler->m_strHeader = m_strHeader;

	return CPropertyPage::OnKillActive();
}

/**********************************************************************/

int CExportPropertyPage1::CreateHeaderLine()
{
	CCSV csv;
	for ( int nPos = 0; nPos < m_listFields.GetItemCount(); nPos++ )
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

/**********************************************************************/

void CExportPropertyPage1::OnButtonDown() 
{
	int nSel = m_listFields.GetCurSel();

	int nMin = 0;
	if (m_pDbExportHandler->m_nExportFlag == nDBEXPORTFIELDS_DATA)	// CardNo must be first in export data file
	{
		nMin = 1;
	}

	if ( ( nSel >= nMin ) && ( nSel < m_listFields.GetItemCount() - 1 ) )
	{
		m_listFields.SwapItems( nSel, nSel + 1 );
	}
}

/**********************************************************************/

void CExportPropertyPage1::OnButtonUp() 
{
	int nMin = 1;
	if (m_pDbExportHandler->m_nExportFlag == nDBEXPORTFIELDS_DATA)	// CardNo must be first in export data file
	{
		nMin = 2;
	}

	int nSel = m_listFields.GetCurSel();
	if ( ( nSel >= nMin ) && ( nSel < m_listFields.GetItemCount() ) )
	{
		m_listFields.SwapItems( nSel, nSel - 1 );
	}
}

//**********************************************************************

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
}

/**********************************************************************/

void CExportPropertyPage1::SetSelectButtonText() 
{
	CString strText = ( m_bSelectAll == FALSE ) ? "Select\n&All" : "&Clear\nAll";
	m_buttonSelect.SetWindowText ( strText );
}

/**********************************************************************/

void CExportPropertyPage1::OnButtonBrowse()
{
	CString strFilename = GetEditBoxText(m_editFilename);

	CString strFileFilters = m_pDbExportHandler->m_strFileFilters;
	strFileFilters += "|All Files (*.*)|*.*||";

	CFileDialog dlg(FALSE, m_pDbExportHandler->m_strDefExtension, NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR, strFileFilters, this);
	dlg.m_ofn.lpstrTitle = "Select Export filename";

	CString strPath = ::StripFilename(strFilename);
	if (strPath == "")
	{
		strPath = ".";
	}

	dlg.m_ofn.lpstrInitialDir = strPath;

	if (DoFileDialog(dlg) == IDOK)
	{
		strFilename = dlg.GetPathName();
		m_editFilename.SetWindowText(strFilename);
	}
}

/**********************************************************************/

void CExportPropertyPage1::CreateDisplayText ( const char* szHeaderText, const char* szUserText )
{
	CString strUserText = szUserText;
	if ( strUserText.GetLength() > 0 )
	{
		if ( strUserText != szHeaderText )
		{
			m_arrayFieldIn.Add ( szHeaderText );

			CString strDisplayText;		
			strDisplayText.Format ( "%s  { %s }", szHeaderText, szUserText );
			m_arrayFieldOut.Add ( strDisplayText );
		}
	}
}

/**********************************************************************/

void CExportPropertyPage1::SetTranslateTable()
{
	CSQLRepositoryUserText Repo;
	CreateDisplayText(Account::UserID.Label, Repo.GetMemberIDText(Account::UserID.Label));
	CreateDisplayText(Account::ExternalRef1.Label, Repo.GetExtRef1Text(Account::ExternalRef1.Label));
	CreateDisplayText(Account::ExternalRef2.Label, Repo.GetExtRef2Text(Account::ExternalRef2.Label));
	CreateDisplayText(Account::Info1.Label, Repo.GetInfo1Text(Account::Info1.Label));
	CreateDisplayText(Account::Info2.Label, Repo.GetInfo2Text(Account::Info2.Label));
	CreateDisplayText(Account::Info3.Label, Repo.GetInfo3Text(Account::Info3.Label));
	CreateDisplayText(Account::Info4.Label, Repo.GetInfo4Text(Account::Info4.Label));
	CreateDisplayText(Account::Info5.Label, Repo.GetInfo5Text(Account::Info5.Label));
	CreateDisplayText(Account::Info6.Label, Repo.GetInfo6Text(Account::Info6.Label));
	CreateDisplayText(Account::Info7.Label, Repo.GetInfo7Text(Account::Info7.Label));
	CreateDisplayText(Account::Info8.Label, Repo.GetInfo8Text(Account::Info8.Label));
	CreateDisplayText(Account::Tick1.Label, Repo.GetTick1Text(Account::Tick1.Label));
	CreateDisplayText(Account::Tick2.Label, Repo.GetTick2Text(Account::Tick2.Label));
	CreateDisplayText(Account::Tick3.Label, Repo.GetTick3Text(Account::Tick3.Label));
	CreateDisplayText(Account::Tick4.Label, Repo.GetTick4Text(Account::Tick4.Label));
	CreateDisplayText(Account::DOB.Label, Repo.GetDOBText(Account::DOB.Label));
}

/**********************************************************************/

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

/**********************************************************************/

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

/**********************************************************************/
