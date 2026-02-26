#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DataManagerBase.h"
#include "PresetReportCustHist.h"
#include "..\SPosEcrManager5\PresetReportEcrStock.h"
#include "PresetReportEpos.h"
#include "PresetReportEposBatch.h"
#include "PresetReportPluHist.h"
#include "..\SPosStockManager5\PresetReportStockAudit.h"
#include "..\SPosStockManager5\PresetReportStockHist.h"
#include "..\SPosStockManager5\PresetReportStockLevel.h"
#include "PresetReportTimeAttend.h"
/**********************************************************************/

class CPresetReportArrayWrapper : public CDataManagerBase
{
public:
	CPresetReportArrayWrapper( int nNodeType, CPresetReportArray* pArray );
	~CPresetReportArrayWrapper();

public:
	bool Open( int nType, CDataManagerInfo& info );
	bool Close( CDataManagerInfo& info );
	bool Write( CDataManagerInfo& info );
	
	bool OpenBuffer( CDataManagerInfo& info );
	bool CloseBuffer( CDataManagerInfo& info );
	bool WriteBuffer( CDataManagerInfo& info );
	
	void SetPasswordAccess(){ m_pArray -> SetPasswordAccess(); }

private:
	int m_nNodeType;
	CPresetReportArray* m_pArray;
	CPresetReportArray m_Buffer;
	
private:
	int m_nStatusArray;
	int m_nStatusBuffer;
};

/**********************************************************************/

class CPresetReportManager
{
public:
	CPresetReportManager();
	
public:
	void PresetButton( int nMode, CSSComboBox& combo, CPresetReportInfo& Preset, CWnd* pParentWnd );
	void FillPresetCombo( int nMode, CSSComboBox& combo, int nPresetIdxSel );
	int SelectFromCombo( int nMode, CSSComboBox& combo, int nPresetNo );

public:
	CPresetReportArray* GetPresetArray( int nMode );
	CPresetReportArrayWrapper* GetPresetWrapper( int nMode );
	bool Open( int nMode, int nType, CDataManagerInfo& info );
	bool Close( int nMode, CDataManagerInfo& info );
	bool Write( int nMode, CDataManagerInfo& info );
	bool OpenBuffer( int nMode, CDataManagerInfo& info );
	bool CloseBuffer( int nMode, CDataManagerInfo& info );
	bool WriteBuffer( int nMode, CDataManagerInfo& info );

public:
	CPresetReportArrayEpos Epos;
	CPresetReportArrayEposBatch EposBatch;
	CPresetReportArrayPluHist PluHist;
	CPresetReportArrayCustHist CustHist;
	CPresetReportArrayTimeAttend TimeAttend;

#ifdef STOCKMAN_SYSTEM
	CPresetReportArrayStockAudit StockAudit;
	CPresetReportArrayStockHist StockHist;
	CPresetReportArrayStockLevel StockLevel;
	CPresetReportArrayStockLevel StockClosing;
#else
	CPresetReportArrayEcrStock EcrStock;
#endif
	

private:
	CPresetReportArrayWrapper WrapperEpos;
	CPresetReportArrayWrapper WrapperEposBatch;
	CPresetReportArrayWrapper WrapperPluHist;
	CPresetReportArrayWrapper WrapperCustHist;
	CPresetReportArrayWrapper WrapperTimeAttend;

#ifdef STOCKMAN_SYSTEM
	CPresetReportArrayWrapper WrapperStockAudit;
	CPresetReportArrayWrapper WrapperStockHist;
	CPresetReportArrayWrapper WrapperStockLevel;
	CPresetReportArrayWrapper WrapperStockClosing;
#else
	CPresetReportArrayWrapper WrapperEcrStock;
#endif

};

/**********************************************************************/
extern CPresetReportManager PresetReportManager;
/**********************************************************************/
#endif
/**********************************************************************/
