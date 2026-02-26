/**********************************************************************/
#include "CustomerStatementErrorInfo.h"
/**********************************************************************/
#include "SMTPErrorDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CSMTPErrorDlg, CDialog)
/**********************************************************************/

CSMTPErrorDlg::CSMTPErrorDlg(int nSessionType, CSMTPEmailHelpers& SMTPHelpers, int nIndex, CWnd* pParent)
	: CDialog(CSMTPErrorDlg::IDD, pParent), m_SMTPHelpers(SMTPHelpers)
{
	m_nIndex = nIndex;
	m_nSessionType = nSessionType;
	m_bGotErrorFile = FALSE;
	m_strErrorFileName = "";

	if ((m_nIndex >= 0) && (m_nIndex < m_SMTPHelpers.GetSessionSize()))
	{
		CSMTPPendingEmailInfo info;
		m_SMTPHelpers.GetSessionEmailInfo(m_nIndex, info);

		if (SMTP_PENDING_WAIT_RETRY == info.m_nStatus)
		{
			m_strErrorFileName = m_SMTPHelpers.GetEmailFileName(m_nIndex);
			m_strErrorFileName += ".ERR";
			m_bGotErrorFile = TRUE;
		}
	}
}

/**********************************************************************/

CSMTPErrorDlg::~CSMTPErrorDlg()
{
}

/**********************************************************************/

void CSMTPErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_EDIT_ERROR, m_editError );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSMTPErrorDlg, CDialog)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSMTPErrorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strHeader1 = "";
	CString strHeader2 = "";
	CString strHeader3 = "";

	switch (m_nSessionType)
	{
	case SMTP_SESSIONTYPE_CUSTOMER:
		strHeader1 = "Location";
		strHeader2 = "Customer";
		strHeader3 = "Result";
		break;

	case SMTP_SESSIONTYPE_PMS:
		strHeader1 = "Subject";
		strHeader2 = "Guest Name";
		strHeader3 = "Result";
		break;

	case SMTP_SESSIONTYPE_ORDER:
		strHeader1 = "Subject";
		strHeader2 = "Supplier";
		strHeader3 = "Result";
		break;

	case SMTP_SESSIONTYPE_TEST:
		strHeader1 = "Subject";
		strHeader2 = "";
		strHeader3 = "Result";
		break;
	}

	m_listResults.SubclassDlgItem(IDC_LIST, this);

	if (SMTP_SESSIONTYPE_TEST != m_nSessionType)
	{
		m_listResults.InsertColumn(0, strHeader1, LVCFMT_LEFT, 140);
		m_listResults.InsertColumn(1, strHeader2, LVCFMT_LEFT, 130);
		m_listResults.InsertColumn(2, strHeader3, LVCFMT_LEFT, 200);
	}
	else
	{
		m_listResults.InsertColumn(0, strHeader1, LVCFMT_LEFT, 270);
		m_listResults.InsertColumn(1, strHeader3, LVCFMT_LEFT, 200);
	}

	m_listResults.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listResults.SetItemCountEx(1);
	m_listResults.SetCurSel(0);

	CSSFile fileError;
	CString strError = "";
	if (fileError.Open(m_strErrorFileName, "rb") == TRUE)
	{
		CString strBuffer;
		while (fileError.ReadString(strBuffer) == TRUE)
		{
			if (strError.GetLength() + strBuffer.GetLength() < 2000)
			{
				strError += strBuffer;
			}
			else
			{
				strError += strBuffer.Left(2000 - strError.GetLength());
				break;
			}
		}
	}

	int nPos = 0;
	int nLength = strError.GetLength();
	CString strError2 = "";

	while (nPos < nLength - 80)
	{
		strError2 += strError.Mid(nPos, 80);
		strError2 += "\r\n";
		nPos += 80;
	}

	if (nPos < nLength)
	{
		strError2 += strError.Mid(nPos, nLength - nPos);
	}

	m_editError.SetWindowText(strError2);

	return TRUE;
}

/**********************************************************************/

void CSMTPErrorDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		CCSV csv('\t');

		if ((m_nIndex >= 0) && (m_nIndex < m_SMTPHelpers.GetSessionSize()))
		{
			CSMTPPendingEmailInfo info;
			m_SMTPHelpers.GetSessionEmailInfo(m_nIndex, info);

			CCSV csvFriendlyName(info.m_strFriendlyName);

			CString strError = "";
			CCustomerStatementErrorInfo infoError;
			infoError.m_nStatementError = info.m_nContextError;
			infoError.m_nSMTPError = info.m_nSMTPError;
			infoError.GetErrorText(strError);

			csv.Add(csvFriendlyName.GetString(0));

			if (SMTP_SESSIONTYPE_TEST != m_nSessionType)
			{
				csv.Add(csvFriendlyName.GetString(1));
			}

			csv.Add(strError);
		}

		lstrcpy(pDispInfo->item.pszText, csv.GetString(pDispInfo->item.iSubItem));
	}

	*pResult = 0;
}

/**********************************************************************/
