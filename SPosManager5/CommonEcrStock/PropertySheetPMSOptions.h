#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/
#include "PMSEmailSignatureBuffer.h"
#include "PropPagePMSOptionsRooms.h"
#include "PropPagePMSOptionsAddress.h"
#include "PropPagePMSOptionsColour.h"
#include "PropPagePMSOptionsQuickPlu.h"
#include "PropPagePMSOptionsPayment.h"
#include "PropPagePMSOptionsDinner.h"
#include "PropPagePMSOptionsInvoice.h"
#include "PropPagePMSOptionsReports.h"
#include "PropPagePMSOptionsFolders.h"
#include "PropPagePMSOptionsMisc.h"
/**********************************************************************/

class CPropertySheetPMSOptions : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetPMSOptions)

public:
	CPropertySheetPMSOptions( bool bCanWriteDatabase, CPMSEmailSignatures& EmailSignatures, CWnd* pWndParent = NULL);
	virtual ~CPropertySheetPMSOptions();

public:
	bool HasChangedDatabase(){ return m_page8.GetHasChangedDatabaseFlag(); }
		
public:
	//{{AFX_VIRTUAL(CPropertySheetPMSOptions)
	protected:
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetPMSOptions)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CPMSEmailSignatures& m_EmailSignatures;
	CPropPagePMSOptionsRooms m_page1;
	CPropPagePMSOptionsColour m_page2;
	CPropPagePMSOptionsAddress m_page3;
	CPropPagePMSOptionsQuickPlu m_page4;
	CPropPagePMSOptionsPayment m_page5;
	CPropPagePMSOptionsDinner m_page6;
	CPropPagePMSOptionsInvoice m_page7;
	CPropPagePMSOptionsReports m_page8;
	CPropPagePMSOptionsFolders m_page9;
	CPropPagePMSOptionsMisc m_page10;
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
