#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "SharedResources.h"
/**********************************************************************/
#include "DataAccessHelpersSet.h" 
#include "DataPageFilter.h"
#include "KeyboardMap.h"
#include "KeyboardNameArray.h"
#include "LocationSelectorEntity.h"
#include "SelectMultipleLocationDlg.h"
#include "TreeNode.h"
/**********************************************************************/

class CPropPageSystemDataList : public CSSPropertyPage
{
public:
	CPropPageSystemDataList();
	void SetHelpers( CDataAccessHelpersSet* pHelpers ){ m_pHelpers = pHelpers; }
	
	//{{AFX_DATA(CPropPageSystemDataList)
	enum { IDD = IDD_PROPPAGE_DATA_LIST };
	CButton	m_buttonAction1;
	CButton	m_buttonAction2;
	CButton	m_buttonAction3;
	CButton	m_buttonAction4;
	CButton	m_buttonAction5;
	CButton	m_buttonAction6;
	//}}AFX_DATA
	CSSListCtrl m_listLocation;
	CSSListCtrl m_listAction;

	//{{AFX_VIRTUAL(CPropPageSystemDataList)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CPropPageSystemDataList)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAction1();
	afx_msg void OnButtonAction2();
	afx_msg void OnButtonAction3();
	afx_msg void OnButtonAction4();
	afx_msg void OnButtonAction5();
	afx_msg void OnButtonAction6();
	//}}AFX_MSG
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnSelectLocation(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectAction(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClickAction(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

public:
	void SetPropSheetPointer( void* p ){ m_pPropSheet = p; }
	void SetDataPageType ( int nType );
	bool IsRelevant( int nAccessType );
	void BuildList();
	void SelectLocation();
	void SelectAction();
	void ForceSelectAction( int nAction );
	void ForceSelectLocation( CLocationSelectorEntity& LocSelEntity );

public:
	bool GotLocalData(){ return m_Filter.GotLocalData( m_nDataPageType ); }
	bool GotGlobalData(){ return m_Filter.GotGlobalData( m_nDataPageType ); }
	bool GotSystemFilterData(){ return m_Filter.GotSystemFilterData( m_nDataPageType ); }
	bool GotTimeSettingData(){ return m_Filter.GotTimeSettingData( m_nDataPageType ); }
	bool GotReportListData(){ return m_Filter.GotReportListData( m_nDataPageType ); }
	bool GotStockpointData(){ return m_Filter.GotStockpointData( m_nDataPageType ); }
	bool CheckCloneDatabase();

public:
	void SelectLine ( CSSListCtrl& list, int nIndex );
	void AddGlobal();
	void AddSystemFilter();
	void AddTimeSetting();
	void AddReportList();
	void AddDatabase( int nDbIdx );
	void AddLocation( int nLocIdx, bool bIndent = TRUE );
	void AddKeyboardMapLoc( int nMapIdx, int nLocIdx, bool bIndent );
	void AddKeyboardMapSet( int nMapIdx, int nKbsIdx, bool bIndent );
	void AddStockpoint( int nSpIdx );
	void AddAction( int nNodeType, const char* szText );
	
public:
	//ECR DATA PAGE
	void PrepareEcrDataPage();
	void BuildEcrDataList();
	void SelectLocationEcrData();
	void SelectActionEcrData();
	void GetEcrDataAction( int& nDbNo, int& nAction, int& nConType );
	void Button1EcrDataPage();
	void Button2EcrDataPage();
	void Button4EcrDataPage();
	void Button5EcrDataPage();

	//PRICE SET PAGE
	void PreparePriceSetPage();
	void BuildPriceSetList();
	void BuildPriceSetListDatabase( int nDbIdx );
	void SelectLocationPriceSet();
	void SelectActionPriceSet();
	void GetPriceSetAction( CLocationSelectorEntity& LocSelEntity, int& nAction );
	void Button1PriceSetPage();
	void Button2PriceSetPage();
	void Button3PriceSetPage();
	void Button4PriceSetPage();
	void Button5PriceSetPage();

	//MIX MATCH PAGE
	void PrepareMixMatchPage();
	void BuildMixMatchList();
	void SelectLocationMixMatch();
	void GetMixMatchAction( int& nDbNo, int& nAction );
	void Button1MixMatchPage();
	void Button2MixMatchPage();

	//KEYBOARD PAGE
	void PrepareKeyboardPage();
	void BuildKeyboardList();
	void SelectLocationKeyboard();
	void SelectActionKeyboard();
	void GetKeyboardAction( CLocationSelectorEntity& LocSelEntity, int& nAction );
	void Button1KeyboardPage();
	void Button2KeyboardPage();
	void Button3KeyboardPage();
	void Button5KeyboardPage();
	void Button6KeyboardPage();

	//CUSTOMER PAGE
	void PrepareCustomerPage();
	void BuildCustomerList();
	void BuildCustomerListDatabase( int nDbIdx );
	void SelectLocationCustomer();
	void SelectActionCustomer();
	void GetCustomerAction( CLocationSelectorEntity& LocSelEntity, int& nAction );
	void Button1CustomerPage();

	//SERVER PAGE
	void PrepareServerPage();
	void BuildServerList();
	void BuildServerListDatabase( int nDbIdx );
	void SelectLocationServer();
	void SelectActionServer();
	void GetServerAction( CLocationSelectorEntity& LocSelEntity, int& nAction );
	void Button1ServerPage();

	//SERVER PRESET PAGE
	void PrepareServerPresetPage();
	void BuildServerPresetList();
	void RebuildServerPresetList();
	void SelectLocationServerPreset();
	void SelectActionServerPreset();
	void GetServerPresetAction( CLocationSelectorEntity& LocSelEntity, int& nAction );
	void Button1ServerPresetPage();
	void Button2ServerPresetPage();
	void Button3ServerPresetPage();
	void EditServerPresets( CLocationSelectorEntity& LocSelEdit );

	//ECR TEXT PAGE
	void PrepareEcrTextPage();
	void BuildEcrTextList();
	void SelectLocationEcrText();
	void SelectActionEcrText();
	void GetEcrTextAction( int& nDbNo, int& nAction );
	void Button1EcrTextPage();
	void Button2EcrTextPage();

	//REPORT SETS PAGE
	void PrepareReportSetPage();
	void BuildReportSetList();
	void SelectLocationReportSet();
	void SelectActionReportSet();
	void GetReportSetAction( int& nDbNo, int& nAction );
	void Button1ReportSetPage();
	void Button2ReportSetPage();

	//BILL TEXT PAGE
	void PrepareBillTextPage();
	void BuildBillTextList();
	void RebuildBillTextList();
	void SelectLocationBillText();
	void SelectActionBillText();
	void GetBillTextAction( CLocationSelectorEntity& LocSelEntity, int& nAction );
	void Button1BillTextPage();
	void Button2BillTextPage();
	void Button3BillTextPage();
	void EditBillReceiptTexts( CLocationSelectorEntity& LocSelEdit );

	//FUNCTION PAGE
	void PrepareFunctionPage();
	void BuildFunctionList();
	void RebuildFunctionList();
	void SelectLocationFunction();
	void SelectActionFunction();
	void GetFunctionAction( CLocationSelectorEntity& LocSelEntity, int& nAction );
	void Button1FunctionPage();
	void Button3FunctionPage();

	//OP GROUP PAGE
	void PrepareOPGroupPage();
	void BuildOPGroupList();
	void RebuildOPGroupList();
	void SelectLocationOPGroup();
	void SelectActionOPGroup();
	void GetOPGroupAction( CLocationSelectorEntity& LocSelEntity, int& nAction );
	void Button1OPGroupPage();
	void Button2OPGroupPage();
	void EditOPGroup();
	void EditOPName();

	//MACRO PAGE
	void PrepareMacroPage();
	void BuildMacroList();
	void RebuildMacroList();
	void SelectLocationMacro();
	void SelectActionMacro();
	void GetMacroAction( CLocationSelectorEntity& LocSelEntity, int& nAction );
	void Button1MacroPage();
	void Button3MacroPage();

	//REASON PAGE
	void PrepareReasonPage();
	void BuildReasonList();
	void RebuildReasonList();
	void SelectLocationReason();
	void SelectActionReason();
	void GetReasonAction(CLocationSelectorEntity& LocSelEntity, int& nAction);
	void Button1ReasonPage();
	void Button3ReasonPage();

	//TABLES PAGE
	void PrepareTablesPage();
	void BuildTablesList();
	void RebuildTablesList();
	void SelectLocationTables();
	void SelectActionTables();
	void GetTablesAction( CLocationSelectorEntity& LocSelEntity, int& nAction );
	void Button1TablesPage();
	
#ifdef STOCKMAN_SYSTEM
	//STOCK DATA PAGE
	void PrepareStockDataPage();
	void BuildStockDataList();
	void BuildStockDataListDatabase( int nDbIdx );
	void SelectLocationStockData();
	void GetStockDataAction( CLocationSelectorEntity& LocSelEntity, int& nAction );
	void Button1StockDataPage();
	void Button2StockDataPage();
	void Button4StockDataPage();
	void Button5StockDataPage();

	//STOCK TEXT PAGE
	void PrepareStockTextPage();
	void BuildStockTextList();
	void SelectLocationStockText();
	void SelectActionStockText();
	void GetStockTextAction( int& nDbNo, int& nAction, int& nConType );
	void Button1StockTextPage();
	void Button2StockTextPage();
#endif
	
public:
	int GetDbIdx( int nDbNo );
	void CloneDatabase ( int nDbNo );
	bool CheckDatabaseValidationToken();

private:
	void* m_pPropSheet;

private:
	int m_nDataPageType;
	CDataPageFilter m_Filter;
	CArray<CLocationSelectorEntity,CLocationSelectorEntity> m_arrayLocation;
	CArray<int,int> m_arrayAction;
	bool m_bCanCloneDatabase;

private:
	CDataAccessHelpersSet* m_pHelpers;
};

/**********************************************************************/
#endif
/**********************************************************************/
