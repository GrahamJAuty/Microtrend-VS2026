/**********************************************************************/
#include "SPOSKeyboardListAddDlg.h"
#include "SPOSKeyboardPropertySheetEditList.h"
#include "SPOSKeyboardPropertySheetEditScreen.h"
/**********************************************************************/
#include "SPOSKeyboardPropPageEditScreenLoggedOff.h"
/**********************************************************************/

CSPOSKeyboardPropPageEditScreenLoggedOff::CSPOSKeyboardPropPageEditScreenLoggedOff() : CSSPropertyPage(CSPOSKeyboardPropPageEditScreenLoggedOff::IDD)
{
	//{{AFX_DATA_INIT(CSPOSKeyboardPropPageEditScreenLoggedOff)
	m_strName = _T("");
	m_nScreenStyle = 0;
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenLoggedOff::SetScreenRecord( CSPOSKeyboardScreenRecord* pRecord )
{
	m_pScreenRecord = pRecord;

	m_nScreenStyle = m_pScreenRecord -> GetLoggedOffScreenStyle() - 1;
	if ( m_nScreenStyle < 0 || m_nScreenStyle > 1 )
		m_nScreenStyle = 0;
}

/**********************************************************************/

CSPOSKeyboardPropPageEditScreenLoggedOff::~CSPOSKeyboardPropPageEditScreenLoggedOff()
{
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenLoggedOff::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardPropPageEditScreenLoggedOff)
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 25);
	DDX_Radio(pDX, IDC_RADIO_STYLE, m_nScreenStyle);
	DDX_Control(pDX, IDC_COMBO_FUNCTION, m_comboFunction);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPropPageEditScreenLoggedOff, CPropertyPage)
	//{{AFX_MSG_MAP(CSPOSKeyboardPropPageEditScreenLoggedOff)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_FUNCTION, OnButtonEditFunction)
	ON_BN_CLICKED(IDC_RADIO_STYLE, OnRadioStyle1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadioStyle2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditScreenLoggedOff::OnInitDialog() 
{
	m_strName = m_pScreenRecord -> GetName();
	
	CPropertyPage::OnInitDialog();

	if ( 0 == m_nScreenStyle )
		RefreshListCombo( m_comboFunction, m_arrayListNoFunction, m_pScreenRecord -> GetLoggedOffListFunction(), LISTTYPE_FUNCTION_STANDARD  );
	else
		RefreshListCombo( m_comboFunction, m_arrayListNoFunction, m_pScreenRecord -> GetLoggedOffListFunctionLarge(), LISTTYPE_ITEM_LARGE  );

	return TRUE;  
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditScreenLoggedOff::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditScreenLoggedOff::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenLoggedOff::OnRadioStyle1() 
{
	CSPOSKeyboardScreenRecord screen;
	screen.SetLoggedOffScreenStyle(1);
	RefreshListCombo( m_comboFunction, m_arrayListNoFunction, -1, screen.GetLoggedOffListTypeFunction() );
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenLoggedOff::OnRadioStyle2() 
{
	CSPOSKeyboardScreenRecord screen;
	screen.SetLoggedOffScreenStyle(2);
	RefreshListCombo( m_comboFunction, m_arrayListNoFunction, -1, screen.GetLoggedOffListTypeFunction() );
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenLoggedOff::RefreshListCombo( CSSComboBox& combo, CArray<__int64,__int64>& arrayListNo, __int64 nListNoToSelect, int nListType )
{
	( ( CSPOSKeyboardPropertySheetEditScreen* ) m_pPropertySheet ) -> RefreshListCombo( combo, arrayListNo, nListNoToSelect, nListType );
}

/**********************************************************************/

__int64 CSPOSKeyboardPropPageEditScreenLoggedOff::GetComboListNo( CSSComboBox& combo, CArray<__int64,__int64>& arrayListNo )
{
	return ( ( CSPOSKeyboardPropertySheetEditScreen* ) m_pPropertySheet ) -> GetComboListNo( combo, arrayListNo );
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenLoggedOff::OnButtonEditFunction()
{
	UpdateData( TRUE, FALSE );

	CSPOSKeyboardScreenRecord screen;
	screen.SetLoggedOffScreenStyle( m_nScreenStyle + 1 );
	
	EditList( m_comboFunction, m_arrayListNoFunction, screen.GetLoggedOffListTypeFunction() );
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenLoggedOff::EditList( CSSComboBox& combo, CArray<__int64,__int64>& arrayListNo, int nListType )
{
	CSPOSKeyboardScreenRecord screen;
	screen.SetLoggedOffScreenStyle( m_nScreenStyle + 1 );
	
	__int64 nListNo = GetComboListNo( combo, arrayListNo );
	if ( nListNo != 0 )
	{
		int nListIdx;
		if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == FALSE )
			return;

		CSPOSKeyboardPropertySheetEditList propSheet( nListIdx, this );
		if ( propSheet.DoModal() == IDOK )
			if ( propSheet.CheckKeyCount() == TRUE )
				propSheet.SaveRecord();

		RefreshListCombo( m_comboFunction, m_arrayListNoFunction, -1, screen.GetLoggedOffListTypeFunction() );
	}
	else
	{
		CSPOSKeyboardListAddDlg dlg ( 0, nListType, this );
		
		if ( dlg.DoModal() == IDOK )
		{
			int nListIdx;
			if ( SPOSKeyboardManager.GetListManager() -> FindOrInsertList( dlg.m_nListNo, nListIdx ) == TRUE )
			{
				__int64 nListNoFunction = -1;
				
				CSPOSKeyboardListRecord ListRecord;
				SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );
				ListRecord.SetListType( nListType );
				ListRecord.SetDefaultListFormat();
				SPOSKeyboardManager.GetListManager() -> SetListRecord( nListIdx, ListRecord );
				SPOSKeyboardManager.GetListManager() -> InitialiseKeysForList( nListIdx );

				CSPOSKeyboardPropertySheetEditList propSheet( nListIdx, this );
				if ( propSheet.DoModal() == IDOK )
					propSheet.SaveRecord();

				__int64 nListNo = ListRecord.GetListNo();
				if ( &combo == &m_comboFunction ) nListNoFunction = nListNo;
				
				RefreshListCombo( m_comboFunction, m_arrayListNoFunction, nListNoFunction, screen.GetLoggedOffListTypeFunction() );
			}
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenLoggedOff::SaveRecord()
{
	m_pScreenRecord -> SetName( m_strName );	
	m_pScreenRecord -> SetLoggedOffScreenStyle( m_nScreenStyle + 1 );
	
	if ( 0 == m_nScreenStyle )
		m_pScreenRecord -> SetLoggedOffListFunction( GetComboListNo( m_comboFunction, m_arrayListNoFunction) );
	else
		m_pScreenRecord -> SetLoggedOffListFunctionLarge( GetComboListNo( m_comboFunction, m_arrayListNoFunction ) );
}

/**********************************************************************/
