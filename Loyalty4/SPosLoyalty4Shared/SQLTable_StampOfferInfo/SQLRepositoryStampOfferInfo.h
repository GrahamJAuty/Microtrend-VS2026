#pragma once
//**********************************************************************
#include "..\Consolidation.h"
#include "..\SQLResultInfo.h"
//**********************************************************************
#include "SQLRowStampOfferInfo.h"
//**********************************************************************

struct CEnabledStampOfferInfo
{
public:
	int m_nOfferID;
	int m_nEdition;

public:
	CEnabledStampOfferInfo()
	{
		m_nOfferID = 1;
		m_nEdition = 1;
	}
};

//**********************************************************************

class CSQLRepositoryStampOfferInfo
{
public:
	CSQLRepositoryStampOfferInfo();

public:
	CSQLResultInfo SelectRow(CSQLRowStampOfferInfo& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo UpdateRow(CSQLRowStampOfferInfo& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo InsertRow(CSQLRowStampOfferInfo& SQLRowOS, CDatabase* pDatabase);
	CSQLResultInfo DeleteRow(CSQLRowStampOfferInfo& SQLRowOS, CDatabase* pDatabase);
	
public:
	void ImportStampOfferInfo(CString strFilename);
	void GetEnabledOffers(CArray<CEnabledStampOfferInfo, CEnabledStampOfferInfo>& arrayOffers);
	void GetOfferName(int nOfferID, int& nLastOfferID, CString& strOfferName, CDatabase* pDatabase);
	CString GetOfferName(int nOfferID, CDatabase* pDatabase);
};

//**********************************************************************

