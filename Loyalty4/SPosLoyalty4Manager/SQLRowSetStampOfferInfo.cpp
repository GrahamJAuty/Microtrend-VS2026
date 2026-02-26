//**********************************************************************
#include "..\SPOSLoyalty4Shared\SQLTable_StampOfferInfo\SQLRepositoryStampOfferInfo.h"
#include "..\SPOSLoyalty4Shared\SQLTable_StampOfferInfo\SQLRecordSetStampOfferInfo.h"
//**********************************************************************
#include "..\SPOSLoyalty4Shared\SQLDb.h"
//**********************************************************************
#include "SQLRowSetStampOfferInfo.h"
//**********************************************************************

CSQLRowSetStampOfferInfo::CSQLRowSetStampOfferInfo()
{
}

//**********************************************************************

void CSQLRowSetStampOfferInfo::LoadDatabaseStampOfferInfo()
{
	m_arrayRows.RemoveAll();
	
	CSQLRecordSetStampOfferInfo RecordSet(NULL, RSParams_StampOffer_NormalNoParams{});

	CSQLRowStampOfferInfo RowSO;
	while (RecordSet.StepSelectAll(RowSO) == TRUE)
	{
		m_arrayRows.Add(RowSO);
	}
}

//**********************************************************************

void CSQLRowSetStampOfferInfo::GetRow(int nRow, CSQLRowStampOfferInfo& Row)
{
	if ((nRow >= 0) && (nRow < GetSize()))
	{
		Row = m_arrayRows.GetAt(nRow);
	}
}

//**********************************************************************

void CSQLRowSetStampOfferInfo::SetRow(int nRow, CSQLRowStampOfferInfo& Row)
{
	if ((nRow >= 0) && (nRow < GetSize()))
	{
		m_arrayRows.SetAt(nRow, Row);
	}
}

//**********************************************************************

