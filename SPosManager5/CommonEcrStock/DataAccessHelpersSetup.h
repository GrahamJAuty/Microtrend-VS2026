#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "DataAccessHelpers.h"
/**********************************************************************/
#include "..\SPosEcrManager5\MainFrmEcrman.h"
#include "..\SPosStockManager5\MainFrmStockman.h"
/**********************************************************************/

class CDataAccessHelpersSetup : public CDataAccessHelpers
{
public:
	CDataAccessHelpersSetup();

public:
	void SetupSystem();
	void EditOptions();
};

/**********************************************************************/
#endif
/**********************************************************************/
