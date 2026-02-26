/**********************************************************************/
#include "..\CommonEcrStockTray\EcrmanSupplierCSVArray.h"
/**********************************************************************/
#include "..\CommonEcrStock\CascadingReasonCSVArray.h"
/**********************************************************************/
#include "DepartmentCSVArray.h"
#include "GroupCSVArrayAllergen.h"
#include "GroupCSVArrayConsol.h"
#include "GroupCSVArrayEpos.h"
#include "GroupCSVArrayPayment.h"
#include "GroupCSVArrayReport.h"
#include "MixMatchCSVArray.h"
#include "NodeTypes.h"
#include "OfferGroupsCSVArray.h"
#include "PaymentCSVArray.h"
#include "PMSRoomCSVArray.h"
#include "PromotionsCSVArray.h"
#include "ServerCSVArray.h"
#include "TableNameCSVArray.h"
#include "TimePlanEntriesCSVArray.h"
#include "TimePlansCSVArray.h"
/**********************************************************************/
#include "NewRecordNumberDlg.h"
/**********************************************************************/

CNewRecordNumberDlg::CNewRecordNumberDlg( int nType, CWnd* pParent /*=NULL*/)
	: CSSDialog(CNewRecordNumberDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewRecordNumberDlg)
	//}}AFX_DATA_INIT
	m_nNumber = 1;
	m_nType = nType;
	m_nCopySource = -1;
}

/**********************************************************************/

void CNewRecordNumberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewRecordNumberDlg)
	DDX_Control(pDX, IDC_STATIC_LABEL, m_staticLabel);
	//}}AFX_DATA_MAP

	DDX_Text(pDX, IDC_EDIT_NUMBER, m_nNumber);
	
	switch ( m_nType )
	{
	case NODE_DEPARTMENT:		
		DDV_MinMaxInt(pDX, m_nNumber, 1, Dept::DeptNo.Max);
		break;

	case NODE_DEPT_GROUP_EPOS:
		DDV_MinMaxInt(pDX, m_nNumber, 1, EposGroup::GroupNo.Max);
		break;

	case NODE_DEPT_GROUP_REPORT:
		DDV_MinMaxInt(pDX, m_nNumber, 0, ReportGroup::GroupNo.Max);
		break;

	case NODE_DEPT_GROUP_CONSOL:
		DDV_MinMaxInt(pDX, m_nNumber, 0, ConsolGroup::GroupNo.Max);
		break;
	
	case NODE_PAYMENT_GROUP:
		DDV_MinMaxInt(pDX, m_nNumber, 1, PaymentGroup::GroupNo.Max);
		break;

	case NODE_ALLERGEN:
		DDV_MinMaxInt(pDX, m_nNumber, 1, Allergen::AllergenNo.Max);
		break;

	case NODE_PAYMENT:		
		DDV_MinMaxInt(pDX, m_nNumber, 1, Payment::PaymentNo.Max);
		break;

	case NODE_SERVER:			
		DDV_MinMaxInt(pDX, m_nNumber, Server::ServerNo.Min, Server::ServerNo.Max);			
		break; 

	case NODE_PROMOTION:			
		DDV_MinMaxInt(pDX, m_nNumber, Promotions::PromoNo.Min, Promotions::PromoNo.Max);			
		break; 

	case NODE_OFFER_GROUP:
	case NODE_PLUFILTER_NAME:
		DDV_MinMaxInt(pDX, m_nNumber, OfferGroups::GroupNo.Min, OfferGroups::GroupNo.Max);			
		break; 

	case NODE_MIXMATCH:	
		DDV_MinMaxInt(pDX, m_nNumber, MixMatch::MixMatchNo.Min, MixMatch::MixMatchNo.Max);	
		break; 
	
	case NODE_TIMEPLAN:
		DDV_MinMaxInt(pDX, m_nNumber, TimePlans::PlanNo.Min, TimePlans::PlanNo.Max);	
		break; 

	case NODE_TIMEPLAN_ENTRY:
	case NODE_TIMEPLAN_TEMPLATE_ENTRY:	
		DDV_MinMaxInt(pDX, m_nNumber, TimePlanEntries::EntryNo.Min, TimePlanEntries::EntryNo.Max);	
		break;

	case NODE_WALLETSTAMPRULE:
		DDV_MinMaxInt(pDX, m_nNumber, WalletStampRule::RuleID.Min, WalletStampRule::RuleID.Max);
		break;


	case NODE_LOYALTY_SCHEME:	
		DDV_MinMaxInt(pDX, m_nNumber, LoyaltyScheme::SchemeNo.Min, LoyaltyScheme::SchemeNo.Max);	
		break;

	case NODE_TABLE_AREA:	
		DDV_MinMaxInt(pDX, m_nNumber, SPosTableArea::AreaNo.Min, SPosTableArea::AreaNo.Max );	
		break;

	case NODE_TABLE_NAME:	
		DDV_MinMaxInt(pDX, m_nNumber, SPosTableName::TableNo.Min, SPosTableName::TableNo.Max );	
		break;

	case NODE_CCMACRO_SYSTEM:	
		DDV_MinMaxInt(pDX, m_nNumber, CascadingMacro::MacroNo.Min, CascadingMacro::MacroNo.Max );	
		break;

	case NODE_CCREASON_SYSTEM:
		DDV_MinMaxInt(pDX, m_nNumber, CascadingReason::ReasonNo.Min, CascadingReason::ReasonNo.Max);
		break;
	
#ifdef COMPILE_PMS

	case NODE_PMS_ROOM:	
		DDV_MinMaxInt(pDX, m_nNumber, PMSRoom::RoomNo.Min, PMSRoom::RoomNo.Max);	
		break; 

#endif
	
#ifndef STOCKMAN_SYSTEM
	case NODE_ECR_SUPPLIER:		
		DDV_MinMaxInt(pDX, m_nNumber, EcrmanSupplier::SupplierNo.Min, EcrmanSupplier::SupplierNo.Max);	
		break; 
#endif
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CNewRecordNumberDlg, CDialog)
	//{{AFX_MSG_MAP(CNewRecordNumberDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CNewRecordNumberDlg::GetEditRecordNo() { return GetEdit( IDC_EDIT_NUMBER ); }
/**********************************************************************/

BOOL CNewRecordNumberDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int nWidth = 2;

	switch ( m_nType )
	{
	case NODE_DEPARTMENT:
		SetWindowText ( "Add Department" );
		m_staticLabel.SetWindowText ( "Dept No" );
		nWidth = 3;
		break;

	case NODE_DEPT_GROUP_EPOS:
	case NODE_DEPT_GROUP_REPORT:
	case NODE_DEPT_GROUP_CONSOL:
	case NODE_PAYMENT_GROUP:
		SetWindowText ( "Add Group" );
		m_staticLabel.SetWindowText ( "Group No" );
		break;

	case NODE_ALLERGEN:
		SetWindowText ( "Add Allergen" );
		m_staticLabel.SetWindowText ( "Allergen No" );
		break;

	case NODE_PAYMENT:
		SetWindowText ( "Add Payment Method" );
		m_staticLabel.SetWindowText ( "Number" );
		break;

	case NODE_SERVER:
		SetWindowText ( "Add Server" );
		m_staticLabel.SetWindowText ( "Number" );
		nWidth = 4;
		break;

	case NODE_PROMOTION:
		SetWindowText ( "Add Promotion" );
		m_staticLabel.SetWindowText ( "Number" );
		break;

	case NODE_OFFER_GROUP:
		SetWindowText ( "Add Offer Group" );
		m_staticLabel.SetWindowText ( "Number" );
		nWidth = 3;
		break;

	case NODE_PLUFILTER_NAME:
		SetWindowText ( "Add Plu Filter" );
		m_staticLabel.SetWindowText ( "Number" );
		nWidth = 3;
		break;

	case NODE_MIXMATCH:
		SetWindowText ( "Add Mix Match Table" );
		m_staticLabel.SetWindowText ( "Table No" );
		nWidth = 3;
		break;

	case NODE_WALLETSTAMPRULE:
		SetWindowText("Add Stamp Offer");
		m_staticLabel.SetWindowText("Offer ID");
		nWidth = 3;
		break;

	case NODE_TIMEPLAN:
		if ( m_nCopySource <= 0 )
		{
			SetWindowText ( "Add Time Plan" );
			m_staticLabel.SetWindowText ( "Plan No" );
			nWidth = 3;
		}
		else
		{
			CString strTitle = "";
			strTitle.Format( "Copy Time Plan %d", m_nCopySource );
			SetWindowText ( strTitle );
			m_staticLabel.SetWindowText ( "New Plan No" );
			nWidth = 3;
		}
		break;

	case NODE_TIMEPLAN_ENTRY:
		SetWindowText ( "Add Time Plan Entry" );
		m_staticLabel.SetWindowText ( "Entry No" );
		nWidth = 3;
		break;

	case NODE_TIMEPLAN_TEMPLATE_ENTRY:
		SetWindowText("Add Template Entry");
		m_staticLabel.SetWindowText("Entry No");
		nWidth = 3;
		break;

	case NODE_PMS_ROOM:
		SetWindowText ( "Add Room" );
		m_staticLabel.SetWindowText ( "Room No" );
		nWidth = 4;
		break;

	case NODE_LOYALTY_SCHEME:
		SetWindowText ( "Add Loyalty Scheme" );
		m_staticLabel.SetWindowText ( "Scheme No" );
		nWidth = 3;
		break;

	case NODE_TABLE_AREA:
		SetWindowText ( "Add Table Area" );
		m_staticLabel.SetWindowText ( "Area No" );
		nWidth = 2;
		break;

	case NODE_TABLE_NAME:
		SetWindowText ( "Add Table" );
		m_staticLabel.SetWindowText ( "Table No" );
		nWidth = 3;
		break;

	case NODE_CCMACRO_SYSTEM:
		SetWindowText ( "Add Macro" );
		m_staticLabel.SetWindowText ( "Macro No" );
		nWidth = 3;
		break;

	case NODE_CCREASON_SYSTEM:
		SetWindowText("Add Reason Text");
		m_staticLabel.SetWindowText("Reason No");
		nWidth = 2;
		break;

#ifndef STOCKMAN_SYSTEM
	case NODE_ECR_SUPPLIER:
		SetWindowText ( "Add Supplier" );
		m_staticLabel.SetWindowText ( "Number" );
		nWidth = 3;
		break;
#endif
	}

	SubclassEdit ( IDC_EDIT_NUMBER, SS_NUM, nWidth, "%d" );
	GetEditRecordNo() -> SetSel( 0, -1 );
	GetEditRecordNo() -> SetFocus();
	return TRUE;  
}

/**********************************************************************/

void CNewRecordNumberDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
		EndDialog ( IDOK );
}

/**********************************************************************/
