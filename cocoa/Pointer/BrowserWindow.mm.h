//
//  BrowserWindow.h
//  Pointer
//
//  Created by Yu Li on 2018-07-31.
//  Copyright © 2018 Yu Li. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>
#import "WebUI.mm.h"
#import "CrawlerRulesPopover.mm.h"
#import "Bookmarks.mm.h"
#import "Extension/PTextView.h"
#import "Downloads.h"
#import "ErrorPage.h"

@interface NSResponder(Pointer)
- (IBAction)menuNewTab:(id)sender;
- (IBAction)menuCloseTab:(id)sender;
- (IBAction)menuRefreshTab:(id)sender;
- (IBAction)menuFocusFindText:(id)sender;
- (IBAction)menuFocusFindSymbol:(id)sender;
- (IBAction)menuFocusAddress:(id)sender;
- (IBAction)menuAddBookmark:(id)sender;
- (IBAction)menuShowNextTab:(id)sender;
- (IBAction)menuShowPrevTab:(id)sender;
- (IBAction)menuEditBookmarks:(id)sender;
- (IBAction)menuEditCrawlerRules:(id)sender;
- (IBAction)menuShowEULA:(id)sender;
@end


@interface BrowserWindowController : NSWindowController
{
    IBOutlet NSOutlineView* m_outlineview;
    IBOutlet NSTabView* m_tabview;
    IBOutlet NSSearchField* m_tab_searchfield;
    IBOutlet AddressBar* m_addressbar;
    IBOutlet NSButton* m_crawler_rules_popover_button;
    IBOutlet NSBox* m_text_find_toolbar;
    IBOutlet NSSearchField* m_text_find_searchfield;
    IBOutlet NSButton* m_text_find_done_button;
    IBOutlet NSButton* m_text_find_next_button;
    IBOutlet NSButton* m_text_find_prev_button;
    IBOutlet NSTextField* m_text_find_label;
    IBOutlet NSButton* m_newtab_button;
    IBOutlet CrawlerRulesPopover* m_crawler_rules_popover;
    IBOutlet NSPopover* m_downloads_popover;
    IBOutlet DownloadsViewController* m_download_viewcontroller;
    IBOutlet NSButton* m_crawler_rule_table_button;
    IBOutlet NSButton* m_downloads_button;
    IBOutlet NSView* m_bookmarks;
    IBOutlet BookmarksViewController* m_bookmarks_viewcontroller;
    IBOutlet NSButton* m_go_back_button;
    IBOutlet NSButton* m_go_forward_button;
}

@property NSOutlineView* outlineview;
@property NSBox* text_find_toolbar;
@property NSSearchField* text_find_searchfield;
@property NSSearchField* tab_searchfield;
@property AddressBar* addressbar;
@property NSButton* newtab_button;
@property NSButton* text_find_done_button;
@property NSButton* text_find_next_button;
@property NSButton* text_find_prev_button;
@property NSTextField* text_find_label;
@property CrawlerRulesPopover* crawler_rules_popover;
@property BookmarksViewController* bookmarks_viewcontroller;
@property NSView* bookmarks;
@end

@interface GeneralTextViewDelegate : NSObject<NSTextViewDelegate>
@end

@interface BrowserWindow : NSWindow
@end

@interface BrowserWindowDelegate : NSObject<NSWindowDelegate>
{
    PTextView* m_general_ptextview;
    GeneralTextViewDelegate* m_general_textviewdelegate;
}
@end

@interface BrowserWindowView : NSView
{
    
}
@end


@interface DownloadPopoverDelegate : NSObject<NSPopoverDelegate>
{
    IBOutlet DownloadsViewController* m_download_popover_viewcontroller;
    IBOutlet NSPopover* m_download_popover;
}
@end
