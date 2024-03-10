import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Basic.impl
import QtQuick.Controls.Material.impl

import PassGenerator.Generator
import "./resources"

ApplicationWindow {
    id: mainwindow
    width: 400
    height: 480
    visible: true
    title: qsTr("Password Generator")

    Material.theme: Material.Dark
    Material.accent: Material.Blue

    Generator {
        id: generator
        objectName: "generator"
    }

    // message text with icon and autoclose
    AutoClosePopup {
        id: msgpopup
        objectName: "popup"
        iconsource: "qrc:/icons/warning"
        msgtext: "Too weak password!"
        focus: false
    }

    Popup {
        id: easteregg
        parent: Overlay.overlay
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) / 2)

        contentItem: AnimatedImage {
            source: "qrc:/images/data.dat"
        }
        background: Rectangle {
            color: "transparent"
        }
        modal: false
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            anchors.margins: -10
            onClicked: easteregg.close()
        }
    }

    Shortcut {
        sequence: "Ctrl+M"
        onActivated: easteregg.open()
    }

    Popup {
        id: pipopopup
        parent: Overlay.overlay
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) / 2)

        contentItem: Frame {
            id: imageFrame
            property var imageSources: ["qrc:/images/egg.dat", "qrc:/images/egg2.dat"]

            Image {
                id: img1
                source:  imageFrame.imageSources[0]
                smooth: true
                scale: 1
                opacity: 1
                anchors.fill: parent

                SequentialAnimation {
                    id: pipofade
                    running: false
                    loops: Animation.Infinite
                    onStopped: { img1.source = imageFrame.imageSources[0]; img1.opacity = 1 }
                    NumberAnimation { target: img1; property: "opacity"; to: 0; duration: 2500 }
                    PropertyAction { target: img1; property: "source"; value: imageFrame.imageSources[1] }
                    NumberAnimation { target: img1; property: "opacity"; to: 1; duration: 2500 }
                    NumberAnimation { target: img1; property: "opacity"; to: 0; duration: 2500 }
                    PropertyAction { target: img1; property: "source"; value: imageFrame.imageSources[0] }
                    NumberAnimation { target: img1; property: "opacity"; to: 1; duration: 2500 }
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            anchors.margins: -10
            hoverEnabled: true         //this line will enable mouseArea.containsMouse
            onClicked: pipopopup.close()
        }

        modal: false
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        onClosed: pipofade.stop()
    }

    Shortcut {
        sequence: "Ctrl+P"
        onActivated: {pipopopup.open(); pipofade.start()}
    }

    ColumnLayout {
        spacing: 20
        // Layout.alignment: Qt.AlignHCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 12
        anchors.verticalCenter: parent.verticalCenter

        Row {
            spacing: 12
            Layout.alignment: Qt.AlignHCenter

            Text {
                id: lengthTxt
                text: "Length:"
                font.pointSize: 12
                font.weight: Font.Medium
                color: Material.secondaryTextColor
                anchors.verticalCenter: parent.verticalCenter
            }
            SpinBox {
                id: lengthInput
                from: 4
                value: 8
                to: 32
                font.pointSize: 12
                editable: true
            }
        }

        Frame {
            id: fOptions
            // Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true

            background: Rectangle {
                radius: 4
                color: fOptions.Material.elevation > 0 ? fOptions.Material.backgroundColor : "transparent"
                border.color: fOptions.Material.frameColor

                layer.enabled: fOptions.enabled && fOptions.Material.elevation > 0
                layer.effect: ElevationEffect {
                    elevation: fOptions.Material.elevation
                }
            }

            Grid {
                columns: 2
                rowSpacing: 12
                columnSpacing: 24
                anchors.horizontalCenter: parent.horizontalCenter

                CheckBox {
                    text: "Lower case"
                    font.pointSize: 12
                    checked: true
                    onCheckedChanged: generator.onLowerChanged(checked)
                }
                CheckBox {
                    text: "Upper case"
                    font.pointSize: 12
                    checked: false
                    onCheckedChanged: generator.onUpperChanged(checked)
                }
                CheckBox {
                    text: "Numbers"
                    font.pointSize: 12
                    checked: false
                    onCheckedChanged: generator.onNumberChanged(checked)
                }
                CheckBox {
                    text: "Symbols"
                    font.pointSize: 12
                    checked: false
                    onCheckedChanged: generator.onSymbolChanged(checked)
                }
            }
        }

        Text {
            text: "Generated password:"
            font.pointSize: 14
            font.weight: Font.DemiBold
            color: Material.secondaryTextColor
            Layout.alignment: Qt.AlignHCenter
        }

        TextField {
            id: generatedPassword
            objectName: "generatedPassword"
            Layout.alignment: Qt.AlignHCenter
            readOnly: false
            font: fixedFont
            Layout.fillWidth: true
            horizontalAlignment: TextInput.AlignHCenter
            onTextEdited: generator.onTextEdited(text)
        }

        GridLayout {
            columns: 3
            Layout.fillWidth: true

            ProgressBar {
                id: weakbar
                from: 0
                value: 0
                to: 100
                Layout.fillWidth: true
                objectName: "weakbar"
                contentItem: ProgressBarImpl {
                    implicitHeight: 6
                    implicitWidth: 116
                    scale: weakbar.mirrored ? -1 : 1
                    progress: weakbar.position
                    indeterminate: weakbar.visible && weakbar.indeterminate
                    color: "#ee9090"
                }
            }
            ProgressBar {
                id: mediumbar
                from: 0
                value: 0
                to: 100
                Layout.fillWidth: true
                objectName: "mediumbar"
                contentItem: ProgressBarImpl {
                    implicitHeight: 6
                    implicitWidth: 116
                    scale: mediumbar.mirrored ? -1 : 1
                    progress: mediumbar.position
                    indeterminate: mediumbar.visible && mediumbar.indeterminate
                    color: "#eeee90"
                }
            }
            ProgressBar {
                id: strongbar
                from: 0
                value: 0
                to: 100
                Layout.fillWidth: true
                objectName: "strongbar"
                contentItem: ProgressBarImpl {
                    implicitHeight: 6
                    implicitWidth: 116
                    scale: strongbar.mirrored ? -1 : 1
                    progress: strongbar.position
                    indeterminate: strongbar.visible && strongbar.indeterminate
                    color: "#90ee90"
                }
            }

            Text {
                text: "weak"
                font.pointSize: 12
                color: Material.secondaryTextColor
                Layout.alignment: Qt.AlignHCenter
            }
            Text {
                text: "medium"
                font.pointSize: 12
                color: Material.secondaryTextColor
                Layout.alignment: Qt.AlignHCenter
            }
            Text {
                text: "strong"
                font.pointSize: 12
                color: Material.secondaryTextColor
                Layout.alignment: Qt.AlignHCenter
            }
        }

        Row {
            spacing: 60
            Layout.alignment: Qt.AlignHCenter
            // Layout.fillWidth: true
            Button {
                id: genBtn
                text: "Generate"
                font.pointSize: 12
                onPressed: generator.generatePassword(lengthInput.value)
            }

            Button {
                text: "Copy"
                width: genBtn.width
                font.pointSize: 12
                onPressed: {generator.copy(generatedPassword.text);
                    if (generator.isWeakPassword()) { msgpopup.open() } }

                ToolTip.delay: 2500
                ToolTip.timeout: 5000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Copy password to clipboard")
            }
        }
    }
}
