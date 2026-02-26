/**********************************************************************/
#include "NewRecordNumberDlg.h"
#include "MixMatchCSVArray.h"
#include "NodeTypes.h"
#include "PromotionsCSVArray.h"
#include "PropertySheetMixMatch.h"
#include "PropertySheetPromotion.h"
#include "TimePlanEntriesCSVArray.h"
/**********************************************************************/
#include "DefEditTimePlanDlg.h"
/**********************************************************************/

CDefEditTimePlanDlg::CDefEditTimePlanDlg(bool bTemplateMode, int nIndex, int nMinIndex, int nMaxIndex, bool bAllowSpin, CListDataDlgChangeFlag& WarnCancelAfterChange, CWnd* pParent /*=NULL*/)
	: CSSDialog(CDefEditTimePlanDlg::IDD, pParent), m_WarnCancelAfterChange(WarnCancelAfterChange)
{
	m_bTemplateMode = bTemplateMode;
	if (FALSE == m_bTemplateMode)
	{
		m_pTimePlanEntryArray = &DataManager.TimePlanEntry;
	}
	else
	{
		m_pTimePlanEntryArray = &DataManager.TimePlanTemplateEntry;
	}

	//{{AFX_DATA_INIT(CDefEditTimePlanDlg)
	m_strDescription = _T("");
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
	m_nMinIndex = nMinIndex;
	m_nMaxIndex = nMaxIndex;
	m_bAllowSpin = bAllowSpin;
	m_nLastType = -1;
}

/**********************************************************************/

void CDefEditTimePlanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditTimePlanDlg)
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_editDescription);
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_checkEnable);
	DDX_Control(pDX, IDC_STATIC_ACTION, m_staticAction);
	DDX_Control(pDX, IDC_STATIC_SELECTION, m_staticSelection);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_Control(pDX, IDC_COMBO_SELECTION, m_comboActionDetail);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_buttonEdit);
	DDX_Control(pDX, IDC_STATIC_STARTDATE, m_staticStartDate);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_STATIC_ENDDATE, m_staticEndDate);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_STATIC_ACTIVE, m_staticActive);
	DDX_Control(pDX, IDC_STATIC_TO, m_staticTo);
	DDX_Control(pDX, IDC_STATIC_ENDPERIOD, m_staticEndPeriod);
	DDX_Control(pDX, IDC_TIMEPICKER_START, m_TimePickerStart);
	DDX_Control(pDX, IDC_TIMEPICKER_END, m_TimePickerEnd);
	DDX_Control(pDX, IDC_CHECK_SUNDAY, m_checkSunday);
	DDX_Control(pDX, IDC_CHECK_MONDAY, m_checkMonday);
	DDX_Control(pDX, IDC_CHECK_TUESDAY, m_checkTuesday);
	DDX_Control(pDX, IDC_CHECK_WEDNESDAY, m_checkWednesday);
	DDX_Control(pDX, IDC_CHECK_THURSDAY, m_checkThursday);
	DDX_Control(pDX, IDC_CHECK_FRIDAY, m_checkFriday);
	DDX_Control(pDX, IDC_CHECK_SATURDAY, m_checkSaturday);
	DDX_Control(pDX, IDC_SPIN, m_SpinCtrl);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDescription);
	DDV_MaxChars(pDX, m_strDescription, TimePlanEntries::Name.Max);
	DDX_Text(pDX, IDC_EDIT_PERIOD, m_nPeriod );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditTimePlanDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditTimePlanDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnSpinButton)
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelectType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditTimePlanDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SubclassEdit( IDC_EDIT_PERIOD, SS_NUM, 2, "%d" );
	SubclassEdit( IDC_EDIT_SELECTION, SS_NUM, 10, "%s" );

	COleDateTime start ( 1800, 1, 1, 0, 0, 0 );
	COleDateTime end ( 9000, 12, 31, 0, 0, 0 ); 
	m_DatePickerFrom.SetRange ( &start, &end );
	m_DatePickerTo.SetRange ( &start, &end );
	m_DatePickerFrom.SetFormat( "ddd dd MMM yyy" );
	m_DatePickerTo.SetFormat( "ddd dd MMM yyy" );
	m_TimePickerStart.SetFormat ( "HH:mm" );
	m_TimePickerEnd.SetFormat ( "HH:mm" );

	m_comboType.AddString( "Set Price Band" );
	m_comboType.AddString( "Permit Promotion" );
	m_comboType.AddString( "Enable Mix Match" );
	m_comboType.AddString( "Operate Gate" );
	m_comboType.AddString( "Screen Mode" );
	
	if ( m_bAllowSpin == FALSE )
	{
		m_SpinCtrl.ShowWindow( SW_HIDE );
		m_SpinCtrl.EnableWindow( FALSE );
	}

	if ( TRUE == m_bTemplateMode )
	{
		ShowAndEnableWindow(&m_staticAction, FALSE);
		ShowAndEnableWindow(&m_staticSelection, FALSE);
		ShowAndEnableWindow(&m_comboType, FALSE);
		ShowAndEnableWindow(&m_comboActionDetail, FALSE);
		ShowAndEnableWindow(&m_buttonEdit, FALSE);
		ShowAndEnableWindow(GetEditSelection(), FALSE);
		MoveControl(&m_staticStartDate, 18, 80);
		MoveControl(&m_DatePickerFrom, 75, 79);
		MoveControl(&m_staticEndDate, 18, 100);
		MoveControl(&m_DatePickerTo, 75, 99);
		MoveControl(&m_staticActive, 18, 131);
		MoveControl(&m_staticTo, 114, 131);
		MoveControl(&m_staticEndPeriod, 18, 151);
		MoveControl(&m_TimePickerStart, 75, 130);
		MoveControl(&m_TimePickerEnd, 126, 130);
		MoveControl(GetEditPeriod(), 75, 150);
	}

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(40);
	m_comboActionDetail.SetTabStops( arrayStops );

	GetData();
	return TRUE;  
}

/**********************************************************************/
CEdit* CDefEditTimePlanDlg::GetEditPeriod(){ return GetEdit( IDC_EDIT_PERIOD ); }
CEdit* CDefEditTimePlanDlg::GetEditSelection(){ return GetEdit( IDC_EDIT_SELECTION ); }
/**********************************************************************/

void CDefEditTimePlanDlg::GetData()
{
	CTimePlanEntriesCSVRecord Entry;
	m_pTimePlanEntryArray->GetAt(m_nIndex, Entry);

	m_strInitialName = Entry.GetName();
	m_nInitialType = Entry.GetType();
	m_nInitialSelection = Entry.GetSelection();

	CString strEntityType = "";
	if (FALSE == m_bTemplateMode)
	{
		strEntityType = "Time Plan";
	}
	else
	{
		strEntityType = "Template";
	}

	CString strTitle = "";
	strTitle.Format("Edit %s Entry (%d, %s)",
		(const char*)strEntityType,
		Entry.GetEntryNo(),
		Entry.GetName());
	SetWindowText(strTitle);

	m_editDescription.SetWindowText(Entry.GetName());

	m_checkEnable.SetCheck(Entry.GetActiveFlag());

	if (FALSE == m_bTemplateMode)
	{
		int nType = Entry.GetType();
		int nSelection = Entry.GetSelection();

		if ((nType < 0) && (nType > 4))
		{
			nType = 0;
		}

		m_nLastType = nType;
		m_comboType.SetCurSel(nType);
		FillSelectionCombo();
		ForceSelection(nSelection);
	}

	COleDateTime dateStart = Entry.GetStartDate();
	if (dateStart.GetStatus() == COleDateTime::valid)
	{
		m_DatePickerFrom.SetTime(dateStart);
	}
	else
	{
		m_DatePickerFrom.SetTime(COleDateTime(2008, 1, 1, 0, 0, 0));
	}

	COleDateTime dateEnd = Entry.GetEndDate();
	if (dateEnd.GetStatus() == COleDateTime::valid)
	{
		m_DatePickerTo.SetTime(dateEnd);
	}
	else
	{
		m_DatePickerTo.SetTime(COleDateTime(2100, 12, 31, 0, 0, 0));
	}

	COleDateTime timeStart = COleDateTime(2008, 1, 1, Entry.GetStartHour(), Entry.GetStartMinute(), 0);
	if (timeStart.GetStatus() == COleDateTime::valid)
	{
		m_TimePickerStart.SetTime(timeStart);
	}
	else
	{
		m_TimePickerStart.SetTime(COleDateTime(2008, 1, 1, 0, 0, 0));
	}

	COleDateTime timeEnd = COleDateTime(2008, 1, 1, Entry.GetEndHour(), Entry.GetEndMinute(), 0);
	if (timeEnd.GetStatus() == COleDateTime::valid)
	{
		m_TimePickerEnd.SetTime(timeEnd);
	}
	else
	{
		m_TimePickerEnd.SetTime(COleDateTime(2008, 1, 1, 0, 0, 0));
	}

	m_checkSunday.SetCheck(Entry.GetSundayFlag() != 0);
	m_checkMonday.SetCheck(Entry.GetMondayFlag() != 0);
	m_checkTuesday.SetCheck(Entry.GetTuesdayFlag() != 0);
	m_checkWednesday.SetCheck(Entry.GetWednesdayFlag() != 0);
	m_checkThursday.SetCheck(Entry.GetThursdayFlag() != 0);
	m_checkFriday.SetCheck(Entry.GetFridayFlag() != 0);
	m_checkSaturday.SetCheck(Entry.GetSaturdayFlag() != 0);

	CString str = "";
	str.Format("%d", Entry.GetEndPeriod());
	GetEditPeriod()->SetWindowText(str);
}

/**********************************************************************/

void CDefEditTimePlanDlg::OnSelectType() 
{
	if ( TRUE == m_bTemplateMode )
	{
		return;
	}

	int nType = m_comboType.GetCurSel();

	if ( nType != m_nLastType )
	{
		FillSelectionCombo();
		m_nLastType = nType;

		switch( nType )
		{
		case 0:
		case 1:
		case 2:
			if (m_comboActionDetail.GetCount() >= 1)
			{
				m_comboActionDetail.SetCurSel(0);
			}
			else
			{
				ForceSelection(0);
			}
			break;

		case 3:
		case 4:
			GetEditSelection() -> SetWindowText( "0" );
			break;
		}
	}
}

/**********************************************************************/

void CDefEditTimePlanDlg::FillSelectionCombo()
{
	if (TRUE == m_bTemplateMode)
	{
		return;
	}

	m_comboActionDetail.ResetContent();

	switch (m_comboType.GetCurSel())
	{
	case 0:
		ShowAndEnableWindow(&m_buttonEdit, FALSE);
		ShowAndEnableWindow(&m_comboActionDetail, TRUE);
		ShowAndEnableWindow(GetEditSelection(), FALSE);

		m_comboActionDetail.AddItem(",None");
		for (int p = 0; p < MAX_PLU_PRICES; p++)
		{
			m_comboActionDetail.AddItem(DataManager.PriceText.GetTabbedComboText(p));
		}

		for (int n = 0; n < m_comboActionDetail.GetCount(); n++)
		{
			m_comboActionDetail.SetItemData(n, n);
		}

		break;

	case 1:
	{
		ShowAndEnableWindow(&m_buttonEdit, TRUE);
		ShowAndEnableWindow(&m_comboActionDetail, TRUE);
		ShowAndEnableWindow(GetEditSelection(), FALSE);

		for (int n = 0; n < DataManager.Promotion.GetSize(); n++)
		{
			int nPromoNo = DataManager.Promotion.GetPromoNoByIndex(n);
			m_comboActionDetail.AddItem(DataManager.Promotion.GetTabbedComboText(nPromoNo));
			m_comboActionDetail.SetItemData(n, nPromoNo);
		}

		m_comboActionDetail.InsertItem(0, "P00,None");
		m_comboActionDetail.SetItemData(0, 0);
	}
	break;

	case 2:
	{
		ShowAndEnableWindow(&m_buttonEdit, TRUE);
		ShowAndEnableWindow(&m_comboActionDetail, TRUE);
		ShowAndEnableWindow(GetEditSelection(), FALSE);

		for (int n = 0; n < DataManager.MixMatch.GetSize(); n++)
		{
			int nMixMatchNo = DataManager.MixMatch.GetMixMatchNoByIndex(n);
			m_comboActionDetail.AddItem(DataManager.MixMatch.GetTabbedComboText(nMixMatchNo));
			m_comboActionDetail.SetItemData(n, nMixMatchNo);
		}

		m_comboActionDetail.InsertItem(0, "M00,None");
		m_comboActionDetail.SetItemData(0, 0);
	}
	break;

	case 3:
	case 4:
		ShowAndEnableWindow(&m_buttonEdit, FALSE);
		ShowAndEnableWindow(&m_comboActionDetail, FALSE);
		ShowAndEnableWindow(GetEditSelection(), TRUE);
		break;
	}
}

/**********************************************************************/

void CDefEditTimePlanDlg::ForceSelection(int nSelection)
{
	if (TRUE == m_bTemplateMode)
	{
		return;
	}

	switch (m_comboType.GetCurSel())
	{
	case 0:
	case 1:
	case 2:
	{
		if (m_comboActionDetail.SelectUsingItemData(nSelection) == FALSE)
		{
			CString strType = "";
			switch (m_comboType.GetCurSel())
			{
			case 0:	strType = "B";	break;
			case 1:	strType = "P";	break;
			case 2:	strType = "M";	break;
			}

			CString str;
			str.Format("%s%2.2d,Unknown",
				(const char*)strType,
				nSelection);

			m_comboActionDetail.InsertItem(0, str);
			m_comboActionDetail.SetItemData(0, nSelection);
			m_comboActionDetail.SetCurSel(0);
		}

		m_comboActionDetail.EnableWindow(m_comboActionDetail.GetCount() > 1);
	}
	break;

	case 3:
	case 4:
	{
		CString strNum = "";
		strNum.Format("%d", nSelection);
		GetEditSelection()->SetWindowText(strNum);
	}
	break;
	}
}

/**********************************************************************/

void CDefEditTimePlanDlg::OnSpinButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if (pNMUpDown->iDelta > 0)
	{
		SpinPrevious();
	}
	else
	{
		SpinNext();
	}

	*pResult = 0;
}

/**********************************************************************/

void CDefEditTimePlanDlg::OnDefaultButton() 
{
	SpinNext();
}

/**********************************************************************/

void CDefEditTimePlanDlg::OnButtonEdit()
{
	switch( m_comboType.GetCurSel() )
	{
	case 1:
		EditPromo();
		break;

	case 2:
		EditMixMatch();
		break;
	}
}

/**********************************************************************/

void CDefEditTimePlanDlg::OnButtonAll()
{
	SetWeekdayFlags(TRUE);
}

/**********************************************************************/

void CDefEditTimePlanDlg::OnButtonNone()
{
	SetWeekdayFlags(FALSE);
}

/**********************************************************************/

void CDefEditTimePlanDlg::SetWeekdayFlags(bool b)
{
	m_checkSunday.SetCheck(b);
	m_checkMonday.SetCheck(b);
	m_checkTuesday.SetCheck(b);
	m_checkWednesday.SetCheck(b);
	m_checkThursday.SetCheck(b);
	m_checkFriday.SetCheck(b);
	m_checkSaturday.SetCheck(b);
}

/**********************************************************************/

void CDefEditTimePlanDlg::EditPromo()
{
	if (TRUE == m_bTemplateMode)
	{
		return;
	}

	int nSel = m_comboActionDetail.GetCurSel();
	if (nSel < 0 || nSel >= m_comboActionDetail.GetCount())
	{
		return;
	}

	int nOldPromoNo = m_comboActionDetail.GetItemData(nSel);

	if (nOldPromoNo == 0)
	{
		int nNewPromoNo = DataManager.Promotion.FindFirstFreeNumber();

		if (nNewPromoNo < 1)
		{
			Prompter.Error("All available promotions are already in use.");
			return;
		}

		CNewRecordNumberDlg dlg(NODE_PROMOTION, this);
		dlg.m_nNumber = nNewPromoNo;
		if (dlg.DoModal() != IDOK)
		{
			return;
		}

		int nPromoIdx = 0;
		nNewPromoNo = dlg.m_nNumber;
		if (DataManager.Promotion.FindTableByNumber(nNewPromoNo, nPromoIdx))
		{
			Prompter.Error("The selected promotion is already in use.");
			return;
		}

		CPromotionsCSVRecord Promo;
		Promo.SetPromoNo(nNewPromoNo);
		Promo.SetDescription("");
		DataManager.Promotion.InsertAt(nPromoIdx, Promo);

		DataManager.Department.SetDeptZero("None");

		CPropertySheetPromotion propSheet(IDC_SPIN, FALSE, nPromoIdx, this);
		if (propSheet.DoModal() == IDOK)
		{
			propSheet.SaveRecord();
		}

		DataManager.Department.SetDeptZero("PROMO");

		FillSelectionCombo();
		ForceSelection(nNewPromoNo);
	}
	else
	{
		int nPromoIdx;
		if (DataManager.Promotion.FindTableByNumber(nOldPromoNo, nPromoIdx) == TRUE)
		{
			DataManager.Department.SetDeptZero("None");

			CPropertySheetPromotion propSheet(IDC_SPIN, FALSE, nPromoIdx, this);
			if (propSheet.DoModal() == IDOK)
			{
				propSheet.SaveRecord();
				FillSelectionCombo();
				ForceSelection(nOldPromoNo);
			}

			DataManager.Department.SetDeptZero("PROMO");
		}
		else
		{
			if (nOldPromoNo >= 1 && nOldPromoNo <= Promotions::PromoNo.Max)
			{
				CPromotionsCSVRecord Promo;
				Promo.SetPromoNo(nOldPromoNo);
				Promo.SetDescription("");
				DataManager.Promotion.InsertAt(nPromoIdx, Promo);

				DataManager.Department.SetDeptZero("None");

				CPropertySheetPromotion propSheet(IDC_SPIN, FALSE, nPromoIdx, this);
				if (propSheet.DoModal() == IDOK)
				{
					propSheet.SaveRecord();
				}

				DataManager.Department.SetDeptZero("PROMO");

				FillSelectionCombo();
				ForceSelection(nOldPromoNo);
			}
		}
	}
}

/**********************************************************************/

void CDefEditTimePlanDlg::EditMixMatch()
{
	if (TRUE == m_bTemplateMode)
	{
		return;
	}

	int nSel = m_comboActionDetail.GetCurSel();
	if (nSel < 0 || nSel >= m_comboActionDetail.GetCount())
	{
		return;
	}

	int nOldMixMatchNo = m_comboActionDetail.GetItemData(nSel);

	if (nOldMixMatchNo == 0)
	{
		int nNewMixMatchNo = DataManager.MixMatch.FindFirstFreeNumber();

		if (nNewMixMatchNo < 1)
		{
			Prompter.Error("All available mix match tables are already in use.");
			return;
		}

		CNewRecordNumberDlg dlg(NODE_MIXMATCH, this);
		dlg.m_nNumber = nNewMixMatchNo;
		if (dlg.DoModal() != IDOK)
		{
			return;
		}

		int nMixMatchIdx = 0;
		nNewMixMatchNo = dlg.m_nNumber;
		if (DataManager.MixMatch.FindTableByNumber(nNewMixMatchNo, nMixMatchIdx))
		{
			Prompter.Error("The selected mix match table is already in use.");
			return;
		}

		CMixMatchCSVRecord MixMatch;
		MixMatch.SetMixMatchNo(nNewMixMatchNo);
		MixMatch.SetDescription("");
		DataManager.MixMatch.InsertAt(nMixMatchIdx, MixMatch);

		CPropertySheetMixMatch propSheet(IDC_SPIN, FALSE, nMixMatchIdx, this);
		if (propSheet.DoModal() == IDOK)
		{
			propSheet.SaveRecord();
		}

		FillSelectionCombo();
		ForceSelection(nNewMixMatchNo);
	}
	else
	{
		int nMixMatchIdx = 0;
		if (DataManager.MixMatch.FindTableByNumber(nOldMixMatchNo, nMixMatchIdx) == TRUE)
		{
			CPropertySheetMixMatch propSheet(IDC_SPIN, FALSE, nMixMatchIdx, this);
			if (propSheet.DoModal() == IDOK)
			{
				propSheet.SaveRecord();
				FillSelectionCombo();
				ForceSelection(nOldMixMatchNo);
			}
		}
		else
		{
			if (nOldMixMatchNo >= 1 && nOldMixMatchNo <= MixMatch::MixMatchNo.Max)
			{
				CMixMatchCSVRecord MixMatch;
				MixMatch.SetMixMatchNo(nOldMixMatchNo);
				MixMatch.SetDescription("");
				DataManager.MixMatch.InsertAt(nMixMatchIdx, MixMatch);

				CPropertySheetMixMatch propSheet(IDC_SPIN, FALSE, nMixMatchIdx, this);
				if (propSheet.DoModal() == IDOK)
				{
					propSheet.SaveRecord();
				}

				FillSelectionCombo();
				ForceSelection(nOldMixMatchNo);
			}
		}
	}
}

/**********************************************************************/

void CDefEditTimePlanDlg::SpinPrevious()
{
	if (UpdateData(TRUE) == TRUE)
	{
		if (m_nIndex > m_nMinIndex)
		{
			SaveData();
			m_nIndex--;
			GetData();

			if (m_pListDataDlg != NULL)
			{
				m_pListDataDlg->SelectLine(m_nIndex);
			}
		}
	}
}

/**********************************************************************/

void CDefEditTimePlanDlg::SpinNext()
{
	if (UpdateData(TRUE) == TRUE)
	{
		if (m_nIndex < m_nMaxIndex)
		{
			SaveData();
			m_nIndex++;
			GetData();

			if (m_pListDataDlg != NULL)
			{
				m_pListDataDlg->SelectLine(m_nIndex);
			}
		}
	}
}

/**********************************************************************/

void CDefEditTimePlanDlg::SaveData()
{
	CTimePlanEntriesCSVRecord Entry;
	m_pTimePlanEntryArray->GetAt(m_nIndex, Entry);

	TrimSpaces(m_strDescription, FALSE);
	if (m_strDescription.IsEmpty())
	{
		if (FALSE == m_bTemplateMode)
		{
			m_strDescription = Entry.GetDefaultText(FALSE);
		}
		else
		{
			m_strDescription.Format("Template %d", Entry.GetEntryNo());
		}
	}

	Entry.SetName(m_strDescription);
	Entry.SetActiveFlag(m_checkEnable.GetCheck() != 0);

	{

		if (FALSE == m_bTemplateMode)
		{
			int nType = m_comboType.GetCurSel();
			int nSelection = 0;

			switch (nType)
			{
			case 0:
			case 1:
			case 2:
			{
				int nComboSel = m_comboActionDetail.GetCurSel();
				if ((nComboSel >= 0) && (nComboSel < m_comboActionDetail.GetCount()))
				{
					nSelection = m_comboActionDetail.GetItemData(nComboSel);
				}
			}
			break;

			case 3:
			case 4:
			{
				CString strNum;
				GetEditSelection()->GetWindowText(strNum);
				nSelection = atoi(strNum);
			}
			break;
			}

			Entry.SetTypeAndSelection(nType, nSelection);
		}
	}

	COleDateTime dateStart;
	m_DatePickerFrom.GetTime(dateStart);
	Entry.SetStartDate(dateStart);

	COleDateTime dateEnd;
	m_DatePickerTo.GetTime(dateEnd);
	Entry.SetEndDate(dateEnd);

	COleDateTime timeStart;
	m_TimePickerStart.GetTime(timeStart);
	Entry.SetStartHour(timeStart.GetHour());
	Entry.SetStartMinute(timeStart.GetMinute());

	COleDateTime timeEnd;
	m_TimePickerEnd.GetTime(timeEnd);
	Entry.SetEndHour(timeEnd.GetHour());
	Entry.SetEndMinute(timeEnd.GetMinute());

	Entry.SetEndPeriod(m_nPeriod);

	Entry.SetSundayFlag(m_checkSunday.GetCheck() != 0);
	Entry.SetMondayFlag(m_checkMonday.GetCheck() != 0);
	Entry.SetTuesdayFlag(m_checkTuesday.GetCheck() != 0);
	Entry.SetWednesdayFlag(m_checkWednesday.GetCheck() != 0);
	Entry.SetThursdayFlag(m_checkThursday.GetCheck() != 0);
	Entry.SetFridayFlag(m_checkFriday.GetCheck() != 0);
	Entry.SetSaturdayFlag(m_checkSaturday.GetCheck() != 0);

	if (FALSE == m_bTemplateMode)
	{
		int nType = Entry.GetType();
		int nSelection = Entry.GetSelection();

		if ((nType != m_nInitialType) || (nSelection != m_nInitialSelection))
		{
			if (Entry.GetName() == m_strInitialName)
			{
				Entry.SetName(Entry.GetDefaultText(FALSE));
			}
		}
	}

	m_pTimePlanEntryArray->SetAt(m_nIndex, Entry);

	m_WarnCancelAfterChange.SetFlag();
}

/**********************************************************************/

void CDefEditTimePlanDlg::OnOK()
{
	if (UpdateData(TRUE) == TRUE)
	{
		switch (m_comboType.GetCurSel())
		{
		case 3:
		case 4:
		{
			__int64 nMax = 0x7FFFFFFF;

			CString strNum = "";
			GetEditSelection()->GetWindowText(strNum);
			__int64 nSel = _atoi64(strNum);

			if ((nSel < 0) || (nSel > nMax))
			{
				CString strError;
				strError.Format("Please enter a number between 0 and %I64d", nMax);
				Prompter.Error(strError);
				GetEditSelection()->SetSel(0, -1);
				GetEditSelection()->SetFocus();
				return;
			}
		}
		break;
		}

		SaveData();
		EndDialog(IDOK);
	}
}

/**********************************************************************/
