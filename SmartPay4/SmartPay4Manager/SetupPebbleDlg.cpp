/**********************************************************************/
#include "..\SmartPay4Shared\SQLConnectionPool.h"
#include "..\SmartPay4Shared\SQLHelpers.h"
/**********************************************************************/
#include "..\SmartPay4Shared\SQLTable_PebblePaymentType\SQLRepositoryPebblePaymentType.h"
#include "..\SmartPay4Shared\SQLTable_PebblePaymentType\SQLRecordSetPebblePaymentType.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
/**********************************************************************/
#include "GroupNoDlg.h"
#include "SetupPebblePaymentDlg.h"
#include "SetupPebbleSPOSPayTypeDlg.h"
#include "SQLTranSavePebblePayment.h"
/**********************************************************************/
#include "SetupPebbleDlg.h"
/**********************************************************************/
static const char* szNOTDEFINED = "Not defined";
/**********************************************************************/

CSingleListContentHelpersPebblePayment::CSingleListContentHelpersPebblePayment(CSQLRowSetPebblePayment& RowSet)
	: CSingleListContentHelpers(), m_RowSet(RowSet)
{
}

/**********************************************************************/

int CSingleListContentHelpersPebblePayment::GetArraySize()
{
	return m_RowSet.GetSize();
}

/**********************************************************************/

bool CSingleListContentHelpersPebblePayment::PrepareLine(int nIndex, CString& strLine)
{
	CSQLRowPebblePaymentType RowPayment;
	m_RowSet.GetRow(nIndex, RowPayment);

	strLine.Format("%d\t%s\t%s",
		RowPayment.GetSPOSPaymentType(),
		(const char*)RowPayment.GetPebblePaymentName(),
		(const char*)RowPayment.GetPebbleTranTypeName());

	return TRUE;
}

/**********************************************************************/

void CSingleListContentHelpersPebblePayment::DeleteDatabaseEntry(int nIndex)
{
	if ((nIndex >= 0) && (nIndex < m_RowSet.GetSize()))
	{
		m_RowSet.RemoveRow(nIndex);
		m_bDeletionEffect = TRUE;
	}
}

/**********************************************************************/

void CSingleListContentHelpersPebblePayment::FilterSelectedItemArray(CReportConsolidationArray<CSortedIntItem>& arraySelected)
{
	m_bPreventDeletion = FALSE;
}

/**********************************************************************/

void CSingleListContentHelpersPebblePayment::SetColumns(CSSListCtrlVirtual& list, CWnd* pParent)
{
	list.SubclassDlgItem(IDC_LIST, pParent);
	list.InsertColumn(0, "SPOS", LVCFMT_LEFT, 50);
	list.InsertColumn(1, "Sale Name", LVCFMT_LEFT, 130);
	list.InsertColumn(2, "Topup Type", LVCFMT_LEFT, 100);
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSetupPebbleDlg::CSetupPebbleDlg(CWnd* pParent)
	: CSSAutoShutdownDialog( AutoShutdownHelper, CSetupPebbleDlg::IDD, pParent)
{
	m_RowSetPayment.LoadPebblePayments();
	m_pContentHelpers = new CSingleListContentHelpersPebblePayment(m_RowSetPayment);
	m_pDisplayHelpers = new CSingleListDisplayHelpers(m_listPayment, *m_pContentHelpers);

	m_pDisplayHelpers->SetTopSingleText("payment type");
	m_pDisplayHelpers->SetTopPluralText("payment types");
	m_pDisplayHelpers->SetBottomSingleText("payment type");
	m_pDisplayHelpers->SetBottomPluralText("payment types");
}

/**********************************************************************/

CSetupPebbleDlg::~CSetupPebbleDlg()
{
	delete m_pDisplayHelpers;
	delete m_pContentHelpers;
}

/**********************************************************************/

void CSetupPebbleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	/*****/
	DDX_Control(pDX, IDC_EDIT_INSTID, m_editInstallationID);
	DDX_Control(pDX, IDC_EDIT_ORGID, m_editOrganisationID);
	DDX_Control(pDX, IDC_COMBO_PID, m_comboPID);
	DDX_Control(pDX, IDC_COMBO_UPN, m_comboUPN);
	DDX_Control(pDX, IDC_DATEPICKER_FROM_TOPUP, m_DatePickerFromTopup);
	DDX_Control(pDX, IDC_TIMEPICKER_FROM_TOPUP, m_TimePickerFromTopup);
	DDX_Control(pDX, IDC_DATEPICKER_FROM_SALE, m_DatePickerFromSale);
	DDX_Control(pDX, IDC_TIMEPICKER_FROM_SALE, m_TimePickerFromSale);
	DDX_Control(pDX, IDC_CHECK_CLEAR_RECENT, m_checkClearRecent);
	DDX_Control(pDX, IDC_STATIC_RETAIN_RECENT, m_staticRetainRecent);
	DDX_Control(pDX, IDC_CHECK_CLEAR_PAYMENT, m_checkClearPayment);
	DDX_Control(pDX, IDC_STATIC_RETAIN_PAYMENT, m_staticRetainPayment);
	DDX_Control(pDX, IDC_EDIT_OFFICEPLU, m_editOfficePLU);
	DDX_Control(pDX, IDC_COMBO_OFFICEVAT, m_comboOfficeVAT);
	DDX_Control(pDX, IDC_TAB_EPOS, m_tabEPOS);
	DDX_Control(pDX, IDC_STATIC_VATA, m_staticVAT[0]);
	DDX_Control(pDX, IDC_STATIC_VATB, m_staticVAT[1]);
	DDX_Control(pDX, IDC_STATIC_VATC, m_staticVAT[2]);
	DDX_Control(pDX, IDC_STATIC_VATD, m_staticVAT[3]);
	DDX_Control(pDX, IDC_STATIC_VATE, m_staticVAT[4]);
	DDX_Control(pDX, IDC_STATIC_VATF, m_staticVAT[5]);
	DDX_Control(pDX, IDC_STATIC_VATG, m_staticVAT[6]);
	DDX_Control(pDX, IDC_STATIC_VATH, m_staticVAT[7]);
	DDX_Control(pDX, IDC_STATIC_VATI, m_staticVAT[8]);
	DDX_Control(pDX, IDC_STATIC_VATJ, m_staticVAT[9]);
	DDX_Control(pDX, IDC_STATIC_VATK, m_staticVAT[10]);
	DDX_Control(pDX, IDC_STATIC_VATL, m_staticVAT[11]);
	DDX_Control(pDX, IDC_COMBO_VATCODEA, m_comboVATCode[0]);
	DDX_Control(pDX, IDC_COMBO_VATCODEB, m_comboVATCode[1]);
	DDX_Control(pDX, IDC_COMBO_VATCODEC, m_comboVATCode[2]);
	DDX_Control(pDX, IDC_COMBO_VATCODED, m_comboVATCode[3]);
	DDX_Control(pDX, IDC_COMBO_VATCODEE, m_comboVATCode[4]);
	DDX_Control(pDX, IDC_COMBO_VATCODEF, m_comboVATCode[5]);
	DDX_Control(pDX, IDC_COMBO_VATCODEG, m_comboVATCode[6]);
	DDX_Control(pDX, IDC_COMBO_VATCODEH, m_comboVATCode[7]);
	DDX_Control(pDX, IDC_COMBO_VATCODEI, m_comboVATCode[8]);
	DDX_Control(pDX, IDC_COMBO_VATCODEJ, m_comboVATCode[9]);
	DDX_Control(pDX, IDC_COMBO_VATCODEK, m_comboVATCode[10]);
	DDX_Control(pDX, IDC_COMBO_VATCODEL, m_comboVATCode[11]);
	DDX_Control(pDX, IDC_STATIC_VATNOTE1, m_staticVATNote[0]);
	DDX_Control(pDX, IDC_STATIC_VATNOTE2, m_staticVATNote[1]);
	DDX_Control(pDX, IDC_STATIC_VATNOTE3, m_staticVATNote[2]);
	DDX_Control(pDX, IDC_STATIC_VATNOTE4, m_staticVATNote[3]);
	DDX_Control(pDX, IDC_STATIC_VATNOTE5, m_staticVATNote[4]);
	DDX_Control(pDX, IDC_STATIC_VATNOTE6, m_staticVATNote[5]);
	DDX_Control(pDX, IDC_BUTTON_PAYADD, m_buttonPayAdd);
	DDX_Control(pDX, IDC_BUTTON_PAYEDIT, m_buttonPayEdit);
	DDX_Control(pDX, IDC_BUTTON_PAYDELETE, m_buttonPayDelete);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSetupPebbleDlg, CSSAutoShutdownDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_EPOS, OnTcnSelchangeTabs)
	ON_BN_CLICKED(IDC_CHECK_CLEAR_RECENT, OnToggleRecent)
	ON_BN_CLICKED(IDC_CHECK_CLEAR_PAYMENT, OnTogglePayment)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST, OnKillFocusPaymentList)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST, OnSetFocusPaymentList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickPaymentList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoPaymentList)
	ON_BN_CLICKED(IDC_BUTTON_PAYADD, OnButtonPayAdd)
	ON_BN_CLICKED(IDC_BUTTON_PAYEDIT, OnButtonPayEdit)
	ON_BN_CLICKED(IDC_BUTTON_PAYDELETE, OnButtonPayDelete)
	ON_CBN_SELCHANGE(IDC_COMBO_VATCODEA, OnSelectVATCode)
	ON_CBN_SELCHANGE(IDC_COMBO_VATCODEB, OnSelectVATCode)
	ON_CBN_SELCHANGE(IDC_COMBO_VATCODEC, OnSelectVATCode)
	ON_CBN_SELCHANGE(IDC_COMBO_VATCODED, OnSelectVATCode)
	ON_CBN_SELCHANGE(IDC_COMBO_VATCODEE, OnSelectVATCode)
	ON_CBN_SELCHANGE(IDC_COMBO_VATCODEF, OnSelectVATCode)
	ON_CBN_SELCHANGE(IDC_COMBO_VATCODEG, OnSelectVATCode)
	ON_CBN_SELCHANGE(IDC_COMBO_VATCODEH, OnSelectVATCode)
	ON_CBN_SELCHANGE(IDC_COMBO_VATCODEI, OnSelectVATCode)
	ON_CBN_SELCHANGE(IDC_COMBO_VATCODEJ, OnSelectVATCode)
	ON_CBN_SELCHANGE(IDC_COMBO_VATCODEK, OnSelectVATCode)
	ON_CBN_SELCHANGE(IDC_COMBO_VATCODEL, OnSelectVATCode)
END_MESSAGE_MAP()

/**********************************************************************/

void CSetupPebbleDlg::CSSAutoShutdownPreInitDialog()
{
	m_config.LoadRow(NULL);
}

/**********************************************************************/

BOOL CSetupPebbleDlg::CSSAutoShutdownPostInitDialog()
{
	m_pContentHelpers->SetColumns(m_listPayment,this);
	m_pDisplayHelpers->PrepareDatabaseList(TRUE, FALSE);
	MoveControl(&m_listPayment, 299, 49, 210, 142);
	MoveControl(&m_buttonPayAdd, 513, 59, 40, 14);
	MoveControl(&m_buttonPayEdit, 513, 84, 40, 14);
	MoveControl(&m_buttonPayDelete, 513, 109, 40, 14);

	m_tabEPOS.InsertItem(0, "VAT Rates");
	m_tabEPOS.InsertItem(1, "Payment Types");
	m_tabEPOS.SetCurSel(0);
	SelectTab();

	if (m_RowSetPayment.GetSize() != 0)
	{
		SelectPaymentLine(0);
	}

	SubclassEdit(IDC_EDIT_CLEAR_RECENT, SS_NUM, 3, "%d");
	SubclassEdit(IDC_EDIT_CLEAR_PAYMENT, SS_NUM, 3, "%d");

	SubclassEdit(IDC_EDIT_VATA, SS_NUM_DP, 7, "%.4f");
	SubclassEdit(IDC_EDIT_VATB, SS_NUM_DP, 7, "%.4f");
	SubclassEdit(IDC_EDIT_VATC, SS_NUM_DP, 7, "%.4f");
	SubclassEdit(IDC_EDIT_VATD, SS_NUM_DP, 7, "%.4f");
	SubclassEdit(IDC_EDIT_VATE, SS_NUM_DP, 7, "%.4f");
	SubclassEdit(IDC_EDIT_VATF, SS_NUM_DP, 7, "%.4f");
	SubclassEdit(IDC_EDIT_VATG, SS_NUM_DP, 7, "%.4f");
	SubclassEdit(IDC_EDIT_VATH, SS_NUM_DP, 7, "%.4f");
	SubclassEdit(IDC_EDIT_VATI, SS_NUM_DP, 7, "%.4f");
	SubclassEdit(IDC_EDIT_VATJ, SS_NUM_DP, 7, "%.4f");
	SubclassEdit(IDC_EDIT_VATK, SS_NUM_DP, 7, "%.4f");
	SubclassEdit(IDC_EDIT_VATL, SS_NUM_DP, 7, "%.4f");

	for (int n = 0; n < 12; n++)
	{
		CPebbleVATInfo VatInfo = m_config.m_Row.GetVATInfo(n);

		m_comboVATCode[n].AddString("S");
		m_comboVATCode[n].AddString("R");
		m_comboVATCode[n].AddString("C");
		m_comboVATCode[n].AddString("O");
		m_comboVATCode[n].AddString("Z");
		m_comboVATCode[n].AddString("X");

		{
			int nCode = VatInfo.m_nVATCode;
			if ((nCode < 0) || (nCode > 5))
			{
				nCode = 0;
			}
			m_comboVATCode[n].SetCurSel(nCode);
		}

		SetEditBoxDouble(*GetEditVAT(n), VatInfo.m_dVATRate, 4);
	}

	OnSelectVATCode();
	
	if (m_config.m_Row.GetRecentCacheDays() == 0)
	{
		m_checkClearRecent.SetCheck(FALSE);
		m_staticRetainRecent.EnableWindow(FALSE);
		GetEditClearRecent()->EnableWindow(FALSE);
		SetEditBoxInt(*GetEditClearRecent(), 999);
	}
	else
	{
		m_checkClearRecent.SetCheck(TRUE);
		m_staticRetainRecent.EnableWindow(TRUE);
		GetEditClearRecent()->EnableWindow(TRUE);
		SetEditBoxInt(*GetEditClearRecent(), m_config.m_Row.GetRecentCacheDays());
	}

	if (m_config.m_Row.GetPaymentCacheDays() == 0)
	{
		m_checkClearPayment.SetCheck(FALSE);
		m_staticRetainPayment.EnableWindow(FALSE);
		GetEditClearPayment()->EnableWindow(FALSE);
		SetEditBoxInt(*GetEditClearPayment(), 999);
	}
	else
	{
		m_checkClearPayment.SetCheck(TRUE);
		m_staticRetainPayment.EnableWindow(TRUE);
		GetEditClearPayment()->EnableWindow(TRUE);
		SetEditBoxInt(*GetEditClearPayment(), m_config.m_Row.GetPaymentCacheDays());
	}

	m_DatePickerFromTopup.SetFormat("ddd dd MMM yyy");
	m_TimePickerFromTopup.SetFormat("HH:mm:ss");
	m_DatePickerFromSale.SetFormat("ddd dd MMM yyy");
	m_TimePickerFromSale.SetFormat("HH:mm:ss");

	m_editInstallationID.LimitText(26);
	m_editOrganisationID.LimitText(26);

	m_editInstallationID.SetWindowText(m_config.m_Row.GetProductionInstallationID());
	m_editOrganisationID.SetWindowText(m_config.m_Row.GetProductionOrganisationID());

	m_comboPID.AddString(szNOTDEFINED);
	m_comboUPN.AddString(szNOTDEFINED);

	for (int n = 1; n <= nMAX_INFOTEXTS; n++)
	{
		CSQLRepositoryUserText RepoText;
		CString strDisplayText = RepoText.GetDisplayInfoText(n);
		m_comboPID.AddString(strDisplayText);
		m_comboUPN.AddString(strDisplayText);
	}

	m_comboPID.SetCurSel(m_config.m_Row.GetMISInfoIndex());
	m_comboUPN.SetCurSel(m_config.m_Row.GetUPNInfoIndex());

	COleDateTime dateMin = COleDateTime(2020, 1, 1, 0, 0, 0);
	COleDateTime dateMax = COleDateTime(2200, 1, 1, 0, 0, 0);
	
	{
		COleDateTime dateSet = COleDateTime(2022, 1, 1, 0, 0, 0);

		{
			CString strDate = m_config.m_Row.GetLastImportDateTopup();
			if (strDate.GetLength() == 8)
			{
				COleDateTime dateTry = COleDateTime(atoi(strDate.Left(4)), atoi(strDate.Mid(4, 2)), atoi(strDate.Right(2)), 0, 0, 0);
				if (dateTry.m_status == COleDateTime::valid)
				{
					if ((dateTry >= dateMin) && (dateTry <= dateMax))
					{
						dateSet = dateTry;
					}
				}
			}
		}

		m_DatePickerFromTopup.SetTime(dateSet);
		m_DatePickerFromTopup.SetRange(&dateMin, &dateMax);

		CString strTime = m_config.m_Row.GetLastImportTimeTopup();
		if (strTime.GetLength() == 6)
		{
			COleDateTime dateTry = COleDateTime(2022, 1, 1, atoi(strTime.Left(2)), atoi(strTime.Mid(2, 2)), atoi(strTime.Right(2)));
			if (dateTry.m_status == COleDateTime::valid)
			{
				dateSet = dateTry;
			}
		}

		m_TimePickerFromTopup.SetTime(dateSet);
	}

	{
		COleDateTime dateSet = COleDateTime(2022, 1, 1, 0, 0, 0);

		{
			CString strDate = m_config.m_Row.GetLastImportDateSale();
			if (strDate.GetLength() == 8)
			{
				COleDateTime dateTry = COleDateTime(atoi(strDate.Left(4)), atoi(strDate.Mid(4, 2)), atoi(strDate.Right(2)), 0, 0, 0);
				if (dateTry.m_status == COleDateTime::valid)
				{
					if ((dateTry >= dateMin) && (dateTry <= dateMax))
					{
						dateSet = dateTry;
					}
				}
			}
		}

		m_DatePickerFromSale.SetTime(dateSet);
		m_DatePickerFromSale.SetRange(&dateMin, &dateMax);

		CString strTime = m_config.m_Row.GetLastImportTimeSale();
		if (strTime.GetLength() == 6)
		{
			COleDateTime dateTry = COleDateTime(2022, 1, 1, atoi(strTime.Left(2)), atoi(strTime.Mid(2, 2)), atoi(strTime.Right(2)));
			if (dateTry.m_status == COleDateTime::valid)
			{
				dateSet = dateTry;
			}
		}

		m_TimePickerFromSale.SetTime(dateSet);
	}

	m_editOfficePLU.LimitText(14);
	SetEditBoxInt64(m_editOfficePLU, m_config.m_Row.GetOfficeMealPluNo());

	{
		CString strVATBand = "A";
		for (int n = 0; n < 12; n++)
		{
			strVATBand.SetAt(0, 'A' + n);
			m_comboOfficeVAT.AddString(strVATBand);
		}
	}

	{
		int nBand = m_config.m_Row.GetOfficeMealVATBand();
		if ((nBand >= 1) && (nBand <= 12))
		{
			m_comboOfficeVAT.SetCurSel(nBand - 1);
		}
		else
		{
			m_comboOfficeVAT.SetCurSel(0);
		}
	}

	return TRUE;
}

/**********************************************************************/

void CSetupPebbleDlg::OnSelectVATCode()
{
	for (int n = 0; n < 12; n++)
	{
		switch (m_comboVATCode[n].GetCurSel())
		{
		case 0:
		case 1:
		case 2:
			GetEditVAT(n)->EnableWindow(TRUE);	
			break;

		default:
			GetEditVAT(n)->EnableWindow(FALSE);
			SetEditBoxDouble(*GetEditVAT(n), 0.0, 4);
			break;
		}
	}
}

/**********************************************************************/

CEdit* CSetupPebbleDlg::GetEditVAT(int n)
{
	switch (n)
	{
	case 0: return GetEdit(IDC_EDIT_VATA);
	case 1: return GetEdit(IDC_EDIT_VATB);
	case 2: return GetEdit(IDC_EDIT_VATC);
	case 3: return GetEdit(IDC_EDIT_VATD);
	case 4: return GetEdit(IDC_EDIT_VATE);
	case 5: return GetEdit(IDC_EDIT_VATF);
	case 6: return GetEdit(IDC_EDIT_VATG);
	case 7: return GetEdit(IDC_EDIT_VATH);
	case 8: return GetEdit(IDC_EDIT_VATI);
	case 9: return GetEdit(IDC_EDIT_VATJ);
	case 10: return GetEdit(IDC_EDIT_VATK);
	case 11: return GetEdit(IDC_EDIT_VATL);
	default: return NULL;
	}
}

/**********************************************************************/

int CSetupPebbleDlg::GetPaymentArraySize()
{
	return m_pContentHelpers->GetArraySize();
}

/**********************************************************************/

void CSetupPebbleDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (nIDCtl == IDC_LIST)
	{
		m_listPayment.AdjustRowHeight(lpMeasureItemStruct, 10);
	}
}

/**********************************************************************/

void CSetupPebbleDlg::SelectPaymentLine(int nIndex)
{
	m_pDisplayHelpers->SelectDatabaseLine(nIndex);
}

/**********************************************************************/

void CSetupPebbleDlg::OnButtonPayAdd()
{
	CString strTitle = "Add Payment Type";

	CSetupPebbleSPOSPayTypeDlg dlg(this);

	if (dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	int nPaymentType = dlg.m_nPaymentType;

	if (nPaymentType >= 0)
	{
		int nPos = 0;
		if (m_RowSetPayment.FindPayment(nPaymentType, nPos) == TRUE)
		{
			Prompter.Error("This Payment Type is already mapped to Pebble", strTitle);
			return;
		}

		CSQLRowPebblePaymentType RowPayment;
		RowPayment.SetSPOSPaymentType(nPaymentType);
		m_RowSetPayment.InsertRow(nPos, RowPayment);

		CSetupPebblePaymentDlg dlgEdit( TRUE, RowPayment, this);

		if (dlgEdit.DoModal() == IDOK)
		{
			m_RowSetPayment.UpdateRow(nPos, RowPayment);
			m_listPayment.SetItemCountEx(GetPaymentArraySize());
			SelectPaymentLine(nPos);
		}
		else
		{
			m_RowSetPayment.RemoveRow(nPos);
			SelectPaymentLine(nPos);
		}
	}
}

/**********************************************************************/

void CSetupPebbleDlg::OnButtonPayEdit()
{
	int nIndex = GetSafePaymentSelection(0,GetPaymentArraySize() - 1);
	if ((nIndex >= 0) && (nIndex < m_RowSetPayment.GetSize()))
	{
		CSQLRowPebblePaymentType RowPayment;
		m_RowSetPayment.GetRow(nIndex, RowPayment);

		CSetupPebblePaymentDlg dlgPayment(FALSE, RowPayment, this);
		if (dlgPayment.DoModal() == IDOK)
		{
			m_RowSetPayment.UpdateRow(nIndex, RowPayment);
		}

		SelectPaymentLine(nIndex);
	}
}

/**********************************************************************/

void CSetupPebbleDlg::OnButtonPayDelete()
{
	m_pDisplayHelpers->HandleDelete(this);
}

/**********************************************************************/

int CSetupPebbleDlg::GetSafePaymentSelection(int nMin, int nMax)
{
	int nSel = m_listPayment.GetCurSel();
	if ((nSel < nMin) || (nSel > nMax) || (nSel >= m_listPayment.GetItemCount()))
	{
		nSel = -1;
	}

	return nSel;
}

/**********************************************************************/

void CSetupPebbleDlg::OnToggleRecent()
{
	if (IsTicked(m_checkClearRecent))
	{
		m_staticRetainRecent.EnableWindow(TRUE);
		GetEditClearRecent()->EnableWindow(TRUE);
	}
	else
	{
		m_staticRetainRecent.EnableWindow(FALSE);
		GetEditClearRecent()->EnableWindow(FALSE);
	}

	SetEditBoxInt(*GetEditClearRecent(), 999);
}

/**********************************************************************/

void CSetupPebbleDlg::OnTogglePayment()
{
	if (IsTicked(m_checkClearPayment))
	{
		m_staticRetainPayment.EnableWindow(TRUE);
		GetEditClearPayment()->EnableWindow(TRUE);
	}
	else
	{
		m_staticRetainPayment.EnableWindow(FALSE);
		GetEditClearPayment()->EnableWindow(FALSE);
	}

	SetEditBoxInt(*GetEditClearPayment(), 999);
}

/**********************************************************************/

void CSetupPebbleDlg::OnTcnSelchangeTabs(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	SelectTab();
}

/**********************************************************************/

void CSetupPebbleDlg::SelectTab()
{
	int nTabSel = m_tabEPOS.GetCurSel();

	if ((nTabSel < 0) || (nTabSel > 1))
	{
		nTabSel = 0;
	}

	for (int n = 0; n < 12; n++)
	{
		bool bShow = (0 == nTabSel);
		ShowAndEnableWindow(&m_staticVAT[n], bShow);
		ShowAndEnableWindow(GetEditVAT(n), bShow);
		ShowAndEnableWindow(&m_comboVATCode[n], bShow);
	}

	for ( int n = 0; n < 6; n++)
	{
		ShowAndEnableWindow(&m_staticVATNote[n], (0 == nTabSel));
	}

	ShowAndEnableWindow(&m_listPayment, (1 == nTabSel));
	ShowAndEnableWindow(&m_checkClearPayment, (1 == nTabSel));
	ShowAndEnableWindow(&m_staticRetainPayment, (1 == nTabSel));
	ShowAndEnableWindow(GetEditClearPayment(), (1 == nTabSel));
	ShowAndEnableWindow(&m_buttonPayAdd, (1 == nTabSel));
	ShowAndEnableWindow(&m_buttonPayEdit, (1 == nTabSel));
	ShowAndEnableWindow(&m_buttonPayDelete, (1 == nTabSel));
}

/**********************************************************************/

bool CSetupPebbleDlg::PreparePaymentLine(int nIndex)
{
	return m_pContentHelpers->PrepareLine(nIndex, m_strPaymentDisplayLine);
}

/**********************************************************************/

void CSetupPebbleDlg::OnGetDispInfoPaymentList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if ((pDispInfo->item.mask & LVIF_TEXT) != 0)
	{
		int nIndex = pDispInfo->item.iItem;
		PreparePaymentLine(nIndex);
		CCSV csv(m_strPaymentDisplayLine, '\t');
		lstrcpy(pDispInfo->item.pszText, csv.GetString(pDispInfo->item.iSubItem));
	}
	*pResult = 0;
}

/**********************************************************************/

void CSetupPebbleDlg::OnSetFocusPaymentList(NMHDR* pNMHDR, LRESULT* pResult)
{
	//SetDefID(GetDefaultButtonID());
	*pResult = 0;
}

/**********************************************************************/

void CSetupPebbleDlg::OnKillFocusPaymentList(NMHDR* pNMHDR, LRESULT* pResult)
{
	SetDefID(IDOK);
	*pResult = 0;
}

/**********************************************************************/

void CSetupPebbleDlg::OnDoubleClickPaymentList(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnButtonPayEdit();
	*pResult = 0;
}

/**********************************************************************/

void CSetupPebbleDlg::OnOK()
{
	CString strError = "";

	int nMISInfoIndex = m_comboPID.GetCurSel();
	int nUPNInfoIndex = m_comboUPN.GetCurSel();

	if (nUPNInfoIndex != 0)
	{
		if (nUPNInfoIndex == nMISInfoIndex)
		{
			strError = "Conflict between MIS ID and UPN database UserText fields!";								// "Conflict between UPN and Person_id Database UserText fields!"
			m_comboUPN.SetFocus();
		}
	}

	if (strError != "")
	{
		Prompter.Error(strError, "Entry Error");
		return;
	}

	m_config.m_Row.SetProductionInstallationID(GetEditBoxText(m_editInstallationID));
	m_config.m_Row.SetProductionOrganisationID(GetEditBoxText(m_editOrganisationID));
	m_config.m_Row.SetMISInfoIndex(nMISInfoIndex);
	m_config.m_Row.SetUPNInfoIndex(nUPNInfoIndex);

	{
		CString strDateFrom = "";

		COleDateTime dateFrom;
		m_DatePickerFromTopup.GetTime(dateFrom);
		strDateFrom.Format("%4.4d%2.2d%2.2d", dateFrom.GetYear(), dateFrom.GetMonth(), dateFrom.GetDay());

		CString strTimeFrom = "";

		COleDateTime timeFrom;
		m_TimePickerFromTopup.GetTime(timeFrom);
		strTimeFrom.Format("%2.2d%2.2d%2.2d", timeFrom.GetHour(), timeFrom.GetMinute(), timeFrom.GetSecond());

		if ((strDateFrom != m_config.m_Row.GetLastImportDateTopup()) || (strTimeFrom != m_config.m_Row.GetLastImportTimeTopup()))
		{
			m_config.m_Row.SetLastImportDateTopup(strDateFrom);
			m_config.m_Row.SetLastImportTimeTopup(strTimeFrom);
			m_config.m_Row.SetFirstImportTopupFlag(TRUE);
		}
	}

	{
		CString strDateFrom = "";

		COleDateTime dateFrom;
		m_DatePickerFromSale.GetTime(dateFrom);
		strDateFrom.Format("%4.4d%2.2d%2.2d", dateFrom.GetYear(), dateFrom.GetMonth(), dateFrom.GetDay());

		CString strTimeFrom = "";

		COleDateTime timeFrom;
		m_TimePickerFromSale.GetTime(timeFrom);
		strTimeFrom.Format("%2.2d%2.2d%2.2d", timeFrom.GetHour(), timeFrom.GetMinute(), timeFrom.GetSecond());

		if ((strDateFrom != m_config.m_Row.GetLastImportDateSale()) || (strTimeFrom != m_config.m_Row.GetLastImportTimeSale()))
		{
			m_config.m_Row.SetLastImportDateSale(strDateFrom);
			m_config.m_Row.SetLastImportTimeSale(strTimeFrom);
			m_config.m_Row.SetFirstImportSaleFlag(TRUE);
		}
	}

	if (IsTicked(m_checkClearRecent))
	{
		int n = GetEditBoxInt(*GetEditClearRecent());
		
		if (n < 0)
		{
			n = 0;
		}

		m_config.m_Row.SetRecentCacheDays(n);
	}
	else
	{
		m_config.m_Row.SetRecentCacheDays(0);
	}

	if (IsTicked(m_checkClearPayment))
	{
		int n = GetEditBoxInt(*GetEditClearPayment());

		if (n < 0)
		{
			n = 0;
		}

		m_config.m_Row.SetPaymentCacheDays(n);
	}
	else
	{
		m_config.m_Row.SetPaymentCacheDays(0);
	}

	for (int n = 0; n < 12; n++)
	{
		CPebbleVATInfo VatInfo;
		VatInfo.m_nVATCode = m_comboVATCode[n].GetCurSel();
		VatInfo.m_dVATRate = GetEditBoxDouble(*GetEditVAT(n));
		m_config.m_Row.SetVATInfo(n, VatInfo);
	}

	m_config.m_Row.SetOfficeMealPluNo(GetEditBoxInt64(m_editOfficePLU));
	m_config.m_Row.SetOfficeMealVATBand(m_comboOfficeVAT.GetCurSel() + 1);

	m_config.SaveRow(NULL);

	/******/

	{
		CString strSQL = "";
		strSQL.Format("DELETE FROM %s",
			(const char*)SQLTableNames::PebblePaymentTypes);

		int nConnection = -1;
		CDatabase* pDatabase = SQLConnectionPool.GetPooledConnection(nConnection);

		if (nConnection != -1)
		{
			CSQLHelpers::ExecuteSQL(pDatabase, strSQL);
			SQLConnectionPool.FreeConnection(nConnection);
		}
	}

	CSQLTranSavePebblePayment Tran;
	Tran.BeginTrans();
	Tran.DoWork(m_RowSetPayment);

	switch (Tran.EndTrans())
	{
	case SQLTRAN_STATE_COMMITOK:
		break;

	default:
		Tran.ShowErrorMessage("Save Payment Types");
		break;
	}

	EndDialog(IDOK);
}

/**********************************************************************/
