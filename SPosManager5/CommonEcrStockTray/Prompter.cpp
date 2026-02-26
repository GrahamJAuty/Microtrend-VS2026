/**********************************************************************/
#include "MyMessageBoxDlg.h"
#include "NodeTypes.h"
 /**********************************************************************/
#include "Prompter.h"
/**********************************************************************/

CPrompter::CPrompter()
{
	m_bPreventParentEnable = FALSE;
}

/**********************************************************************/
/**********************************************************************/

int CPrompter::WarningMessageBox( CStringArray& array, bool bYesNoCancel )
{
	CWnd* pParent = CWnd::FromHandle( CWnd::GetSafeOwner_( NULL, NULL ) );

	BOOL bParentEnabled = FALSE;
	if (pParent != NULL)
	{
		bParentEnabled = pParent->IsWindowEnabled();
	}

	CString strMsg = "";
	strMsg += array.GetAt(1);
	strMsg += "\n\n";

	int n = 1;
	for (int i = 3; i < array.GetSize(); i++)
	{
		CString strTmp = "";

		if (array.GetAt(i) == "")
		{
			strTmp = "\n";
		}
		else if (array.GetAt(i).GetAt(0) != '+')
		{
			strTmp.Format("%d )   %s\n",
				n++,
				(const char*) array.GetAt(i));
		}
		else
		{
			strTmp.Format("        %s\n",
				(const char*)array.GetAt(i).Mid(1));
		}

		strMsg += strTmp;
	}

	strMsg += "\n";
	strMsg += array.GetAt(2);

	int nResult;
	if ( NULL == pParent )
	{
		if ( bYesNoCancel == FALSE )
			nResult = AfxMessageBox ( strMsg, MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2 );
		else
			nResult = AfxMessageBox ( strMsg, MB_ICONEXCLAMATION | MB_YESNOCANCEL | MB_DEFBUTTON2 );
	}
	else if ( EcrmanOptions.GetCustomMessageBoxFlag() == FALSE  )
	{
		if ( bYesNoCancel == FALSE )
			nResult = pParent->MessageBox ( strMsg, array.GetAt(0), MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2 );
		else
			nResult = pParent->MessageBox ( strMsg, array.GetAt(0), MB_ICONEXCLAMATION | MB_YESNOCANCEL | MB_DEFBUTTON2 );
	}
	else
	{
		CMyMessageBoxInfo infoMessage;
		infoMessage.SetIconType( MB_ICONEXCLAMATION );
		infoMessage.SetButtonMode( bYesNoCancel ? MB_YESNOCANCEL : MB_YESNO );
		infoMessage.SetLineGap( EcrmanOptions.GetCustomMessageBoxLineGap() );
		infoMessage.SetFontSize( EcrmanOptions.GetCustomMessageBoxFont() );
		infoMessage.SetBackColour( EcrmanOptions.GetCustomMessageBoxColourBackInt() );
		infoMessage.SetTextColour( EcrmanOptions.GetCustomMessageBoxColourTextInt() );
		infoMessage.SetDefButton(2);
		infoMessage.SetTitle( array.GetAt(0) );
		infoMessage.SetTexts( strMsg );

		CMyMessageBoxDlg dlg( infoMessage, pParent );
		nResult = dlg.DoModal();
	}

	if ( ( FALSE == bParentEnabled ) && ( TRUE == m_bPreventParentEnable ) && ( pParent != NULL ) )
		pParent -> EnableWindow( FALSE );
	
	return nResult;
}

/**********************************************************************/

int CPrompter::Warning( CStringArray& array, bool bYesNoCancel )
{
	array.InsertAt( 1, "On selecting to continue the following irreversible actions take place" );
	array.InsertAt( 2, "Are you sure you wish to continue ?" );
	
	int nResult = WarningMessageBox( array, bYesNoCancel );
	
	array.RemoveAt( 1 );
	array.RemoveAt( 1 );
	
	return nResult;
}

/**********************************************************************/
/**********************************************************************/

void CPrompter::OKMessageBox( const char* szTitle, const char* szMsg, int nIconType )
{
	CWnd* pParent = CWnd::FromHandle( CWnd::GetSafeOwner_( NULL, NULL ) );

	BOOL bParentEnabled = FALSE;
	if ( pParent != NULL )
		bParentEnabled = pParent -> IsWindowEnabled();

	if ( NULL == pParent )
		AfxMessageBox ( szMsg, nIconType | MB_OK );
	else if ( EcrmanOptions.GetCustomMessageBoxFlag() == FALSE )
		pParent -> MessageBox ( szMsg, szTitle, nIconType | MB_OK );
	else
	{
		CMyMessageBoxInfo infoMessage;
		infoMessage.SetIconType( nIconType );
		infoMessage.SetButtonMode( MB_OK );
		infoMessage.SetLineGap( EcrmanOptions.GetCustomMessageBoxLineGap() );
		infoMessage.SetFontSize( EcrmanOptions.GetCustomMessageBoxFont() );
		infoMessage.SetBackColour( EcrmanOptions.GetCustomMessageBoxColourBackInt() );
		infoMessage.SetTextColour( EcrmanOptions.GetCustomMessageBoxColourTextInt() );
		infoMessage.SetTitle( szTitle );
		infoMessage.SetTexts( szMsg );
		
		CMyMessageBoxDlg dlg( infoMessage, pParent );
		dlg.DoModal();
	}

	if ( ( FALSE == bParentEnabled ) && ( TRUE == m_bPreventParentEnable ) && ( pParent != NULL ) )
		pParent -> EnableWindow( FALSE );
}

/**********************************************************************/

void CPrompter::LockError( const char* szType )
{
	CString strMsg = "";
	strMsg.Format ( "You cannot %s while another user is logged on", szType );
	OKMessageBox( "Access Denied", strMsg, MB_ICONEXCLAMATION );
}

/**********************************************************************/

void CPrompter::Info ( const char* szInfo )
{
	OKMessageBox( AfxGetAppName(), szInfo, MB_ICONINFORMATION );
}

/**********************************************************************/

void CPrompter::FatalError ( const char* szError )
{
	OKMessageBox( AfxGetAppName(), szError, MB_ICONSTOP );
}

/**********************************************************************/

void CPrompter::Diagnostic ( const char* szInfo )
{
	OKMessageBox( "Diagnostic", szInfo, 0 );
}

/**********************************************************************/

void CPrompter::FatalReadError( SysFileInfo info )
{
	CString strTitle;
	strTitle.Format ( "Access Denied (%1.1d%4.4d)", info.m_nFamily, info.m_nFileNo );

#ifdef STOCKMAN_SYSTEM
	CString strMsg;
	strMsg.Format ( "Stock Manager was unable to access a system file.\n\n%s\n\nPlease check the network and try again.",
		GetLastErrorMessage() );
#else
	CString strMsg;
	strMsg.Format ( "Ecr Manager was unable to access a system file.\n\n%s\n\nPlease check the network and try again.",
		GetLastErrorMessage() );
#endif

	OKMessageBox( strTitle, strMsg, MB_ICONSTOP );
}

/**********************************************************************/

void CPrompter::Error ( const char* szError, const char* szTitle )
{
	CString strTitle = szTitle;
	if (strTitle == "") strTitle = AfxGetAppName();
	OKMessageBox( strTitle, szError, MB_ICONEXCLAMATION );
}

/**********************************************************************/

void CPrompter::LocationAccessError()
{
	Error( "You are not authorised to access this\noption for any locations." );
}

/**********************************************************************/

void CPrompter::StockpointAccessError()
{
	Error( "You are not authorised to access this\noption for any stockpoints." );
}

/**********************************************************************/

void CPrompter::InContextCommsError()
{
	Error ( "There are no locations connected to this PC on this\ndatabase that are capable of performing this job." );
}

/**********************************************************************/

void CPrompter::ShareError( CDataManagerInfo& info )
{
	CString strMsg = "";

	if ( info.m_nErrorType == DB_ERR_SHARE )
		strMsg.Format ( "The %s file is currently being modified by another user.\n\nPlease try again later.",
			GetNodeString( info.m_nNodeType ) );
	else
		strMsg.Format ( "Unable to open the %s file.\n\n%s", 
			GetNodeString( info.m_nNodeType ), 
			GetErrorMessage( info ) );
	
	OKMessageBox( "Access Denied", strMsg, MB_ICONEXCLAMATION );
}

/**********************************************************************/

void CPrompter::WriteError( CDataManagerInfo& info )
{
	CString strMsg = "";

	strMsg.Format ( "Unable to save changes to the %s file.\n\n%s", 
		GetNodeString( info.m_nNodeType ), 
		GetErrorMessage( info ) );

	OKMessageBox( "Access Denied", strMsg, MB_ICONEXCLAMATION );
}

/**********************************************************************/

void CPrompter::ContactDealer( const char* szError, bool bFatal )
{
	CString strError = szError;
	strError += "\n\nPlease contact your dealer for further advice.";

	if ( bFatal == FALSE )
		Error( strError );
	else
		FatalError( strError );
}

/**********************************************************************/

void CPrompter::SupplierError()
{
#ifdef STOCKMAN_SYSTEM
	CString strMsg;
	strMsg.Format ( "The supplier file contains invalid data and cannot be edited.\n\nError Code 6%2.2d-%6.6X",
		DataManager.Supplier.GetErrorType(),
		DataManager.Supplier.GetErrorLine() );

	ContactDealer( strMsg );
#endif
}

/**********************************************************************/

void CPrompter::CustomFieldError()
{
#ifdef STOCKMAN_SYSTEM
	CString strMsg;
	strMsg.Format ( "The custom field file contains invalid data and cannot be edited.\n\nError Code 6%2.2d-%6.6X",
		DataManager.GetActiveCustomField() -> GetErrorType(),
		DataManager.GetActiveCustomField() -> GetErrorLine() );

	ContactDealer( strMsg );
#endif
}

/**********************************************************************/
/**********************************************************************/

int CPrompter::OkCancel ( const char* szPrompt )
{
	CWnd* pParent = CWnd::FromHandle( CWnd::GetSafeOwner_( NULL, NULL ) );

	BOOL bParentEnabled = FALSE;
	if ( pParent != NULL )
		bParentEnabled = pParent -> IsWindowEnabled();

	int nResult;
	CString strPrompt = szPrompt;

	if ( ( EcrmanOptions.GetCustomMessageBoxFlag() == FALSE ) || ( NULL == pParent ) )
	{
		nResult = AfxMessageBox ( strPrompt, MB_ICONEXCLAMATION | MB_OKCANCEL );
	}
	else
	{
		CMyMessageBoxInfo infoMessage;
		infoMessage.SetIconType( MB_ICONEXCLAMATION );
		infoMessage.SetButtonMode( MB_OKCANCEL );
		infoMessage.SetLineGap( EcrmanOptions.GetCustomMessageBoxLineGap() );
		infoMessage.SetFontSize( EcrmanOptions.GetCustomMessageBoxFont() );
		infoMessage.SetBackColour( EcrmanOptions.GetCustomMessageBoxColourBackInt() );
		infoMessage.SetTextColour( EcrmanOptions.GetCustomMessageBoxColourTextInt() );
		infoMessage.SetTexts( strPrompt );

		CMyMessageBoxDlg dlg( infoMessage, pParent );
		nResult = dlg.DoModal();
	}

	if ( ( FALSE == bParentEnabled ) && ( TRUE == m_bPreventParentEnable ) && ( pParent != NULL ) )
		pParent -> EnableWindow( FALSE );

	return nResult;
}

/**********************************************************************/
/**********************************************************************/

int CPrompter::YesNo ( const char* szPrompt, bool bYes )
{
	CWnd* pParent = CWnd::FromHandle( CWnd::GetSafeOwner_( NULL, NULL ) );

	BOOL bParentEnabled = FALSE;
	if ( pParent != NULL )
		bParentEnabled = pParent -> IsWindowEnabled();

	int nResult;
	CString strPrompt = szPrompt;
	
	if ( ( EcrmanOptions.GetCustomMessageBoxFlag() == FALSE ) || ( NULL == pParent ) )
	{
		if ( bYes == FALSE )
			nResult = AfxMessageBox ( strPrompt, MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 );
		else
			nResult = AfxMessageBox ( strPrompt, MB_ICONQUESTION | MB_YESNO );
	}
	else
	{
		CMyMessageBoxInfo infoMessage;
		infoMessage.SetIconType( MB_ICONQUESTION );
		infoMessage.SetButtonMode( MB_YESNO );
		infoMessage.SetDefButton( bYes ? 1 : 2 );
		infoMessage.SetLineGap( EcrmanOptions.GetCustomMessageBoxLineGap() );
		infoMessage.SetFontSize( EcrmanOptions.GetCustomMessageBoxFont() );
		infoMessage.SetBackColour( EcrmanOptions.GetCustomMessageBoxColourBackInt() );
		infoMessage.SetTextColour( EcrmanOptions.GetCustomMessageBoxColourTextInt() );
		infoMessage.SetTexts( strPrompt );

		CMyMessageBoxDlg dlg( infoMessage, pParent );
		nResult = dlg.DoModal();
	}

	if ( ( FALSE == bParentEnabled ) && ( TRUE == m_bPreventParentEnable ) && ( pParent != NULL ) )
		pParent -> EnableWindow( FALSE );

	return nResult;
}

/**********************************************************************/

int CPrompter::SuggestReadOnly( CDataManagerInfo& info )
{
	CString strMsg = "";
	strMsg.Format ( "The %s file is currently being modified by another user.\n\nWould you like to open this file for read-only access.",
		GetNodeString( info.m_nNodeType ) );
	
	return YesNo( strMsg, TRUE );
}

/**********************************************************************/
/**********************************************************************/

int CPrompter::YesNoCancel ( const char* szPrompt, bool bYes )
{
	CWnd* pParent = CWnd::FromHandle( CWnd::GetSafeOwner_( NULL, NULL ) );

	BOOL bParentEnabled = FALSE;
	if (pParent != NULL)
	{
		bParentEnabled = pParent->IsWindowEnabled();
	}

	int nResult = 0;
	CString strPrompt = szPrompt;
	
	if ( ( EcrmanOptions.GetCustomMessageBoxFlag() == FALSE ) || ( NULL == pParent ) )
	{
		nResult = AfxMessageBox ( strPrompt, MB_ICONQUESTION | MB_YESNOCANCEL );
	}
	else
	{
		CMyMessageBoxInfo infoMessage;
		infoMessage.SetIconType( MB_ICONQUESTION );
		infoMessage.SetButtonMode( MB_YESNOCANCEL );
		infoMessage.SetDefButton(bYes ? 1 : 2);
		infoMessage.SetLineGap( EcrmanOptions.GetCustomMessageBoxLineGap() );
		infoMessage.SetFontSize( EcrmanOptions.GetCustomMessageBoxFont() );
		infoMessage.SetBackColour( EcrmanOptions.GetCustomMessageBoxColourBackInt() );
		infoMessage.SetTextColour( EcrmanOptions.GetCustomMessageBoxColourTextInt() );
		infoMessage.SetTexts( strPrompt );

		CMyMessageBoxDlg dlg( infoMessage, pParent );
		nResult = dlg.DoModal();
	}

	if ((FALSE == bParentEnabled) && (TRUE == m_bPreventParentEnable) && (pParent != NULL))
	{
		pParent->EnableWindow(FALSE);
	}

	return nResult;
}

/**********************************************************************/

const char* CPrompter::GetNodeString( int nNodeType )
{
	switch( nNodeType )
	{
	//COMMON NODE TYPES
	case NODE_ADJUSTTEXT:				m_strNodeString = "adjustment text";			break;
	case NODE_ALLOWANCE:				m_strNodeString = "allowance text";				break;
	case NODE_PRESET_EPOS_BATCH:		m_strNodeString = "batch report";				break;
	case NODE_CATEGORY:					m_strNodeString = "category text";				break;
	case NODE_CATEGORY_SET:				m_strNodeString = "category set";				break;
	case NODE_CUSTOM_FIELD:				m_strNodeString = "custom field value";			break;
	case NODE_CUSTOM_FIELD_NAME:		m_strNodeString = "custom field name";			break;
	case NODE_CUSTOM_FIELD_SET:			m_strNodeString = "custom field set";			break;
	case NODE_CUSTOMER_ACCOUNT:			m_strNodeString = "account customer";			break;
	case NODE_CUSTOMER_ONEOFF:			m_strNodeString = "one-off customer";			break;
	case NODE_DELUNIT:					m_strNodeString = "delivery unit";				break;
	case NODE_DEPARTMENT:				m_strNodeString = "department";					break;
	case NODE_DEPARTMENT_SET:			m_strNodeString = "department set";				break;
	case NODE_DEPT_GROUP_EPOS:			m_strNodeString = "department EPOS group";		break;
	case NODE_DEPT_GROUP_REPORT:		m_strNodeString = "department report group";	break;
	case NODE_DEPT_GROUP_CONSOL:		m_strNodeString = "department consol group";	break;
	case NODE_DEPOSITNAME:				m_strNodeString = "department buyer names";		break;
	case NODE_ECR_SUPPLIER:				m_strNodeString = "supplier";					break;
	case NODE_EMAIL_ADDRESS:			m_strNodeString = "email address";				break;
	case NODE_KEYBOARD_NAME:			m_strNodeString = "keyboard name";				break;
	case NODE_KEYBOARD1:				m_strNodeString = "keyboard";					break;
	case NODE_LABELS_PRODUCT:			m_strNodeString = "product labels";				break;
	case NODE_LABELS_SHELF:				m_strNodeString = "shelf edge labels";			break;
	case NODE_PLU:						m_strNodeString = "Plu";						break;
	case NODE_PLUUNIT:					m_strNodeString = "Plu unit";					break;
	case NODE_PROMOTION:				m_strNodeString = "promotion";					break;
	case NODE_PRICETEXT:				m_strNodeString = "price level texts";			break;
	case NODE_SBPRICETEXT:				m_strNodeString = "Sports Booker price bands";	break;
	case NODE_HISTORY_PLU:
	case NODE_HISTORY_STOCK:			m_strNodeString = "custom reports";				break;
	case NODE_PRESET_PLU_HIST:		
	case NODE_PRESET_STOCK_HIST:		m_strNodeString = "sales history preset";		break;
	case NODE_STOCK:					m_strNodeString = "stock item";					break;
	case NODE_STOCKLEVEL:				m_strNodeString = "stock level";				break;
	case NODE_PRESET_STOCK_LEVEL:		m_strNodeString = "stock report preset";		break;
	case NODE_PRESET_STOCK_CLOSING:		m_strNodeString = "closing stock report preset";break;
	case NODE_STOCKREPORT_SORTORDER:	m_strNodeString = "stock report sort orders";	break;
	case NODE_PRESET_STOCK_AUDIT:		m_strNodeString = "stock audit preset";		break;
	case NODE_STOCKUNIT:				m_strNodeString = "stock unit";					break;
	case NODE_SUBUNIT:					m_strNodeString = "sub unit";					break;
	case NODE_SUPPLIER:					m_strNodeString = "supplier";					break;
	case NODE_SUPPLIER_SET:				m_strNodeString = "supplier set";				break;
	case NODE_TAX:						m_strNodeString = EcrmanOptions.GetTaxString( "%T rates" );					break;
	case NODE_TAX_DATE:					m_strNodeString = EcrmanOptions.GetTaxString( "historical %T rates" );		break;
	case NODE_TIMESLICE_DETAIL:			m_strNodeString = "time slice detail";			break;
	case NODE_TIMESLICE_LIST:			m_strNodeString = "time slice list";			break;
	case NODE_TRANSFER_DETAIL:			m_strNodeString = "transfer detail";			break;
	case NODE_UNITECH_OPTIONS:			m_strNodeString = "Unitech options";			break;
	//SPOS ONLY
	case NODE_BARCODE:					m_strNodeString = "barcode";					break;
	case NODE_MODIFIER:					m_strNodeString = "modifier";					break;
	case NODE_OFFER_GROUP:				m_strNodeString = "offer group";				break;
	case NODE_OFFER_ENTRY:				m_strNodeString = "offer group entry";			break;
	case NODE_PLUFILTER_NAME:			m_strNodeString = "plu filter name";			break;
	case NODE_PLUFILTER_ITEM:			m_strNodeString = "plu filter item";			break;
	case NODE_PAIDIN_TEXT:				m_strNodeString = "paid in text";				break;
	case NODE_PAIDOUT_TEXT:				m_strNodeString = "paid out text";				break;
	case NODE_PAYMENT:					m_strNodeString = "payment";					break;
	case NODE_PAYMENT_GROUP:			m_strNodeString = "payment group";				break;
	case NODE_SBPAYMENTMAP:				m_strNodeString = "Sports Booker payment map";	break;
	case NODE_SEPAYMENTMAP:				m_strNodeString = "Smart Entertainment payment map"; break;
	case NODE_ACAT:						m_strNodeString = "analysis category";			break;
	case NODE_PMS_BOOKING:				m_strNodeString = "guest booking";				break;
	case NODE_PMS_BOOKING_GROUP:		m_strNodeString = "group booking";				break;
	case NODE_PMS_CLOSURE:				m_strNodeString = "room closure";				break;
	case NODE_PMS_CUSTOMER:				m_strNodeString = "customer";					break;
	case NODE_PMS_ROOM:					m_strNodeString = "guest account";				break;
	case NODE_PMS_ROOM_TYPE:			m_strNodeString = "room type";					break;
	case NODE_SERVER:					m_strNodeString = "server";						break;
	case NODE_SERVER_RESTRICT:			m_strNodeString = "server restriction";			break;
	case NODE_HISTORY_CUSTOMER:			m_strNodeString = "custom reports";				break;
	case NODE_PRESET_CUST_HIST:			m_strNodeString = "sales history preset";		break;
	case NODE_PRESET_EPOS:				m_strNodeString = "EPOS report preset";			break;
	case NODE_PRESET_ECR_STOCK:			m_strNodeString = "stock level report preset";	break;
	case NODE_TIMEPLAN:					m_strNodeString = "time plan";					break;
	case NODE_TIMEPLAN_ENTRY:			m_strNodeString = "time plan entry";			break;
	case NODE_TIMEPLAN_TEMPLATE_ENTRY:	m_strNodeString = "time plan template entry";	break;
	case NODE_POSTRAY_FIELDNAMES:		m_strNodeString = "PosTray field names";		break;
	case NODE_POSTRAY_SETTINGS:			m_strNodeString = "PosTray settings";			break;
	case NODE_ALLERGEN:					m_strNodeString = "allergen";					break;
	case NODE_FUNCTION_SYSTEM:			m_strNodeString = "global function";			break;
	case NODE_FUNCTION_DATABASE:		m_strNodeString = "database function";			break;
	case NODE_FUNCTION_LOCATION:		m_strNodeString = "location function";			break;
	case NODE_OPGROUP_SYSTEM:			m_strNodeString = "global Printer groups";		break;
	case NODE_OPGROUP_DATABASE:			m_strNodeString = "database Printer groups";	break;
	case NODE_OPGROUP_LOCATION:			m_strNodeString = "location Printer groups";	break;
	case NODE_LOYALTY_SCHEME:			m_strNodeString = "loyalty scheme";				break;
	case NODE_TABLE_AREA:				m_strNodeString = "table area";					break;
	case NODE_TABLE_NAME:				m_strNodeString = "table name";					break;
	case NODE_TABLE_GROUP:				m_strNodeString = "table group";				break;
	case NODE_TABLE_GROUPSET:			m_strNodeString = "table group set";			break;
	case NODE_CCMACRO_SYSTEM:			m_strNodeString = "global macro";				break;
	case NODE_CCMACRO_DATABASE:			m_strNodeString = "database macro";				break;
	case NODE_CCMACRO_LOCATION:			m_strNodeString = "location macro";				break;
	case NODE_CCMACRO_KEYBOARD:			m_strNodeString = "keyboard macro";				break;
	case NODE_TIMEATTEND_NAMES:			m_strNodeString = "report filenames";			break;
	//DEFAULT
	default:							m_strNodeString = "selected";					break;
	}

	return m_strNodeString;
}

/**********************************************************************/

const char* CPrompter::GetErrorMessage( CDataManagerInfo& info )
{
	CString strText;
	switch( info.m_nErrorType )
	{
	case DATAMANAGER_ERROR_ALREADYOPEN:		strText = "File already open.";	break;
	case DATAMANAGER_ERROR_NOTOPEN:			strText = "File not open.";		break;
	case DATAMANAGER_ERROR_BADSUPPIDX:		strText = "Bad supplier index.";	break;
	case DATAMANAGER_ERROR_READONLY:		strText = "File is read-only.";	break;
	case DATAMANAGER_ERROR_WRITE:			strText = "Unable to write file.";	break;
	case 1:									strText = "Unable to open file";	break;
	case 2:									strText = "Unable to share file";	break;
	default:								strText = GetLastErrorMessage();	break;
	}

	m_strError.Format ( "Error# %d - ", info.m_nErrorType );
	m_strError += strText;

	return m_strError;
}

/**********************************************************************/

void CPrompter::ShowStartupMessage( CMyMessageBoxDlg** ppMessageBox, CWnd* pParent )
{
	CMyMessageBoxInfo info;
	info.SetIconType(MB_ICONEXCLAMATION);
	info.SetButtonMode(MB_OK);
	info.SetLineGap(EcrmanOptions.GetStartupMessageBoxLineGap());
	info.SetFontSize(EcrmanOptions.GetStartupMessageBoxFont());
	info.SetBackColour(EcrmanOptions.GetStartupMessageBoxColourBackInt());
	info.SetTextColour(EcrmanOptions.GetStartupMessageBoxColourTextInt());
	info.SetDefButton(1);

	CString strTitle = "";
	strTitle += AfxGetAppName();
	strTitle += " Startup Message";
	info.SetTitle(strTitle);

	CStringArray arrayTexts;

	CSSFile fileMessage;
	CFilenameUpdater FnUp(SysFiles::StartupMessage);
	if (fileMessage.Open(FnUp.GetFilenameToUse(), "rb") == TRUE)
	{
		CString strText = "";
		while ((fileMessage.ReadString(strText) == TRUE) && (arrayTexts.GetSize() < MAX_STARTUPMSG_LINES))
		{
			arrayTexts.Add(strText.Left(MAX_STARTUPMSG_LINELENGTH));
		}
	}

	if (arrayTexts.GetSize() > 0)
	{
		info.SetTexts(arrayTexts);

		*ppMessageBox = new CMyMessageBoxDlg(ppMessageBox, info, pParent);
		(*ppMessageBox)->Create(IDD_MESSAGE, pParent);
	}
}

/**********************************************************************/
