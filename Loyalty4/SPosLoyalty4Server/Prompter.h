#pragma once
/**********************************************************************/

class CPrompter
{
public:
	CPrompter() {};
	int YesNo(const char* szPrompt, const char* szTitle = "", bool bYes = FALSE) { return IDNO; }
};


/**********************************************************************/
extern CPrompter Prompter;
/**********************************************************************/
