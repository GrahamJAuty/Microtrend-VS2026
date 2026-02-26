/**********************************************************************/
#include "SPOSKeyboardListAddDlg.h"
#include "SPOSKeyboardPropertySheetEditList.h"
#include "SPOSKeyboardPropertySheetEditScreen.h"
/**********************************************************************/
#include "SPOSKeyboardPropPageEditScreenTrading.h"
/**********************************************************************/

CSPOSKeyboardPropPageEditScreenTrading::CSPOSKeyboardPropPageEditScreenTrading() : CSSPropertyPage(CSPOSKeyboardPropPageEditScreenTrading::IDD)
{
	//{{AFX_DATA_INIT(CSPOSKeyboardPropPageEditScreenTrading)
	m_strName = _T("");
	m_nScreenStyle = 0;
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenTrading::SetScreenRecord( CSPOSKeyboardScreenRecord* pRecord )
{
	m_pScreenRecord = pRecord;

	m_nScreenStyle = m_pScreenRecord -> GetTradingScreenStyle() - 1;
	if ( m_nScreenStyle < 0 || m_nScreenStyle > 2 )
		m_nScreenStyle = 0;
}

/**********************************************************************/

CSPOSKeyboardPropPageEditScreenTrading::~CSPOSKeyboardPropPageEditScreenTrading()
{
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenTrading::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardPropPageEditScreenTrading)
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 25);
	DDX_Radio(pDX, IDC_RADIO_STYLE, m_nScreenStyle);
	DDX_Control(pDX, IDC_COMBO_TOPLEVEL, m_comboTopLevel);
	DDX_Control(pDX, IDC_COMBO_FUNCTION, m_comboFunction);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPropPageEditScreenTrading, CPropertyPage)
	//{{AFX_MSG_MAP(CSPOSKeyboardPropPageEditScreenTrading)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_TOPLEVEL, OnButtonEditTopLevel)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_FUNCTION, OnButtonEditFunction)
	ON_BN_CLICKED(IDC_RADIO_STYLE, OnRadioStyle1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadioStyle2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadioStyle3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditScreenTrading::OnInitDialog() 
{
	m_strName = m_pScreenRecord -> GetName();
	
	CPropertyPage::OnInitDialog();

	RefreshListCombo( m_comboTopLevel, m_arrayListNoTopLevel, m_pScreenRecord -> GetTradingListTopLevel(), m_pScreenRecord -> GetTradingListTypeTopLevel()  );
	RefreshListCombo( m_comboFunction, m_arrayListNoFunction, m_pScreenRecord -> GetTradingListFunction(), m_pScreenRecord -> GetTradingListTypeFunction()  );
	
	return TRUE;  
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditScreenTrading::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditScreenTrading::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenTrading::OnRadioStyle1() 
{
	CSPOSKeyboardScreenRecord screen;
	screen.SetTradingScreenStyle(1);
	RefreshListCombo( m_comboTopLevel, m_arrayListNoTopLevel, -1, screen.GetTradingListTypeTopLevel() );
	RefreshListCombo( m_comboFunction, m_arrayListNoFunction, -1, screen.GetTradingListTypeFunction() );
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenTrading::OnRadioStyle2() 
{
	CSPOSKeyboardScreenRecord screen;
	screen.SetTradingScreenStyle(2);
	RefreshListCombo( m_comboTopLevel, m_arrayListNoTopLevel, -1, screen.GetTradingListTypeTopLevel() );
	RefreshListCombo( m_comboFunction, m_arrayListNoFunction, -1, screen.GetTradingListTypeFunction() );
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenTrading::OnRadioStyle3() 
{
	CSPOSKeyboardScreenRecord screen;
	screen.SetTradingScreenStyle(3);
	RefreshListCombo( m_comboTopLevel, m_arrayListNoTopLevel, -1, screen.GetTradingListTypeTopLevel() );
	RefreshListCombo( m_comboFunction, m_arrayListNoFunction, -1, screen.GetTradingListTypeFunction() );
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenTrading::RefreshListCombo( CSSComboBox& combo, CArray<__int64,__int64>& arrayListNo, __int64 nListNoToSelect, int nListType )
{
	( ( CSPOSKeyboardPropertySheetEditScreen* ) m_pPropertySheet ) -> RefreshListCombo( combo, arrayListNo, nListNoToSelect, nListType );
}

/**********************************************************************/

__int64 CSPOSKeyboardPropPageEditScreenTrading::GetComboListNo( CSSComboBox& combo, CArray<__int64,__int64>& arrayListNo )
{
	return ( ( CSPOSKeyboardPropertySheetEditScreen* ) m_pPropertySheet ) -> GetComboListNo( combo, arrayListNo );
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenTrading::OnButtonEditTopLevel()
{
	UpdateData( TRUE, FALSE );

	CSPOSKeyboardScreenRecord screen;
	screen.SetTradingScreenStyle( m_nScreenStyle + 1 );
	
	EditList( m_comboTopLevel, m_arrayListNoTopLevel, screen.GetTradingListTypeTopLevel() );
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenTrading::OnButtonEditFunction()
{
	UpdateData( TRUE, FALSE );

	CSPOSKeyboardScreenRecord screen;
	screen.SetTradingScreenStyle( m_nScreenStyle + 1 );
	
	EditList( m_comboFunction, m_arrayListNoFunction, screen.GetTradingListTypeFunction() );
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenTrading::EditList( CSSComboBox& combo, CArray<__int64, __int64>& arrayListNo, int nListType )
{
	CSPOSKeyboardScreenRecord screen;
	screen.SetTradingScreenStyle( m_nScreenStyle + 1 );
	
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

		RefreshListCombo( m_comboTopLevel, m_arrayListNoTopLevel, -1, screen.GetTradingListTypeTopLevel() );
		RefreshListCombo( m_comboFunction, m_arrayListNoFunction, -1, screen.GetTradingListTypeFunction() );
	}
	else
	{
		CSPOSKeyboardListAddDlg dlg ( 0, nListType, this );
		
		if ( dlg.DoModal() == IDOK )
		{
			int nListIdx;
			if ( SPOSKeyboardManager.GetListManager() -> FindOrInsertList( dlg.m_nListNo, nListIdx ) == TRUE )
			{
				__int64 nListNoTopLevel = -1;
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
				if ( &combo == &m_comboTopLevel ) nListNoTopLevel = nListNo;
				if ( &combo == &m_comboFunction ) nListNoFunction = nListNo;
				
				RefreshListCombo( m_comboTopLevel, m_arrayListNoTopLevel, nListNoTopLevel, screen.GetTradingListTypeTopLevel() );
				RefreshListCombo( m_comboFunction, m_arrayListNoFunction, nListNoFunction, screen.GetTradingListTypeFunction() );
			}
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenTrading::SaveRecord()
{
	m_pScreenRecord -> SetName( m_strName );	
	m_pScreenRecord -> SetTradingScreenStyle( m_nScreenStyle + 1 );
	m_pScreenRecord -> SetTradingListTopLevel( GetComboListNo( m_comboTopLevel, m_arrayListNoTopLevel ) );
	m_pScreenRecord -> SetTradingListFunction( GetComboListNo( m_comboFunction, m_arrayListNoFunction ) );
}

/**********************************************************************/
