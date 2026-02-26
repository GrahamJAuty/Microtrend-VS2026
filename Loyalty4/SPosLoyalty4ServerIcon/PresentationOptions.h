#pragma once
/**********************************************************************/

class CPresentationOptions 
{
public:
	CPresentationOptions();

public:
	bool GetCustomMessageBoxFlag() { return TRUE;  }
	int GetCustomMessageBoxFont() { return 12;  }
	int GetCustomMessageBoxLineGap() { return 10; }
	const char* GetCustomMessageBoxColourBack() { return "FFFFFF"; }
	const char* GetCustomMessageBoxColourText() { return "000000"; }
	int GetCustomMessageBoxColourTextInt() { return 0xFFFFFF; }
	int GetCustomMessageBoxColourBackInt() { return 0x0; }	
};

/**********************************************************************/
extern CPresentationOptions PresentationOptions;		/* global */
/**********************************************************************/
