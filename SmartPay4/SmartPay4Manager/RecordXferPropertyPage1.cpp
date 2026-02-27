//$$******************************************************************
#include "..\SmartPay4Shared\SQLAuditRecord.h"
#include "..\SmartPay4Shared\ImageHandler.h"
#include "..\SmartPay4Shared\PasswordData.h"
#include "..\SmartPay4Shared\ServerData.h"
#include "..\SmartPay4Shared\SQLDefines.h"
#include "..\SmartPay4Shared\SQLSchema\SQLNamespaces.h"
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
#include "..\SmartPay4Shared\TLogFile.h"
//$$******************************************************************
#include "AllergiesDlg.h"
#include "GroupComboHelpers.h"
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "RecordXferPropertyPage1.h"
//$$******************************************************************
static const int nACTIVEBALANCES = 10;
//$$******************************************************************

CRecordXferPropertyPage1::CRecordXferPropertyPage1( CSQLRowAccountFull& RowAccountFrom, CRecordXferData& RecordXferData )
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CRecordXferPropertyPage1::IDD)
	, m_RowAccountFrom( RowAccountFrom)
	, m_Comments(Filenames.GetDBaseCommentsFilename())
	, m_RecordXferData( RecordXferData )
{
	m_dateToday.SetCurrentDate();

	Server.Read ( Filenames.GetServerDataFilename() );
	m_bAllowOverSpend = Server.GetAllowOverdraftFlag();
	m_bAllowPurse3 = System.GetEnablePurse3Flag();

	m_bHotlistReplace = FALSE;
	m_bSelectAllDetails = FALSE;
	m_bSelectAllBalances = FALSE;
	m_dMaxPurse1Credit = 0.0;
	m_dMaxPurse3Credit = 0.0;
	m_dMaxPurse2 = 0.0;
	m_dMaxPurse1Liability = 0.0;
	m_dMaxPurse3Liability = 0.0;
	m_dMaxPurse1SpendTD = 0.0;
	m_dMaxPurse2SpendTD = 0.0;
	m_dMaxPurse3SpendTD = 0.0;
	m_nMaxPoints = 0;
	m_nMaxPointsTD = 0;
	m_nActiveBalances = 0;
	m_nMinPoints = 0;
	m_nMinPointsTD = 0;
	m_dMinPurse1Liability = 0.0;
	m_dMinPurse1Credit = 0.0;
	m_dMinPurse2 = 0.0;
	m_dMinPurse3Liability = 0.0;
	m_dMinPurse3Credit = 0.0;
	m_dMinPurse1SpendTD = 0.0;
	m_dMinPurse2SpendTD = 0.0;
	m_dMinPurse3SpendTD = 0.0;
	m_nPoints = 0;
	m_nPointsTD = 0;
	m_bCanModifyMaxSpend = FALSE;
	m_nActiveBalances = 0;
	m_bHotlistReplace = FALSE;

	m_bCanModifyMaxSpend = FALSE;
	m_nOriginalGroupNo = 0;
	m_nOriginalGroupMaxSpendType = 0;
	m_dOriginalMaxSpendP1 = 0.0;
	m_dOriginalMaxSpendP3 = 0.0;
	m_dOriginalOverSpendP1 = 0.0;
	m_dOriginalOverSpendP3 = 0.0;

	m_dMaxSpendP1 = 0.0;
	m_dMaxSpendP3 = 0.0;
	m_dOverSpendP1 = 0.0;
	m_dOverSpendP3 = 0.0;

	m_dPurse1Credit = 0.0;
	m_dPurse1Liability = 0.0;
	m_dPurse1SpendTD = 0.0;
	m_dPurse2 = 0.0;
	m_dPurse2SpendTD = 0.0;
	m_dPurse3Credit = 0.0;
	m_dPurse3Liability = 0.0;
	m_dPurse3SpendTD = 0.0;
	
	m_bAllowCreditPurse1 = FALSE;
	m_bAllowCreditPurse3 = FALSE;
}

//$$******************************************************************

void CRecordXferPropertyPage1::SetPointer(const char* szCardTo, bool bHotlistReplace)
{
	m_bHotlistReplace = bHotlistReplace;
	m_strToCardNo = szCardTo;
}

//$$******************************************************************

CRecordXferPropertyPage1::~CRecordXferPropertyPage1()
{
}

//$$******************************************************************

void CRecordXferPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_USERNAME, m_checkUserName );
	DDX_Control(pDX, IDC_CHECK_SURNAME, m_checkSurname );
	DDX_Control(pDX, IDC_CHECK_FORENAME, m_checkForename );
	DDX_Control(pDX, IDC_CHECK_GROUPNO, m_checkGroup);
	DDX_Control(pDX, IDC_CHECK_MEMBERID, m_checkMemberID);
	DDX_Control(pDX, IDC_CHECK_EXPIRY, m_checkExpiry);
	/*****/
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUserName );
	DDX_Control(pDX, IDC_EDIT_SURNAME, m_editSurname );
	DDX_Control(pDX, IDC_EDIT_FORENAME, m_editForename );
	DDX_Control(pDX, IDC_COMBO_GROUP, m_tabcomboGroup);
	DDX_Control(pDX, IDC_EDIT_MEMBERID, m_editMemberID);
	DDX_Control(pDX, IDC_BUTTON_SELECTALL, m_buttonSelectAll);
	DDX_Control(pDX, IDC_COMBO_COMMENTS, m_comboComments);
	/*****/
	DDX_Control(pDX, IDC_CHECK_POINTS, m_checkPoints );
	DDX_Control(pDX, IDC_CHECK_POINTSTD, m_checkPointsTD );
	DDX_Control(pDX, IDC_CHECK_PURSE1_LIABILITY, m_checkPurse1Liability );
	DDX_Control(pDX, IDC_CHECK_PURSE1_CREDIT, m_checkPurse1Credit);
	DDX_Control(pDX, IDC_CHECK_PURSE1SPENDTD, m_checkPurse1SpendTD );
	DDX_Control(pDX, IDC_CHECK_PURSE2, m_checkPurse2 );
	DDX_Control(pDX, IDC_CHECK_PURSE2SPENDTD, m_checkPurse2SpendTD );
	DDX_Control(pDX, IDC_CHECK_PURSE3_LIABILITY, m_checkPurse3Liability);
	DDX_Control(pDX, IDC_CHECK_PURSE3_CREDIT, m_checkPurse3Credit);
	DDX_Control(pDX, IDC_CHECK_PURSE3SPENDTD, m_checkPurse3SpendTD );
	DDX_Control(pDX, IDC_CHECK_COPYPURCHASES, m_checkCopyPurchaseHistory);
	DDX_Control(pDX, IDC_CHECK_PURCHASECONTROL, m_checkPurchaseControl);
	DDX_Control(pDX, IDC_BUTTON_SELECTBALANCES, m_buttonSelectBalances);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DateTimePicker);
	/*****/
	DDX_Control(pDX, IDC_CHECK_MAXSPEND_COPY, m_checkMaxSpendCopy);
	DDX_Control(pDX, IDC_CHECK_MAXSPEND_MODIFY, m_checkMaxSpendModify);
	DDX_Control(pDX, IDC_STATIC_MAXSPENDP1, m_staticMaxSpendP1);
	DDX_Control(pDX, IDC_STATIC_MAXSPENDP3, m_staticMaxSpendP3);
	DDX_Control(pDX, IDC_STATIC_OVERSPENDP1, m_staticOverSpendP1);
	DDX_Control(pDX, IDC_STATIC_OVERSPENDP3, m_staticOverSpendP3);
	/*****/
	DDX_Text(pDX, IDC_EDIT_PURSE1_LIABILITY, m_dPurse1Liability);
	DDX_Text(pDX, IDC_EDIT_PURSE1_CREDIT, m_dPurse1Credit);
	DDX_Text(pDX, IDC_EDIT_PURSE2, m_dPurse2);
	DDX_Text(pDX, IDC_EDIT_POINTS, m_nPoints);
	DDX_Text(pDX, IDC_EDIT_PURSE3_LIABILITY, m_dPurse3Liability);
	DDX_Text(pDX, IDC_EDIT_PURSE3_CREDIT, m_dPurse3Credit);
	DDX_Text(pDX, IDC_EDIT_PURSE1SPENDTD, m_dPurse1SpendTD);
	DDX_Text(pDX, IDC_EDIT_PURSE2SPENDTD, m_dPurse2SpendTD);
	DDX_Text(pDX, IDC_EDIT_PURSE3SPENDTD, m_dPurse3SpendTD);
	DDX_Text(pDX, IDC_EDIT_POINTSTD, m_nPointsTD);
	DDX_Text(pDX, IDC_EDIT_MAXSPENDP1, m_dMaxSpendP1);
	DDX_Text(pDX, IDC_EDIT_MAXSPENDP3, m_dMaxSpendP3);
	DDX_Text(pDX, IDC_EDIT_OVERSPENDP1, m_dOverSpendP1);
	DDX_Text(pDX, IDC_EDIT_OVERSPENDP3, m_dOverSpendP3);
	/*****/
	DDV_MinMaxDouble(pDX, m_dPurse1Liability, m_dMinPurse1Liability, m_dMaxPurse1Liability);
	DDV_MinMaxDouble(pDX, m_dPurse1Credit, m_dMinPurse1Credit, m_dMaxPurse1Credit);
	DDV_MinMaxDouble(pDX, m_dPurse2, m_dMinPurse2, m_dMaxPurse2);
	DDV_MinMaxDouble(pDX, m_dPurse3Liability, m_dMinPurse3Liability, m_dMaxPurse3Liability);
	DDV_MinMaxDouble(pDX, m_dPurse3Credit, m_dMinPurse3Credit, m_dMaxPurse3Credit);
	DDV_MinMaxDouble(pDX, m_dPurse1SpendTD, m_dMinPurse1SpendTD, m_dMaxPurse1SpendTD);
	DDV_MinMaxDouble(pDX, m_dPurse2SpendTD, m_dMinPurse2SpendTD, m_dMaxPurse2SpendTD);
	DDV_MinMaxDouble(pDX, m_dPurse3SpendTD, m_dMinPurse3SpendTD, m_dMaxPurse3SpendTD);
	DDV_MinMaxInt(pDX, m_nPoints, m_nMinPoints, m_nMaxPoints);
	DDV_MinMaxInt(pDX, m_nPointsTD, m_nMinPointsTD, m_nMaxPointsTD);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CRecordXferPropertyPage1, CSSAutoShutdownPropertyPage)
	ON_STN_CLICKED(IDC_STATIC_COMMENT, OnClickStaticComment)
	ON_BN_CLICKED(IDC_CHECK_SURNAME, OnToggleSurname)
	ON_BN_CLICKED(IDC_CHECK_FORENAME, OnToggleForename)
	ON_BN_CLICKED(IDC_CHECK_MEMBERID, OnToggleMemberID)
	ON_BN_CLICKED(IDC_CHECK_GROUPNO, OnToggleGroupNo)
	ON_BN_CLICKED(IDC_CHECK_EXPIRY, OnToggleExpiry)
	ON_BN_CLICKED(IDC_CHECK_POINTS, OnTogglePoints)
	ON_BN_CLICKED(IDC_CHECK_POINTSTD, OnTogglePointsTD)
	ON_BN_CLICKED(IDC_CHECK_PURSE1_LIABILITY, OnTogglePurse1Liability)
	ON_BN_CLICKED(IDC_CHECK_PURSE1_CREDIT, OnTogglePurse1Credit)
	ON_BN_CLICKED(IDC_CHECK_PURSE2, OnTogglePurse2)
	ON_BN_CLICKED(IDC_CHECK_PURSE3_LIABILITY, OnTogglePurse3Liability)
	ON_BN_CLICKED(IDC_CHECK_PURSE3_CREDIT, OnTogglePurse3Credit)
	ON_BN_CLICKED(IDC_CHECK_PURSE1SPENDTD, OnTogglePurse1SpendTD)
	ON_BN_CLICKED(IDC_CHECK_PURSE2SPENDTD, OnTogglePurse2SpendTD)
	ON_BN_CLICKED(IDC_CHECK_PURSE3SPENDTD, OnTogglePurse3SpendTD)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, OnButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_SELECTBALANCES, OnButtonSelectBalances)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP, OnSelectGroup)
	ON_BN_CLICKED(IDC_CHECK_MAXSPEND_COPY, OnToggleCopyMaxSpend)
	ON_BN_CLICKED(IDC_CHECK_MAXSPEND_MODIFY, OnToggleModifyMaxSpend)
	ON_BN_CLICKED(IDC_CHECK_COPYPURCHASES, OnToggleCopyPurchases)
	ON_BN_CLICKED(IDC_CHECK_USERNAME, OnToggleUsername)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CRecordXferPropertyPage1::OnInitDialog()
{
	m_bAllowCreditPurse1 = FALSE;
	m_bAllowCreditPurse3 = FALSE;
	m_bAllowCreditPurse1 |= System.GetCreditOptionsFlag();
	m_bAllowCreditPurse3 |= System.GetCreditOptionsFlag();
	m_bAllowCreditPurse1 |= (CompareDoubles(m_RowAccountFrom.GetPurse1Credit(), 0.0, 3) != 0);
	m_bAllowCreditPurse3 |= (CompareDoubles(m_RowAccountFrom.GetPurse3Credit(), 0.0, 3) != 0);

	m_bSelectAllDetails = !m_bHotlistReplace;
	m_bSelectAllBalances = !m_bHotlistReplace;

	m_nPoints = m_RowAccountFrom.GetPoints();
	m_nPointsTD = m_RowAccountFrom.GetPointsToDate();
	m_dPurse1Liability = m_RowAccountFrom.GetPurse1Liability();
	m_dPurse1Credit = m_RowAccountFrom.GetPurse1Credit();
	m_dPurse1SpendTD = m_RowAccountFrom.GetPurse1SpendToDate();
	m_dPurse2 = m_RowAccountFrom.GetPurse2Balance();
	m_dPurse2SpendTD = m_RowAccountFrom.GetPurse2SpendToDate();
	m_dPurse3Liability = m_RowAccountFrom.GetPurse3Liability();
	m_dPurse3Credit = m_RowAccountFrom.GetPurse3Credit();
	m_dPurse3SpendTD = m_RowAccountFrom.GetPurse3SpendToDate();
	m_dMaxSpendP1 = m_RowAccountFrom.GetMaxSpendPurse1Period1();
	m_dMaxSpendP3 = m_RowAccountFrom.GetMaxSpendPurse3Period1();
	m_dOverSpendP1 = m_RowAccountFrom.GetMaxOverdraftPurse1Period1();
	m_dOverSpendP3 = m_RowAccountFrom.GetMaxOverdraftPurse3Period1();

	//*** set limits of balance transfers

	m_nActiveBalances = nACTIVEBALANCES;

	// Purse1
	if (m_dPurse1Liability < 0)
	{
		m_dMinPurse1Liability = m_dPurse1Liability;
		m_dMaxPurse1Liability = 0;
	}
	else
	{
		m_dMinPurse1Liability = 0;
		m_dMaxPurse1Liability = m_dPurse1Liability;

		if (m_dPurse1Liability == 0)
		{
			--m_nActiveBalances;
		}
	}

	//CREDIT SHOULD NEVER BE NEGATIVE
	if (m_dPurse1Credit <= 0.0)
	{
		m_dMinPurse1Credit = 0.0;
		m_dMaxPurse1Credit = 0.0;
		--m_nActiveBalances;
	}
	else
	{
		m_dMinPurse1Credit = 0.0;
		m_dMaxPurse1Credit = m_dPurse1Credit;
	}

	// Purse2
	if (m_dPurse2 < 0)
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

	// purse3
	if (m_dPurse3Liability < 0)
	{
		m_dMinPurse3Liability = m_dPurse3Liability;
		m_dMaxPurse3Liability = 0;
	}
	else
	{
		m_dMinPurse3Liability = 0;
		m_dMaxPurse3Liability = m_dPurse3Liability;

		if (m_dPurse3Liability == 0)
		{
			--m_nActiveBalances;
		}
	}

	//CREDIT SHOULD NEVER BE NEGATIVE
	if (m_dPurse3Credit <= 0.0)
	{
		m_dMinPurse3Credit = 0.0;
		m_dMaxPurse3Credit = 0.0;
		--m_nActiveBalances;
	}
	else
	{
		m_dMinPurse3Credit = 0.0;
		m_dMaxPurse3Credit = m_dPurse3Credit;
	}

	// Points
	if (m_nPoints < 0)
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

	if (m_nPointsTD < 0)					// Points TD
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

	if (m_dPurse1SpendTD < 0)				// Purse1 SpendTD
	{
		m_dMinPurse1SpendTD = m_dPurse1SpendTD;
		m_dMaxPurse1SpendTD = 0;
	}
	else
	{
		m_dMinPurse1SpendTD = 0;
		m_dMaxPurse1SpendTD = m_dPurse1SpendTD;

		if (m_dPurse1SpendTD == 0)
		{
			--m_nActiveBalances;
		}
	}

	if (m_dPurse2SpendTD < 0)				// Purse2 Spend
	{
		m_dMinPurse2SpendTD = m_dPurse2SpendTD;
		m_dMaxPurse2SpendTD = 0;
	}
	else
	{
		m_dMinPurse2SpendTD = 0;
		m_dMaxPurse2SpendTD = m_dPurse2SpendTD;

		if (m_dPurse2SpendTD == 0)
		{
			--m_nActiveBalances;
		}
	}

	if (m_dPurse3SpendTD < 0)				// Purse3 Spend
	{
		m_dMinPurse3SpendTD = m_dPurse3SpendTD;
		m_dMaxPurse3SpendTD = 0;
	}
	else
	{
		m_dMinPurse3SpendTD = 0;
		m_dMaxPurse3SpendTD = m_dPurse3SpendTD;

		if (m_dPurse3SpendTD == 0)
		{
			--m_nActiveBalances;
		}
	}

	CPropertyPage::OnInitDialog();

	m_checkUserName.SetCheck(m_bHotlistReplace);
	m_checkForename.SetCheck(m_bHotlistReplace);
	m_checkSurname.SetCheck(m_bHotlistReplace);
	m_checkGroup.SetCheck(m_bHotlistReplace);
	m_checkMemberID.SetCheck(m_bHotlistReplace);
	m_checkCopyPurchaseHistory.SetCheck(m_bHotlistReplace);
	m_checkPoints.SetCheck(m_bHotlistReplace);
	m_checkPointsTD.SetCheck(m_bHotlistReplace);
	m_checkPurse1Liability.SetCheck(m_bHotlistReplace);
	m_checkPurse1Credit.SetCheck(m_bHotlistReplace);
	m_checkPurse1SpendTD.SetCheck(m_bHotlistReplace);
	m_checkPurse2.SetCheck(m_bHotlistReplace);
	m_checkPurse2SpendTD.SetCheck(m_bHotlistReplace);
	m_checkPurse3Liability.SetCheck(m_bHotlistReplace);
	m_checkPurse3Credit.SetCheck(m_bHotlistReplace);
	m_checkPurse3SpendTD.SetCheck(m_bHotlistReplace);
	m_checkPurchaseControl.SetCheck(m_bHotlistReplace);
	m_checkMaxSpendCopy.SetCheck(FALSE);
	m_checkMaxSpendModify.SetCheck(FALSE);

	m_editUserName.SetWindowText(m_RowAccountFrom.GetUsername());
	m_editForename.SetWindowText(m_RowAccountFrom.GetForename());
	m_editSurname.SetWindowText(m_RowAccountFrom.GetSurname());
	m_editMemberID.SetWindowText(m_RowAccountFrom.GetMemberID());

	// expiry
	{
		m_dateExpiry.SetDate(m_RowAccountFrom.GetExpiryDate());

		if (m_dateExpiry.IsSet() == FALSE)
		{
			m_dateExpiry = m_dateToday;
		}

		CTime timeExpiry(m_dateExpiry.GetYear(), m_dateExpiry.GetMonth(), m_dateExpiry.GetDay(), 0, 0, 0);
		m_DateTimePicker.SetTime(&timeExpiry);

		m_checkExpiry.SetCheck((TRUE == m_bHotlistReplace) && (m_dateToday != m_dateExpiry));
	}

	CString strCaption;
	CString strProcessButtonText;

	if (m_bHotlistReplace == TRUE)
	{
		if (::FileExists(Filenames.GetPurchaseHistoryFilename(m_strToCardNo)) == FALSE)		// see if replacement card as a purchase history alreaay
		{
			m_checkCopyPurchaseHistory.SetWindowText("Carry forward purchase history");
		}
	}
	else
	{
		m_checkCopyPurchaseHistory.ShowWindow(SW_HIDE);
	}

	SubclassEdit(IDC_EDIT_DOB, SS_DATE, 10, "");
	SubclassEdit(IDC_EDIT_POINTS, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_POINTSTD, SS_NUM_S, 8, "%ld");
	SubclassEdit(IDC_EDIT_PURSE1_LIABILITY, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE1_CREDIT, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE2, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE3_LIABILITY, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE3_CREDIT, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE1SPENDTD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE2SPENDTD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_PURSE3SPENDTD, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_MAXSPENDP1, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_OVERSPENDP1, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_MAXSPENDP3, SS_NUM_SDP, 11, "%.2f");
	SubclassEdit(IDC_EDIT_OVERSPENDP3, SS_NUM_SDP, 11, "%.2f");

	//groups
	m_nOriginalGroupNo = m_RowAccountFrom.GetGroupNo();
	m_nOriginalGroupMaxSpendType = 0;
	SelectGroupFromCombo(m_nOriginalGroupNo);

	//max spend
	CSQLRowGroupFull RowGroup;
	RowGroup.SetGroupNo(m_nOriginalGroupNo);

	CSQLRepositoryGroup RepoGroup;
	if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() != SQLCRUD_ERR_NONE)
	{
		m_checkMaxSpendCopy.EnableWindow(FALSE);
		m_checkMaxSpendModify.EnableWindow(FALSE);
	}
	else
	{
		m_nOriginalGroupMaxSpendType = RowGroup.GetMaxSpendType();

		switch (m_nOriginalGroupMaxSpendType)
		{
		case nGRPSPENDTYPE_PERIOD_LOCKED:
		case nGRPSPENDTYPE_PERIOD_RECORD1:
		case nGRPSPENDTYPE_PERIOD_RECORD2:
			m_checkMaxSpendCopy.SetCheck(m_bHotlistReplace);
			ShowAndEnableWindow(&m_checkMaxSpendModify, FALSE);
			ShowAndEnableWindow(&m_staticMaxSpendP3, FALSE);
			ShowAndEnableWindow(&m_staticOverSpendP1, FALSE);
			ShowAndEnableWindow(&m_staticOverSpendP3, FALSE);
			ShowAndEnableWindow(GetEditMaxSpendP1(), FALSE);
			ShowAndEnableWindow(GetEditMaxSpendP3(), FALSE);
			ShowAndEnableWindow(GetEditOverSpendP1(), FALSE);
			ShowAndEnableWindow(GetEditOverSpendP3(), FALSE);
			MoveControl(&m_staticMaxSpendP1, 259, 240, 180, 50);
			m_staticMaxSpendP1.SetWindowText("Max Spend amounts are by audit period, and may be\nedited by group or by account after transfer.");
			break;

		case nGRPSPENDTYPE_DAILY_LOCKED:
		case nGRPSPENDTYPE_DAILY_RECORD1:
		case nGRPSPENDTYPE_DAILY_RECORD2:
		{
			m_checkMaxSpendCopy.SetCheck(m_bHotlistReplace);

			bool bUseGroup = TRUE;
			switch (m_nOriginalGroupMaxSpendType)
			{
			case nGRPSPENDTYPE_DAILY_RECORD1:
				bUseGroup = FALSE;
				m_bCanModifyMaxSpend = TRUE;
				break;

			case nGRPSPENDTYPE_DAILY_RECORD2:
				bUseGroup = (m_RowAccountFrom.GetOwnMaxSpend() == FALSE);
				m_bCanModifyMaxSpend = TRUE;
				break;
			}

			if (TRUE == bUseGroup)
			{
				m_dMaxSpendP1 = RowGroup.GetMaxSpendPurse1Period1();
				m_dMaxSpendP3 = RowGroup.GetMaxSpendPurse3Period1();
				m_dOverSpendP1 = RowGroup.GetMaxOverdraftPurse1Period1();
				m_dOverSpendP3 = RowGroup.GetMaxOverdraftPurse3Period1();
			}
			else
			{
				m_dMaxSpendP1 = m_RowAccountFrom.GetMaxSpendPurse1Period1();
				m_dMaxSpendP3 = m_RowAccountFrom.GetMaxSpendPurse3Period1();
				m_dOverSpendP1 = m_RowAccountFrom.GetMaxOverdraftPurse1Period1();
				m_dOverSpendP3 = m_RowAccountFrom.GetMaxOverdraftPurse3Period1();
			}

			m_dOriginalMaxSpendP1 = m_dMaxSpendP1;
			m_dOriginalMaxSpendP3 = m_dMaxSpendP3;
			m_dOriginalOverSpendP1 = m_dOverSpendP1;
			m_dOriginalOverSpendP3 = m_dOverSpendP3;
			ShowOriginalMaxSpend();

			if (FALSE == m_bCanModifyMaxSpend)
			{
				ShowAndEnableWindow(&m_checkMaxSpendModify, FALSE);
				GetEditMaxSpendP1()->EnableWindow(FALSE);
				GetEditMaxSpendP3()->EnableWindow(FALSE);
				GetEditOverSpendP1()->EnableWindow(FALSE);
				GetEditOverSpendP3()->EnableWindow(FALSE);
				MoveControl(&m_staticMaxSpendP1, 263, 235);
				MoveControl(GetEditMaxSpendP1(), 315, 233);
				MoveControl(&m_staticOverSpendP1, 374, 235);
				MoveControl(GetEditOverSpendP1(), 412, 233);
				MoveControl(&m_staticMaxSpendP3, 263, 253);
				MoveControl(GetEditMaxSpendP3(), 315, 251);
				MoveControl(&m_staticOverSpendP3, 374, 253);
				MoveControl(GetEditOverSpendP3(), 412, 251);
			}
		}
		break;

		default:
			m_checkMaxSpendCopy.EnableWindow(FALSE);
			m_checkMaxSpendModify.EnableWindow(FALSE);
			m_nOriginalGroupMaxSpendType = 0;
			break;
		}
	}

	if (m_checkMaxSpendCopy.GetCheck() == TRUE)
	{
		m_checkGroup.SetCheck(FALSE);
		m_checkGroup.EnableWindow(FALSE);
		m_checkMaxSpendModify.EnableWindow(m_bCanModifyMaxSpend);
	}
	else
	{
		m_checkMaxSpendModify.EnableWindow(FALSE);
	}

	// other

	if (m_nPoints == 0)
	{
		m_checkPoints.EnableWindow(FALSE);
	}

	if (m_nPointsTD == 0)
	{
		m_checkPointsTD.EnableWindow(FALSE);
	}

	if (m_dPurse1Liability == 0.0)
	{
		m_checkPurse1Liability.EnableWindow(FALSE);
	}

	if (m_dPurse1Credit == 0.0)
	{
		m_checkPurse1Credit.EnableWindow(FALSE);
	}

	if (m_dPurse1SpendTD == 0.0)
	{
		m_checkPurse1SpendTD.EnableWindow(FALSE);
	}

	if (m_dPurse2 == 0.0)
	{
		m_checkPurse2.EnableWindow(FALSE);
	}

	if (m_dPurse2SpendTD == 0.0)
	{
		m_checkPurse2SpendTD.EnableWindow(FALSE);
	}

	if (m_dPurse3Liability == 0.0)
	{
		m_checkPurse3Liability.EnableWindow(FALSE);
	}

	if (m_dPurse3Credit == 0.0)
	{
		m_checkPurse3Credit.EnableWindow(FALSE);
	}

	if (m_dPurse3SpendTD == 0.0)
	{
		m_checkPurse3SpendTD.EnableWindow(FALSE);
	}

	CSQLRepositoryUserText RepoText;
	m_checkMemberID.SetWindowText(RepoText.GetMemberIDText());

	OnButtonSelectBalances();
	OnButtonSelect();
	SetDisplayPurse2();

	AddComments();

	if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
	{
		m_checkMemberID.SetCheck(FALSE);
		m_checkMemberID.EnableWindow(FALSE);
		m_editMemberID.EnableWindow(FALSE);
	}

	if (FALSE == m_bAllowCreditPurse1)
	{
		ShowAndEnableWindow(&m_checkPurse1Credit, FALSE);
		ShowAndEnableWindow(GetEditPurse1Credit(), FALSE);
		m_checkPurse1Liability.SetWindowText("Purse 1 Balance");
	}

	if (FALSE == m_bAllowCreditPurse3)
	{
		ShowAndEnableWindow(&m_checkPurse3Credit, FALSE);
		ShowAndEnableWindow(GetEditPurse3Credit(), FALSE);
		m_checkPurse3Liability.SetWindowText("Purse 3 Balance");
	}

	return TRUE;
}

//$$******************************************************************
CEdit* CRecordXferPropertyPage1::GetEditPoints(){ return GetEdit( IDC_EDIT_POINTS ); }
CEdit* CRecordXferPropertyPage1::GetEditPointsTD(){ return GetEdit( IDC_EDIT_POINTSTD ); }
CEdit* CRecordXferPropertyPage1::GetEditPurse1Liability(){ return GetEdit( IDC_EDIT_PURSE1_LIABILITY ); }
CEdit* CRecordXferPropertyPage1::GetEditPurse1Credit() { return GetEdit(IDC_EDIT_PURSE1_CREDIT); }
CEdit* CRecordXferPropertyPage1::GetEditPurse1SpendTD(){ return GetEdit( IDC_EDIT_PURSE1SPENDTD ); }
CEdit* CRecordXferPropertyPage1::GetEditPurse2(){ return GetEdit( IDC_EDIT_PURSE2 ); }
CEdit* CRecordXferPropertyPage1::GetEditPurse2SpendTD(){ return GetEdit( IDC_EDIT_PURSE2SPENDTD ); }
CEdit* CRecordXferPropertyPage1::GetEditPurse3Liability() { return GetEdit(IDC_EDIT_PURSE3_LIABILITY); }
CEdit* CRecordXferPropertyPage1::GetEditPurse3Credit() { return GetEdit(IDC_EDIT_PURSE3_CREDIT); }
CEdit* CRecordXferPropertyPage1::GetEditPurse3SpendTD(){ return GetEdit( IDC_EDIT_PURSE3SPENDTD ); }
CEdit* CRecordXferPropertyPage1::GetEditMaxSpendP1(){ return GetEdit( IDC_EDIT_MAXSPENDP1 ); }
CEdit* CRecordXferPropertyPage1::GetEditMaxSpendP3(){ return GetEdit( IDC_EDIT_MAXSPENDP3 ); }
CEdit* CRecordXferPropertyPage1::GetEditOverSpendP1(){ return GetEdit( IDC_EDIT_OVERSPENDP1 ); }
CEdit* CRecordXferPropertyPage1::GetEditOverSpendP3(){ return GetEdit( IDC_EDIT_OVERSPENDP3 ); }
//$$******************************************************************

void CRecordXferPropertyPage1::SelectGroupFromCombo( int nGroupNo )
{
	CGroupComboHelpers::FillGroupCombo(m_tabcomboGroup, nGroupNo, FALSE);	
}

//$$******************************************************************

void CRecordXferPropertyPage1::AddComments()
{
	for ( int i = 0 ; i < m_Comments.GetSize() ; i++ )		// read previous comments
		m_comboComments.AddString ( m_Comments.GetAt(i) );
}

//$$******************************************************************

CString CRecordXferPropertyPage1::GetExpiryDate()
{
	CString strExpiry = "";
	if ( IsTicked(m_checkExpiry) )
	{
		CTime time;
		m_DateTimePicker.GetTime(time);
		
		CSSDate date;
		date.SetDate(time);
		strExpiry = date.GetDate();
	}		
	return strExpiry;
}


//$$******************************************************************

void CRecordXferPropertyPage1::OnClickStaticComment()
{
	CString strComment;
	m_comboComments.GetWindowText( strComment );

	if ( strComment != "" )
	{
		m_Comments.Delete ( strComment );			// delete comment
		m_comboComments.ResetContent();				// new list
		AddComments();
		m_comboComments.SetWindowText( "" );
	}
}

//$$******************************************************************

void CRecordXferPropertyPage1::OnButtonSelect()
{
	if (UpdateData() == TRUE)
	{
		m_bSelectAllDetails = !m_bSelectAllDetails;
		CString strButton = (m_bSelectAllDetails == TRUE) ? "Clear" : "Select All";
		m_buttonSelectAll.SetWindowText(strButton);

		m_checkUserName.SetCheck(m_bSelectAllDetails);
		m_checkSurname.SetCheck(m_bSelectAllDetails);
		m_checkForename.SetCheck(m_bSelectAllDetails);

		if (m_checkGroup.IsWindowEnabled() == TRUE)
		{
			m_checkGroup.SetCheck(m_bSelectAllDetails);
		}

		m_checkMemberID.SetCheck(m_bSelectAllDetails);
		m_checkExpiry.SetCheck(m_bSelectAllDetails & (m_dateToday != m_dateExpiry));
		m_checkCopyPurchaseHistory.SetCheck(m_bSelectAllDetails);

		UpdateData(FALSE);
		SetDisplay();
	}
}

//$$******************************************************************

void CRecordXferPropertyPage1::OnButtonSelectBalances()
{
	if (UpdateData() == TRUE)
	{
		m_bSelectAllBalances = !m_bSelectAllBalances;
		CString strButton = (m_bSelectAllBalances == TRUE) ? "Clear" : "Select All";

		if (m_nActiveBalances > 0)
		{
			m_buttonSelectBalances.SetWindowText(strButton);
		}
		else
		{
			m_buttonSelectBalances.EnableWindow(FALSE);
		}

		m_checkPoints.SetCheck((0 == m_nPoints) ? FALSE : m_bSelectAllBalances);
		m_checkPointsTD.SetCheck((0 == m_nPointsTD) ? FALSE : m_bSelectAllBalances);
		m_checkPurse1Liability.SetCheck((0 == m_dPurse1Liability) ? FALSE : m_bSelectAllBalances);
		m_checkPurse1Credit.SetCheck((0 == m_dPurse1Credit) ? FALSE : m_bSelectAllBalances);
		m_checkPurse1SpendTD.SetCheck((0 == m_dPurse1SpendTD) ? FALSE : m_bSelectAllBalances);
		m_checkPurse2.SetCheck((0 == m_dPurse2) ? FALSE : m_bSelectAllBalances);
		m_checkPurse2SpendTD.SetCheck((0 == m_dPurse2SpendTD) ? FALSE : m_bSelectAllBalances);
		m_checkPurse3Liability.SetCheck((0 == m_dPurse3Liability) ? FALSE : m_bSelectAllBalances);
		m_checkPurse3Credit.SetCheck((0 == m_dPurse3Credit) ? FALSE : m_bSelectAllBalances);
		m_checkPurse3SpendTD.SetCheck((0 == m_dPurse3SpendTD) ? FALSE : m_bSelectAllBalances);

		bool bAllowed = FALSE;
		{
			CSQLRowGroupFull RowGroup;
			RowGroup.SetGroupNo(ExtractGroupNo());

			CSQLRepositoryGroup RepoGroup;
			if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
			{
				switch (RowGroup.GetMaxSpendType())
				{
				case nGRPSPENDTYPE_DAILY_RECORD1:
				case nGRPSPENDTYPE_DAILY_RECORD2:
				case nGRPSPENDTYPE_PERIOD_RECORD1:
				case nGRPSPENDTYPE_PERIOD_RECORD2:
					bAllowed = TRUE;
					break;
				}
			}
		}

		UpdateData(FALSE);
		SetDisplay();
	}
}

//$$******************************************************************
void CRecordXferPropertyPage1::OnToggleUsername()		{	SetDisplay();	}
void CRecordXferPropertyPage1::OnToggleSurname()		{	SetDisplay();	}
void CRecordXferPropertyPage1::OnToggleForename()		{	SetDisplay();	}
void CRecordXferPropertyPage1::OnToggleMemberID()		{	SetDisplay();	}
void CRecordXferPropertyPage1::OnToggleExpiry()			{	SetDisplay();	}
void CRecordXferPropertyPage1::OnToggleGroupNo()		{	SetDisplay();	}
void CRecordXferPropertyPage1::OnToggleCopyPurchases()	{	SetDisplay();	}
void CRecordXferPropertyPage1::OnTogglePoints()			{	SetDisplay();	}
void CRecordXferPropertyPage1::OnTogglePointsTD()		{	SetDisplay();	}
void CRecordXferPropertyPage1::OnTogglePurse1Liability(){	SetDisplay();	}
void CRecordXferPropertyPage1::OnTogglePurse1Credit()	{	SetDisplay();	}
void CRecordXferPropertyPage1::OnTogglePurse1SpendTD()	{	SetDisplay();	}
void CRecordXferPropertyPage1::OnTogglePurse2()			{	SetDisplay();	}
void CRecordXferPropertyPage1::OnTogglePurse2SpendTD()	{	SetDisplay();	}
void CRecordXferPropertyPage1::OnTogglePurse3Liability(){	SetDisplay();	}
void CRecordXferPropertyPage1::OnTogglePurse3Credit()	{	SetDisplay();	}
void CRecordXferPropertyPage1::OnTogglePurse3SpendTD()	{	SetDisplay();	}
//$$******************************************************************

void CRecordXferPropertyPage1::SetDisplay()
{
	if (UpdateData() == TRUE)
	{
		m_editUserName.EnableWindow(IsTicked(m_checkUserName));
		m_editForename.EnableWindow(IsTicked(m_checkForename));
		m_editSurname.EnableWindow(IsTicked(m_checkSurname));
		m_tabcomboGroup.EnableWindow(IsTicked(m_checkGroup) & (m_checkGroup.IsWindowEnabled() == TRUE));

		if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
		{
			m_editMemberID.EnableWindow(FALSE);
		}
		else
		{
			m_editMemberID.EnableWindow(IsTicked(m_checkMemberID));
		}

		m_DateTimePicker.EnableWindow(IsTicked(m_checkExpiry));
		GetEditPoints()->EnableWindow(IsTicked(m_checkPoints));
		GetEditPointsTD()->EnableWindow(IsTicked(m_checkPointsTD));
		GetEditPurse1Liability()->EnableWindow(IsTicked(m_checkPurse1Liability));
		GetEditPurse1Credit()->EnableWindow(IsTicked(m_checkPurse1Credit) & m_bAllowCreditPurse1);
		GetEditPurse1SpendTD()->EnableWindow(IsTicked(m_checkPurse1SpendTD));
		GetEditPurse2()->EnableWindow(IsTicked(m_checkPurse2));
		GetEditPurse2SpendTD()->EnableWindow(IsTicked(m_checkPurse2SpendTD));
		GetEditPurse3Liability()->EnableWindow(IsTicked(m_checkPurse3Liability));
		GetEditPurse3Credit()->EnableWindow(IsTicked(m_checkPurse3Credit) & m_bAllowCreditPurse3);
		GetEditPurse3SpendTD()->EnableWindow(IsTicked(m_checkPurse3SpendTD));

		bool bEditMaxSpend = (IsTicked(m_checkMaxSpendCopy) && IsTicked(m_checkMaxSpendModify) && m_bCanModifyMaxSpend);
		GetEditMaxSpendP1()->EnableWindow(bEditMaxSpend);
		GetEditMaxSpendP3()->EnableWindow(bEditMaxSpend & m_bAllowPurse3);
		GetEditOverSpendP1()->EnableWindow(bEditMaxSpend & m_bAllowOverSpend);
		GetEditOverSpendP3()->EnableWindow(bEditMaxSpend & m_bAllowPurse3 & m_bAllowOverSpend);
	}
}

//$$******************************************************************

bool CRecordXferPropertyPage1::StartAllowed()
{
	if (IsTicked(m_checkUserName))				return TRUE;
	if (IsTicked(m_checkSurname))				return TRUE;
	if (IsTicked(m_checkForename))				return TRUE;
	if (IsTicked(m_checkGroup))					return TRUE;
	if (IsTicked(m_checkMemberID))				return TRUE;
	if (IsTicked(m_checkExpiry))				return TRUE;
	if (IsTicked(m_checkCopyPurchaseHistory))	return TRUE;
	if (IsTicked(m_checkPoints))				return TRUE;
	if (IsTicked(m_checkPointsTD))				return TRUE;
	if (IsTicked(m_checkPurse1Liability)) 		return TRUE;
	if (IsTicked(m_checkPurse1Credit) && m_bAllowCreditPurse1) return TRUE;
	if (IsTicked(m_checkPurse1SpendTD))			return TRUE;
	if (IsTicked(m_checkPurse2))				return TRUE;
	if (IsTicked(m_checkPurse2SpendTD))			return TRUE;
	if (IsTicked(m_checkPurse3Liability) && m_bAllowCreditPurse3) return TRUE;
	if (IsTicked(m_checkPurse3Credit)) 			return TRUE;
	if (IsTicked(m_checkPurse3SpendTD))			return TRUE;
	if (IsTicked(m_checkPurchaseControl))		return TRUE;
	if (IsTicked(m_checkMaxSpendCopy))			return TRUE;
	return FALSE;
}

//$$******************************************************************

int CRecordXferPropertyPage1::ExtractGroupNo()
{
	return CGroupComboHelpers::GetGroupNoFromCombo(m_tabcomboGroup, Group::GroupNo.Min);
}

//$$******************************************************************

void CRecordXferPropertyPage1::OnSelectGroup()
{
	if ( UpdateData() == TRUE )
	{
		SetDisplayPurse2();
	}
}

//$$******************************************************************

void CRecordXferPropertyPage1::ShowOriginalMaxSpend()
{
	{
		CString strMaxSpend = "";
		strMaxSpend.Format( "%.2f", m_dOriginalMaxSpendP1 );
		GetEditMaxSpendP1() -> SetWindowText( strMaxSpend );
		
		strMaxSpend.Format( "%.2f", m_dOriginalMaxSpendP3 );
		GetEditMaxSpendP3() -> SetWindowText( strMaxSpend );
	}

	{
		CString strOverSpend = "";
		strOverSpend.Format( "%.2f", m_dOriginalOverSpendP1 );
		GetEditOverSpendP1() -> SetWindowText( strOverSpend );

		strOverSpend.Format( "%.2f", m_dOriginalOverSpendP3 );
		GetEditOverSpendP3() -> SetWindowText( strOverSpend );
	}
}

//$$******************************************************************

void CRecordXferPropertyPage1::OnToggleCopyMaxSpend()	
{
	if ( IsTicked( m_checkMaxSpendCopy ) == TRUE )
	{
		m_checkMaxSpendModify.EnableWindow( m_bCanModifyMaxSpend );
		m_checkGroup.SetCheck( TRUE );
		m_checkGroup.EnableWindow(FALSE);
		SelectGroupFromCombo( m_nOriginalGroupNo );
		m_tabcomboGroup.EnableWindow(FALSE);
	}
	else
	{
		m_checkMaxSpendModify.EnableWindow( FALSE );
		m_checkGroup.EnableWindow(TRUE);
		m_tabcomboGroup.EnableWindow( IsTicked( m_checkGroup ) );
	}

	m_checkMaxSpendModify.SetCheck( FALSE );
	GetEditMaxSpendP1() -> EnableWindow( FALSE );
	GetEditMaxSpendP3() -> EnableWindow( FALSE );
	GetEditOverSpendP1() -> EnableWindow( FALSE );
	GetEditOverSpendP3() -> EnableWindow( FALSE );

	if ( TRUE == m_bCanModifyMaxSpend )
		ShowOriginalMaxSpend();
}

//$$******************************************************************

void CRecordXferPropertyPage1::OnToggleModifyMaxSpend()
{
	if ( IsTicked( m_checkMaxSpendCopy ) && m_bCanModifyMaxSpend )
	{
		ShowOriginalMaxSpend();
		GetEditMaxSpendP1() -> EnableWindow( IsTicked(m_checkMaxSpendModify) );
		GetEditMaxSpendP3() -> EnableWindow( IsTicked(m_checkMaxSpendModify) & m_bAllowPurse3 );
		GetEditOverSpendP1() -> EnableWindow( IsTicked(m_checkMaxSpendModify) & m_bAllowOverSpend );
		GetEditOverSpendP3() -> EnableWindow( IsTicked(m_checkMaxSpendModify) & m_bAllowPurse3 & m_bAllowOverSpend );
	}
}

//$$******************************************************************

void CRecordXferPropertyPage1::SetDisplayPurse2()
{
	bool bCheckPurse2Allowed = (m_dPurse2 == 0) ? FALSE : TRUE;
	bool bCheckPurse2SpendAllowed = (m_dPurse2SpendTD == 0) ? FALSE : TRUE;

	bool bEditPurse2Allowed = (m_dPurse2 != 0) && IsTicked(m_checkPurse2);
	bool bEditPurse2SpendAllowed = (m_dPurse2SpendTD != 0) && IsTicked(m_checkPurse2SpendTD);

	CSQLRowGroupFull RowGroup;
	RowGroup.SetGroupNo(ExtractGroupNo());

	CSQLRepositoryGroup RepoGroup;
	if (RepoGroup.SelectRow(RowGroup, NULL).GetSQLError() == SQLCRUD_ERR_NONE)
	{
		switch (RowGroup.GetRefreshType())
		{
		case nREFRESHTYPE_DISABLED:
		case nREFRESHTYPE_MANUAL:
			break;

		default:
			bEditPurse2Allowed = FALSE;
			bCheckPurse2Allowed = FALSE;
			bEditPurse2SpendAllowed = FALSE;
			bCheckPurse2SpendAllowed = FALSE;
			break;
		}
	}

	m_checkPurse2.EnableWindow(bCheckPurse2Allowed);
	GetEditPurse2()->EnableWindow(bEditPurse2Allowed);
	m_checkPurse2SpendTD.EnableWindow(bCheckPurse2SpendAllowed);
	GetEditPurse2SpendTD()->EnableWindow(bEditPurse2SpendAllowed);
}

//$$******************************************************************

void CRecordXferPropertyPage1::OnOK()
{
	if (UpdateData() == TRUE)
	{
		CString strComment;
		m_comboComments.GetWindowText(strComment);
		m_Comments.Save(strComment); // save comment if required

		m_RecordXferData.m_bUserName = (IsTicked(m_checkUserName));
		m_RecordXferData.m_bForename = (IsTicked(m_checkForename));
		m_RecordXferData.m_bSurname = (IsTicked(m_checkSurname));
		m_RecordXferData.m_bGroup = (IsTicked(m_checkGroup));

		if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
		{
			m_RecordXferData.m_bMemberID = FALSE;
		}
		else
		{
			m_RecordXferData.m_bMemberID = (IsTicked(m_checkMemberID));
		}

		m_RecordXferData.m_bExpiry = (IsTicked(m_checkExpiry));
		m_RecordXferData.m_bCopyPurchaseHistory = (IsTicked(m_checkCopyPurchaseHistory));
		/*****/
		m_editUserName.GetWindowText(m_RecordXferData.m_strUserName);
		m_editForename.GetWindowText(m_RecordXferData.m_strForename);
		m_editSurname.GetWindowText(m_RecordXferData.m_strSurname);
		m_RecordXferData.m_nGroupNo = ExtractGroupNo();
		m_RecordXferData.m_strDayGroups = m_RowAccountFrom.GetDayGroups();
		m_editMemberID.GetWindowText(m_RecordXferData.m_strMemberID);
		m_RecordXferData.m_strExpiryDate = GetExpiryDate();
		m_RecordXferData.m_strComment = strComment;
		/*****/
		m_RecordXferData.m_bPoints = (IsTicked(m_checkPoints));
		m_RecordXferData.m_bPointsTD = (IsTicked(m_checkPointsTD));
		m_RecordXferData.m_bPurse1Liability = (IsTicked(m_checkPurse1Liability));
		m_RecordXferData.m_bPurse1Credit = (IsTicked(m_checkPurse1Credit));
		m_RecordXferData.m_bPurse1SpendTD = (IsTicked(m_checkPurse1SpendTD));
		m_RecordXferData.m_bPurse2 = (IsTicked(m_checkPurse2));
		m_RecordXferData.m_bPurse2SpendTD = (IsTicked(m_checkPurse2SpendTD));
		m_RecordXferData.m_bPurse3Liability = (IsTicked(m_checkPurse3Liability));
		m_RecordXferData.m_bPurse3Credit = (IsTicked(m_checkPurse3Credit));
		m_RecordXferData.m_bPurse3SpendTD = (IsTicked(m_checkPurse3SpendTD));
		m_RecordXferData.m_bPurchaseControl = (IsTicked(m_checkPurchaseControl));
		m_RecordXferData.m_bMaxSpend = (IsTicked(m_checkMaxSpendCopy));
		/*****/
		m_RecordXferData.m_nPoints = m_nPoints;
		m_RecordXferData.m_nPointsTD = m_nPointsTD;
		m_RecordXferData.m_dPurse1Liability = m_dPurse1Liability;
		m_RecordXferData.m_dPurse1Credit = m_dPurse1Credit;
		m_RecordXferData.m_dPurse1SpendTD = m_dPurse1SpendTD;
		m_RecordXferData.m_dPurse2 = m_dPurse2;
		m_RecordXferData.m_dPurse2SpendTD = m_dPurse2SpendTD;
		m_RecordXferData.m_dPurse3Liability = m_dPurse3Liability;
		m_RecordXferData.m_dPurse3Credit = m_dPurse3Credit;
		m_RecordXferData.m_dPurse3SpendTD = m_dPurse3SpendTD;
		/*****/
		m_RecordXferData.m_dPurse1Spend = m_RowAccountFrom.GetPurse1LastSpend();
		m_RecordXferData.m_strPurse1DateLastUsed = m_RowAccountFrom.GetPurse1LastSpendDate();
		m_RecordXferData.m_strPurse1TimeLastUsed = m_RowAccountFrom.GetPurse1LastSpendTime();
		/*****/
		m_RecordXferData.m_dPurse2Spend = m_RowAccountFrom.GetPurse2LastSpend();
		m_RecordXferData.m_strPurse2DateLastUsed = m_RowAccountFrom.GetPurse2LastSpendDate();
		m_RecordXferData.m_strPurse2TimeLastUsed = m_RowAccountFrom.GetPurse2LastSpendTime();
		/*****/
		m_RecordXferData.m_dPurse3Spend = m_RowAccountFrom.GetPurse3LastSpend();
		m_RecordXferData.m_strPurse3DateLastUsed = m_RowAccountFrom.GetPurse3LastSpendDate();
		m_RecordXferData.m_strPurse3TimeLastUsed = m_RowAccountFrom.GetPurse3LastSpendTime();
		/*****/
		m_RecordXferData.m_strRefreshedDate = m_RowAccountFrom.GetPurse2RefreshedDate();
		m_RecordXferData.m_strRefreshedTime = m_RowAccountFrom.GetPurse2RefreshedTime();
		/*****/
		m_RecordXferData.m_bOwnMaxSpend = m_RowAccountFrom.GetOwnMaxSpend();
		/*****/
		m_RecordXferData.m_dMaxSpend[1] = m_RowAccountFrom.GetMaxSpendPurse1Period1();
		m_RecordXferData.m_dMaxSpend[2] = m_RowAccountFrom.GetMaxSpendPurse1Period2();
		m_RecordXferData.m_dMaxSpend[3] = m_RowAccountFrom.GetMaxSpendPurse1Period3();
		m_RecordXferData.m_dMaxSpend[4] = m_RowAccountFrom.GetMaxSpendPurse1Period4();
		m_RecordXferData.m_dMaxSpend[5] = m_RowAccountFrom.GetMaxSpendPurse1Period5();
		m_RecordXferData.m_dMaxSpend[6] = m_RowAccountFrom.GetMaxSpendPurse1Period6();
		/*****/
		m_RecordXferData.m_dOverSpend[1] = m_RowAccountFrom.GetMaxOverdraftPurse1Period1();
		m_RecordXferData.m_dOverSpend[2] = m_RowAccountFrom.GetMaxOverdraftPurse1Period2();
		m_RecordXferData.m_dOverSpend[3] = m_RowAccountFrom.GetMaxOverdraftPurse1Period3();
		m_RecordXferData.m_dOverSpend[4] = m_RowAccountFrom.GetMaxOverdraftPurse1Period4();
		m_RecordXferData.m_dOverSpend[5] = m_RowAccountFrom.GetMaxOverdraftPurse1Period5();
		m_RecordXferData.m_dOverSpend[6] = m_RowAccountFrom.GetMaxOverdraftPurse1Period6();
		/*****/
		m_RecordXferData.m_dMaxSpendP3[1] = m_RowAccountFrom.GetMaxSpendPurse3Period1();
		m_RecordXferData.m_dMaxSpendP3[2] = m_RowAccountFrom.GetMaxSpendPurse3Period2();
		m_RecordXferData.m_dMaxSpendP3[3] = m_RowAccountFrom.GetMaxSpendPurse3Period3();
		m_RecordXferData.m_dMaxSpendP3[4] = m_RowAccountFrom.GetMaxSpendPurse3Period4();
		m_RecordXferData.m_dMaxSpendP3[5] = m_RowAccountFrom.GetMaxSpendPurse3Period5();
		m_RecordXferData.m_dMaxSpendP3[6] = m_RowAccountFrom.GetMaxSpendPurse3Period6();
		/*****/
		m_RecordXferData.m_dOverSpendP3[1] = m_RowAccountFrom.GetMaxOverdraftPurse3Period1();
		m_RecordXferData.m_dOverSpendP3[2] = m_RowAccountFrom.GetMaxOverdraftPurse3Period2();
		m_RecordXferData.m_dOverSpendP3[3] = m_RowAccountFrom.GetMaxOverdraftPurse3Period3();
		m_RecordXferData.m_dOverSpendP3[4] = m_RowAccountFrom.GetMaxOverdraftPurse3Period4();
		m_RecordXferData.m_dOverSpendP3[5] = m_RowAccountFrom.GetMaxOverdraftPurse3Period5();
		m_RecordXferData.m_dOverSpendP3[6] = m_RowAccountFrom.GetMaxOverdraftPurse3Period6();
		/*****/
		for (int nPeriod = 1; nPeriod <= 6; nPeriod++)
		{
			for (int nPurse = 1; nPurse <= 3; nPurse++)
			{
				m_RecordXferData.m_bPurseAllowed[nPeriod][nPurse] = m_RowAccountFrom.IsPurseAllowed(nPurse, nPeriod - 1);
			}
		}
		/*****/


		if (IsTicked(m_checkMaxSpendModify) && (m_bCanModifyMaxSpend))
		{
			m_RecordXferData.m_bOwnMaxSpend = TRUE;
			m_RecordXferData.m_dMaxSpend[1] = m_dMaxSpendP1;
			m_RecordXferData.m_dOverSpend[1] = m_dOverSpendP1;
			m_RecordXferData.m_dMaxSpendP3[1] = m_dMaxSpendP3;
			m_RecordXferData.m_dOverSpendP3[1] = m_dOverSpendP3;
		}

		if (FALSE == m_bAllowCreditPurse1)
		{
			m_RecordXferData.m_bPurse1Credit = FALSE;
			m_RecordXferData.m_dPurse1Credit = 0.0;
		}

		if (FALSE == m_bAllowCreditPurse3)
		{
			m_RecordXferData.m_bPurse3Credit = FALSE;
			m_RecordXferData.m_dPurse3Credit = 0.0;
		}

		CSSPropertyPage::OnOK();
	}
}

//$$******************************************************************
