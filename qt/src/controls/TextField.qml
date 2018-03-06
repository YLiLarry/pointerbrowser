import QtQuick 2.4
import QtQuick.Templates 2.3 as T
import QtQuick.Controls 2.3

T.TextField {
    id: textfield
    Control {
        id: ctl
    }
    background: Rectangle {
        border.width: 1
        anchors.fill: textField
    }
    topPadding: 5
    leftPadding: 5
}
