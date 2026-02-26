#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "PosTrayTaskArrays.h"
#include "PosTrayTaskInfo.h"
#include "PosTrayFileSet.h"
#include "PropPagePosTrayOptionsGeneral.h"
#include "PropPagePosTrayOptionsTask.h"
#include "PropPagePosTrayOptionsFNB.h"
#include "PropPagePosTrayOptionsS4Labour.h"
#include "PropPagePosTrayOptionsLIPA.h"
#include "PropPagePosTrayOptionsFTP.h"
#include "PropPagePosTrayOptionsChartwells.h"
/**********************************************************************/

class CPropertySheetPosTrayOptions : public CSSPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetPosTrayOptions)

public:
	CPropertySheetPosTrayOptions( CWnd* pWndParent = NULL);
	virtual ~CPropertySheetPosTrayOptions();
		
public:
	//{{AFX_VIRTUAL(CPropertySheetPosTrayOptions)
	protected:
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropertySheetPosTrayOptions)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	bool RunPosTrayAtStartup();
	bool RemovePosTrayAtStartup();
	void BuildTaskIndex();
	void SaveTaskChanges( bool bMergedOnly = FALSE );

public:
	void AddTaskPages();
	
private:
	void GetPosTrayFileSet( int nType );
	void ReadPosTrayBatchFile( int nBatchNo, int nType );
	void ReadPosTrayTaskFile( int nTaskNo, int nType );
	void ReadFNBFile( int nDbIdx );
	void ReadChartwellsFile();
	void ReadS4LabourFile( int nLocIdx );

private:
	void SavePosTrayBatchFile( int nBatchNo, int nType );
	void SaveBatchEposConsolFile( int nBatchNo );
	void SavePosTrayTaskFile( int nTaskNo, int nType );
	void SavePosTrayMergedTaskFile( int nTaskNo, int nType, CSSFile& fileIndex );
	void SaveFNBFile( int nDbIdx );
	void SaveChartwellsFile();
	void SaveS4LabourFile( int nLocIdx );
	void SaveLIPADate();

public:
	CPropPagePosTrayOptionsGeneral m_Page1;
	CPropPagePosTrayOptionsTask m_Page2;
	CPropPagePosTrayOptionsFNB m_Page3;
	CPropPagePosTrayOptionsFTP m_Page4;
	CPropPagePosTrayOptionsS4Labour m_Page5;
	CPropPagePosTrayOptionsLIPA m_Page6;
	CPropPagePosTrayOptionsChartwells m_Page7;

private:
	CPosTrayFileSet m_PosTrayFileSet;
	CPosTrayTaskArrays m_TaskArrays;
	CReportConsolidationArray<CPosTrayTaskInfo> m_arrayTaskInfoProcessTran;
	CReportConsolidationArray<CPosTrayBatchInfo> m_arrayBatchInfoProcessTran;
	CReportConsolidationArray<CPosTrayTaskInfo> m_arrayTaskInfoEpos;
	CReportConsolidationArray<CPosTrayBatchInfo> m_arrayBatchInfoEpos;
	CReportConsolidationArray<CPosTrayTaskInfo> m_arrayTaskInfoHist;
	CReportConsolidationArray<CPosTrayBatchInfo> m_arrayBatchInfoHist;
	CReportConsolidationArray<CPosTrayTaskInfo> m_arrayTaskInfoStkLevel;
	CReportConsolidationArray<CPosTrayBatchInfo> m_arrayBatchInfoStkLevel;
	CReportConsolidationArray<CPosTrayTaskInfo> m_arrayTaskInfoTimeAttend;
	CReportConsolidationArray<CPosTrayBatchInfo> m_arrayBatchInfoTimeAttend;
	CReportConsolidationArray<CPosTrayTaskInfo> m_arrayTaskInfoExportSales;
	CReportConsolidationArray<CPosTrayBatchInfo> m_arrayBatchInfoExportSales;
	CReportConsolidationArray<CPosTrayTaskInfo> m_arrayTaskInfoExportPayment;
	CReportConsolidationArray<CPosTrayBatchInfo> m_arrayBatchInfoExportPayment;
	CReportConsolidationArray<CPosTrayTaskInfo> m_arrayTaskInfoExportVoid;
	CReportConsolidationArray<CPosTrayBatchInfo> m_arrayBatchInfoExportVoid;
	CReportConsolidationArray<CPosTrayTaskInfo> m_arrayTaskInfoExportDiscount;
	CReportConsolidationArray<CPosTrayBatchInfo> m_arrayBatchInfoExportDiscount;
	CReportConsolidationArray<CPosTrayFNBInfo> m_arrayFNBInfo;
	CReportConsolidationArray<CPosTrayChartwellsInfo> m_arrayChartwellsInfo;
	CReportConsolidationArray<CPosTrayS4LabourInfo> m_arrayS4LabourInfo;
};

/**********************************************************************/
#endif
/**********************************************************************/
