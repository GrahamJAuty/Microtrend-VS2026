//$$******************************************************************
#include "AuditReport.h"
#include "LoyaltyManager.h"
#include "FindDlg.h"
#include "RepmanHandler.h"
//$$******************************************************************
#include "CardArchiveDlg.h"
//$$******************************************************************

CCardArchiveDlg::CCardArchiveDlg( CAuditReportConfig& ReportConfig, CWnd* pParent)
	: CSSDialog(CCardArchiveDlg::IDD, pParent)
	, m_ReportConfig(ReportConfig)
{
	m_ReportConfig.SetSearchType(0);
	m_nAllReqdFlag = 0;
}

//$$******************************************************************

CCardArchiveDlg::~CCardArchiveDlg()
{
}

//$$******************************************************************

void CCardArchiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CARDNO, m_editCardNo);
	DDX_Control(pDX, IDC_BUTTON_ALL, m_buttonAll);
	DDX_Control(pDX, IDC_TAB_SEARCH, m_tabSearch);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_editText);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CCardArchiveDlg, CSSDialog)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SEARCH, OnTcnSelchangeTabSearch)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CCardArchiveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_editCardNo.LimitText(System.GetUserIDLength());

	m_tabSearch.InsertItem(0, System.GetCardNoText());
	m_tabSearch.InsertItem(1, "Name");
	m_tabSearch.InsertItem(2, "Comment");

	m_listFiles.SubclassDlgItem(IDC_LIST_ARCHIVE, this);
	m_listFiles.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listFiles.InsertColumn(0, "", LVCFMT_LEFT, 140);
	m_listFiles.SetLockItemZeroFlag(FALSE);

	CString strName;
	strName.Format("????????.%s", m_ReportConfig.GetArchiveExtension());

	CFileFind FileFinder;
	BOOL bWorking = FileFinder.FindFile(Filenames.GetAtcArchivePath(strName));

	while (bWorking)
	{
		bWorking = FileFinder.FindNextFile();
		if (FileFinder.IsDots() == TRUE || FileFinder.IsDirectory() == TRUE)
			continue;

		CSSListTaggedSelectItem item;
		item.m_strText = FileFinder.GetFileName();
		m_listFiles.AddSortedItemToList(item);
	}

	// scan list and change the text from yyyymmdd.xxx to dd/mm/yyyy

	for (int i = 0; i < m_listFiles.GetItemCount(); i++)
	{
		CSSListTaggedSelectItem item;
		m_listFiles.GetListItem(i, item);

		CString strFilename = item.m_strText;

		if (strFilename.GetAt(8) == '.')
		{
			CString strNewName = strFilename.Mid(6, 2);
			strNewName += "/";
			strNewName += strFilename.Mid(4, 2);
			strNewName += "/";
			strNewName += strFilename.Left(4);

			item.m_strText = strNewName;
			m_listFiles.SetListItem(i, item);
		}
	}

	if (::FileExists(m_ReportConfig.GetAuditFilename()) == TRUE)	// current audit file
	{
		CSSListTaggedSelectItem item;
		item.m_strText = szCURRENTAUDIT;
		m_listFiles.AddItemToList(item);
	}

	OnButtonAll();											// select all files

	SetTabs();

	return TRUE;
}

//$$******************************************************************

void CCardArchiveDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST_ARCHIVE )  
   {
	   m_listFiles.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

//$$******************************************************************

void CCardArchiveDlg::OnButtonAll()
{
	m_nAllReqdFlag ^= 0x01;

	if (1 == m_nAllReqdFlag)
	{
		m_listFiles.SelectAll();
	}
	else
	{
		m_listFiles.SelectNone();
	}

	SetAllButtonText();
}

//$$******************************************************************

void CCardArchiveDlg::SetAllButtonText() 
{
	if (m_nAllReqdFlag == 0)
	{
		m_buttonAll.SetWindowText("All");
	}
	else
	{
		m_buttonAll.SetWindowText("Clear");
	}
}

//$$******************************************************************

void CCardArchiveDlg::OnButtonBrowse()
{
	CFindDlg dlg{};
	if (dlg.DoModal() == IDOK)
	{
		m_editCardNo.SetWindowText(SimpleFormatInt64Value(dlg.m_nRequestedCardNo));
		m_editCardNo.SetFocus();
	}
}

//$$******************************************************************

void CCardArchiveDlg::OnTcnSelchangeTabSearch(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	m_ReportConfig.SetSearchType(m_tabSearch.GetCurSel());
	SetTabs();
}

//$$******************************************************************

void CCardArchiveDlg::SetTabs()
{
	int nShowCardNo = SW_SHOW;
	int nShowBrowse = SW_SHOW;
	int nShowText = SW_HIDE;

	if (m_ReportConfig.GetSearchType() != 0)
	{
		nShowCardNo = SW_HIDE;
		nShowBrowse = SW_HIDE;
		nShowText = SW_SHOW;
	}

	m_editCardNo.ShowWindow(nShowCardNo);
	m_buttonBrowse.ShowWindow(nShowBrowse);
	m_editText.ShowWindow(nShowText);
}

//$$******************************************************************

void CCardArchiveDlg::OnOK()
{
	m_ReportConfig.SetSearchType(m_tabSearch.GetCurSel());
	m_ReportConfig.SetSearchText("");
	m_ReportConfig.SetSearchCardNo("");

	if (m_ReportConfig.GetSearchType() == 0)		// search for AccountNo
	{
		CString strUserID = GetEditBoxText(m_editCardNo);
		strUserID.TrimLeft('0');
		m_ReportConfig.SetSearchCardNo(strUserID);

		if (strUserID.GetLength() == 0)
		{
			MessageBeep(MB_ICONSTOP);
			Prompter.FatalError("No Account has been specified", "Entry error");
			m_editCardNo.SetFocus();
			return;
		}
		m_editCardNo.SetWindowText(strUserID);
	}
	else														// search for 1=Name, 2=audit commnet
	{
		m_ReportConfig.SetSearchText(GetEditBoxText(m_editText));

		if (m_ReportConfig.GetSearchText() == "")
		{
			CString strMsg;
			if (m_ReportConfig.GetSearchType() == 1)
			{
				strMsg = "No name text has been specified!";
			}
			else
			{
				strMsg = "No audit Comment text has been specified!";
			}

			MessageBeep(MB_ICONSTOP);
			Prompter.FatalError(strMsg, "Entry error");
			m_editText.SetFocus();
			return;
		}
	}

	m_ReportConfig.SearchFilenamesReset();
	for (int i = 0; i < m_listFiles.GetItemCount(); i++)
	{
		if (m_listFiles.GetItemTagState(i) == 1)
		{
			CSSListTaggedSelectItem item;
			m_listFiles.GetListItem(i, item);
			m_ReportConfig.SearchFilenamesAddItem(item.m_strText);
		}
	}

	if (m_ReportConfig.SearchFilenamesGetCount() == 0)
	{
		MessageBeep(MB_ICONSTOP);
		Prompter.FatalError("No files have been selected from the list!", "Entry error");
		return;
	}

	CRepmanHandler RepmanHandler("", this);
	RepmanHandler.SetTitle(m_ReportConfig.GetReportTitle());

	CAuditReport AuditReport( m_ReportConfig);

	int nErrNo = AuditReport.CreateWithArchive(m_ReportConfig.GetReportFilename());

	if (nREPORT_NOERROR == nErrNo)
	{
		RepmanHandler.DisplayReport(m_ReportConfig.GetReportFilename(), m_ReportConfig.GetParamsFilename(), m_ReportConfig.GetReportKey());
	}
	else
	{
		RepmanHandler.DisplayError(nErrNo);
	}
}

//$$******************************************************************
