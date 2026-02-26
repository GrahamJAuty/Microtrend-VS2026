#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "..\CommonEcrStock\CascadingMacroCSVArray.h"
#include "..\CommonEcrStock\CascadingReasonCSVArray.h"
/**********************************************************************/
#include "CustomerStatementHeaderLines.h"
#include "DefTextUnit.h"
#include "FunctionAuthArray.h"
#include "GroupCSVArrayAllergen.h"
#include "LoyaltySchemeCSVArray.h"
#include "OPGroupArray.h"
#include "OPNameArray.h"
#include "SalesHistoryCSVArrayCustomer.h"
#include "ServerPresetArray.h"
#include "ServerRestrictionCSVArray.h"
#include "TableAreaCSVArray.h"
/**********************************************************************/

class CDataManagerNonDbFuncMain : virtual public CDataManagerBase
{
public:
	CDataManagerNonDbFuncMain();

public:
	void CloseFilesMain( CWordArray& arrayFiles );

public:
	bool OpenAllergen( int nType, CDataManagerInfo& info );
	bool OpenCCMacroDatabase( int nDbIdx, int nType, CDataManagerInfo& info );
	bool OpenCCMacroKeyboard( int nType, CDataManagerInfo& info );
	bool OpenCCMacroLocation( int nLocIdx, int nType, CDataManagerInfo& info );
	bool OpenCCMacroSystem( int nType, CDataManagerInfo& info );
	bool OpenCCReasonDatabase(int nDbIdx, int nAccessType, int nReasonType, CDataManagerInfo& info);
	bool OpenCCReasonLocation(int nLocIdx, int nAccessType, int nReasonType, CDataManagerInfo& info);
	bool OpenCCReasonSystem(int nAccessType, int nReasonType, CDataManagerInfo& info);
	bool OpenCustomerHeader( int nType, CDataManagerInfo& info );
	bool OpenFunctionDatabase( int nDbIdx, int nType, CDataManagerInfo& info );
	bool OpenFunctionLocation( int nLocIdx, int nType, CDataManagerInfo& info );
	bool OpenFunctionSystem( int nType, CDataManagerInfo& info );
	bool OpenOPGroupLocation( int nLocIdx, int nType, CDataManagerInfo& info );
	bool OpenOPGroupDatabase( int nDbIdx, int nType, CDataManagerInfo& info );
	bool OpenOPGroupSystem( int nType, CDataManagerInfo& info );
	bool OpenOPNameLocation(int nLocIdx, int nType, CDataManagerInfo& info);
	bool OpenOPNameDatabase(int nDbIdx, int nType, CDataManagerInfo& info);
	bool OpenOPNameSystem(int nType, CDataManagerInfo& info);
	bool OpenPluUnit( int nType, CDataManagerInfo& info );
	bool OpenSalesHistoryCustomer( int nType, CDataManagerInfo& info );
	bool OpenServerPresetDatabase( int nDbIdx, int nType, CDataManagerInfo& info );
	bool OpenServerPresetLocation( int nLocIdx, int nType, CDataManagerInfo& info );
	bool OpenServerPresetSystem( int nType, CDataManagerInfo& info );
	bool OpenServerPresetTerminal( int nLocIdx, int nTNo, int nType, CDataManagerInfo& info );
	bool OpenServerRestriction( int nType, int nLocIdx, CDataManagerInfo& info );
	bool OpenTableAreas( int nType, int nLocIdx, CDataManagerInfo& info );
	
public:
	bool OpenPluUnitBuffer( CDataManagerInfo& info );
	bool OpenSalesHistoryCustomerBuffer( CDataManagerInfo& info );

public:
	bool CloseAllergen( CDataManagerInfo& info );
	bool CloseCustomerHeader( CDataManagerInfo& info );
	bool CloseCCMacroDatabase( CDataManagerInfo& info );
	bool CloseCCMacroKeyboard( CDataManagerInfo& info );
	bool CloseCCMacroLocation( CDataManagerInfo& info );
	bool CloseCCMacroSystem( CDataManagerInfo& info );
	bool CloseCCReasonDatabase(CDataManagerInfo& info);
	bool CloseCCReasonLocation(CDataManagerInfo& info);
	bool CloseCCReasonSystem(CDataManagerInfo& info);
	bool CloseFunctionDatabase( CDataManagerInfo& info );
	bool CloseFunctionLocation( CDataManagerInfo& info );
	bool CloseFunctionSystem( CDataManagerInfo& info );
	bool CloseOPGroupLocation( CDataManagerInfo& info );
	bool CloseOPGroupSystem( CDataManagerInfo& info );
	bool CloseOPGroupDatabase( CDataManagerInfo& info );
	bool CloseOPNameLocation(CDataManagerInfo& info);
	bool CloseOPNameSystem(CDataManagerInfo& info);
	bool CloseOPNameDatabase(CDataManagerInfo& info);
	bool ClosePluUnit( CDataManagerInfo& info );
	bool CloseSalesHistoryCustomer( CDataManagerInfo& info );
	bool CloseServerPresetDatabase( CDataManagerInfo& info );
	bool CloseServerPresetLocation( CDataManagerInfo& info );
	bool CloseServerPresetSystem( CDataManagerInfo& info );
	bool CloseServerPresetTerminal( CDataManagerInfo& info );
	bool CloseServerRestriction( CDataManagerInfo& info );
	bool CloseTableAreas( CDataManagerInfo& info );

public:
	bool ClosePluUnitBuffer( CDataManagerInfo& info );
	bool CloseSalesHistoryCustomerBuffer( CDataManagerInfo& info );
	
public:
	bool WriteAllergen( CDataManagerInfo& info );
	bool WriteCustomerHeader( CDataManagerInfo& info );
	bool WriteCCMacroDatabase( CDataManagerInfo& info );
	bool WriteCCMacroKeyboard( CDataManagerInfo& info );
	bool WriteCCMacroLocation( CDataManagerInfo& info );
	bool WriteCCMacroSystem( CDataManagerInfo& info );
	bool WriteCCReasonDatabase(CDataManagerInfo& info);
	bool WriteCCReasonLocation(CDataManagerInfo& info);
	bool WriteCCReasonSystem(CDataManagerInfo& info);
	bool WriteFunctionDatabase( CDataManagerInfo& info );
	bool WriteFunctionLocation( CDataManagerInfo& info );
	bool WriteFunctionSystem( CDataManagerInfo& info );
	bool WriteOPGroupDatabase( CDataManagerInfo& info );
	bool WriteOPGroupLocation( CDataManagerInfo& info );
	bool WriteOPGroupSystem( CDataManagerInfo& info );
	bool WriteOPNameDatabase(CDataManagerInfo& info);
	bool WriteOPNameLocation(CDataManagerInfo& info);
	bool WriteOPNameSystem(CDataManagerInfo& info);
	bool WritePluUnit( CDataManagerInfo& info );
	bool WriteSalesHistoryCustomer( CDataManagerInfo& info );
	bool WriteServerPresetDatabase( CDataManagerInfo& info );
	bool WriteServerPresetLocation( CDataManagerInfo& info );
	bool WriteServerPresetSystem( CDataManagerInfo& info );
	bool WriteServerPresetTerminal( CDataManagerInfo& info );
	bool WriteServerRestriction( CDataManagerInfo& info );
	bool WriteTableAreas( CDataManagerInfo& info );
		
public:
	bool WritePluUnitBuffer( CDataManagerInfo& info );
	bool WriteSalesHistoryCustomerBuffer( CDataManagerInfo& info );
	
protected:
	const char* GetFilePathServerRestriction( int nLocIdx );
	const char* GetFilePathTableArea( int nLocIdx );

public:
	void FixTaxFunction(){ m_FunctionAuthTable.FixTaxFunction(); }
	void SetKeyboardMacroFilename( const char* sz ){ m_strKeyboardMacroFilename = sz; }
	
private:
	void GetSysInfoReasonText(int nReasonType, SysFileInfo& SysInfo);

private:
	CFunctionAuthTable m_FunctionAuthTable;
	CServerPresetTable m_ServerPresetTable;
	COPGroupTable m_OPGroupTable;
	COPNameTable m_OPNameTable;
	
public:
	CGroupCSVArrayAllergen Allergen;
	CCascadingMacroCSVArray CCMacroDatabase;
	CCascadingMacroCSVArray CCMacroKeyboard;
	CCascadingMacroCSVArray CCMacroLocation;
	CCascadingMacroCSVArray CCMacroSystem;
	CCascadingMacroCSVArray CCMacroActive;
	CCascadingReasonCSVArray CCReasonDatabase;
	CCascadingReasonCSVArray CCReasonLocation;
	CCascadingReasonCSVArray CCReasonSystem;
	CCustomerStatementHeaderLines CustomerHeader;
	CFunctionAuthArray FunctionDatabase;
	CFunctionAuthArray FunctionLocation;
	CFunctionAuthArray FunctionSystem;
	COPGroupArray OPGroupDatabase;
	COPGroupArray OPGroupLocation;
	COPGroupArray OPGroupSystem;
	COPNameArray OPNameDatabase;
	COPNameArray OPNameLocation;
	COPNameArray OPNameSystem;
	CDefTextUnit PluUnit;
	CSalesHistoryCSVArrayCustomer SalesHistoryCustomer;
	CServerPresetArray ServerPresetDatabase;
	CServerPresetArray ServerPresetLocation;
	CServerPresetArray ServerPresetSystem;
	CServerPresetArray ServerPresetTerminal;
	CServerRestrictionCSVArray ServerRestriction;
	CTableAreaCSVArray TableAreas;
	
public:
	CDefTextUnit PluUnitBuffer;
	CSalesHistoryCSVArrayCustomer SalesHistoryCustomerBuffer;
	
protected:
	int m_nStatusAllergen;
	int m_nStatusCCMacroDatabase;
	int m_nStatusCCMacroKeyboard;
	int m_nStatusCCMacroLocation;
	int m_nStatusCCMacroSystem;
	int m_nStatusCCReasonDatabase;
	int m_nStatusCCReasonLocation;
	int m_nStatusCCReasonSystem;
	int m_nStatusCustomerHeader;
	int m_nStatusFunctionDatabase;
	int m_nStatusFunctionLocation;
	int m_nStatusFunctionSystem;
	int m_nStatusOPGroupDatabase;
	int m_nStatusOPGroupLocation;
	int m_nStatusOPGroupSystem;
	int m_nStatusOPNameDatabase;
	int m_nStatusOPNameLocation;
	int m_nStatusOPNameSystem;
	int m_nStatusPluUnit;
	int m_nStatusSalesHistoryCustomer;
	int m_nStatusServerPresetDatabase;
	int m_nStatusServerPresetLocation;
	int m_nStatusServerPresetSystem;
	int m_nStatusServerPresetTerminal;
	int m_nStatusServerRestriction;
	int m_nStatusTableAreas;

protected:
	int m_nStatusPluUnitBuffer;
	int m_nStatusSalesHistoryCustomerBuffer;
	
private:
	CString m_strFilePathMain;
	CString m_strKeyboardMacroFilename;
};

/**********************************************************************/
#endif
/**********************************************************************/
