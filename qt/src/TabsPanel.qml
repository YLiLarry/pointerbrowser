import QtQuick 2.7
import QtQuick.Controls 2.2
import Backend 1.0

TabsPanelForm {
    id: tabsPanel
    signal userOpensNewTab
    signal userClosesTab(int index)
    signal userOpensTab(int index)
    signal userOpensSavedTab(int index)

    tabHeight: 30

    flickable {
        rebound: Transition {
            NumberAnimation {
                properties: "x,y"
                duration: {
                    switch (Qt.platform.os) {
                    case "ios": return 2500; break;
                    default: return 500
                    }
                }
                easing.type: Easing.OutQuint
            }
        }

        boundsBehavior: {
            //            if (Qt.platform.os == "ios") {
            return Flickable.DragAndOvershootBounds
            //            } else {
            //                return Flickable.StopAtBounds
            //            }
        }
    }

    tabsListHeight: TabsModel.count * tabHeight
    searchListHeight: SearchDB.searchResult.count * tabHeight
    tabsList.model: TabsModel
    searchList.model: SearchDB.searchResult

    function filterModelBySymbol(sym) {
        SearchDB.search(sym)
    }

    tabsSearch.onTextEdited: {
        console.log("tabsSearch: ", tabsSearch.text)
        if (tabsSearch.text.length > 1) {
            filterModelBySymbol(tabsSearch.text)
        } else if (tabsSearch.text.length == 0) {
            filterModelBySymbol("")
        }
    }

    tabsSearch.onAccepted: {
        filterModelBySymbol(tabsSearch.text)
    }

    function setCurrentIndex(i) {
        tabsList.setHighlightAt(i)
    }

    Component.onCompleted: {
        searchList.setHighlightAt(-1);
        //        SearchDB.search("")
        console.log("searchList.model", searchList.model, searchList.model.count)
    }

    Connections {
        target: newTabButton
        onClicked: {
            tabsPanel.userOpensNewTab()
        }
    }

    Connections {
        target: tabsList
        onUserClosesTab: {
            userClosesTab(index)
        }
        onUserClicksTab: {
            setCurrentIndex(index)
            userOpensTab(index)
        }
    }

    Connections {
        target: searchList
        onUserDoubleClicksTab: {
            userOpensSavedTab(index)
        }
    }

    Shortcut {
        sequence: "Ctrl+Shift+F"
        onActivated: {
            tabsSearch.forceActiveFocus()
            tabsSearch.selectAll()
        }
    }
}
