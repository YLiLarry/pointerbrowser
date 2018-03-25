import Backend 1.0
import QtQuick 2.9
import QtWebView 1.1
import QtQuick.Controls 2.3

BrowserForm {
    id: browser
    function currentWebView() {
        return browserWebViews.currentWebView()
    }
    function currentIndex() {
        return browserWebViews.currentIndex()
    }

    property bool ctrlKeyPressing: false
    prevEnabled: currentWebView() && currentWebView().canGoBack
    nextEnabled: currentWebView() && currentWebView().canGoForward

    ListModel {
        id: tabsModel
    }

    onCtrlKeyPressingChanged: {
        console.log("onCtrlKeyPressingChanged", ctrlKeyPressing)
    }

    Keys.onPressed: {
        console.log("Keys.onPressed:", event.key, Qt.Key_Control)
        if (event.key === Qt.Key_Control) {
            browser.ctrlKeyPressing = true
        }
    }
    Keys.onReleased: {
        console.log("Keys.onReleased:", event.key, Qt.Key_Control)
        if (event.key === Qt.Key_Control) {
            browser.ctrlKeyPressing = false
        }
    }
    Component.onCompleted: {
        for (var i = 0; i < TabsModel.tabs.length; i++) {
            tabsModel.append(TabsModel.tabs[i])
        }
        if (TabsModel.tabs.length > 0) {
            openTab(0)
        }
    }

    browserWebViews.tabsModel: tabsModel
    tabsList.tabsModel: tabsModel
    browserAddressBar.progress: browserWebViews.loadProgress

    function newTab(url, switchToView) {
        console.log("newTab:", url, switchToView)
        url = url || "https://google.ca"
        TabsModel.insertTab(0, url, "", "")
        if (switchToView) {
            openTab(0)
        } else {
            openTab(currentIndex() + 1)
        }
    }

    function openTab(index) {
        console.log("openTab", "index=", index, "tabsModel.count=", tabsModel.count)

        browserWebViews.setCurrentIndex(index)
        var wp = browserWebViews.webViewAt(index)
        wp.forceActiveFocus()
        browserAddressBar.update(wp.url, wp.title)
        browserBookmarkButton.checked = true
        tabsList.setHighlightAt(index)
    }

    function closeTab(index) {
        console.log("browser.closeTab", "index=", index, "tabsModel.count=", tabsModel.count)
        // todo: remove from backend
        if (currentIndex() === index) {
            // when removing current tab
            // if there's one before, open that
            if (index >= 1) {
                TabsModel.removeTab(index)
                browser.openTab(index - 1)
                // if there's one after, open that
            } else if (index + 1 < tabsModel.count) {
                TabsModel.removeTab(index)
                browser.openTab(index)
                // if this is the only one
            } else {
                browser.newTab("",true)
                TabsModel.removeTab(index+1)
            }
        } else if (currentIndex() > index) {
            browser.openTab(currentIndex() - 1)
            TabsModel.removeTab(index)
        }
    }

    Connections {
        target: tabsPanel
        onUserOpensNewTab: browser.newTab("", true)
    }

    // Warning: only use this to semi-sync TabsModel and tabsModel
    // do not take any ui actions here
    Connections {
        target: TabsModel
        onTabInserted: {
            console.log("onTabInserted:", webpage.title, webpage.url)
            tabsModel.insert(index, webpage)
        }
        onTabRemoved: {
            console.log("onTabRemoved", index, webpage.title, webpage.url)
            tabsModel.remove(index)
        }
    }

    Connections {
        target: tabsList
        onUserClicksTab: browser.openTab(index)
        onUserClosesTab: browser.closeTab(index)
    }

    Connections {
        target: browserWebViews
        onUserOpensLinkInWebView: {
            browserAddressBar.update(url, "")
            currentWebView().forceActiveFocus()
        }
        onUserOpensLinkInNewTab: {
            newTab(url)
        }
        onWebViewLoadingSucceeded: {
            var wp = browserWebViews.webViewAt(index)
            //            TabsModel.tabs[index].title = wp.title
            if (index === currentIndex()) {
                browserBookmarkButton.checked = true
                browserAddressBar.update(wp.url, wp.title)
            }
            //            console.log("onWebViewLoadingSucceeded", TabsModel.tabs[index].title)
        }
    }

    Connections {
        target: browserAddressBar
        onUserEntersUrl: {
            currentWebView().url = url
        }
    }

    Connections {
        target: browserBackButton
        onClicked: {
            currentWebView().goBack()
        }
    }

    Connections {
        target: browserForwardButton
        onClicked: {
            currentWebView().goForward()
        }
    }

    Connections {
        target: browserRefreshButton
        onClicked: {
            browserWebViews.reloadCurrentWebView()
        }
    }

    Connections {
        target: browserDocviewSwitch
        onCheckedChanged: {
            var js = FileManager.readFileQrc("docview.js")
            if (browserDocviewSwitch.checked) {
                currentWebView().runJavaScript(js + "Docview.turnOn()",
                                             function (result) {
                                                 print(result)
                                             })
            } else {
                currentWebView().runJavaScript(js + "Docview.turnOff()",
                                             function (result) {
                                                 print(result)
                                             })
            }

        }
    }

    Shortcut {
        sequence: "Ctrl+R"
        onActivated: {
            browser.ctrlKeyPressing = false
            browserWebViews.reloadCurrentWebView()
        }
    }

    Shortcut {
        sequence: "Ctrl+W"
        onActivated: {
            browser.ctrlKeyPressing = false
            closeTab(currentIndex())
        }
    }
}
