/**********************************************************************/
#include "SPOSKeyboardListAddDlg.h"
#include "SPOSKeyboardPropertySheetEditList.h"
#include "SPOSKeyboardPropertySheetEditScreen.h"
/**********************************************************************/
#include "SPOSKeyboardPropPageEditScreenPayment.h"
/**********************************************************************/

CSPOSKeyboardPropPageEditScreenPayment::CSPOSKeyboardPropPageEditScreenPayment() : CSSPropertyPage(CSPOSKeyboardPropPageEditScreenPayment::IDD)
{
	//{{AFX_DATA_INIT(CSPOSKeyboardPropPageEditScreenPayment)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenPayment::SetScreenRecord( CSPOSKeyboardScreenRecord* pRecord )
{
	m_pScreenRecord = pRecord;
}

/**********************************************************************/

CSPOSKeyboardPropPageEditScreenPayment::~CSPOSKeyboardPropPageEditScreenPayment()
{
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenPayment::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardPropPageEditScreenPayment)
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 25);
	DDX_Control(pDX, IDC_COMBO_TOPLEVEL, m_comboTopLevel);
	DDX_Control(pDX, IDC_COMBO_FUNCTION, m_comboFunction);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPropPageEditScreenPayment, CPropertyPage)
	//{{AFX_MSG_MAP(CSPOSKeyboardPropPageEditScreenPayment)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_TOPLEVEL, OnButtonEditTopLevel)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_FUNCTION, OnButtonEditFunction)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditScreenPayment::OnInitDialog() 
{
	m_strName = m_pScreenRecord -> GetName();
	
	CPropertyPage::OnInitDialog();

	RefreshListCombo( m_comboTopLevel, m_arrayListNoTopLevel, m_pScreenRecord -> GetPaymentListTopLevel(), LISTTYPE_ITEM_STANDARD  );
	RefreshListCombo( m_comboFunction, m_arrayListNoFunction, m_pScreenRecord -> GetPaymentListFunction(), LISTTYPE_FUNCTION_STANDARD  );
	
	return TRUE;  
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditScreenPayment::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditScreenPayment::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenPayment::RefreshListCombo( CSSComboBox& combo, CArray<__int64,__int64>& arrayListNo, __int64 nListNoToSelect, int nListType )
{
	( ( CSPOSKeyboardPropertySheetEditScreen* ) m_pPropertySheet ) -> RefreshListCombo( combo, arrayListNo, nListNoToSelect, nListType );
}

/**********************************************************************/

__int64 CSPOSKeyboardPropPageEditScreenPayment::GetComboListNo( CSSComboBox& combo, CArray<__int64,__int64>& arrayListNo )
{
	return ( ( CSPOSKeyboardPropertySheetEditScreen* ) m_pPropertySheet ) -> GetComboListNo( combo, arrayListNo );
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenPayment::OnButtonEditTopLevel()
{
	UpdateData( TRUE, FALSE );
	EditList( m_comboTopLevel, m_arrayListNoTopLevel, LISTTYPE_ITEM_STANDARD );
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenPayment::OnButtonEditFunction()
{
	UpdateData( TRUE, FALSE );
	EditList( m_comboFunction, m_arrayListNoFunction, LISTTYPE_FUNCTION_STANDARD );
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenPayment::EditList( CSSComboBox& combo, CArray<__int64,__int64>& arrayListNo, int nListType )
{
	__int64  nListNo = GetComboListNo( combo, arrayListNo );

	if ( nListNo != 0 )
	{
		int nListIdx;
		if ( SPOSKeyboardManager.GetListManager() -> FindList( nListNo, nListIdx ) == FALSE )
			return;

		CSPOSKeyboardPropertySheetEditList propSheet( nListIdx, this );
		if ( propSheet.DoModal() == IDOK )
			if ( propSheet.CheckKeyCount() == TRUE )
				propSheet.SaveRecord();

		RefreshListCombo( m_comboTopLevel, m_arrayListNoTopLevel, -1, LISTTYPE_ITEM_STANDARD );
		RefreshListCombo( m_comboFunction, m_arrayListNoFunction, -1, LISTTYPE_FUNCTION_STANDARD );
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
				
				RefreshListCombo( m_comboTopLevel, m_arrayListNoTopLevel, nListNoTopLevel, LISTTYPE_ITEM_STANDARD );
				RefreshListCombo( m_comboFunction, m_arrayListNoFunction, nListNoFunction, LISTTYPE_FUNCTION_STANDARD );
			}
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditScreenPayment::SaveRecord()
{
	m_pScreenRecord -> SetName( m_strName );	
	m_pScreenRecord -> SetPaymentListTopLevel( GetComboListNo( m_comboTopLevel, m_arrayListNoTopLevel ) );
	m_pScreenRecord -> SetPaymentListFunction( GetComboListNo( m_comboFunction, m_arrayListNoFunction ) );
}

/**********************************************************************/
