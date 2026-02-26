#pragma once
/**********************************************************************/
#include "..\CommonEcrStockTray\EcrmanSupplierCSVArray.h"
/**********************************************************************/

class CDataManagerNonDbFuncEcrman : virtual public CDataManagerBase
{
public:
	CDataManagerNonDbFuncEcrman();

public:
	bool OpenEcrmanSupplier( int nType, CDataManagerInfo& info );

public:
	bool CloseEcrmanSupplier( CDataManagerInfo& info );	
	
public:
	bool WriteEcrmanSupplier( CDataManagerInfo& info );	
	
public:
	bool OpenEcrmanSupplierBuffer( CDataManagerInfo& info );	
	
public:
	bool CloseEcrmanSupplierBuffer( CDataManagerInfo& info );
	
public:
	bool WriteEcrmanSupplierBuffer( CDataManagerInfo& info );
	
public:
	CEcrmanSupplierCSVArray EcrmanSupplier;

public:
	CEcrmanSupplierCSVArray EcrmanSupplierBuffer;

protected:
	int m_nStatusEcrmanSupplier;

protected:
	int m_nStatusEcrmanSupplierBuffer;

};

/**********************************************************************/
