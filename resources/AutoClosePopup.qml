import QtQuick
import QtQuick.Controls.Material

Popup {
    id: popup
    parent: Overlay.overlay
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)
    property var iconsource
    property var msgtext

    contentItem: Row {
        spacing: 12
        IconImage {
            source: popup.iconsource
            width: 30
            height: 30
        }
        Text {
            id: popuptext
            text: popup.msgtext
            font.pointSize: 12
            color: Material.secondaryTextColor
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    modal: false
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    // auto close the popup
    Timer {
        id: popuptimer
        interval: 2000
        running: false
        repeat: false
        onTriggered: popup.close()
    }
    onOpened: popuptimer.start()
}
