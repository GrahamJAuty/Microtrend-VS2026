#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#define BUTMGR_MODE_USER 0
#define BUTMGR_MODE_EDIT_LAYOUT 1
#define BUTMGR_MODE_EDIT_LIST 2
/**********************************************************************/
#define BUTMGR_MAX_LIST_NO 99
#define BUTMGR_MAX_LINE_NO 4
#define BUTMGR_MAX_BUTTON_NO 9
#define BUTMGR_MAX_DETAIL1 99
#define BUTMGR_MAX_DETAIL2 1999999
#define BUTMGR_MAX_DETAIL3 9999
#define BUTMGR_MAX_TEXTLEN 30
#define BUTMGR_MAX_LISTNAME 30
/**********************************************************************/
#define BUTTON_TYPE_HIDE 0
#define BUTTON_TYPE_UNUSED 1
#define BUTTON_TYPE_ACTION 2
#define BUTTON_TYPE_LIST 3
#define BUTTON_TYPE_HOME 4
#define BUTTON_TYPE_BACK 5
#define BUTTON_TYPE_PARENT 6
#define BUTTON_TYPE_LIST_SELECT 7
#define BUTTON_TYPE_LIST_NAME 8
#define BUTTON_TYPE_LIST_SAVE 9
/**********************************************************************/
#define BUTTON_DETAIL1_SETUP 1
#define BUTTON_DETAIL1_EDITEPOS 2
#define BUTTON_DETAIL1_REPORTS 3
#define BUTTON_DETAIL1_KEYBOARD 4

#ifdef STOCKMAN_SYSTEM
#define BUTTON_DETAIL1_EDITSTK 5
#define BUTTON_DETAIL1_MOVESTK 6
#endif

#define BUTTON_DETAIL1_TASKS 7
#define BUTTON_DETAIL1_PSET_LOC 8
#define BUTTON_DETAIL1_PSET_DB 9
#define BUTTON_DETAIL1_EXTERNAL 10
/**********************************************************************/
#define BUTTON_DETAIL2_SETUP_SYSTEM 1
#define BUTTON_DETAIL2_SETUP_OPTIONS 2
/**********************************************************************/
#define BUTTON_DETAIL2_EDITEPOS_PLU 1
#define BUTTON_DETAIL2_EDITEPOS_DEPT 2
#define BUTTON_DETAIL2_EDITEPOS_GROUP 3
#define BUTTON_DETAIL2_EDITEPOS_TAX 4
#define BUTTON_DETAIL2_EDITEPOS_ACAT 5
#define BUTTON_DETAIL2_EDITEPOS_PAYMENT 6
#define BUTTON_DETAIL2_EDITEPOS_SERVER 7
#define BUTTON_DETAIL2_EDITEPOS_MIXMATCH 8
#define BUTTON_DETAIL2_EDITEPOS_PROMOTION 9
#define BUTTON_DETAIL2_EDITEPOS_OFFERGROUP 10
#define BUTTON_DETAIL2_EDITEPOS_TIMEPLAN 11
#define BUTTON_DETAIL2_EDITEPOS_WALLETSTAMPRULE 12
#define BUTTON_DETAIL2_EDITEPOS_LOYALTYSCHEME 13
/**********************************************************************/
#define BUTTON_DETAIL2_EDITSTK_STOCK 1
#define BUTTON_DETAIL2_EDITSTK_SUPPLIER 2
#define BUTTON_DETAIL2_EDITSTK_CATEGORY 3
#define BUTTON_DETAIL2_EDITSTK_CUSTFLD 4
#define BUTTON_DETAIL2_EDITSTK_ALLOWANCE 5
#define BUTTON_DETAIL2_EDITSTK_DELUNIT 6
#define BUTTON_DETAIL2_EDITSTK_STKUNIT 7
#define BUTTON_DETAIL2_EDITSTK_SUBUNIT 8
#define BUTTON_DETAIL2_EDITSTK_ADJTEXT 9
/**********************************************************************/
#define BUTTON_DETAIL2_MOVESTK_OPENING 1
#define BUTTON_DETAIL2_MOVESTK_DELIVERY 2
#define BUTTON_DETAIL2_MOVESTK_RETURN 3
#define BUTTON_DETAIL2_MOVESTK_SALEPLU 4
#define BUTTON_DETAIL2_MOVESTK_SALESTK 5
#define BUTTON_DETAIL2_MOVESTK_ADJUSTPLU 6
#define BUTTON_DETAIL2_MOVESTK_ADJUSTSTK 7
#define BUTTON_DETAIL2_MOVESTK_TRANSFER 8
/**********************************************************************/
#define BUTTON_DETAIL2_REPORTS_PLUHISTORY 1
#define BUTTON_DETAIL2_REPORTS_EPOS_VIEW 2
#define BUTTON_DETAIL2_REPORTS_STKHISTORY 3
#define BUTTON_DETAIL2_REPORTS_STKLEVEL 4
#define BUTTON_DETAIL2_REPORTS_CUSTOMER 5
#define BUTTON_DETAIL2_REPORTS_STKAUDIT 6
#define BUTTON_DETAIL2_REPORTS_EPOS_PRINT 7
#define BUTTON_DETAIL2_REPORTS_ECRMANSTK 8
#define BUTTON_DETAIL2_REPORTS_TIMEATTEND 9
/**********************************************************************/
#define BUTTON_DETAIL2_TASKS_ADHOC 1
#define BUTTON_DETAIL2_TASKS_EDITPRESET 2
#define BUTTON_DETAIL2_TASKS_RUNPRESET 3
/**********************************************************************/
#define BUTTON_DETAIL2_KEYBOARD_USERSELECT 0
/**********************************************************************/
#define BUTTON_DETAIL2_PSET_USERSELECT 0
/**********************************************************************/
#define BUTTON_DETAIL2_EXTERNAL_LOYALTY 1
/**********************************************************************/
#define BUTTON_DETAIL3_EDIT_USERSELECT 0
/**********************************************************************/
#define BUTTON_DETAIL3_KEYBOARD_USERSELECT 0
/**********************************************************************/
#define BUTTON_DETAIL3_PSET_USERSELECT 0
/**********************************************************************/
#define BUTTON_DETAIL3_REPORTS_OPENDIALOG 0
#define BUTTON_DETAIL3_REPORTS_USERSELECT 9999
/**********************************************************************/
#define BUTTON_DETAIL3_TASKS_USERSELECT 0
/**********************************************************************/
#endif
/**********************************************************************/
