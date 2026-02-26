#pragma once
/**********************************************************************/
#include "DataManagerNonDbFuncCommon.h"
#include "DataManagerNonDbFuncEcrman.h"
#include "..\CommonEcrStock\DataManagerNonDbFuncMain.h"
#include "DataManagerNonDbFuncPMS.h"
#include "..\SPosStockManager5\DataManagerNonDbFuncStockman.h"
/**********************************************************************/
#ifdef POSTRAY_UTILITY
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/

//POSTRAY WITH PMS 
class CDataManagerNonDb : public CDataManagerNonDbFuncCommon, public CDataManagerNonDbFuncEcrman, public CDataManagerNonDbFuncPMS 
{
public:
	void CloseFiles( CWordArray& arrayFiles );
};

/**********************************************************************/
#else
/**********************************************************************/

//POSTRAY WITHOUT PMS 
class CDataManagerNonDb : public CDataManagerNonDbFuncCommon, public CDataManagerNonDbFuncEcrman 
{
public:
	void CloseFiles( CWordArray& arrayFiles );
};

/**********************************************************************/
#endif
/**********************************************************************/

/**********************************************************************/
#else
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/

//STOCKMAN WITH PMS
class CDataManagerNonDb : public CDataManagerNonDbFuncCommon, public CDataManagerNonDbFuncMain, public CDataManagerNonDbFuncStockman, public CDataManagerNonDbFuncPMS 
{
public:
	void CloseFiles( CWordArray& arrayFiles );
};

/**********************************************************************/
#else
/**********************************************************************/

//STOCKMAN WITHOUT PMS
class CDataManagerNonDb : public CDataManagerNonDbFuncCommon, public CDataManagerNonDbFuncMain, public CDataManagerNonDbFuncStockman 
{
public:
	void CloseFiles( CWordArray& arrayFiles );
};

/**********************************************************************/
#endif
/**********************************************************************/

/**********************************************************************/
#else
/**********************************************************************/

/**********************************************************************/
#ifdef COMPILE_PMS
/**********************************************************************/

//ECRMAN WITH PMS
class CDataManagerNonDb : public CDataManagerNonDbFuncCommon, public CDataManagerNonDbFuncMain, public CDataManagerNonDbFuncEcrman, public CDataManagerNonDbFuncPMS 
{
public:
	void CloseFiles( CWordArray& arrayFiles );
};

/**********************************************************************/
#else
/**********************************************************************/

//ECRMAN WITHOUT PMS
class CDataManagerNonDb : public CDataManagerNonDbFuncCommon, public CDataManagerNonDbFuncMain, public CDataManagerNonDbFuncEcrman 
{
public:
	void CloseFiles( CWordArray& arrayFiles );
};

/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/
#endif
/**********************************************************************/

/**********************************************************************/
extern CDataManagerNonDb DataManagerNonDb;
/**********************************************************************/

