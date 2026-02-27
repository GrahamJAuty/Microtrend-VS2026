//*******************************************************************
//*** exactly same file used in KioskCardLink \ KioskBioLink ********
//*******************************************************************
#pragma once
//********************************************************************
#include "defines.h"
//********************************************************************
void ReportManager ( CWnd* pParent, const char* szReport, const char* szParams, const char* szParamsKey, bool bPrint );
CString FormatDoubleValue ( double dValue );
//********************************************************************
