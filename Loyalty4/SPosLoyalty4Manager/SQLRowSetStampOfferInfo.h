#pragma once
//**********************************************************************
#include "..\SPOSLoyalty4Shared\SQLTable_StampOfferInfo\SQLRowStampOfferInfo.h"
#include "..\SPOSLoyalty4Shared\SQLTable_StampOfferInfo\SQLFieldValidatorStampOfferInfo.h"
//**********************************************************************

class CSQLRowSetStampOfferInfo
{
public:
	CSQLRowSetStampOfferInfo();

public:
	void LoadDatabaseStampOfferInfo();
	
public:
	int GetSize() { return m_arrayRows.GetSize(); }
	void GetRow(int n, CSQLRowStampOfferInfo& Row);
	void SetRow(int n, CSQLRowStampOfferInfo& Row);
	
private:
	CArray<CSQLRowStampOfferInfo, CSQLRowStampOfferInfo> m_arrayRows;
};

//**********************************************************************
