/**********************************************************************/
#include "..\CommonEcrStockTray\PluTypeHelpers.h"
/**********************************************************************/
#include "MaxLengths.h"
#include "NetworkCSVArray.h"
#include "PluCSVArray.h"
#include "PropertySheetLocation.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPageLocationUpload.h"
/**********************************************************************/

CPropPageLocationUpload::CPropPageLocationUpload() : CSSPropertyPage(CPropPageLocationUpload::IDD)
{
	//{{AFX_DATA_INIT(CPropPageLocationUpload)
	//}}AFX_DATA_INIT

	m_pLocationBuffer = NULL;

	m_bWantID = FALSE;

	m_nConnectionType = CONNECTION_TYPE_STANDARD_NONE;

	m_IDPluType[0] = IDC_CHECK_PLUTYPE1;
	m_IDPluType[1] = IDC_CHECK_PLUTYPE2;
	m_IDPluType[2] = IDC_CHECK_PLUTYPE3;
	m_IDPluType[3] = IDC_CHECK_PLUTYPE4;
	m_IDPluType[4] = IDC_CHECK_PLUTYPE5;
	m_IDPluType[5] = IDC_CHECK_PLUTYPE6;
	m_IDPluType[6] = IDC_CHECK_PLUTYPE7;
	m_IDPluType[7] = IDC_CHECK_PLUTYPE8;
	m_IDPluType[8] = IDC_CHECK_PLUTYPE9;
	m_IDPluType[9] = IDC_CHECK_PLUTYPE10;
	m_IDPluType[10] = IDC_CHECK_PLUTYPE11;
	m_IDPluType[11] = IDC_CHECK_PLUTYPE12;
	m_IDPluType[12] = IDC_CHECK_PLUTYPE13;

	for (int n = 0; n <= 12; n++)
	{
		m_nPluType[n] = CPluTypeHelpers::ComboToMicrotrend(n);
	}

	m_IDPluProtect[0]	= IDC_CHECK_PLUPROTECT1;
}

/**********************************************************************/

CPropPageLocationUpload::~CPropPageLocationUpload()
{
}

/**********************************************************************/

void CPropPageLocationUpload::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLocationUpload)
	DDX_Control(pDX, IDC_EDIT_PATH_EXPORT, m_editPathExport);
	DDX_Control(pDX, IDC_STATIC_ID, m_staticID);
	DDX_Control(pDX, IDC_EDIT_ID, m_editID);
	DDX_Control(pDX, IDC_TAB_FILETYPE, m_TabFileType );
	DDX_Control(pDX, IDC_STATIC_PLUTYPE, m_staticPluType);
	DDX_Control(pDX, IDC_RADIO_IMPORTTYPE1, m_radioImportType1);
	DDX_Control(pDX, IDC_RADIO_IMPORTTYPE2, m_radioImportType2);
	DDX_Control(pDX, IDC_RADIO_IMPORTTYPE3, m_radioImportType3);
	DDX_Control(pDX, IDC_RADIO_PLU_EXISTING1, m_radioPluExisting1);
	DDX_Control(pDX, IDC_RADIO_PLU_EXISTING2, m_radioPluExisting2);
	DDX_Control(pDX, IDC_RADIO_PLU_EXISTING3, m_radioPluExisting3);
	DDX_Control(pDX, IDC_RADIO_PLU_MODIFIER1, m_radioPluModifier1);
	DDX_Control(pDX, IDC_RADIO_PLU_MODIFIER2, m_radioPluModifier2);
	DDX_Control(pDX, IDC_RADIO_PLU_MODIFIER3, m_radioPluModifier3);

	for (int n = 0; n < UPLOAD_TICKCOUNT_TYPE; n++)
	{
		DDX_Control(pDX, m_IDPluType[n], m_checkPluType[n]);
	}

	for (int n = 0; n < UPLOAD_TICKCOUNT_PROTECT; n++)
	{
		DDX_Control(pDX, m_IDPluProtect[n], m_checkPluProtect[n]);
	}

	DDX_Text(pDX, IDC_EDIT_PATH_EXPORT, m_strPathExport);
	
	if ( TRUE == m_bWantID )
	{
		DDX_Text(pDX, IDC_EDIT_ID, m_strID);
		DDV_MaxChars(pDX, m_strID, MAX_LENGTH_REMOTE_ID );
	}

	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageLocationUpload, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageLocationUpload)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_FILETYPE, OnTcnSelchangeTabs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageLocationUpload::OnInitDialog() 
{
	if (SysInfo.GetMaxRemoteLocations() != 0)
	{
		if (m_nConnectionType != CONNECTION_TYPE_STANDARD_BATCH)
		{
			m_bWantID = FALSE;
		}
	}

	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_radioPluModifier1.SetWindowText( "Ignore imported modifier settings" );
	m_radioPluModifier2.SetWindowText( "Apply imported modifier settings if base item exists" );
	m_radioPluModifier3.SetWindowText( "Apply imported modifier settings and create base item if it does not exist" );

	MoveControl( &m_radioImportType1, 16, 137 );
	MoveControl( &m_radioImportType2, 16, 152 );
	MoveControl( &m_radioImportType3, 16, 167 );
	MoveControl( &m_radioPluExisting1, 16, 137 );
	MoveControl( &m_radioPluExisting2, 16, 152 );
	MoveControl( &m_radioPluExisting3, 16, 167 );

	{
		int nYPos = EcrmanOptions.GetPluPriceSetsFlag() ? 192 : 177;
		MoveControl( &m_radioPluModifier1, 16, nYPos, 300, 10 );
		MoveControl( &m_radioPluModifier2, 16, nYPos + 15, 300, 10 );
		MoveControl( &m_radioPluModifier3, 16, nYPos + 30, 300, 10 );
	}

	MoveControl( &m_staticPluType, 16, 137, 300, 10 );
	MoveControl( &m_checkPluType[0], 16, 152, 150, 10 );
	MoveControl( &m_checkPluType[1], 16, 167, 150, 10 );
	MoveControl( &m_checkPluType[2], 16, 182, 150, 10 );
	MoveControl( &m_checkPluType[3], 16, 197, 150, 10 );
	MoveControl( &m_checkPluType[4], 16, 212, 150, 10 );
	MoveControl( &m_checkPluType[5], 16, 227, 150, 10 );
	MoveControl( &m_checkPluType[6], 176, 152, 150, 10 );
	MoveControl( &m_checkPluType[7], 176, 167, 150, 10 );
	MoveControl( &m_checkPluType[8], 176, 182, 150, 10 );
	MoveControl( &m_checkPluType[9], 176, 197, 150, 10 );
	MoveControl( &m_checkPluType[10], 176, 212, 150, 10 );
	MoveControl( &m_checkPluType[11], 176, 227, 150, 10 );
	MoveControl( &m_checkPluType[12], 176, 242, 150, 10);
	MoveControl( &m_checkPluProtect[0], 16, 152, 150, 10 );

	{
		CPluCSVRecord PluRecord;
		for ( int n = 0; n < UPLOAD_TICKCOUNT_TYPE; n++ )
		{
			int nType = m_nPluType[n];
			m_checkPluType[n].SetWindowText( CPluTypeHelpers::GetText(nType) );
			m_checkPluType[n].SetCheck( ( m_nPluImportMask & ( 1 << nType ) ) != 0 );
		}

		for (int n = 0; n < UPLOAD_TICKCOUNT_PROTECT; n++)
		{
			m_checkPluProtect[n].SetCheck((m_nPluProtectMask & (1 << n)) != 0);
		}

		m_checkPluProtect[0].SetWindowText( "Category" );
	}

	{
		CString strExisting1 = "";
		strExisting1 += "Ignore imported settings for Plu items that already exist in ";
		strExisting1 += AfxGetAppName();
		strExisting1 += " database";
		m_radioPluExisting1.SetWindowText( strExisting1 );
	}

	{
		CString strExisting2 = "";
		strExisting2 += "Apply imported settings for existing Plu items to main ";
		strExisting2 += AfxGetAppName();
		strExisting2 += " database";
		m_radioPluExisting2.SetWindowText( strExisting2 );
	}
	
	if ( EcrmanOptions.GetPluProdCSVImportType() != 0 )
	{
		m_arrayTabPages.Add( UPLOAD_TABTYPE_PLU_IMPORT );
		m_arrayTabPages.Add( UPLOAD_TABTYPE_PLU_ITEM );
		m_arrayTabPages.Add( UPLOAD_TABTYPE_PLU_TYPE );
		m_arrayTabPages.Add( UPLOAD_TABTYPE_PLU_PROTECT );
	}

	if (EcrmanOptions.GetFeaturesCustomerNameImportType() != 0)
	{
		m_arrayTabPages.Add(UPLOAD_TABTYPE_CUSTOMER_IMPORT);
	}

	if ( m_arrayTabPages.GetSize() == 0 )
	{
		m_arrayTabPages.Add( UPLOAD_TABTYPE_PLU_IMPORT );
		m_arrayTabPages.Add( UPLOAD_TABTYPE_PLU_ITEM );
		m_arrayTabPages.Add( UPLOAD_TABTYPE_PLU_TYPE );
		m_arrayTabPages.Add( UPLOAD_TABTYPE_PLU_PROTECT );
	}

	for ( int n = 0; n < m_arrayTabPages.GetSize(); n++ )
	{
		switch( m_arrayTabPages.GetAt(n) )
		{
		case UPLOAD_TABTYPE_PLU_IMPORT:			m_TabFileType.InsertItem( n, "Plu Import" );		break;
		case UPLOAD_TABTYPE_PLU_ITEM:			m_TabFileType.InsertItem( n, "Plu Items" );			break;
		case UPLOAD_TABTYPE_PLU_TYPE:			m_TabFileType.InsertItem( n, "Plu Types" );			break;
		case UPLOAD_TABTYPE_PLU_PROTECT:		m_TabFileType.InsertItem( n, "Plu Protect" );		break;
		case UPLOAD_TABTYPE_CUSTOMER_IMPORT:	m_TabFileType.InsertItem( n, "Customer Import" );	break;
		}
	}

	m_nTabPage = 0;
	SelectTab();
	
	if ( FALSE == m_bWantID )
	{
		m_staticID.ShowWindow( SW_HIDE );
		m_editID.ShowWindow( SW_HIDE );
	}
	else
	{
		CString strID = "";
		
		if ( SysInfo.GetMaxRemoteLocations() == 0 )
		{
			strID += "Location ID for link to head office ";
			strID += AfxGetAppName();
		}
		else
		{
			strID += "Location ID for link to ";
			strID += AfxGetAppName();
			strID += " at EPOS terminal site";
		}

		m_staticID.SetWindowText( strID );
	}
	
	return TRUE;  
}

/**********************************************************************/

bool CPropPageLocationUpload::ExistingItemModeIsGlobal( int& nGlobalMode )
{
	nGlobalMode = DealerFlags.GetProdCSVImportExistingMode();
	return ( ( nGlobalMode >= 1 ) && ( nGlobalMode <= 3 ) );
}

/**********************************************************************/

void CPropPageLocationUpload::ValidateExistingItemMode()
{
	if ((m_nPluExistingItemMode < 0) || (m_nPluExistingItemMode > 2))
	{
		m_nPluExistingItemMode = 2;
	}

	if (EcrmanOptions.GetPluPriceSetsFlag() == FALSE)
	{
		if (2 == m_nPluExistingItemMode)
		{
			m_nPluExistingItemMode = 1;
		}
	}
}

/**********************************************************************/

void CPropPageLocationUpload::ValidatePluImportType()
{
	switch( m_nPluImportType )
	{
	case EODIMPORT_FOLDERTYPE_NONE:
	case EODIMPORT_FOLDERTYPE_ROOT:
	case EODIMPORT_FOLDERTYPE_INTRADING:
		break;

	default:
		m_nPluImportType = EODIMPORT_FOLDERTYPE_NONE;
		break;
	}
}

/**********************************************************************/

void CPropPageLocationUpload::ValidateCustomerImportType()
{
	switch( m_nCustomerImportType )
	{
	case EODIMPORT_FOLDERTYPE_ROOT:
	case EODIMPORT_FOLDERTYPE_INTRADING:
		break;

	default:
		m_nCustomerImportType = EODIMPORT_FOLDERTYPE_ROOT;
		break;
	}
}

/**********************************************************************/

void CPropPageLocationUpload::OnTcnSelchangeTabs(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	SaveTabSettings();

	m_nTabPage = m_TabFileType.GetCurSel();
	if ((m_nTabPage < 0) || (m_nTabPage >= m_arrayTabPages.GetSize()))
	{
		m_nTabPage = 0;
	}

	SelectTab();
}

/**********************************************************************/

void CPropPageLocationUpload::EnableTicks( bool bType, bool bProtect )
{
	for (int n = 0; n < UPLOAD_TICKCOUNT_TYPE; n++)
	{
		ShowAndEnableWindow(&m_checkPluType[n], bType);
	}

	for (int n = 0; n < UPLOAD_TICKCOUNT_PROTECT; n++)
	{
		ShowAndEnableWindow(&m_checkPluProtect[n], bProtect);
	}
}

/**********************************************************************/

void CPropPageLocationUpload::SelectTab()
{
	bool bPriceSets = EcrmanOptions.GetPluPriceSetsFlag();
	bool bGlobalItemMode = FALSE;
	{
		int nGlobalMode;
		bGlobalItemMode = ExistingItemModeIsGlobal( nGlobalMode );
	}

	int nTabType = m_arrayTabPages.GetAt( m_nTabPage );

	{
		bool bShowImport1 = FALSE;
		bool bShowImport2 = FALSE;
		bool bShowImport3 = FALSE;
		bool bShowExisting1 = FALSE;
		bool bShowExisting2 = FALSE;
		bool bShowExisting3 = FALSE;
		bool bEnableExisting = FALSE;
		bool bShowModifier = FALSE;
		
		switch( nTabType )
		{
		case UPLOAD_TABTYPE_PLU_IMPORT:
			bShowImport1 = TRUE;
			bShowImport2 = TRUE;
			bShowImport3 = TRUE;
			break;

		case UPLOAD_TABTYPE_PLU_ITEM:
			bShowExisting1 = TRUE;
			bShowExisting2 = TRUE;
			bShowExisting3 = ( TRUE == bPriceSets );
			bEnableExisting = ( FALSE == bGlobalItemMode );
			bShowModifier = TRUE;
			break;

		case UPLOAD_TABTYPE_CUSTOMER_IMPORT:
			bShowImport1 = TRUE;
			bShowImport2 = TRUE;
			break;
		}

		ShowAndEnableWindow( &m_radioImportType1, bShowImport1 );
		ShowAndEnableWindow( &m_radioImportType2, bShowImport2 );
		ShowAndEnableWindow( &m_radioImportType3, bShowImport3 );
		ShowAndEnableWindow( &m_radioPluExisting1, bShowExisting1, bEnableExisting );
		ShowAndEnableWindow( &m_radioPluExisting2, bShowExisting2, bEnableExisting );
		ShowAndEnableWindow( &m_radioPluExisting3, bShowExisting3, bEnableExisting );
		ShowAndEnableWindow( &m_radioPluModifier1, bShowModifier );
		ShowAndEnableWindow( &m_radioPluModifier2, bShowModifier );
		ShowAndEnableWindow( &m_radioPluModifier3, bShowModifier );
	}

	switch ( nTabType )
	{
	case UPLOAD_TABTYPE_PLU_IMPORT:
		
		m_staticPluType.ShowWindow( SW_HIDE );
		EnableTicks( FALSE, FALSE );

		m_radioImportType1.SetWindowText( "Disable Plu import for this location" );
		m_radioImportType2.SetWindowText( "Import Plu changes from EOD files in export root folder" );

		ValidatePluImportType();

		m_radioImportType1.SetCheck( EODIMPORT_FOLDERTYPE_NONE == m_nPluImportType );
		m_radioImportType2.SetCheck( EODIMPORT_FOLDERTYPE_ROOT == m_nPluImportType );
		m_radioImportType3.SetCheck( EODIMPORT_FOLDERTYPE_INTRADING == m_nPluImportType );

		break;

	case UPLOAD_TABTYPE_PLU_ITEM:
		
		m_staticPluType.ShowWindow( SW_HIDE );
		EnableTicks( FALSE, FALSE );

		ValidateExistingItemMode();

		m_radioPluExisting1.SetCheck( 0 == m_nPluExistingItemMode );
		m_radioPluExisting2.SetCheck( 1 == m_nPluExistingItemMode );
		m_radioPluExisting3.SetCheck( 2 == m_nPluExistingItemMode );

		m_radioPluModifier1.SetCheck( 0 == m_nPluImportModifierMode );
		m_radioPluModifier2.SetCheck( 1 == m_nPluImportModifierMode );
		m_radioPluModifier3.SetCheck( 2 == m_nPluImportModifierMode );

		break;

	case UPLOAD_TABTYPE_PLU_TYPE:
		m_staticPluType.SetWindowText( "Filter changes to existing items by Plu Type" );
		m_staticPluType.ShowWindow( SW_SHOW );
		EnableTicks( TRUE, FALSE );
		break;

	case UPLOAD_TABTYPE_PLU_PROTECT:
		m_staticPluType.SetWindowText( "Protect selected fields for existing items" );
		m_staticPluType.ShowWindow( SW_SHOW );
		EnableTicks( FALSE, TRUE );
		break;

	case UPLOAD_TABTYPE_CUSTOMER_IMPORT:

		m_staticPluType.ShowWindow( SW_HIDE );
		EnableTicks( FALSE, FALSE );
		
		m_radioImportType1.SetWindowText( "Import customer names from EOD files in export root folder" );
		m_radioImportType2.SetWindowText( "Import customer names from real time files in INTRADING subfolder of export root" );

		ValidateCustomerImportType();

		m_radioImportType1.SetCheck( EODIMPORT_FOLDERTYPE_ROOT == m_nCustomerImportType );
		m_radioImportType2.SetCheck( EODIMPORT_FOLDERTYPE_INTRADING == m_nCustomerImportType );
		m_radioImportType3.SetCheck( FALSE );

		break;
	}
}

/**********************************************************************/

void CPropPageLocationUpload::SaveTabSettings()
{
	switch (m_arrayTabPages.GetAt(m_nTabPage))
	{
	case UPLOAD_TABTYPE_PLU_IMPORT:

		if (m_radioImportType1.GetCheck() == TRUE)
		{
			m_nPluImportType = EODIMPORT_FOLDERTYPE_NONE;
		}
		else if (m_radioImportType2.GetCheck() == TRUE)
		{
			m_nPluImportType = EODIMPORT_FOLDERTYPE_ROOT;
		}
		else if (m_radioImportType3.GetCheck() == TRUE)
		{
			m_nPluImportType = EODIMPORT_FOLDERTYPE_INTRADING;
		}

		break;

	case UPLOAD_TABTYPE_PLU_ITEM:

		if (m_radioPluExisting1.GetCheck() == TRUE)
		{
			m_nPluExistingItemMode = 0;
		}
		else if (m_radioPluExisting2.GetCheck() == TRUE)
		{
			m_nPluExistingItemMode = 1;
		}
		else if (m_radioPluExisting3.GetCheck() == TRUE)
		{
			m_nPluExistingItemMode = 2;
		}

		m_nPluImportModifierMode = 0;

		if (IsTicked(m_radioPluModifier2))
		{
			m_nPluImportModifierMode = 1;
		}

		if (IsTicked(m_radioPluModifier3))
		{
			m_nPluImportModifierMode = 2;
		}

		break;

	case UPLOAD_TABTYPE_PLU_TYPE:

		m_nPluImportMask = 0;
		for (int n = 0; n < UPLOAD_TICKCOUNT_TYPE; n++)
		{
			if (m_checkPluType[n].GetCheck() != 0)
			{
				m_nPluImportMask += (1 << m_nPluType[n]);
			}
		}
		
		break;

	case UPLOAD_TABTYPE_PLU_PROTECT:

		m_nPluProtectMask = 0;
		{
			for (int n = 0; n < UPLOAD_TICKCOUNT_PROTECT; n++)
			{
				if (m_checkPluProtect[n].GetCheck() != 0)
				{
					m_nPluProtectMask += (1 << m_nPluType[n]);
				}
			}
		}
		
		break;

	case UPLOAD_TABTYPE_CUSTOMER_IMPORT:

		if (m_radioImportType1.GetCheck() == TRUE)
		{
			m_nCustomerImportType = EODIMPORT_FOLDERTYPE_ROOT;
		}
		else if (m_radioImportType2.GetCheck() == TRUE)
		{
			m_nCustomerImportType = EODIMPORT_FOLDERTYPE_INTRADING;
		}

		break;
	}
}

/**********************************************************************/

BOOL CPropPageLocationUpload::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageLocationUpload::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

bool CPropPageLocationUpload::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageLocationUpload::Refresh()
{
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPropPageLocationUpload::GetRecordData()
{
	m_strPathExport = m_pLocationBuffer -> GetTerminalExportPath(0);
	m_strID = m_pLocationBuffer -> GetRemoteID(0);
	
	m_nPluImportType = m_pLocationBuffer -> GetProdCSVImportFolderType(0);
	m_nCustomerImportType = m_pLocationBuffer -> GetCustomerImportFolderType(0);

	int nGlobalMode;
	if (ExistingItemModeIsGlobal(nGlobalMode) == TRUE)
	{
		m_nPluExistingItemMode = nGlobalMode;
	}
	else
	{
		m_nPluExistingItemMode = m_pLocationBuffer->GetProdCSVImportExistingMode(0);
	}

	m_nPluImportMask = m_pLocationBuffer -> GetProdCSVImportTypeMask(0);
	m_nPluProtectMask = m_pLocationBuffer -> GetProdCSVProtectFieldMask(0);

	m_nPluImportModifierMode = m_pLocationBuffer -> GetProdCSVImportModifierMode(0);

	ValidateExistingItemMode();
}

/**********************************************************************/

void CPropPageLocationUpload::SaveRecord()
{
	SaveTabSettings();
	ValidatePluImportType();
	ValidateCustomerImportType();

	m_pLocationBuffer -> SetTerminalExportPath( 0, m_strPathExport );

	if (TRUE == m_bWantID)
	{
		m_pLocationBuffer->SetRemoteID(0, m_strID);
	}

	m_pLocationBuffer -> SetProdCSVImportFolderType( 0, m_nPluImportType );
	m_pLocationBuffer -> SetProdCSVImportTypeMask( 0, m_nPluImportMask );
	m_pLocationBuffer -> SetProdCSVProtectFieldMask( 0, m_nPluProtectMask );
	m_pLocationBuffer -> SetProdCSVImportModifierMode( 0, m_nPluImportModifierMode );
	m_pLocationBuffer -> SetCustomerImportFolderType( 0, m_nCustomerImportType );

	if ( EODIMPORT_FOLDERTYPE_NONE != m_nPluImportType )
	{
		int nGlobalMode;
		if ( ExistingItemModeIsGlobal( nGlobalMode ) == FALSE )
		{
			ValidateExistingItemMode();
			m_pLocationBuffer -> SetProdCSVImportExistingMode( 0, m_nPluExistingItemMode );
		}
	}
}

/**********************************************************************/
