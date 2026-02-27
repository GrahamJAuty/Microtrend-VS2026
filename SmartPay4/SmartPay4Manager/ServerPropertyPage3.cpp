//$$******************************************************************
#include "AutoShutdownStacker.h"
#include "ServerPropertySheet.h"
#include "SmartPay4Manager.h"
#include "ReportCSVFile.h"
//$$******************************************************************
#include "ServerPropertyPage3.h"
//$$******************************************************************
extern CSysset Sysset;
//$$******************************************************************

CServerPropertyPage3::CServerPropertyPage3()
	: CSSAutoShutdownPropertyPage(AutoShutdownHelper, CServerPropertyPage3::IDD)
{
	m_nRevaluationType = 0;
}

//$$******************************************************************

void CServerPropertyPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_PURSEREVALUE, m_staticPurseRevalue);
	DDX_Control(pDX, IDC_EDIT_PLU_PURSE1, m_editPluPurse1);
	DDX_Control(pDX, IDC_EDIT_PLU_PURSE3, m_editPluPurse3);
	/*****/
	DDX_Control(pDX, IDC_STATIC_PURSEBONUS, m_staticPurseBonus);
	DDX_Control(pDX, IDC_STATIC_PURSE1TRIP, m_staticPurse1Trip);
	DDX_Control(pDX, IDC_STATIC_PURSE3TRIP, m_staticPurse3Trip);
	DDX_Control(pDX, IDC_STATIC_BONUSCOMMENT, m_staticBonusComment);
	DDX_Control(pDX, IDC_RADIO_REVALUE_POINTS, m_radioRevaluePoints);
	DDX_Control(pDX, IDC_RADIO_REVALUE_VALUE, m_radioRevalueValue);
	DDX_Control(pDX, IDC_EDIT_COMMENT, m_editComment);
	/*****/
	DDX_Control(pDX, IDC_CHECK_HISTORY, m_checkHistory);
	DDX_Control(pDX, IDC_CHECK_HISTORY_DISABLEACZERO, m_checkHistoryDisableACZero);
	DDX_Control(pDX, IDC_STATIC_PLUDATAPATH, m_staticEcrmanImportFolder);
	DDX_Control(pDX, IDC_EDIT_PLUDATAPATH, m_editEcrmanImportFolder);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CServerPropertyPage3, CSSAutoShutdownPropertyPage)
	ON_BN_CLICKED(IDC_RADIO_REVALUE_POINTS, OnRadioRevaluePoints)
	ON_BN_CLICKED(IDC_RADIO_REVALUE_VALUE, OnRadioRevalueValue)
	/*****/
	ON_BN_CLICKED(IDC_CHECK_HISTORY, OnToggleHistory)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_TEST_PLU, OnButtonTestPlu)
	ON_BN_CLICKED(IDC_BUTTON_TEST_TERMNAME, OnButtonTestTermName)
	ON_BN_CLICKED(IDC_BUTTON_TEST_RULE, OnButtonTestRule)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CServerPropertyPage3::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	SubclassEdit ( IDC_EDIT_REVALUE_POINTS,		SS_NUM_S,	 8, "%ld" );
	SubclassEdit ( IDC_EDIT_TRIP_PURSE1,		SS_NUM_SDP, 11, "%.2f" ); 
	SubclassEdit ( IDC_EDIT_TRIP_PURSE3,		SS_NUM_SDP, 11, "%.2f" ); 
	SubclassEdit ( IDC_EDIT_REVALUE_VALUE,		SS_NUM_SDP, 11, "%.2f" ); 

	m_editPluPurse1.SetWindowText( Server.GetPluNo() );
	m_editPluPurse3.SetWindowText( Server.GetPluNo2() );
	
	/*****/
	
	{
		CString strTripPurse1;
		strTripPurse1.Format( "%.2f", Server.GetFileRevaluationTrip() );
		GetEditTripPurse1() -> SetWindowText( strTripPurse1 );
	}

	{
		CString strTripPurse3;
		strTripPurse3.Format( "%.2f", Server.GetFileRevaluationTrip2() );
		GetEditTripPurse3() -> SetWindowText( strTripPurse3 );
	}

	{
		CString strRevaluePoints;
		strRevaluePoints.Format( "%d", Server.GetRevaluationPoints() );
		GetEditRevaluePoints() -> SetWindowText( strRevaluePoints );
	}

	{
		CString strRevalueValue;
		strRevalueValue.Format( "%.2f", Server.GetRevaluationValue() );
		GetEditRevalueValue() -> SetWindowText( strRevalueValue );
	}

	switch( Server.GetRevaluationType() )
	{
	case 1:
		OnRadioRevalueValue();
		break;

	case 0:
	default:
		OnRadioRevaluePoints();
		break;
	}

	m_editComment.SetWindowText( Server.GetRevaluationComment() );

	/*****/

	if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
	{
		m_checkHistory.SetCheck(TRUE);
		m_checkHistory.EnableWindow(FALSE);
	}
	else
	{
		m_checkHistory.SetCheck(Server.GetFileEnablePurchaseHistoryFlag());
	}

	m_checkHistoryDisableACZero.SetCheck(Server.GetDisablePurchaseHistoryAccountZeroFlag());
	m_editEcrmanImportFolder.SetWindowText( Server.GetEcrManagerImportFolder() );
	
	OnToggleHistory();

	if (System.GetWebPaymentType() == nWEBPAYMENT_PEBBLE)
	{
		ShowAndEnableWindow(&m_staticPurseBonus, FALSE);
		ShowAndEnableWindow(&m_staticPurse1Trip, FALSE);
		ShowAndEnableWindow(&m_staticPurse3Trip, FALSE);
		ShowAndEnableWindow(&m_staticBonusComment, FALSE);
		ShowAndEnableWindow(&m_radioRevaluePoints, FALSE);
		ShowAndEnableWindow(&m_radioRevalueValue, FALSE);
		ShowAndEnableWindow(&m_editComment, FALSE);
		ShowAndEnableWindow(GetEditTripPurse1(), FALSE);
		ShowAndEnableWindow(GetEditTripPurse3(), FALSE);
		ShowAndEnableWindow(GetEditRevaluePoints(), FALSE);
		ShowAndEnableWindow(GetEditRevalueValue(), FALSE);
		MoveControl(&m_staticPurseRevalue, 7, 7, 466, 174);
	}
	
	return TRUE;
}

//$$******************************************************************
CEdit* CServerPropertyPage3::GetEditTripPurse1(){ return GetEdit( IDC_EDIT_TRIP_PURSE1 ); }
CEdit* CServerPropertyPage3::GetEditTripPurse3(){ return GetEdit( IDC_EDIT_TRIP_PURSE3 ); }
CEdit* CServerPropertyPage3::GetEditRevaluePoints(){ return GetEdit( IDC_EDIT_REVALUE_POINTS ); }
CEdit* CServerPropertyPage3::GetEditRevalueValue(){ return GetEdit( IDC_EDIT_REVALUE_VALUE ); }
//$$******************************************************************

void CServerPropertyPage3::OnButtonBrowse()
{
	CAutoShutdownPropSheetStacker Stacker((CServerPropertySheet*)GetParent());

	CString strPathname = "";
	if (BrowseFolder(strPathname, "Locate the Ecr / Stock Manager import folder", NULL, this) == TRUE)
	{
		m_editEcrmanImportFolder.SetWindowText(strPathname);
	}
}

//$$******************************************************************

void CServerPropertyPage3::OnButtonTestPlu()
{
	HandleTestButton("plupts.csv", "Plu", "R218");
}

//$$******************************************************************

void CServerPropertyPage3::OnButtonTestRule()
{
	HandleTestButton("purchasectrl.csv", "Purchase Control", "R226");
}

//$$******************************************************************

void CServerPropertyPage3::OnButtonTestTermName()
{
	HandleTestButton("termnames.csv", "Terminal Names", "R230");
}

//$$******************************************************************

void CServerPropertyPage3::HandleTestButton(CString strFilename, CString strText, CString strReportType)
{
	CAutoShutdownPropSheetStacker Stacker((CServerPropertySheet*)GetParent());

	CString strMsg = "";
	int nType = 0;

	CString strImportFolder = GetEditBoxText(m_editEcrmanImportFolder);

	CString strFilePath = "";
	strFilePath += strImportFolder;
	strFilePath += "\\";
	strFilePath += strFilename;

	if (::ExistSubdirectory(strImportFolder) == FALSE)
	{
		strMsg.Format("Path to import folder ' %s 'is invalid!",
			(const char*)strImportFolder);
	}
	else
	{
		if (::ValidateFolderAccess(strImportFolder) == FALSE)
		{
			strMsg.Format("Unable to write to folder ' %s '!",
				(const char*)strImportFolder);
		}
		else
		{
			strMsg = "Path to import folder is valid.";
			if (::FileExists(strFilePath) == TRUE)
			{
				strMsg += "\n\n";
				strMsg += strText;
				strMsg += " update file found, would you like to view it?";
				nType = MB_ICONQUESTION | MB_YESNO;
			}
			else
			{
				strMsg += "\n\nNo outstanding ";
				strMsg += strText;
				strMsg += " updates found.";
			}
		}
	}

	if (MessageBox(strMsg, "Validation Check", nType) == IDYES)
	{
		CCSVFileReport report(strReportType, strFilePath, this);
		report.Show(strText + " Update File");
	}
}

//$$******************************************************************

void CServerPropertyPage3::OnToggleHistory()
{
	m_checkHistoryDisableACZero.EnableWindow(IsTicked(m_checkHistory));
}

//$$******************************************************************

void CServerPropertyPage3::OnRadioRevaluePoints() 
{ 
	m_nRevaluationType = 0;
	SetRevaluationDisplay(); 
}

//$$******************************************************************

void CServerPropertyPage3::OnRadioRevalueValue()  
{ 
	m_nRevaluationType = 1;
	SetRevaluationDisplay(); 
}

//$$******************************************************************

void CServerPropertyPage3::SetRevaluationDisplay()
{
	if ( 1 == m_nRevaluationType )
	{
		m_radioRevaluePoints.SetCheck( FALSE );
		m_radioRevalueValue.SetCheck( TRUE );
		GetEditRevaluePoints() -> EnableWindow( FALSE );
		GetEditRevalueValue() -> EnableWindow( TRUE );
		GetEditRevalueValue() -> SetFocus();
	}
	else
	{
		m_radioRevaluePoints.SetCheck( TRUE );
		m_radioRevalueValue.SetCheck( FALSE );
		GetEditRevaluePoints() -> EnableWindow( TRUE );
		GetEditRevalueValue() -> EnableWindow( FALSE );
		GetEditRevaluePoints() -> SetFocus();
	}
}

//*******************************************************************

BOOL CServerPropertyPage3::OnKillActive()
{
	Server.SetPluNo(GetEditBoxText(m_editPluPurse1));
	Server.SetPluNo2(GetEditBoxText(m_editPluPurse3));
	/*****/
	Server.SetFileRevaluationTrip(atof(GetEditBoxText(*GetEditTripPurse1())));
	Server.SetFileRevaluationTrip2(atof(GetEditBoxText(*GetEditTripPurse3())));
	Server.SetRevaluationType(m_nRevaluationType);
	Server.SetRevaluationPoints(atoi(GetEditBoxText(*GetEditRevaluePoints())));
	Server.SetRevaluationValue(atof(GetEditBoxText(*GetEditRevalueValue())));
	Server.SetRevaluationComment(GetEditBoxText(m_editComment));
	/*****/
	
	if (System.GetWebPaymentType() != nWEBPAYMENT_PEBBLE)
	{
		Server.SetFileEnablePurchaseHistoryFlag(IsTicked(m_checkHistory));
	}

	Server.SetEcrManagerImportFolder(GetEditBoxText(m_editEcrmanImportFolder));
	/*****/
	Server.SetDisablePurchaseHistoryAccountZeroFlag(IsTicked(m_checkHistoryDisableACZero));
	/*****/
	return CPropertyPage::OnKillActive();
}

//$$******************************************************************
