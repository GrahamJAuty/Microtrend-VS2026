/**********************************************************************/
#include "EcrmanSupplierCSVArray.h"
/**********************************************************************/
#include "EcrmanSupplierSelector.h"
/**********************************************************************/

CEcrmanSupplierSelector::CEcrmanSupplierSelector( CEdit& edit, CSSComboBox& combo, bool bAll ) : m_EditBox ( edit ), m_ComboBox ( combo )
{
	m_nSuppNo = 0;
	m_bAll = bAll;
}

/**********************************************************************/

void CEcrmanSupplierSelector::FillEcrmanSupplierCombo( int nSuppNo )
{
	m_ComboBox.ResetContent();

	for ( int n = 0; n < DataManagerNonDb.EcrmanSupplier.GetSize(); n++ )
	{
		CEcrmanSupplierCSVRecord Supplier;
		DataManagerNonDb.EcrmanSupplier.GetAt( n, Supplier );

		CString strName = Supplier.GetSuppName();
		if ( strName != Supplier.GetDefaultName() )
		{
			int nInsertionPoint = m_ComboBox.AddString( strName );
			m_ComboBox.SetItemData( nInsertionPoint, Supplier.GetSuppNo() );
		}
	}

	for ( int n = 0; n < DataManagerNonDb.EcrmanSupplier.GetSize(); n++ )
	{
		CEcrmanSupplierCSVRecord Supplier;
		DataManagerNonDb.EcrmanSupplier.GetAt( n, Supplier );

		CString strName = Supplier.GetSuppName();
		if ( strName == Supplier.GetDefaultName() )
		{
			int nInsertionPoint = m_ComboBox.InsertString( -1, strName );
			m_ComboBox.SetItemData( nInsertionPoint, Supplier.GetSuppNo() );
		}
	}

	m_ComboBox.InsertString ( 0, "Unspecified" );
	m_ComboBox.SetItemData( 0, 0 );

	if ( m_bAll == TRUE )
	{
		m_ComboBox.InsertString ( 0, "All" );
		m_ComboBox.SetItemData( 0, ECRMANSUPPLIER_ALL );
	}

	m_nSuppNo = nSuppNo;
	
	bool bFound = FALSE;
	for ( int nIndex = 0; nIndex < m_ComboBox.GetCount(); nIndex++ )
	{
		if ( m_ComboBox.GetItemData ( nIndex ) == (WORD) m_nSuppNo )
		{
			m_ComboBox.SetCurSel ( nIndex );
			bFound = TRUE;
			break;
		}
	}

	if ( bFound == FALSE )
	{
		CString strSupplier;
		strSupplier.Format ( "Supplier %d", m_nSuppNo );
		m_ComboBox.InsertString( 0, strSupplier );
		m_ComboBox.SetItemData( 0, ECRMANSUPPLIER_UNKNOWN );
		m_ComboBox.SetCurSel(0);
	}

	UpdateEcrmanSupplierEditBox();
}

/**********************************************************************/

void CEcrmanSupplierSelector::UpdateEcrmanSupplierCombo( int nNewSuppNo )
{
	m_nSuppNo = nNewSuppNo;
	bool bFound = 0;

	for ( int nIndex = 0; nIndex <= m_ComboBox.GetCount(); nIndex++ )
	{
		if ( m_ComboBox.GetItemData ( nIndex ) == ( WORD ) m_nSuppNo )
		{
			m_ComboBox.SetCurSel ( nIndex );
			bFound = TRUE;
			break;
		}
	}

	if ( bFound == FALSE )
	{
		if ( m_ComboBox.GetItemData(0) == ECRMANSUPPLIER_UNKNOWN )
			m_ComboBox.DeleteString(0);

		CString strSupplier;
		strSupplier.Format ( "Supplier %d", m_nSuppNo );
		m_ComboBox.InsertString( 0, strSupplier );
		m_ComboBox.SetItemData( 0, ECRMANSUPPLIER_UNKNOWN );
		
		m_ComboBox.SetCurSel(0);
	}
	else
	{
		if ( m_ComboBox.GetItemData(0) == ECRMANSUPPLIER_UNKNOWN )
			m_ComboBox.DeleteString(0);
	}
}

/**********************************************************************/

void CEcrmanSupplierSelector::UpdateEcrmanSupplierEditBox()
{
	CString strSuppNo = "";

	if ( m_nSuppNo != ECRMANSUPPLIER_ALL )
		strSuppNo.Format ( "%d", m_nSuppNo );
	
	m_EditBox.SetWindowText ( strSuppNo );
}

/**********************************************************************/

void CEcrmanSupplierSelector::SelectEcrmanSupplierFromCombo() 
{
	int nIndex = m_ComboBox.GetCurSel();
	
	if ( nIndex != CB_ERR )
	{
		int nNewSuppNo = m_ComboBox.GetItemData ( nIndex );

		if ( ( nNewSuppNo != m_nSuppNo ) && ( nNewSuppNo != ECRMANSUPPLIER_UNKNOWN ) )
		{
			m_nSuppNo = nNewSuppNo;
			UpdateEcrmanSupplierEditBox();

			if ( m_ComboBox.GetItemData(0) == ECRMANSUPPLIER_UNKNOWN )
				m_ComboBox.DeleteString(0);
		}
	}
}

/**********************************************************************/

void CEcrmanSupplierSelector::SelectEcrmanSupplierFromEditBox() 
{
	CString strEcrmanSupplierNo;
	m_EditBox.GetWindowText( strEcrmanSupplierNo );
	
	::TrimSpaces ( strEcrmanSupplierNo, FALSE );
	
	if ( strEcrmanSupplierNo == "" )
	{
		UpdateEcrmanSupplierEditBox();
		return;
	}

	int nNewEcrmanSupplierNo = atoi ( strEcrmanSupplierNo );
	if ( nNewEcrmanSupplierNo != m_nSuppNo )
	{
		UpdateEcrmanSupplierCombo( nNewEcrmanSupplierNo );
		UpdateEcrmanSupplierEditBox();
	}
}

/**********************************************************************/
