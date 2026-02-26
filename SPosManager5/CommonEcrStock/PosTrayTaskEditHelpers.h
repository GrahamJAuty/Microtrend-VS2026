#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/

class CPosTrayTaskEditHelpers
{
public:
	CPosTrayTaskEditHelpers( CSSTabbedComboBox& comboPriceLevel, CSSTabbedComboBox& comboTaxBand, CSSTabbedComboBox& comboModifier );
	
public:
	void InitTabbedCombos();
	void FillTabbedCombos( int nDbIdx );

public:
	void LoadTaskSettings( CPosTrayTask& BatchEmail );
	void SaveTaskSettings( CPosTrayTask& BatchEmail );

public:
	void HandleButtonTaxBand( CWnd* pParent );
	void HandleButtonPriceLevel( CWnd* pParent );
	void HandleButtonModifier( CWnd* pParent );

public:
	void UpdateCustomTaxFilter();
	void UpdateCustomPriceLevelFilter();
	void UpdateCustomModifierFilter();

private:
	CSSTabbedComboBox& m_comboTaxBand;
	CSSTabbedComboBox& m_comboPriceLevel;
	CSSTabbedComboBox& m_comboModifier;
};

/**********************************************************************/
#endif
/**********************************************************************/
