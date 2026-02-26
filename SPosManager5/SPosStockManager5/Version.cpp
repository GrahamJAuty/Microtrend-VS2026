/********************************************************/

//STOCK MANAGER

/********************************************************/
//V4.43
/********************************************************/

//Replaced backup library with Chilkat.
//ZIP64 used for large backups, removes need for mini zip file workarounds.
//Compression level can be set between 0 and 9.

//PosTray supports export file generation for Chartwells.

//Loyalty Scheme names are exported for use by Loyalty Manager.

//EPOS Terminal names are exported for use by SmartPay.

//PosTray tasks can be filtered by day of the month.

/********************************************************/
//V4.42
/********************************************************/

//Mix Match Offers with Fixed Cost by PLU can use the current Price Band.

//Custom EPOS report for PLU Sales with separate columns per Price Band.

//The Loyalty Manager menu option is displayed for systems with SPOS Loyalty v4.

/********************************************************/
//V4.41
/********************************************************/

//PosTray can send emails.

//Test email button works with nicknames

//Plu add works correctly when database is sorted by
//department etc.

/********************************************************/
//FEATURES (V4.40)
/********************************************************/

//PLU Report Texts and Stock Descriptions can be up to 100 chars

//PLU Link tables are updated correctly when using spin buttons
//to move between PLU items while the PLU Link tab is not
//selected.

//The Allow Modifier option may be set or cleared for base PLU
//items within the modifiable range.

//The Allow Modifier option is cleared on download for 
//modifier items.

//Mix Match Usage counts are included in reports (requires
//CashRSP v44 or later)

//Button bar options and special keys options cannot be used if
//the system database was invalid at startup.

//New font sizes 26, 28, 36, 48, 72 and 120 are available in
//the List Layout editor.

//Reply-to header can be sent for SMTP emails

//Test button for SMTP emails


/********************************************************/
//FEATURES (V4.39)
/********************************************************/

//Supports SPOS V4.2.1618 with UpTo option for Markdown
//promotions.

//Supports CashRSP v44.

//Supports SPOS SMTP Server

//Does not require separate utility to send emails.

//Promo discount on Mix Match offer special items is
//reallocated to discounted items.

//MISC51 Repurposed as Cheapest One Free flag.

//Selecting Multiple Promos for Loyalty Schemes works correctly.

//Processing of MM Offers with N-Split works correctly.

//Stock Import Sheets can be created without opening stocktake

//Stock Import Sheets support alphabetical sorting.

//Alphabetical sorting for Stock Import Sheets and Stock Sheets is not case sensitive.

//Category and supplier sorting for stock import sheets works correctly.


/********************************************************/
//FEATURES (V4.38)
/********************************************************/

//Hidden option MISC49 to ignore the repeated transaction time when checking for
//duplicated $TRAN lines.

//Hidden option MISC50 to clarify reporting of recycled deposits with a non cash
//payment method.

//Hidden option MISC51 to ignore $PDISC lines for transactions which also have
//$MMDISC lines.

/********************************************************/
//FEATURES (V4.37)
/********************************************************/

//Supports SPOS V4.2.1605 with option to control how tied
//items are charged for and listed on receipts.

//PLU items can be edited when setting up Offer Groups,
//Stamp Offers or Purchase Controls.

//New hidden options MISC46 and MISC47 to fix reporting 
//discrepancies with gratuities.

//New hidden option MISC48 to fix reporting issue with
//tied PLU items not reported as chargeable.

/********************************************************/
//BUG FIXES (V4.37)
/********************************************************/

//Fixed consolidation of special 15 digit items in PLU
//sales history.

//Zero sized files are not counted when preparing a list
//of pending CashRSP files to process.

//Allergens, Printer Groups and Custom Stock Fields are
//loaded into memory when edting a PLU item from the
//keyboard layout context.

//The apparent stock and stock level tabs are not shown
//when editing a PLU item from the keyboard layout
//context

/********************************************************/
//FEATURES (V4.36)
/********************************************************/

//Supports SmartPay Purchase Controls.

//Plu Records can be imported by stock code.

/********************************************************/
//BUG FIXES (V4.36)
/********************************************************/

//Modifier List keys are not changed to base PLU item
//after editing PLU from Layout Editor.

//Custom Payment Detail Report works correctly.

/********************************************************/
//FEATURES (V4.35)
/********************************************************/

//Custom Payment Detail report.

//Department Loyalty Settings.

//Transaction report shows table name.

/********************************************************/
//FEATURES (V4.34.003)
/********************************************************/

//PosTray does not need update token

/********************************************************/
//FEATURES (V4.34.002)
/********************************************************/

//Added more dealers for update tokens

/********************************************************/
//FEATURES (V4.34.001)
/********************************************************/

//Dealer demos can use update tokens.

//Separate modes for LIPA and LJMU exports.

//Britannia Hotels export has Department Set selector.

/********************************************************/
//FEATURES (V4.34)
/********************************************************/

//Web updates use token files.

/********************************************************/
//FEATURES (V4.33)
/********************************************************/

//Macro Help dialog.

//Order Printer Names can be edited.

//Option to NOT use alternate filenames when modifiers
//are disabled.

//Added function 155 (Show Noticeboard)

//DPI awareness flag turned off in build settings.

//Custom Transaction Log reports can be created when
//Loyalty or Smartpay are not present.

//Order Printer Group information is loaded when editing
//a PLU item from the master database via the Keyboard
//Layout editor.		

/********************************************************/
//FEATURES (V4.32)
/********************************************************/

//Editing and download of Loyalty Stamp Offers

/********************************************************/
//BUG FIXES (V4.31)
/********************************************************/

//The Adhoc Delivery and Adjustment dialogs can be opened.

/********************************************************/
//FEATURES (V4.30)
/********************************************************/

//EPOS Reports and PLU Sales History have option to
//summarise transaction date ranges for terminals by
//location or system

//Access to backup options can be controlled on a per PC
//basis.

//Option to not send PosTray reports if empty.

//Option to not send PosTray reports in unchanged since
//previous similar report.

//EPOS Reports and PLU Sales History have option to
//summarise transaction date ranges for terminals by
//location or system

//Option to create multiple stocksheets per stocktake
//in a single folder and consolidate the totals
//together after stock levels have been entered.

//Selection of Promo or Mixmatch links from a timeplan
//or loyalty scheme works correctly.

/********************************************************/
//FEATURES (V4.28)
/********************************************************/

//Loyalty and SmartPay Reconciliation Report

//Simple text search for Promotions, Departments etc

//Optional EPOS Text column for Plu Database

//Supports new PLU type for Loyalty Staff Gift

//Supports CashRSP V43

//Supports SPOS 4.2.1458

/********************************************************/
//BUG FIXES (V4.27)
/********************************************************/

//The root folder location for automatic backups can
//be edited.

//Kitchen printers 10 to 15 can be assigned to table
//areas.

//Transactions that consist of voided items and Loyalty
//top-ups are counted as payment on account transactions
//in Payment Summary reports rather than as mixed item
//and payment on account transactions. 

/********************************************************/
//FEATURES (V4.26)
/********************************************************/

//Link to Greyeye Stockmate application for stock taking on Android devices.

//Cashback may be enabled for the External Payment type.

//Hidden option Misc40 to ignore unpaid transactions.

//Link to Loyalty4 and SmartPay4.

//Added function 154 (Bill Progressive)

//Improvements to Time & Attendance Reports
//1 .. Option to show estimated pay for hours worked.
//2 .. Option to include hours from open shifts.
//3 .. Reports all have separate names.

/********************************************************/
//FEATURES (V4.25)
/********************************************************/

//Analysis Categories may be used in Plu Filters.

//The individual reports within each PosTray tasks may
//be linked to one of five email nicknames that are
//defined for each task.

/********************************************************/
//BUG FIXES (V4.25)
/********************************************************/

//PLU number 0 is invalid for imports etc.

//Reason texts are downloaded for manual entry reasons.

//Email settings are passed to Report Manager when modifiers are disabled.

/********************************************************/
//INTERNAL (V4.25)
/********************************************************/

//Changed various texts from Sharp Pos to SPOS

/********************************************************/
//FEATURES (V4.24)
/********************************************************/

//Hidden option MISC39 to work around refund issue with
//SPOS external paymants

//Option for custom sales history reports by weekday
//to show quantity and value columns for each day.

//PluPts.CSV file includes dept no and tax code.

/********************************************************/
//BUG FIXES (V4.23)
/********************************************************/

//The Payment Detail report shows Cashback and Gratuity
//in the correct order.

//The Closing Stock Variance report shows the estimated
//cost price of Closing Stock correctly. (V423_001)

/********************************************************/
//FEATURES (V4.22)
/********************************************************/

//Service Charge promotions may be locked to Tax Band A,
//irrespective of the Tax Bands of sold items.

//Prices Sets may override the Tax band field.

/********************************************************/
//BUG FIXES (V4.22)
/********************************************************/

//The Modifier Order Printer Group setting can be
//imported into price sets.

//System, database and location macros are not copied
//into the keyboard macro file when opening a keyboard
//for editing.

/********************************************************/
//FEATURES (V4.21)
/********************************************************/

//Maximum email addresses per nickname increased to 20.

/********************************************************/
//FEATURES (V4.21)
/********************************************************/

//Hidden option MISC37 allows a default home location to
//be specified for database or system level servers.

//Hidden option MISC38 allows servers to be imported or
//exported via CSV file.

/********************************************************/
//FEATURES (V4.20)
/********************************************************/

//Option to show Premium total separately from discounts
//in various reports.

//Option to exclude specific Keyboard Lists from the PLU
//item drop down for menu, qualifier and extra.

//Added function 153 (EFT Login/Logout)

/********************************************************/
//FEATURES (V4.19)
/********************************************************/

//Reason texts may be edited and downloaded.

/********************************************************/
//Bug Fixes (V4.19)
/********************************************************/

//Price Band filters work correctly with PosTray Plu Sales 
//History reports.

//Subtotal Adjustments in Covers and Group reports do not affect the
//gross total of sales for each block when this is shown separately
//from the total net of discounts. 

//Time and Attendance reports for multiple locations show the overall
//date range of available data for each location at the bottom of the
//report.

/********************************************************/
//FEATURES (V4.18)
/********************************************************/

//The list of logged on users includes the Stock Manager
//user name.

//PosTray task to process pending transactions without 
//creating and reports or exports.

/********************************************************/
//FEATURES (V4.17)
/********************************************************/

//Table settings may be copied to new or existing tables.

//Export filenames for PosTray EPOS tasks where several
//files are generated for the same report may include
//the location or database name for each file instead
//of an automatically generated number.

//Keyboard Editor option to remove images from keys.

//Option to use description text from PLU qualifier as
//an attribute text for the parent item.

//The EPOS Sales by Tax band report includes a summary 
//of tax band totals for blocks which have items from
//more than one band.

/********************************************************/
//INTERNAL (V4.17)
/********************************************************/

//Hidden option MISC34 allows access to PosTray settings
//when multiple users are logged off.

/********************************************************/
//FEATURES (V4.16)
/********************************************************/

//Supports SPOS v4.2.1087
//1 ... 16 Order Printers are available.

//Added function 152 (Scanner)

//Passwords and Usernames may be up to 32 characters.

/********************************************************/
//INTERNAL (V4.16)
/********************************************************/

//Loyalty Schemes may be edited if Loyalty Manager is
//not present.

/********************************************************/
//FEATURES (V4.15)
/********************************************************/

//Supports SPOS v4.2.1081
//1 ... 12 VAT (tax) bands are available. 

//PLU Database filters can include a PLU range.

//User accounts can specify a preset PLU filter that will
//apply to that user when editing the PLU database.

/********************************************************/
//BUG FIXES (V4.15)
/********************************************************/

//Sales History by Region reports can be created.

//Price band filters are applied correctly to PosTray
//exports.

//Item discounts for PMS Manual Sales are assigned to
//the VAT band of the discounted item.

//Time and Attendance reports and exports may be sorted
//by location or server.

/********************************************************/
//INTERNAL (V4.15)
/********************************************************/

//First version built with Visual Studio 2019