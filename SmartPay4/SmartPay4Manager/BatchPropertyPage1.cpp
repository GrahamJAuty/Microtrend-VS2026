//$$******************************************************************
#include "..\SmartPay4Shared\AlertText.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRepositoryAccount.h"
#include "..\SmartPay4Shared\SQLTable_UserText\SQLRepositoryUserText.h"
//$$******************************************************************
#include "..\SmartPay4ManagerBgnd\DbExportHandler.h"
//$$******************************************************************
#include "BatchUpdate.h"
#include "CardPickerDlg.h"
#include "GroupComboHelpers.h"
#include "ReportDefaultHandler.h"
#include "SmartPay4Manager.h"
//$$******************************************************************
#include "BatchPropertyPage1.h"
//$$******************************************************************

CBatchPropertyPage1::CBatchPropertyPage1()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CBatchPropertyPage1::IDD)
	, m_strComment(_T(""))
	, m_bIncludeAlert(FALSE)
	, m_bIncludeValid(FALSE)
	, m_bIncludeExpired(FALSE)
	, m_bIncludeInactive(FALSE)
	, m_nCardRange(0)
	, m_nGroupIndex(0)
	, m_nSingleGroupNo(0)
	, m_nTick1Filter(0)
	, m_nTick2Filter(0)
	, m_nTick3Filter(0)
	, m_nTick4Filter(0)
	, m_bIncludeUnissued(FALSE)
{
	m_strLabel			= "";
	m_strFromCardNo		= System.GetLowCardNo();
	m_strToCardNo		= System.GetHighCardNo();

	m_nRangeFromCardNo	= 1;
	m_nRangeToCardNo	= System.GetMaxCardNo();
}

//$$******************************************************************

CBatchPropertyPage1::~CBatchPropertyPage1()
{
}

//$$******************************************************************

void CBatchPropertyPage1::SetPointer ( int nUpdateType )
{
	m_nUpdateType	= nUpdateType;
}

//$$******************************************************************

void CBatchPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INCLUDE, m_staticInclude);
	DDX_Control(pDX, IDC_CHECK_ALERT, m_checkAlert);
	DDX_Control(pDX, IDC_STATIC_USER, m_staticUser);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_tabcomboGroup);
	DDX_Control(pDX, IDC_EDIT_FROMCARDNO, m_editFromCardNo);
	DDX_Control(pDX, IDC_EDIT_TOCARDNO, m_editToCardNo);
	DDX_Control(pDX, IDC_BUTTON_LIST, m_buttonList);
	DDX_Control(pDX, IDC_STATIC_AUDIT, m_staticAudit);
	DDX_Control(pDX, IDC_STATIC_COMMENTS, m_staticComments);
	DDX_Control(pDX, IDC_COMBO_COMMENTS, m_comboComments);
	DDX_Control(pDX, IDC_STATIC_TICKBOX, m_staticTickbox);
	DDX_Control(pDX, IDC_STATIC_TICK1, m_staticTick1);
	DDX_Control(pDX, IDC_RADIO_TICK1, m_radioTick1);
	DDX_Control(pDX, IDC_RADIO_TICKYES1, m_radioTickYes1);
	DDX_Control(pDX, IDC_RADIO_TICKNO1, m_radioTickNo1);
	DDX_Control(pDX, IDC_STATIC_TICK2, m_staticTick2);
	DDX_Control(pDX, IDC_RADIO_TICK2, m_radioTick2);
	DDX_Control(pDX, IDC_RADIO_TICKYES2, m_radioTickYes2);
	DDX_Control(pDX, IDC_RADIO_TICKNO2, m_radioTickNo2);
	DDX_Control(pDX, IDC_STATIC_TICK3, m_staticTick3);
	DDX_Control(pDX, IDC_RADIO_TICK3, m_radioTick3);
	DDX_Control(pDX, IDC_RADIO_TICKYES3, m_radioTickYes3);
	DDX_Control(pDX, IDC_RADIO_TICKNO3, m_radioTickNo3);
	DDX_Control(pDX, IDC_STATIC_TICK4, m_staticTick4);
	DDX_Control(pDX, IDC_RADIO_TICK4, m_radioTick4);
	DDX_Control(pDX, IDC_RADIO_TICKYES4, m_radioTickYes4);
	DDX_Control(pDX, IDC_RADIO_TICKNO4, m_radioTickNo4);
	DDX_Check(pDX, IDC_CHECK_VALID, m_bIncludeValid);
	DDX_Check(pDX, IDC_CHECK_EXPIRED, m_bIncludeExpired);
	DDX_Check(pDX, IDC_CHECK_INACTIVE, m_bIncludeInactive);
	DDX_Check(pDX, IDC_CHECK_ALERT, m_bIncludeAlert);
	DDX_Text(pDX, IDC_EDIT_FROMCARDNO, m_strFromCardNo);
	DDV_MaxChars(pDX, m_strFromCardNo, 10);
	DDX_Text(pDX, IDC_EDIT_TOCARDNO, m_strToCardNo);
	DDV_MaxChars(pDX, m_strToCardNo, 10);
	DDX_Radio(pDX, IDC_RADIO_RANGE, m_nCardRange);
	DDX_CBIndex(pDX, IDC_COMBO_GROUP, m_nGroupIndex);
	DDX_CBString(pDX, IDC_COMBO_COMMENTS, m_strComment);
	DDX_Radio(pDX, IDC_RADIO_TICK1, m_nTick1Filter);
	DDX_Radio(pDX, IDC_RADIO_TICK2, m_nTick2Filter);
	DDX_Radio(pDX, IDC_RADIO_TICK3, m_nTick3Filter);
	DDX_Radio(pDX, IDC_RADIO_TICK4, m_nTick4Filter);
	DDX_Check(pDX, IDC_CHECK_UNISSUED, m_bIncludeUnissued);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CBatchPropertyPage1, CSSAutoShutdownPropertyPage)
	ON_STN_CLICKED(IDC_STATIC_COMMENT, OnStnClickedStaticComment)
	ON_BN_CLICKED(IDC_RADIO_RANGE, OnBnClickedRadioRange)
	ON_BN_CLICKED(IDC_RADIO_LIST, OnBnClickedRadioList)
	ON_BN_CLICKED(IDC_BUTTON_LIST, OnBnClickedButtonList)
	ON_BN_DOUBLECLICKED(IDC_RADIO_LIST, OnBnDoubleclickedRadioList)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CBatchPropertyPage1::OnInitDialog()
{
	int nDateCursel = 1;
	int nTimeCursel	= 0;
	int nGroupCurSel= 0;
	CString strCommentText = "";

	if ( m_nUpdateType == nBATCHUPDATE_DELETE )									// see if batch delete
	{
		m_strLabel = "R108";													// Batch deletes - always make user enter selections
		CFileRecycler::SendToRecycleBin ( Filenames.GetReportCardListFilename(m_strLabel));	// delete last list file, if found

		m_Comments.ReadComments ( Filenames.GetDBaseCommentsFilename() );
		strCommentText = "Batch deletion comment.";
	}
	else
	{
		if ( m_nUpdateType == nBATCHUPDATE_REDEEM )
		{
			m_strLabel = "R138";											// batch redeem
			strCommentText = "Batch redeem comment.";
		}
		else
		{
			m_strLabel = "R107";											// batch updates
			strCommentText = "Batch update comment.";
		}
		m_Comments.ReadComments ( Filenames.GetAuditCommentsFilename() );

		CString strLine;
		CReportDefaultHandler repdefs;
		if ( repdefs.Read ( m_strLabel, strLine ) == TRUE )
		{
			CCSV csv ( strLine );
			if ( csv.IsEmpty(0) == FALSE )	m_nCardRange		= csv.GetInt(0);
			if ( csv.IsEmpty(1) == FALSE )	m_strFromCardNo		= csv.GetString(1);
			if ( csv.IsEmpty(2) == FALSE )	m_strToCardNo		= csv.GetString(2);
			if ( csv.IsEmpty(3) == FALSE )	m_nSingleGroupNo	= csv.GetInt(3);
			if ( csv.IsEmpty(4) == FALSE )	m_bIncludeValid		= csv.GetBool(4);
			if ( csv.IsEmpty(5) == FALSE )	m_bIncludeExpired	= csv.GetBool(5);
			if ( csv.IsEmpty(6) == FALSE )	m_bIncludeInactive	= csv.GetBool(6);
			if ( csv.IsEmpty(7) == FALSE )	m_bIncludeAlert		= csv.GetBool(7);
			if ( csv.IsEmpty(8) == FALSE )	m_nTick1Filter		= csv.GetInt(8);
			if ( csv.IsEmpty(9) == FALSE )	m_nTick2Filter		= csv.GetInt(9);
			if ( csv.IsEmpty(10) == FALSE )	m_nTick3Filter		= csv.GetInt(10);
			if ( csv.IsEmpty(11) == FALSE )	m_nTick4Filter		= csv.GetInt(11);
			if ( csv.IsEmpty(12) == FALSE )	m_bIncludeUnissued	= csv.GetBool(12);
		}
	}

	CPropertyPage::OnInitDialog();

//groups

	CGroupComboHelpers::FillGroupCombo(m_tabcomboGroup, m_nSingleGroupNo, TRUE);

	CAlertText alert;
	alert.Open (Filenames.GetAlertFilename() );
	if ( alert.GetAlertCount() == 0 )
	{
		ShowAndEnableWindow( &m_checkAlert, FALSE );
	}
	else
	{
		if ( System.GetInterfaceType() == nINTERFACE_MIFAREv1 )
			m_checkAlert.SetWindowText( "Hotlist" );
	}


	SetReportingRange();

// audit comments

	m_staticComments.SetWindowText( strCommentText ); 
	AddComments();

// user ticks

	int nTickCount = 4;	
	CSQLRepositoryUserText RepoText;
	CString strText = RepoText.GetTick1Text();
	if ( strText == "" )
	{
		ShowAndEnableWindow( &m_staticTick1, FALSE );
		ShowAndEnableWindow( &m_radioTick1, FALSE );
		ShowAndEnableWindow( &m_radioTickYes1, FALSE );
		ShowAndEnableWindow( &m_radioTickNo1, FALSE );
		--nTickCount;
	}
	else
	{
		m_staticTick1.SetWindowText(strText);
	}

	strText = RepoText.GetTick2Text();
	if ( strText == "" )
	{
		ShowAndEnableWindow( &m_staticTick2, FALSE );
		ShowAndEnableWindow( &m_radioTick2, FALSE );
		ShowAndEnableWindow( &m_radioTickYes2, FALSE );
		ShowAndEnableWindow( &m_radioTickNo2, FALSE );
		--nTickCount;
	}
	else
	{
		m_staticTick2.SetWindowText(strText);
	}

	strText = RepoText.GetTick3Text();
	if ( strText == "" )
	{
		ShowAndEnableWindow( &m_staticTick3, FALSE );
		ShowAndEnableWindow( &m_radioTick3, FALSE );
		ShowAndEnableWindow( &m_radioTickYes3, FALSE );
		ShowAndEnableWindow( &m_radioTickNo3, FALSE );
		--nTickCount;
	}
	else
	{
		m_staticTick3.SetWindowText(strText);
	}

	strText = RepoText.GetTick4Text();
	if ( strText == "" )
	{
		ShowAndEnableWindow( &m_staticTick4, FALSE );
		ShowAndEnableWindow( &m_radioTick4, FALSE );
		ShowAndEnableWindow( &m_radioTickYes4, FALSE );
		ShowAndEnableWindow( &m_radioTickNo4, FALSE );
		--nTickCount;
	}
	else
	{
		m_staticTick4.SetWindowText(strText);
	}

	if ( nTickCount == 0 )
	{
		ShowAndEnableWindow( &m_staticTickbox, FALSE );
		ResizeControl( &m_staticInclude, 120, 171 );
		ResizeControl( &m_staticUser, 258, 171 );
		MoveControl( &m_staticAudit, 7, 187, 386, 106 );
		MoveControl( &m_staticComments, 22, 208 );
		MoveControl( &m_comboComments, 120, 207 );
	}

	return TRUE;
}

//$$******************************************************************

void CBatchPropertyPage1::AddComments()
{
	for ( int i = 0 ; i < m_Comments.GetSize() ; i++ )		// read previous comments
		m_comboComments.AddString ( m_Comments.GetAt(i) );
}

//$$******************************************************************

void CBatchPropertyPage1::OnStnClickedStaticComment()
{
	if ( UpdateData() == TRUE )
	{
		if ( m_strComment != "" )
		{
			m_Comments.Delete ( m_strComment );			// delete comment
			m_comboComments.ResetContent();				// new list
			AddComments();

			m_strComment = "";
			UpdateData(FALSE);
		}
	}
}

//$$******************************************************************

BOOL CBatchPropertyPage1::OnKillActive()
{
	if ( UpdateData() == TRUE )
	{
		m_nSingleGroupNo = 0;
		if ( m_tabcomboGroup.GetCurSel() > 0 )
		{
			m_nSingleGroupNo = CGroupComboHelpers::GetGroupNoFromCombo(m_tabcomboGroup, 0);
		}

		CCSV csv;
		csv.Add ( m_nCardRange );
		csv.Add ( m_strFromCardNo );
		csv.Add ( m_strToCardNo );
		csv.Add ( m_nSingleGroupNo );
		csv.Add ( m_bIncludeValid );
		csv.Add ( m_bIncludeExpired );
		csv.Add ( m_bIncludeInactive );
		csv.Add ( m_bIncludeAlert );
		csv.Add ( m_nTick1Filter );
		csv.Add ( m_nTick2Filter );
		csv.Add ( m_nTick3Filter );
		csv.Add ( m_nTick4Filter );
		csv.Add ( m_bIncludeUnissued );

		CReportDefaultHandler repdefs;
		repdefs.Save ( m_strLabel, &csv );

		m_nRangeFromCardNo	 = _atoi64 ( m_strFromCardNo );
		m_nRangeToCardNo	 = _atoi64 ( m_strToCardNo );
	}

	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

void CBatchPropertyPage1::OnBnClickedRadioRange()
{
	if ( UpdateData()== TRUE )
	{
		m_strFromCardNo	= System.GetLowCardNo();
		m_strToCardNo	= System.GetHighCardNo();
		UpdateData(FALSE);
		SetReportingRange();
	}
}

//$$******************************************************************
void CBatchPropertyPage1::OnBnClickedRadioList()	{ SetReportingRange(); }
//$$******************************************************************

void CBatchPropertyPage1::SetReportingRange()
{
	if ( UpdateData() == TRUE )
	{
		bool bAllowFrom		= TRUE;
		bool bAllowTo		= TRUE;
		bool bAllowList		= TRUE;
		bool bAllowGroup	= ( m_tabcomboGroup.GetCount() < 2 ) ? FALSE : TRUE;

		switch ( m_nCardRange )
		{
		case nBATCH_RANGE:		bAllowList	 = FALSE;
								break;

		case nBATCH_LIST:		bAllowTo		= FALSE;
								bAllowFrom		= FALSE;
								bAllowGroup		= FALSE;
								break;
		}

		m_editFromCardNo.EnableWindow(bAllowFrom);
		m_editToCardNo.EnableWindow(bAllowTo);
		m_buttonList.EnableWindow(bAllowList);
		m_tabcomboGroup.EnableWindow(bAllowGroup);

		switch ( m_nCardRange )
		{
		case nBATCH_RANGE:		m_editFromCardNo.SetFocus();	break;
		case nBATCH_LIST:		m_buttonList.SetFocus();		break;
		}
	}
}

//$$******************************************************************
void CBatchPropertyPage1::OnBnDoubleclickedRadioList() { OnBnClickedButtonList(); }
//$$******************************************************************

void CBatchPropertyPage1::OnBnClickedButtonList()
{
	if ( UpdateData() == TRUE )
	{
		CArray<CSQLRowAccountList, CSQLRowAccountList> arrayDummy;
		CString strReportList = Filenames.GetReportCardListFilename ( m_strLabel );
		CCardPickerDlg dlg ( "Batch Updates", strReportList, arrayDummy, this );

		dlg.m_bValidAllowed			= ( m_bIncludeValid != 0 );
		dlg.m_bExpiredAllowed		= ( m_bIncludeExpired != 0 );
		dlg.m_bInactiveAllowed		= ( m_bIncludeInactive != 0 );
		dlg.m_bAlertAllowed			= ( m_bIncludeAlert != 0 );

		if ( dlg.DoModal() == IDOK )
			m_Picker.Reset();
	}
}

//$$******************************************************************

__int64 CBatchPropertyPage1::RecordsInRange()
{
	__int64 nCount = 0;

	if (m_nCardRange == nBATCH_LIST)
	{
		nCount = ::GetFileLineCount(Filenames.GetReportCardListFilename(m_strLabel));
	}
	else
	{
		CSQLRepositoryAccount RepoAccount;
		nCount = (RepoAccount.CountRowsByRange(_atoi64(m_strFromCardNo), m_nRangeToCardNo, NULL).GetSQLResult());
	}

	return nCount;
}

//$$******************************************************************

bool CBatchPropertyPage1::IsValid(CSQLRowAccountFull& RowAccount)
{
	if (m_nCardRange == nBATCH_LIST)
	{
		return m_Picker.IsValid(RowAccount.GetUserIDString(), Filenames.GetReportCardListFilename(m_strLabel));
	}

	__int64 nCardNo = RowAccount.GetUserID();

	if (nCardNo < m_nRangeFromCardNo || nCardNo > m_nRangeToCardNo)
	{
		return FALSE;
	}

	if ((m_nGroupIndex > 0) && RowAccount.GetGroupNo() != m_nSingleGroupNo)	// not in single group
	{
		return FALSE;
	}

	bool bOK = FALSE;
	bool bIsInactive = RowAccount.GetInactive();
	bool bIsAlert = RowAccount.IsAlertSet();
	bool bIsExpired = RowAccount.HasUserExpired();

	bool bIsUnissued = FALSE;
	if (System.GetInterfaceType() == nINTERFACE_MIFAREv2)						//27/03/2014 - v1.06b
	{
		bIsUnissued = (RowAccount.GetExternalRef1() == "") ? TRUE : FALSE;
	}

	if (m_bIncludeValid == TRUE && (bIsInactive == FALSE && bIsExpired == FALSE && bIsAlert == FALSE && bIsUnissued == FALSE))
	{
		bOK = TRUE;
	}
	else if (m_bIncludeUnissued == TRUE && bIsUnissued == TRUE)
	{
		bOK = TRUE;
	}
	else if (m_bIncludeInactive == TRUE && bIsInactive == TRUE)
	{
		bOK = TRUE;
	}
	else if (m_bIncludeAlert == TRUE && bIsAlert == TRUE)
	{
		bOK = TRUE;
	}
	else if (m_bIncludeExpired == TRUE && bIsExpired == TRUE)
	{
		bOK = TRUE;
	}

	if (bOK == TRUE)
	{
		if (ValidateTickReqd() == TRUE)					// chcek validate on Gender \ User ticks
		{
			bOK = CheckUserTicks(RowAccount);
		}
	}

	return bOK;
}

//$$******************************************************************

bool CBatchPropertyPage1::ValidateTickReqd()
{
	if ( m_nTick1Filter != nFILTER_IGNORE )	return TRUE;
	if ( m_nTick2Filter != nFILTER_IGNORE ) return TRUE;
	if ( m_nTick3Filter != nFILTER_IGNORE ) return TRUE;
	if ( m_nTick4Filter != nFILTER_IGNORE ) return TRUE;

	return FALSE;
}

//$$******************************************************************

bool CBatchPropertyPage1::CheckUserTicks(CSQLRowAccountFull& RowAccount)
{
	bool bTick1		= TRUE;
	bool bTick2		= TRUE;
	bool bTick3		= TRUE;
	bool bTick4		= TRUE;

	if ((m_nTick1Filter == nFILTER_YES && RowAccount.GetTick1() == FALSE) || (m_nTick1Filter == nFILTER_NO && RowAccount.GetTick1() == TRUE))
	{
		bTick1 = FALSE;
	}
	if ((m_nTick2Filter == nFILTER_YES && RowAccount.GetTick2() == FALSE) || (m_nTick2Filter == nFILTER_NO && RowAccount.GetTick2() == TRUE))
	{
		bTick2 = FALSE;
	}
	if ((m_nTick3Filter == nFILTER_YES && RowAccount.GetTick3() == FALSE) || (m_nTick3Filter == nFILTER_NO && RowAccount.GetTick3() == TRUE))
	{
		bTick3 = FALSE;
	}
	if ((m_nTick4Filter == nFILTER_YES && RowAccount.GetTick4() == FALSE) || (m_nTick4Filter == nFILTER_NO && RowAccount.GetTick4() == TRUE))
	{
		bTick4 = FALSE;
	}

	if ( bTick1 == FALSE )		return FALSE;
	if ( bTick2 == FALSE )		return FALSE;
	if ( bTick3 == FALSE )		return FALSE;
	if ( bTick4 == FALSE )		return FALSE;
	return TRUE;
}

//$$******************************************************************

bool CBatchPropertyPage1::HaveRecords()
{
	if ( m_bIncludeValid == TRUE )		return TRUE;
	if ( m_bIncludeUnissued == TRUE )	return TRUE;
	if ( m_bIncludeInactive == TRUE )	return TRUE;
	if ( m_bIncludeAlert == TRUE )		return TRUE;
	if ( m_bIncludeExpired == TRUE )	return TRUE;
	return FALSE;
}

//$$******************************************************************

void CBatchPropertyPage1::OnOK()
{
	if ( UpdateData() == TRUE )
	{
		m_Comments.Save ( m_strComment );				// save comment if required
		EndDialog(IDOK);
	}
}

//$$******************************************************************