import QtQuick 2.9

BrowserAddressBarForm {
    id: form
    property int progress: 0
    property string url: "url"
    property string title: "title"
    signal userEntersUrl(string url)

    progressBar.opacity: 0.3
    progressBar.width: 0

    function update(url, title) {
        textField.focus = false
        form.url = url
        form.title = title
        if (title !== "") {
            textField.text = title
            textField.horizontalAlignment = Text.AlignHCenter
        } else {
            textField.text = url
            textField.horizontalAlignment = Text.AlignHCenter
//            textField.horizontalAlignment = Text.AlignLeft
            textField.ensureVisible(0)
        }
    }

    textField.onAccepted: {
        var url = textField.text
        var exp = new RegExp("http://|https://")
        if (!exp.test(url)) {
            url = "http://www.google.com/search?query=" + url
        }
        textField.focus = false
        userEntersUrl(url)
    }

    textField.onFocusChanged: {
        if (textField.focus) {
            textField.horizontalAlignment = Text.AlignHCenter
            textField.text = this.url
            textField.ensureVisible(0)
            textField.selectAll()
        } else {
            textField.deselect()
            textField.horizontalAlignment = Text.AlignHCenter
            textField.text = this.title
        }
    }

    onProgressChanged: {
        if (progress === 0) {
            barWidthAnimation.enabled = true
            progressBar.opacity = 0.3
        } else if (progress === 100) {
            fadeProgress.start()
        } else {
        }
        var newW = progress/100 * textField.width
        progressBar.width = newW
        console.log("progress=", progress, "newW=",newW,"progressBar.width=",progressBar.width, "progressBar.opacity=", progressBar.opacity, "barWidthAnimation.enabled=", barWidthAnimation.enabled)
    }

    Behavior on progressBar.width {
        id: barWidthAnimation
        enabled: progress < 100
        SmoothedAnimation {
            duration: 100
        }
    }


    PropertyAnimation {
        id: fadeProgress
        target: progressBar
        properties: "opacity"
        to: 0
        duration: 1000
        onStopped: {
            progressBar.width = 0
        }
    }

    Shortcut {
        sequence: "Ctrl+Space"
        onActivated: {
            textField.focus = true
            textField.selectAll()
        }
    }

}
