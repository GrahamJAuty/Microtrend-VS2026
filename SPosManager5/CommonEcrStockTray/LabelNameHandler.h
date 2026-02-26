#pragma once
/**********************************************************************/
 
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CLabelNameHandler  
{
public:
	CLabelNameHandler();
	void Read();
	void FillComboBox ( CSSComboBox& combo, const char* szName, bool bCanEnableCombo );

private:
	CString m_strFilename;
	CStringArray m_arrayLabelNames;
};

/**********************************************************************/

