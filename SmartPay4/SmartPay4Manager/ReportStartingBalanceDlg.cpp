//**********************************************************************
#include "SmartPay4Manager.h"
#include "ReportDefaultHandler.h"
//**********************************************************************
#include "ReportStartingBalanceDlg.h"
//**********************************************************************

CReportStartingBalanceDlg::CReportStartingBalanceDlg(CWnd* pParent)
	: CSSAutoShutdownDialog(AutoShutdownHelper, CReportStartingBalanceDlg::IDD, pParent)
{
	m_strReportLabel = "";
	m_dateReference.SetCurrentDate();
	m_strDateRange = "";
	m_bGroupSort = FALSE;
}

//**********************************************************************

CReportStartingBalanceDlg::~CReportStartingBalanceDlg()
{
}

//**********************************************************************

void CReportStartingBalanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DATERANGE, m_comboDateRange);
	DDX_Control(pDX, IDC_CHECK_GROUPSORT, m_checkGroupSort);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DatePicker);
	DDX_CBString(pDX, IDC_COMBO_DATERANGE, m_strDateRange);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_ctimeDateFrom);
}

//**********************************************************************

BEGIN_MESSAGE_MAP(CReportStartingBalanceDlg, CSSAutoShutdownDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_DATERANGE, OnSelectDate)
END_MESSAGE_MAP()

//**********************************************************************

BOOL CReportStartingBalanceDlg::CSSAutoShutdownPostInitDialog()
{
	int nDateCurSel = 0;
	bool bGroupSort = FALSE;

	CString strLine = "";
	CReportDefaultHandler repdefs;
	if (repdefs.Read(m_strReportLabel, strLine) == TRUE)
	{
		CCSV csv(strLine);
		if (csv.IsEmpty(0) == FALSE)
		{
			nDateCurSel = csv.GetInt(0);
			bGroupSort = csv.GetBool(1);
		}
	}

	m_comboDateRange.AddString(szRANGE_TODAY);
	m_comboDateRange.AddString(szRANGE_YESTERDAY);
	m_comboDateRange.AddString(szRANGE_THIS_WEEK);
	m_comboDateRange.AddString(szRANGE_THIS_MONTH);
	m_comboDateRange.AddString(szRANGE_THIS_YEAR);
	m_comboDateRange.AddString(szRANGE_LAST_WEEK);
	m_comboDateRange.AddString(szRANGE_LAST_MONTH);
	m_comboDateRange.AddString(szRANGE_LAST_YEAR);
	m_comboDateRange.AddString(szRANGE_LAST_3_MONTHS);
	m_comboDateRange.AddString(szRANGE_LAST_6_MONTHS);
	m_comboDateRange.AddString(szRANGE_LAST_9_MONTHS);
	m_comboDateRange.AddString(szRANGE_LAST_12_MONTHS);
	m_comboDateRange.SetCurSel(nDateCurSel);

	m_DatePicker.SetFormat("ddd dd MMM yyy");

	OnSelectDate();
	m_checkGroupSort.SetCheck(bGroupSort);

	return TRUE;
}

//**********************************************************************

void CReportStartingBalanceDlg::OnSelectDate()
{
	if (UpdateData() == TRUE)
	{
		CSSDateRange date(System.GetStartOfWeek(), 1, 1, 1);
		CSSDate dateTo;
		date.GetRange(m_strDateRange, m_dateReference, dateTo);

		CTime timeFrom(m_dateReference.GetYear(), m_dateReference.GetMonth(), m_dateReference.GetDay(), 0, 0, 0);
		m_ctimeDateFrom = timeFrom;

		UpdateData(FALSE);
	}
}

//**********************************************************************

void CReportStartingBalanceDlg::OnOK()
{
	if (UpdateData() == TRUE)
	{
		m_dateReference.SetDate(m_ctimeDateFrom);

		CString strDate = "";
		strDate.Format("%2.2d/%2.2d/%4.4d", m_ctimeDateFrom.GetDay(), m_ctimeDateFrom.GetMonth(), m_ctimeDateFrom.GetYear() - 1);
		m_dateBegin.SetDate(strDate);

		strDate.Format("%2.2d/%2.2d/%4.4d", m_ctimeDateFrom.GetDay(), m_ctimeDateFrom.GetMonth(), m_ctimeDateFrom.GetYear() + 1);
		m_dateEnd.SetDate(strDate);

		m_bGroupSort = IsTicked(m_checkGroupSort);

		CCSV csv;
		csv.Add(m_comboDateRange.GetCurSel());
		csv.Add(m_bGroupSort);

		CReportDefaultHandler repdefs;
		repdefs.Save(m_strReportLabel, &csv);

		EndDialog(IDOK);
	}
}

/*******************************************************************
reply	-1 = Date before StartDate
		-2 = Date after EndDate
		0  = CardNo / GroupNo out of range
		1  = Date before reference date
		2  = Date = reference date
		3  = date after reference date
*********************************************************************/

int CReportStartingBalanceDlg::IsValidAtc ( CDbReportHandler* pExport, CSQLAuditRecord* atcRecord )
{
	CString strDate = "";
	switch (atcRecord->GetSourceType())
	{
	case AUDIT_POS:
	case AUDIT_KIOSKLINK:
	case AUDIT_VENDING:
	case AUDIT_EXT:
		strDate = atcRecord->GetDateLastUsed();
		break;

	default:
		strDate = atcRecord->GetDateFiled();
		break;
	}
				
	CSSDate dateAudit ( strDate );

	if (dateAudit.IsSet() == FALSE || dateAudit < m_dateBegin)						// date 1 year before reference date
	{
		return -1;
	}

	if (dateAudit > m_dateEnd)						// date 1 year after reference date
	{
		return -2;
	}

	if (pExport->IsCardInRange(atcRecord->GetUserID()) == FALSE)
	{
		return 0;
	}

	if ((pExport->GetGroupNo() > 0) && (pExport->GetGroupNo() != atcRecord->GetGroupNo()))
	{
		return 0;
	}

	if (dateAudit < m_dateReference)					// date before reference date
	{
		return 1;
	}

	if (dateAudit == m_dateReference)					// date = reference date
	{
		return 2;
	}

	return 3;											// date after reference date
}
 
/*******************************************************************
reply	0   = CardNo / GroupNo out of range
		1	= lastused before Reference date ( or not set )
		2	= lastused = Reference date
		3	= lastused after Reference date
********************************************************************/

int CReportStartingBalanceDlg::IsValidRecord( CSQLRowAccountFull& RowAccount, CDbReportHandler* pExport)
{
	if (pExport->IsCardInRange(RowAccount.GetUserIDString()) == FALSE)
	{
		return 0;
	}

	if (pExport->GetGroupNo() > 0 && pExport->GetGroupNo() != RowAccount.GetGroupNo())
	{
		return 0;
	}

	CSSDate date;
	date.SetDate(RowAccount.GetPurse1LastSpendDate());			// see when last used

	if (date.IsSet() == FALSE || date < m_dateReference)		// last used before reference date
	{
		return 1;
	}

	if (date == m_dateReference)					// lastused = reference date
	{
		return 2;
	}

	return 3;										// last used after reference date
}

//*******************************************************************

