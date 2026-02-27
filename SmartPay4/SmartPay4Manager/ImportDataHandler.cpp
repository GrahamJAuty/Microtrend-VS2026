//*******************************************************************
#include "..\SmartPay4ManagerBgnd\PhotoIDCopy.h"
//*******************************************************************
#include "ImportDataHandler.h"
//*******************************************************************

CImportDataHandler::CImportDataHandler ( CWnd* pParent )
{
	m_pParent = pParent;
	m_bBackgroundMode = FALSE;
}

//*******************************************************************

void CImportDataHandler::ScanAll()
{
	CPhotoIDCopy photo{};					// see if any photos to import
	photo.Import();							// will put in epos export if reqd	
}

//*********************************************************************
