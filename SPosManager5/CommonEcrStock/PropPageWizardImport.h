#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStock\LocationSelector.h"
/**********************************************************************/
#include "SQLImporter.h"
#include "WizardImportInfo.h"
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#define DTW_TICKSPERPAGE 17
/**********************************************************************/

class CPropPageWizardImport : public CSSPropertyPage
{
public:
	CPropPageWizardImport();
	~CPropPageWizardImport();

	//{{AFX_DATA(CPropPageWizardImport)
	enum { IDD = IDD_PROPPAGE_WIZARD_IMPORT };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPropPageWizardImport)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL
	virtual void OnCheckFileType1();
	virtual void OnCheckFileType2();
	virtual void OnCheckFileType3();
	virtual void OnCheckFileType4();
	virtual void OnCheckFileType5();
	virtual void OnCheckFileType6();
	virtual void OnCheckFileType7();
	virtual void OnCheckFileType8();
	virtual void OnCheckFileType9();
	virtual void OnCheckFileType10();
	virtual void OnCheckFileType11();
	virtual void OnCheckFileType12();
	virtual void OnCheckFileType13();
	virtual void OnCheckFileType14();
	virtual void OnCheckFileType15();
	virtual void OnCheckFileType16();
	virtual void OnCheckFileType17();
	virtual void OnButtonAll();
	virtual void OnButtonNone();

public:
	void SetLocationType( int n );
	void SetSQLImporter( CSQLImporter* p ){ m_pSQLImporter = p; }
	void ToggleFileType( int n );

	bool IsRelevant();

public:
	int GetDbIdx(){ return m_nDbIdx; }
	int GetLocIdx(){ return m_nLocIdx; }
	void GetFileTypeTicks( CWizardImportInfo& info );

protected:
	//{{AFX_MSG(CPropPageWizardImport)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	void SaveRecord();
	void GetRecordData();
	void SetRecordControls();

private:
	CSSComboBoxEx m_comboDatabase;
	CSSTabbedComboBox m_comboServer;
	CStatic m_staticDestination;
	CStatic m_staticTypes;

private:
	CLocationSelector m_LocationSelector;
	CSQLImporter* m_pSQLImporter;

private:
	int m_nLocationType;
	
	int m_nFileTypeID[DTW_TICKSPERPAGE];
	int m_nFileType[DTW_TICKSPERPAGE];

	CButton m_checkFileType[DTW_TICKSPERPAGE];
	CButton m_checkStock;
	CButton m_checkClear;

	int m_nDbIdx;
	int m_nLocIdx;
	bool m_bCreateStock;
	bool m_bClearExisting;
	bool m_bSpecialServerMode;
	CWordArray m_arrayFileTypes;	
};

/**********************************************************************/
#endif
/**********************************************************************/
