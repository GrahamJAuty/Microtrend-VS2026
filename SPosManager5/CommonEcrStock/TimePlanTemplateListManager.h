#pragma once
/**********************************************************************/
#include "Consolidation.h"
#include "ComboListCtrl.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

class CTimePlanTemplateListManager
{
public:
	CTimePlanTemplateListManager(CComboListCtrl& list, CWnd* pParent);
	
public:
	void InitList();
	void CreateTemplateMap(CPromotionsCSVRecord* pPromo);
	void CreateTemplateMap(CMixMatchCSVRecord* pMixMatch);
	void AddList();
	CString GetTimePlanTemplateList();

public:
	void HandleGetCellType(WPARAM wParam, LPARAM lParam);
	void HandlePopulateComboList(WPARAM wParam, LPARAM lParam);
	void HandleEndLabelEdit(WPARAM wParam, LPARAM lParam);
	void HandleCellButton(WPARAM wParam, LPARAM lParam);	
	
private:
	void CreateNewTimeplanTemplateEntry(int nPlanNo);
	void EditTimeplanTemplateEntry(bool bNew, int nPlanNo, int nPos);

public:
	static void GetTimeplanReportDetails(int nType, int& nLineNo, CReportConsolidationArray<CSortedTimeplanEntry>& arrayTimeplanEntries);
	static void GetTimeplanReportDetails(int& nLineNo, CPromotionsCSVRecord& Promo, CReportConsolidationArray<CSortedTimeplanEntry>& arrayTimeplanEntries);
	static void GetTimeplanReportDetails(int& nLineNo, CMixMatchCSVRecord& Promo, CReportConsolidationArray<CSortedTimeplanEntry>& arrayTimeplanEntries);
	static void GetReportDetailsEntryRange(int nPromoNo, CReportConsolidationArray<CSortedTimeplanEntry>& arrayTimeplanEntries, int& nEntryStartIdx, int& nEntryEndIdx);
	static void ReportExtraTimeplans(CReportFile& ReportFile, CReportConsolidationArray<CSortedTimeplanEntry>& arrayTimeplanEntries, int nEntryStartIdx, int nEntryEndIdx, int nBlanks);
	static void GetTimeplanEntryDetails(CReportConsolidationArray<CSortedTimeplanEntry>& arrayTimeplanEntries, int nEntryIdx, CString& strPlanName, CString& strEntryName);

private:
	static void GetTimeplanReportDetails(int& nLineNo, int nPromoNo, CReportConsolidationArray<CSortedIntByInt>& TimeplanMap, CReportConsolidationArray<CSortedTimeplanEntry>& arrayTimeplanEntries);

private:
	CReportConsolidationArray<CSortedIntByInt> m_arrayTemplates;
	CArray<CellTypeInfo, CellTypeInfo> m_arrayCellTypes;
	CComboListCtrl& m_List;
	CWnd* m_pParent = nullptr;
	CString m_strTimePlanTemplateList = "";
};

/**********************************************************************/
