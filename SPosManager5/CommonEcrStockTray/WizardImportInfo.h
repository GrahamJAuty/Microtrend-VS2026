#pragma once
/**********************************************************************/
#define WIZARD_IMPORT_NONE 0
#define WIZARD_IMPORT_MERGE 1
#define WIZARD_IMPORT_CLEAR 2
/**********************************************************************/

struct CWizardImportInfo
{
public:
	CWizardImportInfo();

public:
	int m_nBarcode;
	int m_nDept;
	int m_nCategory;
	int m_nPaidInOutText;
	int m_nGroup;
	int m_nAllergen;
	int m_nMixMatch2;
	int m_nWalletStampRule;
	int m_nTimePlan;
	int m_nModifier;
	int m_nPayment;
	int m_nServer;
	int m_nServerLogon;
	int m_nServerRestrict;
	int m_nServerPreset;
	int m_nPlu;
	int m_nStock;
	int m_nTaxRate;
	int m_nLoyaltyScheme;
	int m_nTableArea;
	int m_nTableName;
	int m_nFunctionSystem;
	int m_nFunctionDatabase;
	int m_nFunctionLocation;
	int m_nCCMacroSystem;
	int m_nCCMacroDatabase;
	int m_nCCMacroLocation;
	int m_nCCReasonSystem;
	int m_nCCReasonDatabase;
	int m_nCCReasonLocation;
};

/**********************************************************************/

