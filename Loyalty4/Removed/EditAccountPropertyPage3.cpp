//$$******************************************************************
#include "ColumnHandler.h"
#include "FamilyEditDlg.h"
#include "LoyaltyManager.h"
#include "PasswordData.h"
#include "ReportCSVFile.h"
//$$******************************************************************
#include "EditAccountPropertyPage3.h"
//$$******************************************************************

CEditAccountPropertyPage3::CEditAccountPropertyPage3(CSQLRowAccountFull& RowAccount)
	: CSSPropertyPage(CEditAccountPropertyPage3::IDD), m_RowAccount(RowAccount)
{
}

//$$******************************************************************

CEditAccountPropertyPage3::~CEditAccountPropertyPage3()
{
}

//$$******************************************************************

void CEditAccountPropertyPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_buttonEdit);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_BUTTON_PRINT, m_buttonPrint);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CEditAccountPropertyPage3, CPropertyPage)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_FAMILY, OnGetDispInfoList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FAMILY, OnDoubleClickList)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
END_MESSAGE_MAP()

//$$******************************************************************
static int nColumnCount = 6;
//$$******************************************************************

BOOL CEditAccountPropertyPage3::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	CColumnHandler column;
	if (column.Read("FAMILY", nColumnCount) == FALSE)
	{
		column.SetWidth(0, 140);						// default values
		column.SetWidth(1, 140);						// default values
		column.SetWidth(2, 100);
		column.SetWidth(3, 100);
		column.SetWidth(4, 60);
		column.SetWidth(5, 60);
	}

	m_list.SubclassDlgItem(IDC_LIST_FAMILY, this);
	m_list.InsertColumn(0, "Forename", LVCFMT_LEFT, column.GetWidth(0));
	m_list.InsertColumn(1, "Surname", LVCFMT_LEFT, column.GetWidth(1));
	m_list.InsertColumn(2, "Member", LVCFMT_LEFT, column.GetWidth(2));
	m_list.InsertColumn(3, "DOB", LVCFMT_LEFT, column.GetWidth(3));
	m_list.InsertColumn(4, "Gender", LVCFMT_LEFT, column.GetWidth(5));
	m_list.InsertColumn(5, "Age", LVCFMT_RIGHT, column.GetWidth(4));
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	bool bAllowed = Passwords.IsEditGeneralAllowed();
	int nMode = (bAllowed == FALSE) ? DB_READONLY : DB_READWRITE;

	CString strPathname = Filenames.GetFamilyPath(SimpleFormatInt64Value(m_RowAccount.GetUserID()));
	if (m_dbFamily.OpenForSharing(strPathname, nMode) == TRUE)
	{
		m_dbFamily.MoveFirst();
		DisplayScreen();
	}
	else
	{
		if (bAllowed == TRUE)						// in readonly don't display 'no database file found' if not found
		{
			Prompter.Error(m_dbFamily.GetError());
			bAllowed = FALSE;
		}
		else
		{
			m_buttonPrint.EnableWindow(FALSE);
		}
	}

	m_buttonAdd.EnableWindow(bAllowed);
	m_buttonEdit.EnableWindow(bAllowed);
	m_buttonDelete.EnableWindow(bAllowed);
	m_list.EnableWindow(bAllowed);

	return TRUE;
}

//$$******************************************************************

void CEditAccountPropertyPage3::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST_FAMILY)
	{
		m_list.AdjustRowHeight(lpMeasureItemStruct, 10);
	}
}

/**********************************************************************/

void CEditAccountPropertyPage3::DisplayScreen()
{
	m_list.SetItemCountEx ( m_dbFamily.GetRecordCount() );
	m_list.SetCurSel ( m_dbFamily.GetCurrentRow() );
	m_list.Invalidate();							// display screen
}

//$$******************************************************************

void CEditAccountPropertyPage3::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nRecordIndex = m_dbFamily.GetCurrentRow();	// get current record index

		int nListIndex = pDispInfo->item.iItem;
		m_dbFamily.MoveTo(nListIndex);

		CString strText;
		switch (pDispInfo->item.iSubItem)
		{
		case 0:
			lstrcpy(pDispInfo->item.pszText, (const char*)m_dbFamily.GetForename());
			break;

		case 1:
			lstrcpy(pDispInfo->item.pszText, m_dbFamily.GetSurname());
			break;

		case 2:
			lstrcpy(pDispInfo->item.pszText, m_dbFamily.GetMemberTypeText());
			break;

		case 3:
			lstrcpy(pDispInfo->item.pszText, m_dbFamily.GetDobDate());
			break;

		case 4:
			lstrcpy(pDispInfo->item.pszText, m_dbFamily.GetGenderText());
			break;

		case 5:
			strText.Format("%d", m_dbFamily.GetAge());
			lstrcpy(pDispInfo->item.pszText, strText);
			break;

			//			case 4:	strText.Format("%d", m_dbFamily.GetVisitCount() );
			//					strcpy(pDispInfo->item.pszText, strText );
			//					break;
		}

		m_dbFamily.MoveTo(nRecordIndex);							// restore current index pointer
	}

	*pResult = 0;
}

//$$******************************************************************

void CEditAccountPropertyPage3::OnButtonAdd()
{
	int nReply = IDNO;
	bool bExitAllowed = FALSE;

	CFamilyEditInfo info;
	info.m_strForename = "";
	info.m_strSurname = "";
	info.m_strDOB = "";
	info.m_strGender = "Unknown";
	info.m_nMemberType = 0;

	CFamilyEditDlg dlg("Add Family Member", info, this);
	while (bExitAllowed == FALSE)
	{
		if (dlg.DoModal() == IDOK)
		{
			CString strForename = info.m_strForename;

			if (m_dbFamily.FindRecord(strForename) == FALSE)
			{
				if (m_dbFamily.AddBlankRecord(strForename) == FALSE)
				{
					CString strMsg = "";
					strMsg.Format("Unable to add record.\n\n%s", m_dbFamily.GetError());
					Prompter.Error(strMsg, "Database Error");
				}
				else
				{
					nReply = IDYES;
				}
			}
			else
			{
				CString strMsg = "";
				strMsg.Format("A family member called ' %s ' already exists!\n\nDo you wish to replace it?", strForename);
				nReply = Prompter.YesNo(strMsg, "Record Exists");
			}

			if (nReply == IDYES)
			{
				m_dbFamily.SetForename(info.m_strForename);
				m_dbFamily.SetSurname(info.m_strSurname);
				m_dbFamily.SetDob(info.m_strDOB);
				m_dbFamily.SetGender(info.m_strGender);
				m_dbFamily.SetMemberType(info.m_nMemberType);
				m_dbFamily.SetDateAdded();
				bExitAllowed = TRUE;
			}
		}
		else
		{
			bExitAllowed = TRUE;
		}
	}

	if (nReply == IDYES)
	{
		DisplayScreen();
	}
}

//$$******************************************************************

void CEditAccountPropertyPage3::OnDoubleClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnButtonEdit();
	*pResult = 0;
}

//$$******************************************************************

void CEditAccountPropertyPage3::OnButtonEdit()
{
	int nIndex;
	if ((nIndex = m_list.GetCurSel()) >= 0)
	{
		m_dbFamily.MoveTo(nIndex);							// restore current index pointer

		CFamilyEditInfo info;
		info.m_strForename = m_dbFamily.GetForename();
		info.m_strSurname = m_dbFamily.GetSurname();
		info.m_strGender = m_dbFamily.GetGenderText();
		info.m_strDOB = m_dbFamily.GetDobDate();
		info.m_nMemberType = m_dbFamily.GetMemberType();

		CFamilyEditDlg dlg("Edit Family Member", info, this);
		
		if (dlg.DoModal() == TRUE)
		{
			CString strDate = m_dbFamily.GetDateAdded();		// get orginal added date
			m_dbFamily.DeleteRecord();							// delete record incase key has been changed

			m_dbFamily.AddBlankRecord(info.m_strForename);			// add record again
			m_dbFamily.SetSurname(info.m_strSurname);
			m_dbFamily.SetDob(info.m_strDOB);
			m_dbFamily.SetGender(info.m_strGender);
			m_dbFamily.SetMemberType(info.m_nMemberType);
			m_dbFamily.SetDateAdded(strDate);

			DisplayScreen();
		}
	}
}

//$$******************************************************************

void CEditAccountPropertyPage3::OnButtonDelete()
{
	int nIndex = 0;
	if ((nIndex = m_list.GetCurSel()) >= 0)
	{
		m_dbFamily.MoveTo(nIndex);							// restore current index pointer

		CString strMsg = "";
		strMsg.Format("Are you sure you wish to remove ' %s ' from the family member list?", m_dbFamily.GetForename());
		if (Prompter.YesNo(strMsg, "Confirmation required") == IDYES)
		{
			m_dbFamily.DeleteRecord();
			DisplayScreen();
		}
	}
}

//$$******************************************************************

void CEditAccountPropertyPage3::OnButtonPrint()
{
	CString strCSVFile = Filenames.GetFamilyPath("xxxxxx");				// create a temp family database ( ..\P47\Family\xxxxxx.dat ) to dispaly

	CFamilyDatabase dbCopy;
	if (dbCopy.OpenForSharing(strCSVFile, DB_READWRITE) == TRUE)
	{
		for (int nRow = 0; nRow < m_dbFamily.GetRecordCount(); nRow++)
		{
			if (m_dbFamily.MoveTo(nRow) == TRUE)
			{
				m_dbFamily.CopyToDatabase(&dbCopy);
			}
		}

		dbCopy.Close();
	}

	CString strTitle = "";		
	strTitle.Format("%s Family Members", m_RowAccount.GetFullname());
	CCSVFileReport csv("R126", strCSVFile, this);
	csv.SetMode(P_WAIT);
	csv.Show(strTitle);

	::SendToRecycleBin(strCSVFile);
}

//$$******************************************************************

void CEditAccountPropertyPage3::OnOK()
{
	bool bExitAllowed = m_dbFamily.m_bModified;

	CColumnHandler column;
	column.Save("FAMILY", 0, nColumnCount, m_list);

	CPropertyPage::OnOK();
}

//$$******************************************************************

void CEditAccountPropertyPage3::OnCancel()
{
	m_dbFamily.m_bModified = FALSE;					// don't save any changes
	CPropertyPage::OnCancel();
}

//$$******************************************************************
