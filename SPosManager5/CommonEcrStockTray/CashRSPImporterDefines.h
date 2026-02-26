#pragma once
/**********************************************************************/
#define MAX_CASHRSP_VERSION 44
/**********************************************************************/
#define CASHRSP_SALES_UNKNOWN -2
#define CASHRSP_SALES_PENDING -1
#define CASHRSP_OBSOLETE_TERMSAVE 0
#define CASHRSP_SALES_TRADING 1
#define CASHRSP_OBSOLETE_TRMSAVE2 2
#define CASHRSP_SALES_TIMEATTEND 3
#define CASHRSP_SALES_PMS 4
#define CASHRSP_SALES_DEPOSIT 5
#define CASHRSP_SALES_EOD 6
#define CASHRSP_SALES_NONDUPLICATE 7
/**********************************************************************/
#define CASHRSP_DAILYFILE_TERM 0
#define CASHRSP_DAILYFILE_SALES_TRAN 1
//2 WAS DAILY TRANSACTION SUMMARY FILE
#define CASHRSP_DAILYFILE_ACCOUNT 3
//4 WAS DAILY DISCOUNT SUMMARY FILE
#define CASHRSP_STOCKFILE_SALES 5
#define CASHRSP_STOCKFILE_DELIVERY 6
#define CASHRSP_DAILYFILE_SALES_ITEM 7
#define CASHRSP_DAILYFILE_REASONS_TRAN 8
#define CASHRSP_DAILYFILE_REASONS_ITEM 9
#define CASHRSP_DAILYFILE_ITEMPAY 10
#define CASHRSP_TERMFILE_NONDUPLICATE 11
/**********************************************************************/
#define CASHRSP_PROCESSTYPE_NEWSALE 0
#define CASHRSP_PROCESSTYPE_REBUILD 1
//#define CASHRSP_PROCESSTYPE_TERMONLY 2
#define CASHRSP_PROCESSTYPE_RETRY 3
/**********************************************************************/
#define CASHRSP_TRANSACTION_TYPE_NONE 0
#define CASHRSP_TRANSACTION_TYPE_SALE 1
#define CASHRSP_TRANSACTION_TYPE_VOID 2
#define CASHRSP_TRANSACTION_TYPE_WASTE 3
#define CASHRSP_TRANSACTION_TYPE_NOSALE 4
#define CASHRSP_TRANSACTION_TYPE_AGEVERIFY 5
#define CASHRSP_TRANSACTION_TYPE_EOD 6
/**********************************************************************/