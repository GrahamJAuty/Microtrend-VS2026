/**********************************************************************/
 
/**********************************************************************/
#include "BackgroundLayoutInfo.h"
#include "ButtonDefines.h"
#include "ButtonOptions.h"
#include "FileRemover.h"
//#include "globalfunctions.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ButtonInfo.h"
/**********************************************************************/

CButtonInfo::CButtonInfo()
{
	Reset();
}

/**********************************************************************/

void CButtonInfo::Reset()
{
	m_nListNo = 0;
	m_nLineNo = 0;
	m_nButtonNo = 0;
	m_nButtonType = BUTTON_TYPE_UNUSED;
	m_nDetail1 = 0;
	m_nDetail2 = 0;
	m_nDetail3 = 0;
	m_bActionFlag1 = FALSE;
	m_bOwnColour = FALSE;
	m_nOwnBackColour = 0xFFFFFF;
	m_nOwnTextColour = 0x000000;
	m_strTextLine1 = "";
	m_strTextLine2 = "";
	m_strTextLine3 = "";
	m_strInternalParams = "";
	m_nFontSize = 10;
	m_rectDisplay = CRect( 0, 0, 10, 10 );
	m_strImageType = "";
	m_strImageTypeTemp = "";
	m_bImageTransparent = FALSE;
}

/**********************************************************************/

void CButtonInfo::SetListNo( int n )
{
	if ( ( n >= 0 ) && ( n <= BUTMGR_MAX_LIST_NO ) )
		m_nListNo = n;
}

/**********************************************************************/

void CButtonInfo::SetLineNo( int n )
{
	if ( ( n >= 0 ) && ( n <= BUTMGR_MAX_LINE_NO ) ) 
		m_nLineNo = n;
}

/**********************************************************************/

void CButtonInfo::SetButtonNo( int n )
{
	if ( ( n >= 0 ) && ( n <= BUTMGR_MAX_BUTTON_NO ) )
		m_nButtonNo = n;
}

/**********************************************************************/

void CButtonInfo::SetButtonType( int n )
{
	switch( n )
	{
	case BUTTON_TYPE_HIDE:
	case BUTTON_TYPE_UNUSED:
	case BUTTON_TYPE_ACTION:
	case BUTTON_TYPE_LIST:
	case BUTTON_TYPE_HOME:
	case BUTTON_TYPE_BACK:
	case BUTTON_TYPE_PARENT:
	case BUTTON_TYPE_LIST_SELECT:
	case BUTTON_TYPE_LIST_NAME:
	case BUTTON_TYPE_LIST_SAVE:
		m_nButtonType = n;
		break;
	}
}

/**********************************************************************/

void CButtonInfo::SetDetail1( int n )
{
	if ( ( n >= 0 ) && ( n <= BUTMGR_MAX_DETAIL1 ) )
		m_nDetail1 = n;
}

/**********************************************************************/

void CButtonInfo::SetDetail2( int n )
{
	if ( ( n >= 0 ) && ( n <= BUTMGR_MAX_DETAIL2 ) )
		m_nDetail2 = n;
}

/**********************************************************************/

void CButtonInfo::SetDetail3( int n )
{
	if ( ( n >= 0 ) && ( n <= BUTMGR_MAX_DETAIL3 ) )
		m_nDetail3 = n;
}

/**********************************************************************/

void CButtonInfo::SetOwnBackColour( int n )
{
	if ( ( n >= 0x00 ) && ( n <= 0xFFFFFF ) )
		m_nOwnBackColour = n;
}

/**********************************************************************/

void CButtonInfo::SetOwnTextColour( int n )
{
	if ( ( n >= 0x00 ) && ( n <= 0xFFFFFF ) )
		m_nOwnTextColour = n;
}

/**********************************************************************/

void CButtonInfo::SetFontSize( int n )
{
	if ( ( n >= 8 ) && ( n <= 30 ) && ( ( n % 2 ) == 0 ) )
		m_nFontSize = n;
}

/**********************************************************************/

void CButtonInfo::SetImageType( const char* sz )
{
	CString strType = sz;
	strType.MakeUpper();

	if ( ( strType == "" ) || ( strType == "BMP" ) || ( strType == "GIF" ) || ( strType == "JPG" ) || ( strType == "PNG" ) )
		m_strImageType = strType;
}

/**********************************************************************/

void CButtonInfo::SetImageTypeTemp( const char* sz )
{
	CString strType = sz;
	strType.MakeUpper();

	if ( ( strType == "" ) || ( strType == "XXX" ) || ( strType == "BMP" ) || ( strType == "GIF" ) || ( strType == "JPG" ) || ( strType == "PNG" ) )
		m_strImageTypeTemp = strType;
}

/**********************************************************************/

void CButtonInfo::Add (CButtonInfo& source )
{
}

/**********************************************************************/

int CButtonInfo::Compare( CButtonInfo& source, int nHint )
{
	if ( m_nListNo != source.m_nListNo )
		return ( ( m_nListNo > source.m_nListNo ) ? 1 : -1 );

	if ( m_nLineNo != source.m_nLineNo )
		return ( ( m_nLineNo > source.m_nLineNo ) ? 1 : -1 );

	if ( m_nButtonNo != source.m_nButtonNo )
		return ( ( m_nButtonNo > source.m_nButtonNo ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

void CButtonInfo::CreateSpecialButton( int nType )
{
	bool bOK = FALSE;

	CString strNewTextLine1 = "";
	CString strNewTextLine2 = "";
	CString strNewTextLine3 = "";
	
	switch( nType )
	{
	case BUTTON_TYPE_HOME:
		strNewTextLine1 = "HOME";
		m_nOwnBackColour = ButtonOptions.GetButtonColourBackControlInt();
		m_nOwnTextColour = ButtonOptions.GetButtonColourTextControlInt();
		bOK = TRUE;
		break;

	case BUTTON_TYPE_BACK:
		strNewTextLine1 = "BACK";
		m_nOwnBackColour = ButtonOptions.GetButtonColourBackControlInt();
		m_nOwnTextColour = ButtonOptions.GetButtonColourTextControlInt();
		bOK = TRUE;
		break;

	case BUTTON_TYPE_UNUSED:
		m_nOwnBackColour = ButtonOptions.GetButtonColourBackUnusedInt();
		m_nOwnTextColour = 0x0;
		bOK = TRUE;
		break;

	case BUTTON_TYPE_LIST_SELECT:
		strNewTextLine1 = "Select";
		strNewTextLine2 = "List";
		m_nOwnBackColour = ButtonOptions.GetButtonColourBackControlInt();
		m_nOwnTextColour = ButtonOptions.GetButtonColourTextControlInt();
		bOK = TRUE;
		break;

	case BUTTON_TYPE_LIST_NAME:
		strNewTextLine1 = "Change";
		strNewTextLine2 = "List";
		strNewTextLine3 = "Name";
		m_nOwnBackColour = ButtonOptions.GetButtonColourBackControlInt();
		m_nOwnTextColour = ButtonOptions.GetButtonColourTextControlInt();
		bOK = TRUE;
		break;

	case BUTTON_TYPE_LIST_SAVE:
		strNewTextLine1 = "Save";
		strNewTextLine2 = "Changes";
		m_nOwnBackColour = ButtonOptions.GetButtonColourBackControlInt();
		m_nOwnTextColour = ButtonOptions.GetButtonColourTextControlInt();
		bOK = TRUE;
		break;

	case BUTTON_TYPE_HIDE:
		bOK = TRUE;
		break;
	}

	if ( TRUE == bOK )
	{
		m_nButtonType = nType;
		m_strTextLine1 = strNewTextLine1;
		m_strTextLine2 = strNewTextLine2;
		m_strTextLine3 = strNewTextLine3;
		m_nDetail1 = 0;
		m_nDetail2 = 0;
		m_nDetail3 = 0;
		m_bOwnColour = FALSE;
		m_nFontSize = 14;
	}
}	

/**********************************************************************/

void CButtonInfo::ParseFileText( const char* szText )
{
	CCSV csv( szText, '|', 34, 3 );
	SetTextLines( csv );
}

/**********************************************************************/

const char* CButtonInfo::CreateFileText()
{
	m_strFileText = "";

	CStringArray arrayTexts;
	GetDisplayTexts( arrayTexts );
	
	for ( int n = 0; n < arrayTexts.GetSize(); n++ )
	{
		m_strFileText += arrayTexts.GetAt(n);

		if ( n < arrayTexts.GetSize() - 1 )
			m_strFileText += "|";
	}

	return m_strFileText;
}

/**********************************************************************/

bool CButtonInfo::Equals( CButtonInfo& other )
{
	if ( m_nListNo != other.m_nListNo )
		return FALSE;

	if ( m_nLineNo != other.m_nLineNo )
		return FALSE;

	if ( m_nButtonNo != other.m_nButtonNo )
		return FALSE;

	if ( m_nButtonType != other.m_nButtonType )
		return FALSE;

	if ( m_nDetail1 != other.m_nDetail1 )
		return FALSE;
	
	if ( m_nDetail2 != other.m_nDetail2 )
		return FALSE;
	
	if ( m_nDetail3 != other.m_nDetail3 )
		return FALSE;

	if ( m_bActionFlag1 != other.m_bActionFlag1 )
		return FALSE;

	if ( m_strTextLine1 != other.m_strTextLine1 )
		return FALSE;

	if ( m_strTextLine2 != other.m_strTextLine2 )
		return FALSE;

	if ( m_strTextLine3 != other.m_strTextLine3 )
		return FALSE;

	if ( m_bOwnColour != other.m_bOwnColour )
		return FALSE;

	if ( m_nOwnBackColour != other.m_nOwnBackColour )
		return FALSE;

	if ( m_nOwnTextColour != other.m_nOwnTextColour )
		return FALSE;

	if ( m_nFontSize != other.m_nFontSize )
		return FALSE;

	if ( m_bImageTransparent != other.m_bImageTransparent )
		return FALSE;

	return TRUE;
}

/**********************************************************************/

void CButtonInfo::LibraryToDisplay( CButtonInfo& infoLibrary )
{
	CRect rect = m_rectDisplay;
	*this = infoLibrary;
	m_rectDisplay = rect;
}

/**********************************************************************/

void CButtonInfo::GetTextLines( CStringArray& arrayTexts )
{
	arrayTexts.RemoveAll();
	arrayTexts.Add( m_strTextLine1 );
	arrayTexts.Add( m_strTextLine2 );
	arrayTexts.Add( m_strTextLine3 );
}

/**********************************************************************/

void CButtonInfo::CopyTextLines( CButtonInfo& source )
{
	m_strTextLine1 = source.m_strTextLine1;
	m_strTextLine2 = source.m_strTextLine2;
	m_strTextLine3 = source.m_strTextLine3;
}

/**********************************************************************/

void CButtonInfo::SetTextLines( const char* szLines )
{
	CCSV csv( szLines );
	SetTextLines( csv );
}

/**********************************************************************/

void CButtonInfo::SetTextLines( CCSV& csv )
{
	m_strTextLine1 = "";
	m_strTextLine2 = "";
	m_strTextLine3 = "";
	
	int nSize = csv.GetSize();
	
	if ( nSize >= 1 )
	{
		CString strText = csv.GetString(0);
		if ( strText.GetLength() <= BUTMGR_MAX_TEXTLEN )
			m_strTextLine1 = strText;
	}

	if ( nSize >= 2 )
	{
		CString strText = csv.GetString(1);
		if ( strText.GetLength() <= BUTMGR_MAX_TEXTLEN )
			m_strTextLine2 = strText;
	}

	if ( nSize >= 3 )
	{
		CString strText = csv.GetString(2);
		if ( strText.GetLength() <= BUTMGR_MAX_TEXTLEN )
			m_strTextLine3 = strText;
	}
}

/**********************************************************************/

void CButtonInfo::GetDisplayTexts( CStringArray& arrayTexts )
{
	GetTextLines( arrayTexts );

	for ( int n = arrayTexts.GetSize() - 1; n >= 0; n-- )
	{
		CString strText = arrayTexts.GetAt(n);
			
		if ( strText == "[]" )
			arrayTexts.SetAt( n, "" );
		else
		{
			::TrimSpaces( strText, FALSE );
		
			if ( strText != "" )
				break;

			arrayTexts.RemoveAt(n);
		}
	}
}

/**********************************************************************/

void CButtonInfo::SetDefaultTexts()
{
	CString strText = "";
	
	switch( m_nButtonType )
	{
	case BUTTON_TYPE_LIST:
		strText.Format( "Open,List,%d", m_nDetail1 );
		break;

	case BUTTON_TYPE_ACTION:
		switch( m_nDetail1 )
		{
		case BUTTON_DETAIL1_SETUP:
			switch( m_nDetail2 )
			{
			case BUTTON_DETAIL2_SETUP_SYSTEM:		strText = "Setup,System";				break;
			case BUTTON_DETAIL2_SETUP_OPTIONS:		strText = "Setup,Options";				break;
			}
			break;

		case BUTTON_DETAIL1_EDITEPOS:
			switch( m_nDetail2 )
			{
			case BUTTON_DETAIL2_EDITEPOS_PLU:
				strText = "Edit,Plu";		
				break;

			case BUTTON_DETAIL2_EDITEPOS_DEPT:
				strText = "Edit,Department";
				break;

			case BUTTON_DETAIL2_EDITEPOS_GROUP:	
				strText = "Edit,Group";			
				break;

			case BUTTON_DETAIL2_EDITEPOS_TAX:	
				strText = EcrmanOptions.GetTaxString( "Edit,%T Bands" ); 
				break;

			case BUTTON_DETAIL2_EDITEPOS_ACAT:	
				strText = "Edit,Analyis,Categories";
				break;

			case BUTTON_DETAIL2_EDITEPOS_PAYMENT:
				strText = "Edit,Payment,Methods";
				break;

			case BUTTON_DETAIL2_EDITEPOS_SERVER:	
				strText = "Edit,Servers";		
				break;

			case BUTTON_DETAIL2_EDITEPOS_MIXMATCH:
				strText = "Edit,Mix Match";		
				break;

			case BUTTON_DETAIL2_EDITEPOS_LOYALTYSCHEME:	
				strText = "Edit,Loyalty,Schemes";		
				break;

			case BUTTON_DETAIL2_EDITEPOS_WALLETSTAMPRULE:
				switch(SysInfo.GetLoyaltyOrSmartPayType())
				{ 
				case LOYALTY_TYPE_SMP_V4:
					strText = "Edit,Purchase,Control";		
					break;

				case LOYALTY_TYPE_LOY_V4:
				default:
					strText = "Edit,Stamp,Offers";			
					break;
				}
				break;
			
			case BUTTON_DETAIL2_EDITEPOS_OFFERGROUP:
				strText = "Edit,Offer Groups";		
				break;

			case BUTTON_DETAIL2_EDITEPOS_PROMOTION:
				strText = "Edit,Promotions";	
				break;

			case BUTTON_DETAIL2_EDITEPOS_TIMEPLAN:	
				strText = "Edit,Time Plans";		
				break;
			}
			break;

#ifdef STOCKMAN_SYSTEM

		case BUTTON_DETAIL1_EDITSTK:
			switch( m_nDetail2 )
			{
			case BUTTON_DETAIL2_EDITSTK_STOCK:		strText = "Edit,Stock";					break;
			case BUTTON_DETAIL2_EDITSTK_SUPPLIER:	strText = "Edit,Suppliers";				break;
			case BUTTON_DETAIL2_EDITSTK_CATEGORY:	strText = "Edit,Categories";			break;
			case BUTTON_DETAIL2_EDITSTK_CUSTFLD:	strText = "Edit,Custom,Fields";			break;
			case BUTTON_DETAIL2_EDITSTK_ALLOWANCE:	strText = "Edit,Allowances";			break;
			case BUTTON_DETAIL2_EDITSTK_DELUNIT:	strText = "Edit,Delivery,Units";		break;
			case BUTTON_DETAIL2_EDITSTK_STKUNIT:	strText = "Edit,Stock,Units";			break;
			case BUTTON_DETAIL2_EDITSTK_SUBUNIT:	strText = "Edit,Sub,Units";				break;
			case BUTTON_DETAIL2_EDITSTK_ADJTEXT:	strText = "Edit,Adjustment,Texts";		break;
			}
			break;

		case BUTTON_DETAIL1_MOVESTK:
			switch( m_nDetail2 )
			{
			case BUTTON_DETAIL2_MOVESTK_OPENING:	strText = "Opening,Stock";				break;
			case BUTTON_DETAIL2_MOVESTK_DELIVERY:	strText = "Adhoc,Deliveries";			break;	
			case BUTTON_DETAIL2_MOVESTK_RETURN:		strText = "Stock,Returns";				break;
			case BUTTON_DETAIL2_MOVESTK_SALEPLU:	strText = "Manual,Sales,(PLU)";			break;
			case BUTTON_DETAIL2_MOVESTK_SALESTK:	strText = "Manual,Sales,(Stock)";		break;
			case BUTTON_DETAIL2_MOVESTK_ADJUSTPLU:	strText = "Adjustments,(PLU)";			break;
			case BUTTON_DETAIL2_MOVESTK_ADJUSTSTK:	strText = "Adjustments,(Stock)";		break;
			case BUTTON_DETAIL2_MOVESTK_TRANSFER:	strText = "Stock,Transfers";			break;
			}
			break;

#endif
		
		case BUTTON_DETAIL1_REPORTS:
			switch( m_nDetail2 )
			{
			case BUTTON_DETAIL2_REPORTS_EPOS_VIEW:	strText = "View,Epos,Reports";			break;
			case BUTTON_DETAIL2_REPORTS_EPOS_PRINT:	strText = "Print,Epos,Reports";			break;
			case BUTTON_DETAIL2_REPORTS_CUSTOMER:	strText = "Customer,Reports";			break;
			case BUTTON_DETAIL2_REPORTS_TIMEATTEND:	strText = "Time &,Attendance,Reports";	break;
			case BUTTON_DETAIL2_REPORTS_PLUHISTORY:	strText = "Plu,Sales,History";			break;
			case BUTTON_DETAIL2_REPORTS_STKHISTORY:	strText = "Stock,Sales,History";		break;
			case BUTTON_DETAIL2_REPORTS_STKLEVEL:	strText = "Stock,Level,Reports";		break;
			case BUTTON_DETAIL2_REPORTS_STKAUDIT:	strText = "Stock,Audit,Reports";		break;
			case BUTTON_DETAIL2_REPORTS_ECRMANSTK:	strText = "Plu,Stock,Levels";			break;
			}
			break;

		case BUTTON_DETAIL1_KEYBOARD:
			strText = "Edit,Keyboard";
			break;

		case BUTTON_DETAIL1_PSET_LOC:
			strText = "Edit,Location,Price Set";
			break;

		case BUTTON_DETAIL1_PSET_DB:
			strText = "Edit,Database,Price Set";
			break;

		case BUTTON_DETAIL1_TASKS:
			switch( m_nDetail2 )
			{
			case BUTTON_DETAIL2_TASKS_ADHOC:		strText = "Adhoc,Task";					break;
			case BUTTON_DETAIL2_TASKS_EDITPRESET:	strText = "Edit,Preset,Tasks";			break;
			case BUTTON_DETAIL2_TASKS_RUNPRESET:	strText = "Run,Preset,Task";			break;
			}
			break;

		case BUTTON_DETAIL1_EXTERNAL:
			switch( m_nDetail2 )
			{
			case BUTTON_DETAIL2_EXTERNAL_LOYALTY:
				if( SysInfo.GotLoyaltyLink() )
				{
					strText = "Launch,Loyalty,Manager";
				}
				else
				{
					strText = "Launch,SmartPay";
				}
			}
			break;
		}
		break;
	}

	SetTextLines( strText );
}

/**********************************************************************/

int CButtonInfo::GetDisplayBackColour()
{
	int nColour = 0;
	if ( TRUE == m_bOwnColour )
		nColour = m_nOwnBackColour;
	else
	{
		switch( m_nButtonType )
		{
		case BUTTON_TYPE_HOME:
			nColour = ButtonOptions.GetButtonColourBackControlInt();
			break;

		case BUTTON_TYPE_BACK:
			nColour = ButtonOptions.GetButtonColourBackControlInt();
			break;

		case BUTTON_TYPE_ACTION:
			nColour = ButtonOptions.GetButtonColourBackActionInt();
			break;

		case BUTTON_TYPE_LIST:
			nColour = ButtonOptions.GetButtonColourBackListInt();
			break;

		case BUTTON_TYPE_UNUSED:
			nColour = ButtonOptions.GetButtonColourBackUnusedInt();
			break;

		case BUTTON_TYPE_PARENT:
			nColour = ButtonOptions.GetButtonColourBackParentInt();
			break;

		case BUTTON_TYPE_LIST_SELECT:
			nColour = ButtonOptions.GetButtonColourBackControlInt();
			break;

		case BUTTON_TYPE_LIST_NAME:
			nColour = ButtonOptions.GetButtonColourBackControlInt();
			break;

		case BUTTON_TYPE_LIST_SAVE:
			nColour = ButtonOptions.GetButtonColourBackControlInt();
			break;
		}
	}
	return nColour;
}

/**********************************************************************/

int CButtonInfo::GetDisplayTextColour()
{
	int nColour = 0;
	if ( TRUE == m_bOwnColour )
		nColour = m_nOwnTextColour;
	else
	{
		switch( m_nButtonType )
		{
		case BUTTON_TYPE_HOME:
			nColour = ButtonOptions.GetButtonColourTextControlInt();
			break;

		case BUTTON_TYPE_BACK:
			nColour = ButtonOptions.GetButtonColourTextControlInt();
			break;

		case BUTTON_TYPE_ACTION:
			nColour = ButtonOptions.GetButtonColourTextActionInt();
			break;

		case BUTTON_TYPE_LIST:
			nColour = ButtonOptions.GetButtonColourTextListInt();
			break;

		case BUTTON_TYPE_UNUSED:
			nColour = 0x0;
			break;

		case BUTTON_TYPE_PARENT:
			nColour = ButtonOptions.GetButtonColourTextParentInt();
			break;

		case BUTTON_TYPE_LIST_SELECT:
			nColour = ButtonOptions.GetButtonColourTextControlInt();
			break;

		case BUTTON_TYPE_LIST_NAME:
			nColour = ButtonOptions.GetButtonColourTextControlInt();
			break;

		case BUTTON_TYPE_LIST_SAVE:
			nColour = ButtonOptions.GetButtonColourTextControlInt();
			break;
		}
	}
	return nColour;
}

/**********************************************************************/

bool CButtonInfo::GetButtonImageFilename( int nListNo, int nTempMode, CString& strFilename )
{
	switch( nTempMode )
	{
	case 0:
		if ( m_strImageType == "" )
		{
			strFilename = "";
			return FALSE;
		}
		break;

	case 1:
		if ( m_strImageTypeTemp == "" )
		{
			strFilename = "";
			return FALSE;
		}
		else if ( m_strImageTypeTemp == "XXX" )
		{
			strFilename = "";
			return TRUE;
		}
		break;
	}

	CString strImagePath = "";

	switch( nTempMode )
	{
	case 1:
	case 2:
		strImagePath = BackgroundLayoutInfo.GetButtonImagePathTemp();
		break;

	case 0:
	default:
		strImagePath = BackgroundLayoutInfo.GetButtonImagePath();
		break;
	}

	strFilename.Format( "%s%3.3d%3.3d%3.3d.", 
		(const char*) strImagePath,
		nListNo,
		m_nLineNo,
		m_nButtonNo );

	switch( nTempMode )
	{
	case 1:		
		strFilename += "001";
		break;
	
	case 2:		
		strFilename += "002";	
		break;

	default:	
		strFilename += m_strImageType;
		break;
	}

	return TRUE;
}

/**********************************************************************/
