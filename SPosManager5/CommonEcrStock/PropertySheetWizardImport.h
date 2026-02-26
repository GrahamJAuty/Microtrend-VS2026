#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SQLImporter.h"
/**********************************************************************/
#include "PropPageWizardImport.h"
#include "WizardImportInfo.h"
/**********************************************************************/

class CPropertySheetWizardImport : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetWizardImport)

public:
	CPropertySheetWizardImport( CSQLImporter& SQLImporter, CWnd* pWndParent = NULL);
	virtual ~CPropertySheetWizardImport();
		
public:
	//{{AFX_VIRTUAL(CPropertySheetWizardImport)
	protected:
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetWizardImport)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	bool GetImportedDataFlag(){ return m_bImportedData; }
	void ImportWizardData();

public:
	CPropPageWizardImport m_Page1;
	CPropPageWizardImport m_Page2;
	CPropPageWizardImport m_Page3;
	CPropPageWizardImport m_Page4;

private:
	CSQLImporter& m_SQLImporter;
	CWizardImportInfo m_infoImport;

private:
	bool m_bImportedData;
};

/**********************************************************************/
#endif
/**********************************************************************/
