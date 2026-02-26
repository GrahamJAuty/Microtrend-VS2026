/**********************************************************************/
/* version number													  */
/**********************************************************************/

//4.25
//*********

//Signed EXEs
//Flag to set whether server can run as service or desktop app.


//4.23
//*********

//Option to specify port number in connect.ini file.

//Option to specify multiple acceptable ODBC driver versions in connect.ini file.

//Manual Spend button can appear when manual Topup button is disabled.

//4.22
//*********

//Uses OBDCDriver 17 by default, with option to change via INI file.

//Offer stamp merging by date works correctly.

//4.21
//*********

//Replaced backup library with Chilkat.
//ZIP64 used for large backups, removes need for mini zip file workarounds.
//Compression level can be set between 0 and 9.
 
//Improvements to auto create new account options.
//New account creation can be restricted to a specific card number range.
//The unknown account prompt text can be modified.
//Option to set default names for pending and finalised new accounts.

//Schemes can be programmed by account.

//Supports JSON messages to help with registering accounts.

//SQL Server Locks replace file based system.

//Tray icon images are cached.

//Audits can be filtered by action type.

//Option for searches to exclude inactive accounts.

//Compulsory field ticks work for username, forename and surname.

//MiFare is allowed in RealTime+ mode.

//QR lookups can be redirected to main account after removing a prefix.

//Non UI option to log cards read by Loyalty Manager.

//The account transfer dialog does not show max spend or overdraft options if these settings
//are controlled by the group.

//4.20
//*********

//Plu Ranking Reports dialog saves New Page and Unknown Items flag.

//Supports up to 5 topup PLUs for each purse.

//Optional prompt before cancelling account edit changes.

//Purchase reports have default item texts for all entries.

//Purse2 can be included when transferring account details from one card to another.

//Plu Ranking report identifies unknown items correctly.

//Plu Ranking reports shows item discounts with a quanitity of zero.

//Sales with no chargeable items are added to the audit log.

//Server lock performance is logged.

//Audit reports show default comment for all actions if no specific comments given

//4.19
//*********

//Spend bonus audit shows transaction value.

//Stamp Offer audit shows events between 23:59 and 00:00.

//Cashless Liability report shows adjustments correctly for large card numbers.

//Purse2 Topup Button in Edit Account dialog and context menu.

//Pos Display options include Purse1 Balance, Purse2 Balance and available spend.

//Bonuses can be directed to Purse2.

//Purse1 can be used as a priority for general spend.

//4.18
//*********

//Supports multiple Ecr Manager databases.

//Manual Spend button.

//Grand totals for Payment History and Purchase Ranking reports.

//Improved performance of Purchase Ranking report.

//Transaction Summary report.

//Option to not preload account details for the main account list.

//Option to not use colours for account status in the main account list.

//More default settings for the account find dialog.

//Automatic clean up of temporary files in LOYREQ folders.

//Fixed Test button for SMTP Email


//4.17
//*********

//ReplyTo Header for SMTP Email.

//Test button on SMTP Email Options tab.

//4.16
//*********

//Supports use of SPOS managed SMTP server.

//EmailSender no longer required for SMTP.

//4.15
//*********

//4 separate top-up PLUs with indepedent bonus settings.

//Improved Member Mojo import

//Department spend bonuses work correctly

//4.14
//*********

//Option to prevent general spend bonus from being awarded for items
//which have already received PLU points.

//TopUp bonuses can be specified separately for each of the supported
//TopUp PLUs.

//The Group Specific points bonus for TopUps is stored correctly.

//4.13
//*********
//Stamp offer threshold and stamps per row increased to 999999.

//Manual deletion of expired stamps works correctly.

//Revaluation PLU setting is imported correctly from previous versions.


//4.12
//*********

//Department spend bonus.

//Option column in main account list to show account status.

//Status colours in the main account list work correctly.

//Multipler points for PLU sales appear correctly in Purchase History
//and on till receipts. 

//4.11
//*********
//Supports update tokens


//V4.10.005
//*********
//Stamp offer audits work correctly when two or more different free
//items are awarded in the same transaction for the same offer.

//V4.10.004
//*********
//Conditional exports work when only one condition is set.
//DPI Scaling is handled by Windows	

//V4.10.003
//*********
//Manual import of stamp offers works when Server is running as service.

//V4.10.002
//*********
//Database update from v6 to v7 works correctly.

//V4.10.001
//*********
//Stamp Offers are imported correctly.

//V4.10
//*****
//Loyalty Stamp Offers
 
//V4.09
//*****
//SQL Backup uses FORMAT and INIT to clear previous backup file
//Hidden option to force restore from script rather than backup.

//V4.08.004
//*********
//Unknown cards can be auto imported by server in all interface modes

//V4.08.003
//*********
//SQL Restore uses MOVE clause to copy database and log to local folders


//V4.08.002
//*********
//Main window commands blocked during backup


//V4.08.001
//*********
//Backup moved to idle time.


//V4.08
//*********
//Supports unlimited database file sizes, option to shrink log files


//V4.07.002
//*********
//Renewal dates work with February 29th


//V4.07.001
//*********
//Database schema created as version 6 when building from scratch

//Backup and restore use SQL BACKUP and RESTORE commands.


//V4.07
//*****

//Separate tick for alert set and hotlisted in account find.

//Removed extra ticks for option to exclude hotlisted accounts from
//SPOS search.


//V4.06.003
//**********

//The restore options sees automatic backups that are split into multiple files.

//Option to control access to backup option by PC.

//Fixed filename for PC Specific options. 

//V4.06x002
//**********
//New balance on receipt works correctly 

//V4.06x.001
//**********
//CCNo of zero can be imported 

//V4.06
//**********
//Manual edits to the backup folder are saved.

//Account transfers are not prevented by internal rounding of values.

//V4.05.002
//**********
//Purse2 Refresh value can be zero

//V4.05.001
//**********
//Show QR Info as default can be turned off

//Upgrade works if legacy CC No or Audit Period files do not exist

//V4.05
//**********
//Period Audit and CCNo files converted to SQL.

//Backups with an earlier database version can be restored.

//SQLConnect.INI file moved to SQL folder

//V4.04
//**********
//Support for accessing SQL database from multiple PCs.

//V4.03.003
//**********
//Support for multiple staff gifts in same transaction

//V4.03.002
//**********
//Zero value staff gift is not audited if customer ID same as staff ID.

//V4.03.001
//**********
//Staff Gift Exception Amount is reset after each tranaction

//V4.03
//**********
//Supports Loyalty Staff Gift

//Supports Ecr Manager Reconciliation Report

//LOG files with multiple transactions are processed correctly

//Renewal PLUs are processed correctly

//Carriage returns codes are discarded when importing CSV data from previous versions


//V4.02
//**********
//Flashing Green and Yellow tray icon if Server running but unable to 
//process files.

//Diagnostic option to save copies of incoming Loyalty Log files.

//Incoming Loyalty Log files that appear to be empty are not deleted.

//Account number cache to improve speed of audit reports.

//Server Log differentiates SQL access errors from account finding errors.


//V4.01
//**********
//The error code is cleared before each attempt to upgrade earlier system data.
//The cancel button is removed from the import progress dialog for upgrades.