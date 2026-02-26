/**********************************************************************/
#include "SalesHistoryFields.h"
#include "SalesHistoryModifierDlg.h"
#include "SalesHistoryPriceLevelDlg.h"
#include "SalesHistoryTaxDlg.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PosTrayTaskEditHelpers.h"
/**********************************************************************/

CPosTrayTaskEditHelpers::CPosTrayTaskEditHelpers( CSSTabbedComboBox& comboPriceLevel, CSSTabbedComboBox& comboTaxBand, CSSTabbedComboBox& comboModifier ) : m_comboPriceLevel( comboPriceLevel ), m_comboTaxBand( comboTaxBand ), m_comboModifier( comboModifier )
{
}

/**********************************************************************/

void CPosTrayTaskEditHelpers::InitTabbedCombos()
{
	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(30);
	m_comboPriceLevel.SetTabStops( arrayStops );
	m_comboModifier.SetTabStops( arrayStops );

	arrayStops.Add(130);
	m_comboTaxBand.SetTabStops( arrayStops );
}

/**********************************************************************/

void CPosTrayTaskEditHelpers::FillTabbedCombos( int nDbIdx )
{
	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( nDbIdx, info, FALSE );
	
	{	
		int nSel = m_comboTaxBand.GetCurSel();

		m_comboTaxBand.ResetContent();
		m_comboTaxBand.AddItem ( ",All" );
		m_comboTaxBand.SetItemData (0, TAXBAND_FILTER_ALL );

		int nMask = 2;
		CString strLetter = "A";
		for ( int nTaxBand = 1; nTaxBand <= MAX_TAX_BANDS; nTaxBand++ )
		{
			CTaxRateInfo TaxRateInfo;
			DataManager.TaxRates.GetTaxRateInfo( nTaxBand, TaxRateInfo );

			strLetter.SetAt( 0, 'A' + nTaxBand - 1 );

			CString strRate;
			strRate.Format ( "%.4lf", TaxRateInfo.m_dTaxRate );

			CCSV csv;
			csv.Add( strLetter );
			csv.Add( TaxRateInfo.m_strReportText );
			csv.Add( strRate );
			m_comboTaxBand.AddItem( csv.GetLine() );
			m_comboTaxBand.SetItemData( nTaxBand, nMask );
			
			nMask *= 2;
		}

		CCSV csv;
		csv.Add( "" );
		csv.Add( SysInfo.GetCustomTaxFilterString() );
		m_comboTaxBand.AddItem( csv.GetLine() );
		m_comboTaxBand.SetItemData( MAX_TAX_BANDS + 1, SysInfo.GetCustomTaxFilter() );
		
		if ( ( nSel >= 0 ) && ( nSel < m_comboTaxBand.GetCount() ) )
			m_comboTaxBand.SetCurSel( nSel );
		else
			m_comboTaxBand.SetCurSel( 0 );
	}

	{
		int nSel = m_comboModifier.GetCurSel();

		m_comboModifier.ResetContent();
		m_comboModifier.AddItem ( ",All" );
		m_comboModifier.SetItemData (0, MODIFIER_FILTER_ALL );

		int nMask = 1;
		for ( int nMod = 0; nMod < 10; nMod++ )
		{
			if ( 0 == nMod )
			{
				m_comboModifier.AddItem( "00,Base Item" );
				m_comboModifier.SetItemData( nMod + 1, nMask );
			}
			else
			{
				CString strNum;
				strNum.Format( "%2.2d", nMod );

				CCSV csv;
				csv.Add( strNum );
				csv.Add( DataManager.Modifier.GetDisplayName( nMod ) );
				m_comboModifier.AddItem( csv.GetLine() );
				m_comboModifier.SetItemData ( nMod + 1, nMask );
			}
			nMask *= 2;
		}

		CCSV csv;
		csv.Add( "" );
		csv.Add( SysInfo.GetCustomModifierFilterString() );
		m_comboModifier.AddItem( csv.GetLine() );
		m_comboModifier.SetItemData( 11, SysInfo.GetCustomModifierFilter() );
		
		if ( ( nSel >= 0 ) && ( nSel < m_comboModifier.GetCount() ) )
			m_comboModifier.SetCurSel( nSel );
		else
			m_comboModifier.SetCurSel( 0 );
	}

	if ( m_comboPriceLevel.GetCount() == 0 )
	{
		m_comboPriceLevel.ResetContent();
		m_comboPriceLevel.AddItem ( ",All" );
		m_comboPriceLevel.SetItemData (0, PRICELEVEL_FILTER_ALL );

		int nMask = 1;
		for ( int nLevel = 0; nLevel < 10; nLevel++ )
		{
			CString strNum;
			strNum.Format( "%2.2d", nLevel + 1 );

			CCSV csv;
			csv.Add( strNum );
			csv.Add( DataManager.PriceText.GetReportText( nLevel ) );
			m_comboPriceLevel.AddItem( csv.GetLine() );
			m_comboPriceLevel.SetItemData ( nLevel + 1, nMask );
			nMask *= 2;
		}

		CCSV csv;
		csv.Add( "" );
		csv.Add( SysInfo.GetCustomPriceLevelFilterString() );
		m_comboPriceLevel.AddItem( csv.GetLine() );
		m_comboPriceLevel.SetItemData( 11, SysInfo.GetCustomPriceLevelFilter() );
		
		m_comboPriceLevel.SetCurSel( 0 );
	}
}

/**********************************************************************/

void CPosTrayTaskEditHelpers::HandleButtonTaxBand( CWnd* pParent ) 
{
	CSalesHistoryTaxDlg dlg ( pParent );
	
	if ( dlg.DoModal() == IDOK )
	{
		UpdateCustomTaxFilter();
		m_comboTaxBand.SetCurSel( m_comboTaxBand.GetCount() - 1 );
	}
}

/**********************************************************************/

void CPosTrayTaskEditHelpers::UpdateCustomTaxFilter() 
{
	CString strItem = ",";
	strItem += SysInfo.GetCustomTaxFilterString();

	int nPos = m_comboTaxBand.GetCount() - 1;
	m_comboTaxBand.DeleteString( nPos );
	m_comboTaxBand.AddItem ( strItem );
	m_comboTaxBand.SetItemData( nPos, SysInfo.GetCustomTaxFilter() );
}

/**********************************************************************/

void CPosTrayTaskEditHelpers::HandleButtonModifier( CWnd* pParent ) 
{
	CSalesHistoryModifierDlg dlg ( pParent );

	if ( dlg.DoModal() == IDOK )
	{
		UpdateCustomModifierFilter();
		m_comboModifier.SetCurSel( m_comboModifier.GetCount() - 1 );
	}
}

/**********************************************************************/

void CPosTrayTaskEditHelpers::UpdateCustomModifierFilter()
{	
	CString strItem = ",";
	strItem += SysInfo.GetCustomModifierFilterString();

	int nPos = m_comboModifier.GetCount() - 1;
	m_comboModifier.DeleteString( nPos );
	m_comboModifier.AddItem ( strItem );
	m_comboModifier.SetItemData( nPos, SysInfo.GetCustomModifierFilter() );
}

/**********************************************************************/

void CPosTrayTaskEditHelpers::HandleButtonPriceLevel( CWnd* pParent ) 
{
	CSalesHistoryPriceLevelDlg dlg ( pParent );
	
	if ( dlg.DoModal() == IDOK )
	{
		UpdateCustomPriceLevelFilter();
		m_comboPriceLevel.SetCurSel( m_comboPriceLevel.GetCount() - 1 );
	}
}

/**********************************************************************/

void CPosTrayTaskEditHelpers::UpdateCustomPriceLevelFilter() 
{
	CString strItem = ",";
	strItem += SysInfo.GetCustomPriceLevelFilterString();

	int nPos = m_comboPriceLevel.GetCount() - 1;
	m_comboPriceLevel.DeleteString( nPos );
	m_comboPriceLevel.AddItem ( strItem );
	m_comboPriceLevel.SetItemData( nPos, SysInfo.GetCustomPriceLevelFilter() );
}

/**********************************************************************/

void CPosTrayTaskEditHelpers::LoadTaskSettings( CPosTrayTask& BatchEmail ) 
{
	//MODIFIER LEVEL
	{
		int nCustom = BatchEmail.GetBatchModifierCustom();

		if ( ( nCustom < 0 ) || ( nCustom > 0x3FF ) )
			nCustom = 0x3FF;

		SysInfo.SetCustomModifierFilter( nCustom );
		UpdateCustomModifierFilter();
	}

	//MODIFIER FILTER TYPE
	{
		int nFilter = BatchEmail.GetBatchModifierFilter();

		if ( ( nFilter >= 0 ) && ( nFilter <= 10 ) )
			m_comboModifier.SetCurSel( nFilter );
		else if ( 999 == nFilter )
			m_comboModifier.SetCurSel( 11 );
		else
			m_comboModifier.SetCurSel( 0 );
	}

	//CUSTOM TAX FILTER
	{
		int nCustom = BatchEmail.GetBatchTaxBandCustom();

		if ( ( nCustom < 0 ) || ( nCustom > TAXBAND_FILTER_ALL ) )
			nCustom = TAXBAND_FILTER_ALL;

		SysInfo.SetCustomTaxFilter( nCustom );
		UpdateCustomTaxFilter();
	}

	//TAX FILTER TYPE
	{
		int nFilter = BatchEmail.GetBatchTaxBandFilter();

		if ( ( nFilter >= 0 ) && ( nFilter <= MAX_TAX_BANDS ) )
			m_comboTaxBand.SetCurSel( nFilter );
		else if ( 999 == nFilter )
			m_comboTaxBand.SetCurSel( MAX_TAX_BANDS + 1 );
		else
			m_comboTaxBand.SetCurSel( 0 );
	}

	//CUSTOM PRICE LEVEL
	{
		int nCustom = BatchEmail.GetBatchPriceLevelCustom();

		if ( ( nCustom < 0 ) || ( nCustom > 0x3FF ) )
			nCustom = 0x3FF;

		SysInfo.SetCustomPriceLevelFilter( nCustom );
		UpdateCustomPriceLevelFilter();
	}

	//PRICE LEVEL FILTER TYPE
	{
		int nFilter = BatchEmail.GetBatchPriceLevelFilter();

		if ( ( nFilter >= 0 ) && ( nFilter <= 10 ) )
			m_comboPriceLevel.SetCurSel( nFilter );
		else if ( 999 == nFilter )
			m_comboPriceLevel.SetCurSel( 11 );
		else
			m_comboPriceLevel.SetCurSel( 0 );
	}
}

/**********************************************************************/

void CPosTrayTaskEditHelpers::SaveTaskSettings( CPosTrayTask& BatchEmail ) 
{
	//CUSTOM MODIFIER FILTER
	BatchEmail.SetBatchModifierCustom( SysInfo.GetCustomModifierFilter() );

	//MODIFIER FILTER TYPE
	{
		int nSel = m_comboModifier.GetCurSel();

		if ( ( nSel >= 0 ) && ( nSel <= 10 ) )
			BatchEmail.SetBatchModifierFilter( nSel );
		else if ( 11 == nSel )
			BatchEmail.SetBatchModifierFilter( 999 );
		else
			BatchEmail.SetBatchModifierFilter( 0 );
	}

	//CUSTOM TAX FILTER
	BatchEmail.SetBatchTaxBandCustom( SysInfo.GetCustomTaxFilter() );

	//TAX FILTER TYPE
	{
		int nSel = m_comboTaxBand.GetCurSel();

		if ( ( nSel >= 0 ) && ( nSel <= MAX_TAX_BANDS ) )
			BatchEmail.SetBatchTaxBandFilter( nSel );
		else if ( MAX_TAX_BANDS + 1 == nSel )
			BatchEmail.SetBatchTaxBandFilter( 999 );
		else
			BatchEmail.SetBatchTaxBandFilter( 0 );
	}

	//CUSTOM PRICE BAND FILTER
	BatchEmail.SetBatchPriceLevelCustom( SysInfo.GetCustomPriceLevelFilter() );

	//PRICE BAND FILTER TYPE
	{
		int nSel = m_comboPriceLevel.GetCurSel();

		if ( ( nSel >= 0 ) && ( nSel <= 10 ) )
			BatchEmail.SetBatchPriceLevelFilter( nSel );
		else if ( 11 == nSel )
			BatchEmail.SetBatchPriceLevelFilter( 999 );
		else
			BatchEmail.SetBatchPriceLevelFilter( 0 );
	}
}

/**********************************************************************/
