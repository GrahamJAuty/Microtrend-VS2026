//*******************************************************************
#include "..\SPosLoyalty4Shared\AccountFieldValidator.h"
#include "..\SPosLoyalty4Shared\ImageHandler.h"
#include "..\SPosLoyalty4Shared\LoyaltyHeaderHelpers.h"
#include "..\SPosLoyalty4Shared\LoyaltySubs.h"
#include "..\SPosLoyalty4Shared\UserTexts.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SPosLoyalty4Shared\SQLTable_Group\SQLRepositoryGroup.h"
//*******************************************************************
#include "AuditRecord.h"
#include "GroupComboHelpers.h"
#include "LoyaltyManager.h"
#include "PasswordData.h"
#include "TLogFile.h"
//*******************************************************************
#include "RecordTransferDlg.h"
//*******************************************************************
static const int nACTIVEBALANCES = 9;
//*******************************************************************

CRecordTransferInfo::CRecordTransferInfo()
{
	Reset(TRUE);
}

//*******************************************************************

void CRecordTransferInfo::Reset(bool bAll)
{
	m_bSetName = FALSE;
	m_bSetGroup = FALSE;
	m_bSetScheme = FALSE;
	m_bSetDOB = FALSE;
	m_bSetExpiry = FALSE;
	m_bSetAlert = FALSE;
	m_bSetInactive = FALSE;
	m_bSetGender = FALSE;
	m_bSetAddress1 = FALSE;
	m_bSetAddress2 = FALSE;
	m_bSetAddress3 = FALSE;
	m_bSetAddress4 = FALSE;
	m_bSetAddress5 = FALSE;
	m_bSetPhone1 = FALSE;
	m_bSetPhone2 = FALSE;
	m_bSetInfo1 = FALSE;
	m_bSetInfo2 = FALSE;
	m_bSetInfo3 = FALSE;
	m_bSetInfo4 = FALSE;
	m_bSetPoints = FALSE;
	m_bSetPointsTD = FALSE;
	m_bSetPurse1 = FALSE;
	m_bSetPurse1TD = FALSE;
	m_bSetPurse2 = FALSE;
	m_bSetPurse2TD = FALSE;
	m_bSetCashTD = FALSE;
	m_bSetMaxSpend = FALSE;
	m_bSetOverSpend = FALSE;
	m_bSetCopyPurchaseHistory = FALSE;
	m_bSetOfferStamps = FALSE;
	/*****/
	m_nSchemeNo = 0;
	m_strDOB = "";
	m_strExpiry = "";
	m_strName = "";
	m_strAddress1 = "";
	m_strAddress2 = "";
	m_strAddress3 = "";
	m_strAddress4 = "";
	m_strAddress5 = "";
	m_strPhone1 = "";
	m_strPhone2 = "";
	m_strInfo1 = "";
	m_strInfo2 = "";
	m_strInfo3 = "";
	m_strInfo4 = "";
	m_strComment = "";
	/*****/
	m_nPoints = 0;
	m_nPointsTD = 0;
	m_dPurse1 = 0.0;
	m_dPurse1TD = 0.0;
	m_dPurse2 = 0.0;
	m_dPurse2TD = 0.0;
	m_dCashTD = 0.0;
	/*****/
	m_dMaxSpend = 0.0;
	m_dOverSpend = 0.0;
	/*****/
	m_dPurse1LastSpend = 0.0;
	m_strPurse1DateLastUsed = "";
	m_strPurse1TimeLastUsed = "";
	m_dPurse2LastSpend = 0.0;
	m_strPurse2DateLastUsed = "";
	m_strPurse2TimeLastUsed = "";
	m_strDateLastRefreshed = "";
	m_strTimeLastRefreshed = "";

	if (TRUE == bAll)
	{
		m_strGender = "";
		m_strAlertCodes = "";
		m_bInactive = FALSE;
		m_nGroupNo = 1;
	}
}

//*******************************************************************
//*******************************************************************
//*******************************************************************

CRecordTransferDlg::CRecordTransferDlg(CSQLRowAccountFull& RowFrom, CSQLRowAccountFull& RowTo, bool bHotlistReplace, CWnd* pParent /*=NULL*/)
	: CSSDialog(CRecordTransferDlg::IDD, pParent)
	, m_Comments(Filenames.GetDBaseCommentsFilename())
	, m_RowFrom(RowFrom)
	, m_RowTo(RowTo)
{
	m_bHotlistReplace = bHotlistReplace;
	m_bCanTransferMaxSpend = FALSE;
	
	m_XferInfo.m_nGroupNo = m_RowFrom.GetGroupNo();
	m_XferInfo.m_strGender = m_RowFrom.GetGender();
	m_XferInfo.m_strAlertCodes = m_RowFrom.GetAlertCodes();
	m_XferInfo.m_bInactive = m_RowFrom.GetInactive();

	m_nPoints = m_RowFrom.GetPoints();
	m_nPointsTD = m_RowFrom.GetPointsToDate();
	m_dPurse1 = m_RowFrom.GetPurse1Balance();
	m_dPurse1TD = m_RowFrom.GetPurse1SpendToDate();
	m_dPurse2TD = m_RowFrom.GetPurse2SpendToDate();
	m_dCashTD = m_RowFrom.GetCashSpendToDate();
	m_dOverSpend = m_RowFrom.GetMaxOverdraft();
	m_dMaxSpend = m_RowFrom.GetMaxSpend();

	CSQLRowGroup RowGroup;
	RowGroup.SetGroupNo(m_RowFrom.GetGroupNo());
	CSQLRepositoryGroup repoGroup;
	if (repoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		m_dPurse2 = LoyaltySubs.GetPurse2NextSpendBalance(m_RowFrom, RowGroup, FALSE);
		m_bCanTransferMaxSpend = (RowGroup.GetMaxSpendType() == 1);
	}
	else
	{
		m_dPurse2 = m_RowFrom.GetPurse2Balance();
		m_bCanTransferMaxSpend = FALSE;
	}

	m_dPurse1LastSpend = m_RowFrom.GetPurse1LastSpend();
	m_dPurse2LastSpend = m_RowFrom.GetPurse2LastSpend();

	m_strPurse1DateLastUsed = m_RowFrom.GetPurse1LastSpendDate();
	m_strPurse1TimeLastUsed = m_RowFrom.GetPurse1LastSpendTime();

	m_strPurse2DateLastUsed = m_RowFrom.GetPurse2LastSpendDate();
	m_strPurse2TimeLastUsed = m_RowFrom.GetPurse2LastSpendTime();

	m_strDateLastRefreshed = m_RowFrom.GetPurse2RefreshedDate();
	m_strTimeLastRefreshed = m_RowFrom.GetPurse2RefreshedTime();

	m_bSelectAllDetails = !bHotlistReplace;
	m_bSelectAllBalances = !bHotlistReplace;

	//*** set limits of balance transfers

	m_nActiveBalances = nACTIVEBALANCES;

	// points

	if (m_nPoints < 0)			// negative
	{
		m_nMinPoints = m_nPoints;
		m_nMaxPoints = 0;
	}
	else
	{
		m_nMinPoints = 0;
		m_nMaxPoints = m_nPoints;

		if (m_nPoints == 0)
		{
			--m_nActiveBalances;
		}
	}

	// pointsTD

	if (m_nPointsTD < 0)				// negative
	{
		m_nMinPointsTD = m_nPointsTD;
		m_nMaxPointsTD = 0;
	}
	else
	{
		m_nMinPointsTD = 0;
		m_nMaxPointsTD = m_nPointsTD;

		if (m_nPointsTD == 0)
		{
			--m_nActiveBalances;
		}
	}

	// purse1

	if (m_dPurse1 < 0)				// negative
	{
		m_dMinPurse1 = m_dPurse1;
		m_dMaxPurse1 = 0;
	}
	else
	{
		m_dMinPurse1 = 0;
		m_dMaxPurse1 = m_dPurse1;

		if (m_dPurse1 == 0)
		{
			--m_nActiveBalances;
		}
	}

	// purse2

	if (m_dPurse2 < 0)				// negative
	{
		m_dMinPurse2 = m_dPurse2;
		m_dMaxPurse2 = 0;
	}
	else
	{
		m_dMinPurse2 = 0;
		m_dMaxPurse2 = m_dPurse2;

		if (m_dPurse2 == 0)
		{
			--m_nActiveBalances;
		}
	}

	// purse1SpendTD

	if (m_dPurse1TD < 0)			// negative
	{
		m_dMinPurse1TD = m_dPurse1TD;
		m_dMaxPurse1TD = 0;
	}
	else
	{
		m_dMinPurse1TD = 0;
		m_dMaxPurse1TD = m_dPurse1TD;

		if (m_dPurse1TD == 0)
		{
			--m_nActiveBalances;
		}
	}

	// purse2SpendTD

	if (m_dPurse2TD < 0)			// negative
	{
		m_dMinPurse2TD = m_dPurse2TD;
		m_dMaxPurse2TD = 0;
	}
	else
	{
		m_dMinPurse2TD = 0;
		m_dMaxPurse2TD = m_dPurse2TD;

		if (m_dPurse2TD == 0)
		{
			--m_nActiveBalances;
		}
	}

	// Cash SpendTD

	if (m_dCashTD < 0)			// negative
	{
		m_dMinCashTD = m_dCashTD;
		m_dMaxCashTD = 0;
	}
	else
	{
		m_dMinCashTD = 0;
		m_dMaxCashTD = m_dCashTD;

		if (m_dCashTD == 0)
		{
			--m_nActiveBalances;
		}
	}

	m_bDateInfo1 = FALSE;
	m_bDateInfo2 = FALSE;
	m_bDateInfo3 = FALSE;
	m_bDateInfo4 = FALSE;
}

//*******************************************************************

CRecordTransferDlg::~CRecordTransferDlg()
{
}

//*******************************************************************

void CRecordTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_CARDNO, m_editCardNo);
	DDX_Control(pDX, IDC_CHECK_GROUPNO, m_checkGroup);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_comboGroup);
	DDX_Control(pDX, IDC_CHECK_SCHEME, m_checkScheme);
	DDX_Control(pDX, IDC_COMBO_SCHEME, m_comboScheme);
	DDX_Control(pDX, IDC_CHECK_NAME, m_checkName);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_CHECK_SETEXPIRY, m_checkSetExpiry);
	DDX_Control(pDX, IDC_CHECK_DATEEXPIRY, m_checkDateExpiry);
	DDX_Control(pDX, IDC_DATEPICKER_EXPIRY, m_datePickerExpiry);
	DDX_Control(pDX, IDC_CHECK_SETDOB, m_checkSetDOB);
	DDX_Control(pDX, IDC_CHECK_DATEDOB, m_checkDateDOB);
	DDX_Control(pDX, IDC_DATEPICKER_DOB, m_datePickerDOB);
	DDX_Control(pDX, IDC_CHECK_ALERT, m_checkAlert);
	DDX_Control(pDX, IDC_CHECK_INACTIVE, m_checkInactive);
	DDX_Control(pDX, IDC_CHECK_GENDER, m_checkGender);
	DDX_Control(pDX, IDC_CHECK_ADD1, m_checkAddress1);
	DDX_Control(pDX, IDC_EDIT_ADD1, m_editAddress1);
	DDX_Control(pDX, IDC_CHECK_ADD2, m_checkAddress2);
	DDX_Control(pDX, IDC_EDIT_ADD2, m_editAddress2);
	DDX_Control(pDX, IDC_CHECK_ADD3, m_checkAddress3);
	DDX_Control(pDX, IDC_EDIT_ADD3, m_editAddress3);
	DDX_Control(pDX, IDC_CHECK_ADD4, m_checkAddress4);
	DDX_Control(pDX, IDC_EDIT_ADD4, m_editAddress4);
	DDX_Control(pDX, IDC_CHECK_ADD5, m_checkAddress5);
	DDX_Control(pDX, IDC_EDIT_ADD5, m_editAddress5);
	DDX_Control(pDX, IDC_CHECK_TEL1, m_checkPhone1);
	DDX_Control(pDX, IDC_EDIT_TEL1, m_editPhone1);
	DDX_Control(pDX, IDC_CHECK_TEL2, m_checkPhone2);
	DDX_Control(pDX, IDC_EDIT_TEL2, m_editPhone2);
	DDX_Control(pDX, IDC_CHECK_INFO1, m_checkInfo1);
	DDX_Control(pDX, IDC_EDIT_INFO1, m_editInfo1);
	DDX_Control(pDX, IDC_CHECK_DATEINFO1, m_checkDateInfo1);
	DDX_Control(pDX, IDC_DATEPICKER_INFO1, m_datePickerInfo1);
	DDX_Control(pDX, IDC_CHECK_INFO2, m_checkInfo2);
	DDX_Control(pDX, IDC_EDIT_INFO2, m_editInfo2);
	DDX_Control(pDX, IDC_CHECK_DATEINFO2, m_checkDateInfo2);
	DDX_Control(pDX, IDC_DATEPICKER_INFO2, m_datePickerInfo2);
	DDX_Control(pDX, IDC_CHECK_INFO3, m_checkInfo3);
	DDX_Control(pDX, IDC_EDIT_INFO3, m_editInfo3);
	DDX_Control(pDX, IDC_CHECK_DATEINFO3, m_checkDateInfo3);
	DDX_Control(pDX, IDC_DATEPICKER_INFO3, m_datePickerInfo3);
	DDX_Control(pDX, IDC_CHECK_INFO4, m_checkInfo4);
	DDX_Control(pDX, IDC_EDIT_INFO4, m_editInfo4);
	DDX_Control(pDX, IDC_CHECK_DATEINFO4, m_checkDateInfo4);
	DDX_Control(pDX, IDC_DATEPICKER_INFO4, m_datePickerInfo4);
	DDX_Control(pDX, IDC_CHECK_POINTS, m_checkPoints);
	DDX_Control(pDX, IDC_CHECK_POINTSTD, m_checkPointsTD);
	DDX_Control(pDX, IDC_CHECK_PURSE1, m_checkPurse1);
	DDX_Control(pDX, IDC_CHECK_PURSE1TD, m_checkPurse1TD);
	DDX_Control(pDX, IDC_CHECK_PURSE2, m_checkPurse2);
	DDX_Control(pDX, IDC_CHECK_PURSE2TD, m_checkPurse2TD);
	DDX_Control(pDX, IDC_CHECK_CASHTD, m_checkCashTD);
	DDX_Control(pDX, IDC_CHECK_MAXSPEND, m_checkMaxSpend);
	DDX_Control(pDX, IDC_CHECK_OVERSPEND, m_checkOverSpend);
	DDX_Control(pDX, IDC_CHECK_COPYPURCHASES, m_checkCopyPurchaseHistory);
	DDX_Control(pDX, IDC_CHECK_OFFERSTAMP, m_checkOfferStamp);
	DDX_Control(pDX, IDC_COMBO_COMMENT, m_comboComments);
	DDX_Control(pDX, IDC_BUTTON_SELECTALL, m_buttonSelectAll);
	DDX_Control(pDX, IDC_BUTTON_SELECTBALANCES, m_buttonSelectBalances);
	DDX_Control(pDX, IDOK, m_buttonOK);
	/*****/
	DDX_Text(pDX, IDC_EDIT_POINTS, m_nPoints);
	DDX_Text(pDX, IDC_EDIT_POINTSTD, m_nPointsTD);
	DDX_Text(pDX, IDC_EDIT_PURSE1, m_dPurse1);
	DDX_Text(pDX, IDC_EDIT_PURSE1TD, m_dPurse1TD);
	DDX_Text(pDX, IDC_EDIT_PURSE2, m_dPurse2);
	DDX_Text(pDX, IDC_EDIT_PURSE2TD, m_dPurse2TD);
	DDX_Text(pDX, IDC_EDIT_CASHTD, m_dCashTD);
	DDX_Text(pDX, IDC_EDIT_MAXSPEND, m_dMaxSpend);
	DDX_Text(pDX, IDC_EDIT_OVERSPEND, m_dOverSpend);
}

//*******************************************************************

BEGIN_MESSAGE_MAP(CRecordTransferDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_SCHEME, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_NAME, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_GROUPNO, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_SETEXPIRY, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_SETDOB, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_INACTIVE, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_ALERT, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_GENDER, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_ADD1, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_ADD2, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_ADD3, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_ADD4, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_ADD5, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_TEL1, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_TEL2, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_INFO1, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_INFO2, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_INFO3, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_INFO4, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_POINTS, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_POINTSTD, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_PURSE1, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_PURSE1TD, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_PURSE2, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_PURSE2TD, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_CASHTD, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_MAXSPEND, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_OVERSPEND, SetDisplay)
	ON_BN_CLICKED(IDC_CHECK_OFFERSTAMP, SetDisplay)
	/*****/
	ON_BN_CLICKED(IDC_CHECK_DATEEXPIRY, OnToggleDateExpiry)
	ON_BN_CLICKED(IDC_CHECK_DATEDOB, OnToggleDateDOB)
	ON_BN_CLICKED(IDC_CHECK_DATEINFO1, OnToggleDateInfo1)
	ON_BN_CLICKED(IDC_CHECK_DATEINFO2, OnToggleDateInfo2)
	ON_BN_CLICKED(IDC_CHECK_DATEINFO3, OnToggleDateInfo3)
	ON_BN_CLICKED(IDC_CHECK_DATEINFO4, OnToggleDateInfo4)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, OnButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_SELECTBALANCES, OnButtonSelectBalances)
	ON_STN_CLICKED(IDC_STATIC_COMMENT, OnClickStaticComment)
END_MESSAGE_MAP()

//*******************************************************************

BOOL CRecordTransferDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CUserTexts texts;
	texts.Read();
	m_bDateInfo1 = texts.GetDateFlag(LOYALTYFIELD_INFO1);
	m_bDateInfo2 = texts.GetDateFlag(LOYALTYFIELD_INFO2);
	m_bDateInfo3 = texts.GetDateFlag(LOYALTYFIELD_INFO3);
	m_bDateInfo4 = texts.GetDateFlag(LOYALTYFIELD_INFO4);

	MoveControl(&m_checkDateInfo1, 78, 260);
	MoveControl(&m_checkDateInfo2, 78, 279);
	MoveControl(&m_checkDateInfo3, 78, 298);
	MoveControl(&m_checkDateInfo4, 78, 317);
	MoveControl(&m_datePickerInfo1, 94, 258);
	MoveControl(&m_datePickerInfo2, 94, 277);
	MoveControl(&m_datePickerInfo3, 94, 296);
	MoveControl(&m_datePickerInfo4, 94, 315);

	CString strTitle = "";
	CString strProcessButtonText = "";
	if (m_bHotlistReplace == TRUE)
	{
		strTitle.Format("Select items to transfer to replacement account %s", 
			(const char*) System.FormatCardNo(m_RowTo.GetUserIDString()));

		strProcessButtonText = "Process Replacement";

		if (::FileExists(Filenames.GetPurchaseHistoryFilename(m_RowTo.GetUserIDString())) == FALSE)		// see if replacement card as a purchase history alreaay
		{
			m_checkCopyPurchaseHistory.SetWindowText("Carry forward purchase history");	// Overwrite replacements purchase history
		}
	}
	else
	{
		strTitle.Format("Select items to Copy / Transfer to account %s", 
			(const char*) System.FormatCardNo(m_RowTo.GetUserIDString()));

		strProcessButtonText = "Process Transfer";
		m_checkCopyPurchaseHistory.ShowWindow(SW_HIDE);
	}

	//card no
	SetEditBoxInt64(m_editCardNo, m_RowFrom.GetUserID());
	
	//group can be copied but not changed
	m_checkGroup.SetCheck(m_bHotlistReplace);
	CGroupComboHelpers::FillGroupCombo(m_comboGroup, m_XferInfo.m_nGroupNo, FALSE);
	m_comboGroup.EnableWindow(FALSE);						

	//scheme
	m_checkScheme.SetCheck(m_bHotlistReplace);
	CGroupComboHelpers::FillSchemeCombo(m_comboScheme, m_XferInfo.m_nGroupNo, m_RowFrom.GetSchemeNo(), TRUE, TRUE);

	//name
	m_checkName.SetCheck(m_bHotlistReplace);
	m_editName.SetWindowText(m_RowFrom.GetFullname());

	//DOB
	m_checkSetDOB.SetCheck(m_bHotlistReplace);
	LoadDateInfo(TRUE, m_RowFrom.GetDOB(), m_checkDateDOB, m_datePickerDOB);

	//EXPIRY
	m_checkSetExpiry.SetCheck(m_bHotlistReplace);
	LoadDateInfo(FALSE, m_RowFrom.GetExpiryDate(), m_checkDateExpiry, m_datePickerExpiry);

	//ticks
	m_checkInactive.SetCheck(m_bHotlistReplace);
	m_checkAlert.SetCheck(m_bHotlistReplace);
	m_checkGender.SetCheck(m_bHotlistReplace);

	//address
	m_checkAddress1.SetCheck(m_bHotlistReplace);
	m_editAddress1.SetWindowText(m_RowFrom.GetAddress1());

	m_checkAddress2.SetCheck(m_bHotlistReplace);
	m_editAddress2.SetWindowText(m_RowFrom.GetAddress2());

	m_checkAddress3.SetCheck(m_bHotlistReplace);
	m_editAddress3.SetWindowText(m_RowFrom.GetAddress3());

	m_checkAddress4.SetCheck(m_bHotlistReplace);
	m_editAddress4.SetWindowText(m_RowFrom.GetAddress4());

	m_checkAddress5.SetCheck(m_bHotlistReplace);
	m_editAddress5.SetWindowText(m_RowFrom.GetAddress5());

	m_checkPhone1.SetCheck(m_bHotlistReplace);
	m_editPhone1.SetWindowText(m_RowFrom.GetPhone1());

	m_checkPhone2.SetCheck(m_bHotlistReplace);
	m_editPhone2.SetWindowText(m_RowFrom.GetPhone2());

	//INFO1
	SetInfoValue(m_bDateInfo1, m_checkInfo1, m_editInfo1, m_checkDateInfo1, m_datePickerInfo1, m_RowFrom.GetInfo1());

	//INFO2
	SetInfoValue(m_bDateInfo2, m_checkInfo2, m_editInfo2, m_checkDateInfo2, m_datePickerInfo2, m_RowFrom.GetInfo2());

	//INFO3
	SetInfoValue(m_bDateInfo3, m_checkInfo3, m_editInfo3, m_checkDateInfo3, m_datePickerInfo3, m_RowFrom.GetInfo3());

	//INFO4
	SetInfoValue(m_bDateInfo4, m_checkInfo4, m_editInfo4, m_checkDateInfo4, m_datePickerInfo4, m_RowFrom.GetInfo4());

	m_checkPoints.SetCheck(m_bHotlistReplace && (m_nPoints != 0));
	m_checkPointsTD.SetCheck(m_bHotlistReplace && (m_nPointsTD != 0));
	m_checkPurse1.SetCheck(m_bHotlistReplace && (m_dPurse1 != 0.0));
	m_checkPurse1TD.SetCheck(m_bHotlistReplace && (m_dPurse1TD != 0.0));
	m_checkPurse2.SetCheck(m_bHotlistReplace && (m_dPurse2 != 0.0));
	m_checkPurse2TD.SetCheck(m_bHotlistReplace && (m_dPurse2TD != 0.0));
	m_checkCashTD.SetCheck(m_bHotlistReplace && (m_dCashTD != 0.0));
	m_checkOfferStamp.SetCheck(m_bHotlistReplace);

	SetWindowText(strTitle);
	m_buttonOK.SetWindowText(strProcessButtonText);

	SubclassEdit(IDC_EDIT_POINTS, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_POINTSTD, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_PURSE1, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE1TD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE2, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE2TD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_CASHTD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_MAXSPEND, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_OVERSPEND, SS_NUM_SDP, 11, "%.2f");

	Server.Read(Filenames.GetServerDataFilename());
	if (Server.GetAllowOverSpendFlag() == FALSE)
	{
		GetEditOverSpend() -> ShowWindow(SW_HIDE);
		m_checkOverSpend.ShowWindow(SW_HIDE);
	}

	if (m_bHotlistReplace == TRUE)
	{
		m_checkPoints.EnableWindow(FALSE);
		m_checkPointsTD.EnableWindow(FALSE);
		m_checkPurse1.EnableWindow(FALSE);
		m_checkPurse1TD.EnableWindow(FALSE);
		m_checkPurse2.EnableWindow(FALSE);
		m_checkPurse2TD.EnableWindow(FALSE);
		m_checkCashTD.EnableWindow(FALSE);
		m_checkMaxSpend.EnableWindow(FALSE);
		m_checkOverSpend.EnableWindow(FALSE);
	}

	if (m_nPoints == 0)				m_checkPoints.EnableWindow(FALSE);
	if (m_nPointsTD == 0)			m_checkPointsTD.EnableWindow(FALSE);
	if (m_dPurse1 == 0.0)			m_checkPurse1.EnableWindow(FALSE);
	if (m_dPurse1TD == 0.0)			m_checkPurse1TD.EnableWindow(FALSE);
	if (m_dPurse2 == 0.0)			m_checkPurse2.EnableWindow(FALSE);
	if (m_dPurse2TD == 0.0)			m_checkPurse2TD.EnableWindow(FALSE);
	if (m_dCashTD == 0.0)			m_checkCashTD.EnableWindow(FALSE);

	if (FALSE == m_bCanTransferMaxSpend)
	{
		ShowAndEnableWindow(&m_checkMaxSpend, FALSE);
		ShowAndEnableWindow(GetEditMaxSpend(), FALSE);
		ShowAndEnableWindow(&m_checkOverSpend, FALSE);
		ShowAndEnableWindow(GetEditOverSpend(), FALSE);
		MoveControl(&m_checkOfferStamp, 333, 184);
	}

	m_checkAddress1.SetWindowText(texts.GetText(LOYALTYFIELD_ADDRESS1, "Add1"));
	m_checkAddress2.SetWindowText(texts.GetText(LOYALTYFIELD_ADDRESS2, "Add2"));
	m_checkAddress3.SetWindowText(texts.GetText(LOYALTYFIELD_ADDRESS3, "Add3"));
	m_checkAddress4.SetWindowText(texts.GetText(LOYALTYFIELD_ADDRESS4, "Add4"));
	m_checkAddress4.SetWindowText(texts.GetText(LOYALTYFIELD_ADDRESS5, "Add5"));
	m_checkPhone1.SetWindowText(texts.GetText(LOYALTYFIELD_PHONE1, "Tel1"));
	m_checkPhone2.SetWindowText(texts.GetText(LOYALTYFIELD_PHONE2, "Tel2"));
	m_checkInfo1.SetWindowText(texts.GetText(LOYALTYFIELD_INFO1, "Info1"));
	m_checkInfo2.SetWindowText(texts.GetText(LOYALTYFIELD_INFO2, "Info2"));
	m_checkInfo3.SetWindowText(texts.GetText(LOYALTYFIELD_INFO3, "Info3"));
	m_checkInfo4.SetWindowText(texts.GetText(LOYALTYFIELD_INFO4, "Info4"));
	m_checkSetDOB.SetWindowText(texts.GetText(LOYALTYFIELD_DOB, "Anniversary"));

	AddComments();

	SetDisplay();
	OnButtonSelectBalances();
	OnButtonSelectAll();

	return TRUE;
}

//*******************************************************************
CEdit* CRecordTransferDlg::GetEditPoints() { return GetEdit(IDC_EDIT_POINTS); }
CEdit* CRecordTransferDlg::GetEditPointsTD() { return GetEdit(IDC_EDIT_POINTSTD); }
CEdit* CRecordTransferDlg::GetEditPurse1() { return GetEdit(IDC_EDIT_PURSE1); }
CEdit* CRecordTransferDlg::GetEditPurse1TD() { return GetEdit(IDC_EDIT_PURSE1TD); }
CEdit* CRecordTransferDlg::GetEditPurse2() { return GetEdit(IDC_EDIT_PURSE2); }
CEdit* CRecordTransferDlg::GetEditPurse2TD() { return GetEdit(IDC_EDIT_PURSE2TD); }
CEdit* CRecordTransferDlg::GetEditCashTD() { return GetEdit(IDC_EDIT_CASHTD); }
CEdit* CRecordTransferDlg::GetEditMaxSpend() { return GetEdit(IDC_EDIT_MAXSPEND); }
CEdit* CRecordTransferDlg::GetEditOverSpend() { return GetEdit(IDC_EDIT_OVERSPEND); }
//*******************************************************************

void CRecordTransferDlg::SetInfoValue(bool bUseDate, CButton& checkSet, CEdit& edit, CButton& checkDate, CDateTimeCtrl& dtc, CString strValue)
{
	checkSet.SetCheck(m_bHotlistReplace);

	if (FALSE == bUseDate)
	{
		ShowAndEnableWindow(&checkDate, FALSE);
		ShowAndEnableWindow(&dtc, FALSE);
		edit.SetWindowText(strValue);
	}
	else
	{
		ShowAndEnableWindow(&edit, FALSE);
		LoadDateInfo(FALSE, strValue, checkDate, dtc);
	}
}

//*******************************************************************

void CRecordTransferDlg::LoadDateInfo(bool bDOB, CString strDate, CButton& checkBox, CDateTimeCtrl& datePicker)
{
	if (TRUE == bDOB)
	{
		COleDateTime oleMin = CAccountFieldValidator::GetMinDob();
		COleDateTime oleMax = CAccountFieldValidator::GetMaxDob();
		datePicker.SetRange(&oleMin, &oleMax);
	}
	else
	{
		COleDateTime oleMin = CAccountFieldValidator::GetMinExpiry();
		COleDateTime oleMax = CAccountFieldValidator::GetMaxExpiry();
		datePicker.SetRange(&oleMin, &oleMax);
	}

	datePicker.SetTime(COleDateTime::GetCurrentTime());
	datePicker.EnableWindow(FALSE);

	checkBox.EnableWindow(m_bHotlistReplace);
	checkBox.SetCheck(FALSE);

	COleDateTime oleDate;
	if (CAccountFieldValidator::StringToOleDate(strDate, oleDate) == TRUE)
	{
		bool bOK = FALSE;
		if (TRUE == bDOB)
		{
			bOK = CAccountFieldValidator::ValidateDOB(oleDate);
		}
		else
		{
			bOK = CAccountFieldValidator::ValidateExpiry(oleDate);
		}

		if (TRUE == bOK)
		{
			checkBox.SetCheck(TRUE);
			datePicker.SetTime(oleDate);
			datePicker.EnableWindow(m_bHotlistReplace);
		}
	}
}

//*******************************************************************

void CRecordTransferDlg::AddComments()
{
	for (int i = 0; i < m_Comments.GetSize(); i++)		// read previous comments
	{
		m_comboComments.AddString(m_Comments.GetAt(i));
	}
}

//*******************************************************************

void CRecordTransferDlg::OnToggleDateDOB()
{
	m_datePickerDOB.EnableWindow(IsTicked(m_checkSetDOB) && (IsTicked(m_checkDateDOB)));
}

//*******************************************************************

void CRecordTransferDlg::OnToggleDateExpiry()
{
	m_datePickerExpiry.EnableWindow(IsTicked(m_checkSetExpiry) && (IsTicked(m_checkDateExpiry)));
}

//*******************************************************************

void CRecordTransferDlg::OnToggleDateInfo1()
{
	m_datePickerInfo1.EnableWindow(IsTicked(m_checkInfo1) && (IsTicked(m_checkDateInfo1)));
}

//*******************************************************************

void CRecordTransferDlg::OnToggleDateInfo2()
{
	m_datePickerInfo2.EnableWindow(IsTicked(m_checkInfo2) && (IsTicked(m_checkDateInfo2)));
}

//*******************************************************************

void CRecordTransferDlg::OnToggleDateInfo3()
{
	m_datePickerInfo3.EnableWindow(IsTicked(m_checkInfo3) && (IsTicked(m_checkDateInfo3)));
}

//*******************************************************************

void CRecordTransferDlg::OnToggleDateInfo4()
{
	m_datePickerInfo4.EnableWindow(IsTicked(m_checkInfo4) && (IsTicked(m_checkDateInfo4)));
}

//*******************************************************************

void CRecordTransferDlg::SetDisplay()
{
	if (Passwords.IsEditGeneralAllowed() == FALSE)
	{
		m_comboScheme.EnableWindow(FALSE);
		m_editName.EnableWindow(FALSE);
		m_editAddress1.EnableWindow(FALSE);
		m_editAddress2.EnableWindow(FALSE);
		m_editAddress3.EnableWindow(FALSE);
		m_editAddress4.EnableWindow(FALSE);
		m_editAddress5.EnableWindow(FALSE);
		m_editPhone1.EnableWindow(FALSE);
		m_editPhone2.EnableWindow(FALSE);
		m_editInfo1.EnableWindow(FALSE);
		m_editInfo2.EnableWindow(FALSE);
		m_editInfo3.EnableWindow(FALSE);
		m_editInfo4.EnableWindow(FALSE);
		m_datePickerInfo1.EnableWindow(FALSE);
		m_datePickerInfo2.EnableWindow(FALSE);
		m_datePickerInfo3.EnableWindow(FALSE);
		m_datePickerInfo4.EnableWindow(FALSE);
		m_checkDateDOB.EnableWindow(FALSE);
		m_datePickerDOB.EnableWindow(FALSE);
		m_checkDateExpiry.EnableWindow(FALSE);
		m_datePickerExpiry.EnableWindow(FALSE);
	}
	else
	{
		m_comboScheme.EnableWindow(IsTicked(m_checkScheme));
		m_editName.EnableWindow(IsTicked(m_checkName));
		m_editAddress1.EnableWindow(IsTicked(m_checkAddress1));
		m_editAddress2.EnableWindow(IsTicked(m_checkAddress2));
		m_editAddress3.EnableWindow(IsTicked(m_checkAddress3));
		m_editAddress4.EnableWindow(IsTicked(m_checkAddress4));
		m_editAddress5.EnableWindow(IsTicked(m_checkAddress5));
		m_editPhone1.EnableWindow(IsTicked(m_checkPhone1));
		m_editPhone2.EnableWindow(IsTicked(m_checkPhone2));
		m_editInfo1.EnableWindow(IsTicked(m_checkInfo1));
		m_editInfo2.EnableWindow(IsTicked(m_checkInfo2));
		m_editInfo3.EnableWindow(IsTicked(m_checkInfo3));
		m_editInfo4.EnableWindow(IsTicked(m_checkInfo4));
		m_checkDateDOB.EnableWindow(IsTicked(m_checkSetDOB));
		m_datePickerDOB.EnableWindow(IsTicked(m_checkSetDOB) && IsTicked(m_checkDateDOB));
		m_checkDateExpiry.EnableWindow(IsTicked(m_checkSetExpiry));
		m_datePickerExpiry.EnableWindow(IsTicked(m_checkSetExpiry) && IsTicked(m_checkDateExpiry));
		m_checkDateInfo1.EnableWindow(IsTicked(m_checkInfo1));
		m_checkDateInfo2.EnableWindow(IsTicked(m_checkInfo2));
		m_checkDateInfo3.EnableWindow(IsTicked(m_checkInfo3));
		m_checkDateInfo4.EnableWindow(IsTicked(m_checkInfo4));
		m_datePickerInfo1.EnableWindow(IsTicked(m_checkInfo1) && IsTicked(m_checkDateInfo1));
		m_datePickerInfo2.EnableWindow(IsTicked(m_checkInfo2) && IsTicked(m_checkDateInfo2));
		m_datePickerInfo3.EnableWindow(IsTicked(m_checkInfo3) && IsTicked(m_checkDateInfo3));
		m_datePickerInfo4.EnableWindow(IsTicked(m_checkInfo4) && IsTicked(m_checkDateInfo4));
	}

	if (Passwords.IsEditPointsAllowed() == FALSE || m_bHotlistReplace == TRUE)
	{
		GetEditPoints()->EnableWindow(FALSE);
		GetEditPointsTD()->EnableWindow(FALSE);
	}
	else
	{
		GetEditPoints()->EnableWindow(IsTicked(m_checkPoints));
		GetEditPointsTD()->EnableWindow(IsTicked(m_checkPointsTD));
	}

	if ((Passwords.IsEditPurseAllowed() == FALSE) || (m_bHotlistReplace == TRUE))
	{
		GetEditPurse1()->EnableWindow(FALSE);
		GetEditPurse1TD()->EnableWindow(FALSE);
		GetEditPurse2()->EnableWindow(FALSE);
		GetEditPurse2TD()->EnableWindow(FALSE);
		GetEditCashTD()->EnableWindow(FALSE);
		GetEditMaxSpend()->EnableWindow(FALSE);
		GetEditOverSpend()->EnableWindow(FALSE);
		m_checkOfferStamp.EnableWindow(FALSE);
	}
	else
	{
		GetEditPurse1()->EnableWindow(IsTicked(m_checkPurse1));
		GetEditPurse1TD()->EnableWindow(IsTicked(m_checkPurse1TD));
		GetEditPurse2()->EnableWindow(IsTicked(m_checkPurse2));
		GetEditPurse2TD()->EnableWindow(IsTicked(m_checkPurse2TD));
		GetEditCashTD()->EnableWindow(IsTicked(m_checkCashTD));
		GetEditMaxSpend()->EnableWindow(IsTicked(m_checkMaxSpend) && m_bCanTransferMaxSpend);
		GetEditOverSpend()->EnableWindow(IsTicked(m_checkOverSpend) && m_bCanTransferMaxSpend);
		m_checkOfferStamp.EnableWindow(TRUE);
	}

	m_buttonOK.EnableWindow(StartAllowed());
}

//*******************************************************************

bool CRecordTransferDlg::StartAllowed()
{
	bool bResult = FALSE;
	bResult |= IsTicked(m_checkScheme);
	bResult |= IsTicked(m_checkName);
	bResult |= IsTicked(m_checkAlert);
	bResult |= IsTicked(m_checkInactive);
	bResult |= IsTicked(m_checkGender);
	bResult |= IsTicked(m_checkGroup);
	bResult |= IsTicked(m_checkAddress1);
	bResult |= IsTicked(m_checkAddress2);
	bResult |= IsTicked(m_checkAddress3);
	bResult |= IsTicked(m_checkAddress4);
	bResult |= IsTicked(m_checkAddress5);
	bResult |= IsTicked(m_checkPhone1);
	bResult |= IsTicked(m_checkPhone2);
	bResult |= IsTicked(m_checkInfo1);
	bResult |= IsTicked(m_checkInfo2);
	bResult |= IsTicked(m_checkInfo3);
	bResult |= IsTicked(m_checkInfo4);
	bResult |= IsTicked(m_checkPoints);
	bResult |= IsTicked(m_checkPointsTD);
	bResult |= IsTicked(m_checkCashTD);
	bResult |= (IsTicked(m_checkMaxSpend) && (m_bCanTransferMaxSpend));
	bResult |= (IsTicked(m_checkOverSpend) && (m_bCanTransferMaxSpend));
	bResult |= IsTicked(m_checkPurse1);
	bResult |= IsTicked(m_checkPurse1TD);
	bResult |= IsTicked(m_checkPurse2);
	bResult |= IsTicked(m_checkPurse2TD);
	bResult |= IsTicked(m_checkCopyPurchaseHistory);
	bResult |= IsTicked(m_checkOfferStamp);
	bResult |= IsTicked(m_checkSetExpiry);
	bResult |= IsTicked(m_checkSetDOB);
	return bResult;
}

//*******************************************************************

void CRecordTransferDlg::OnClickStaticComment()
{
	CString strComment = "";
	m_comboComments.GetWindowText(strComment);

	if (strComment != "")
	{
		m_Comments.Delete(strComment);			// delete comment
		m_comboComments.ResetContent();				// new list
		AddComments();
	}
}

//*******************************************************************

void CRecordTransferDlg::OnButtonSelectBalances()
{
	if (UpdateData() == TRUE)
	{
		if (m_bHotlistReplace == TRUE)
		{
			m_bSelectAllBalances = TRUE;
			m_buttonSelectBalances.ShowWindow(SW_HIDE);
		}
		else
		{
			m_bSelectAllBalances = !m_bSelectAllBalances;
			CString strButton = (m_bSelectAllBalances == TRUE) ? "Select None" : "Select All";

			if (m_nActiveBalances > 0)
			{
				m_buttonSelectBalances.SetWindowText(strButton);
			}
			else
			{
				m_buttonSelectBalances.EnableWindow(FALSE);
			}
		}

		m_checkPoints.SetCheck(m_bSelectAllBalances && (m_nPoints != 0));
		m_checkPointsTD.SetCheck(m_bSelectAllBalances && (m_nPointsTD != 0));
		m_checkPurse1.SetCheck(m_bSelectAllBalances && (m_dPurse1 != 0.0));
		m_checkPurse1TD.SetCheck(m_bSelectAllBalances && (m_dPurse1TD != 0.0));
		m_checkPurse2.SetCheck(m_bSelectAllBalances && (m_dPurse2 != 0.0));
		m_checkPurse2TD.SetCheck(m_bSelectAllBalances && (m_dPurse2TD != 0.0));
		m_checkCashTD.SetCheck(m_bSelectAllBalances && (m_dCashTD != 0.0));
		m_checkMaxSpend.SetCheck(m_bSelectAllBalances && m_bCanTransferMaxSpend);
		m_checkOverSpend.SetCheck(m_bSelectAllBalances && m_bCanTransferMaxSpend);
		m_checkOfferStamp.SetCheck(m_bSelectAllBalances);

		UpdateData(FALSE);
		SetDisplay();
	}
}

//*******************************************************************

void CRecordTransferDlg::OnButtonSelectAll()
{
	m_bSelectAllDetails = !m_bSelectAllDetails;
	CString strButton = (m_bSelectAllDetails == TRUE) ? "Select None" : "Select All";
	m_buttonSelectAll.SetWindowText(strButton);

	m_checkScheme.SetCheck(m_bSelectAllDetails);
	m_checkName.SetCheck(m_bSelectAllDetails);
	m_checkAlert.SetCheck(m_bSelectAllDetails);
	m_checkInactive.SetCheck(m_bSelectAllDetails);
	m_checkGender.SetCheck(m_bSelectAllDetails);
	m_checkAddress1.SetCheck(m_bSelectAllDetails);
	m_checkAddress2.SetCheck(m_bSelectAllDetails);
	m_checkAddress3.SetCheck(m_bSelectAllDetails);
	m_checkAddress4.SetCheck(m_bSelectAllDetails);
	m_checkAddress5.SetCheck(m_bSelectAllDetails);
	m_checkPhone1.SetCheck(m_bSelectAllDetails);
	m_checkPhone2.SetCheck(m_bSelectAllDetails);
	m_checkInfo1.SetCheck(m_bSelectAllDetails);
	m_checkInfo2.SetCheck(m_bSelectAllDetails);
	m_checkInfo3.SetCheck(m_bSelectAllDetails);
	m_checkInfo4.SetCheck(m_bSelectAllDetails);
	m_checkGroup.SetCheck(m_bSelectAllDetails);
	m_checkSetExpiry.SetCheck(m_bSelectAllDetails);
	m_checkSetDOB.SetCheck(m_bSelectAllDetails);

	if (TRUE == m_bHotlistReplace)
	{
		m_checkCopyPurchaseHistory.SetCheck(m_bSelectAllDetails);
	}

	UpdateData(FALSE);
	SetDisplay();
}

//*******************************************************************

void CRecordTransferDlg::OnOK()
{
	if (UpdateData(TRUE) == FALSE)
	{
		return;
	}

	m_XferInfo.Reset(FALSE);

	//NAME
	m_XferInfo.m_bSetName = IsTicked(m_checkName);
	m_XferInfo.m_strName = GetEditBoxText(m_editName);

	//GROUPS
	m_XferInfo.m_bSetGroup = IsTicked(m_checkGroup);

	//SCHEME
	m_XferInfo.m_bSetScheme = IsTicked(m_checkScheme);
	m_XferInfo.m_nSchemeNo = CGroupComboHelpers::GetGroupNoFromCombo(m_comboScheme, 0);

	//EXPIRY
	if (IsTicked(m_checkSetExpiry))
	{
		GetNewDateInfo(FALSE, m_XferInfo.m_bSetExpiry, m_XferInfo.m_strExpiry, m_checkDateExpiry, m_datePickerExpiry);
	}

	//DOB
	if (IsTicked(m_checkSetDOB))
	{
		GetNewDateInfo(TRUE, m_XferInfo.m_bSetDOB, m_XferInfo.m_strDOB, m_checkDateDOB, m_datePickerDOB);
	}

	//ADDRESS 1
	m_XferInfo.m_bSetAddress1 = IsTicked(m_checkAddress1);
	m_XferInfo.m_strAddress1 = GetEditBoxText(m_editAddress1);

	//ADDRESS 2
	m_XferInfo.m_bSetAddress2 = IsTicked(m_checkAddress2);
	m_XferInfo.m_strAddress2 = GetEditBoxText(m_editAddress2);

	//ADDRESS 3
	m_XferInfo.m_bSetAddress3 = IsTicked(m_checkAddress3);
	m_XferInfo.m_strAddress3 = GetEditBoxText(m_editAddress3);

	//ADDRESS 4
	m_XferInfo.m_bSetAddress4 = IsTicked(m_checkAddress4);
	m_XferInfo.m_strAddress4 = GetEditBoxText(m_editAddress4);

	//ADDRESS5 5
	m_XferInfo.m_bSetAddress5 = IsTicked(m_checkAddress5);
	m_XferInfo.m_strAddress5 = GetEditBoxText(m_editAddress5);

	//PHONE 1
	m_XferInfo.m_bSetPhone1 = IsTicked(m_checkPhone1);
	m_XferInfo.m_strPhone1 = GetEditBoxText(m_editPhone1);

	//PHONE 2
	m_XferInfo.m_bSetPhone2 = IsTicked(m_checkPhone2);
	m_XferInfo.m_strPhone2 = GetEditBoxText(m_editPhone2);

	//INFO 1
	CheckTransferValue(m_bDateInfo1, m_checkInfo1, m_editInfo1, m_checkDateInfo1, m_datePickerInfo1, m_XferInfo.m_bSetInfo1, m_XferInfo.m_strInfo1);

	//INFO 2
	CheckTransferValue(m_bDateInfo2, m_checkInfo2, m_editInfo2, m_checkDateInfo2, m_datePickerInfo2, m_XferInfo.m_bSetInfo2, m_XferInfo.m_strInfo2);

	//INFO 3
	CheckTransferValue(m_bDateInfo3, m_checkInfo3, m_editInfo3, m_checkDateInfo3, m_datePickerInfo3, m_XferInfo.m_bSetInfo3, m_XferInfo.m_strInfo3);

	//INFO 4
	CheckTransferValue(m_bDateInfo4, m_checkInfo4, m_editInfo4, m_checkDateInfo4, m_datePickerInfo4, m_XferInfo.m_bSetInfo4, m_XferInfo.m_strInfo4);

	//INACTIVE
	m_XferInfo.m_bSetInactive = IsTicked(m_checkInactive);

	//ALERT
	m_XferInfo.m_bSetAlert = IsTicked(m_checkAlert);

	//GENDER
	m_XferInfo.m_bSetGender = IsTicked(m_checkGender);

	//POINTS
	if (CheckTransferValue("Points", m_checkPoints, GetEditPoints(), m_nPoints, m_nMinPoints, m_nMaxPoints, m_XferInfo.m_bSetPoints, m_XferInfo.m_nPoints) == FALSE)
	{
		return;
	}

	//POINTS TD
	if (CheckTransferValue("PointsTD", m_checkPointsTD, GetEditPointsTD(), m_nPointsTD, m_nMinPointsTD, m_nMaxPointsTD, m_XferInfo.m_bSetPointsTD, m_XferInfo.m_nPointsTD) == FALSE)
	{
		return;
	}

	//PURSE1	
	if (CheckTransferValue("Purse1", m_checkPurse1, GetEditPurse1(), m_dPurse1, m_dMinPurse1, m_dMaxPurse1, m_XferInfo.m_bSetPurse1, m_XferInfo.m_dPurse1) == FALSE)
	{
		return;
	}

	//PURSE1 TD	
	if (CheckTransferValue("Purse1 SpendTD", m_checkPurse1TD, GetEditPurse1TD(), m_dPurse1TD, m_dMinPurse1TD, m_dMaxPurse1TD, m_XferInfo.m_bSetPurse1TD, m_XferInfo.m_dPurse1TD) == FALSE)
	{
		return;
	}

	//PURSE2
	if (CheckTransferValue("Purse2", m_checkPurse2, GetEditPurse2(), m_dPurse2, m_dMinPurse2, m_dMaxPurse2, m_XferInfo.m_bSetPurse2, m_XferInfo.m_dPurse2) == FALSE)
	{
		return;
	}

	//PURSE2 TD
	if (CheckTransferValue("Purse2 SpendTD", m_checkPurse2TD, GetEditPurse2TD(), m_dPurse2TD, m_dMinPurse2TD, m_dMaxPurse2TD, m_XferInfo.m_bSetPurse2TD, m_XferInfo.m_dPurse2TD) == FALSE)
	{
		return;
	}

	//CASH TD
	if (CheckTransferValue("Cash SpendTD", m_checkCashTD, GetEditCashTD(), m_dCashTD, m_dMinCashTD, m_dMaxCashTD, m_XferInfo.m_bSetCashTD, m_XferInfo.m_dCashTD) == FALSE)
	{
		return;
	}

	if (TRUE == m_bCanTransferMaxSpend)
	{
		m_XferInfo.m_bSetMaxSpend = IsTicked(m_checkMaxSpend);
		m_XferInfo.m_bSetOverSpend = IsTicked(m_checkOverSpend);
		m_XferInfo.m_dMaxSpend = m_dMaxSpend;
		m_XferInfo.m_dOverSpend = m_dOverSpend;
	}
	else
	{
		m_XferInfo.m_bSetMaxSpend = FALSE;
		m_XferInfo.m_bSetOverSpend = FALSE;
		m_XferInfo.m_dMaxSpend = 0.0;
		m_XferInfo.m_dOverSpend = 0.0;
	}

	m_XferInfo.m_bSetOfferStamps = IsTicked(m_checkOfferStamp);
	m_XferInfo.m_bSetCopyPurchaseHistory = IsTicked(m_checkCopyPurchaseHistory);
	m_XferInfo.m_dPurse1LastSpend = m_dPurse1LastSpend;
	m_XferInfo.m_strPurse1DateLastUsed = m_strPurse1DateLastUsed;
	m_XferInfo.m_strPurse1TimeLastUsed = m_strPurse1TimeLastUsed;
	m_XferInfo.m_dPurse2LastSpend = m_dPurse2LastSpend;
	m_XferInfo.m_strPurse2DateLastUsed = m_strPurse2DateLastUsed;
	m_XferInfo.m_strPurse2TimeLastUsed = m_strPurse2TimeLastUsed;
	m_XferInfo.m_strDateLastRefreshed = m_strDateLastRefreshed;
	m_XferInfo.m_strTimeLastRefreshed = m_strTimeLastRefreshed;

	CString strComment = "";
	m_comboComments.GetWindowText(strComment);
	m_XferInfo.m_strComment = strComment;
	m_Comments.Save(strComment);				// save comment if required

	CDialog::OnOK();
}

//*******************************************************************

bool CRecordTransferDlg::CheckTransferValue(CString strType, CButton& check, CEdit* pEdit, int nValue, int nMinValue, int nMaxValue, bool& bOutFlag, int& nOutValue)
{
	bool bResult = TRUE;

	bOutFlag = IsTicked(check);

	if (TRUE == bOutFlag)
	{
		if (ValidateBalance(strType, nValue, nMinValue, nMaxValue) == FALSE)
		{
			pEdit->SetFocus();
			bResult = FALSE;
		}
		else
		{
			nOutValue = nValue;
		}
	}

	return bResult;
}

//*******************************************************************

bool CRecordTransferDlg::CheckTransferValue( CString strType, CButton& check, CEdit* pEdit, double dValue, double dMinValue, double dMaxValue, bool& bOutFlag, double& dOutValue )
{
	bool bResult = TRUE;

	bOutFlag = IsTicked(check);

	if (TRUE == bOutFlag)
	{
		if (ValidateBalance(strType, dValue, dMinValue, dMaxValue) == FALSE)
		{
			pEdit->SetFocus();
			bResult = FALSE;
		}
		else
		{
			dOutValue = dValue;
		}
	}

	return bResult;
}

//*******************************************************************

void CRecordTransferDlg::CheckTransferValue(bool bUseDate, CButton& checkSet, CEdit& edit, CButton& checkDate, CDateTimeCtrl& dtc, bool& bOutFlag, CString& strOutValue)
{
	bOutFlag = IsTicked(checkSet);
	
	if (TRUE == bOutFlag)
	{
		if (FALSE == bUseDate)
		{

			strOutValue = GetEditBoxText(edit);
		}
		else
		{
			GetNewDateInfo(FALSE, bOutFlag, strOutValue, checkDate, dtc);
		}
	}
}

//*******************************************************************

bool CRecordTransferDlg::ValidateBalance ( const char* szTitle, int& nValue, int nMin, int nMax )
{
	int nError = 0;

	if ( nValue < nMin )
	{
		nValue = nMin;
		nError = 1;						// below indicator
	}
	else if ( nValue > nMax )
	{
		nValue = nMax;
		nError = 2;						// aove indicator
	}
	else
	{
		return TRUE;
	}

	TransferAdjusted ( szTitle, nError );
	return FALSE;
}

//*******************************************************************

bool CRecordTransferDlg::ValidateBalance ( const char* szTitle, double& dValue, double dMin, double dMax )
{
	int nError = 0;

	if ( ::CompareDoubles( dValue, dMin, 3 ) == -1 )
	{
		dValue = dMin;
		nError = 1;						// below indicator
	}
	else if (::CompareDoubles(dValue, dMax, 3) == 1 )
	{
		dValue = dMax;
		nError = 2;						// above indicator
	}
	else
	{
		return TRUE;
	}

	TransferAdjusted ( szTitle, nError );
	return FALSE;
}

//*******************************************************************

void CRecordTransferDlg::GetNewDateInfo(bool bDOB, bool& bSaveTick, CString& strSaveDate, CButton& checkBox, CDateTimeCtrl& datePicker)
{
	bSaveTick = TRUE;
	strSaveDate = "";

	if (IsTicked(checkBox))
	{
		COleDateTime oleDate;
		datePicker.GetTime(oleDate);

		bool bOK = FALSE;
		if (TRUE == bDOB)
		{
			bOK = CAccountFieldValidator::ValidateDOB(oleDate);
		}
		else
		{
			bOK = CAccountFieldValidator::ValidateExpiry(oleDate);
		}

		if (FALSE == bOK)
		{
			bSaveTick = FALSE;
		}
		else
		{
			strSaveDate = CAccountFieldValidator::OleDateToString(oleDate);
		}
	}
}

//*******************************************************************

void CRecordTransferDlg::TransferAdjusted ( const char* szTitle, int nErrorNo )
{
	CString strMsg1 = ( nErrorNo == 1 ) ? "below the minimum" : "above the maximum";
	
	CString strMsg;
	strMsg.Format ( "Input adjusted - the entry is %s allowable value!", 
		(const char*) strMsg1 );

	MessageBeep ( MB_ICONSTOP );

	CString strTitle;
	strTitle.Format ( "Transfer %s Balance", szTitle );
	Prompter.FatalError( strMsg, strTitle );
	UpdateData(FALSE);
}

//*******************************************************************
