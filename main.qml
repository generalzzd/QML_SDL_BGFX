import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Text {
        id: element
        x: 308
        y: 234
        text: qsTr("Text")
        font.pixelSize: 12
    }

    Rectangle {
        id: rectangle
        x: 213
        y: 14
        width: 200
        height: 200
        color: "#ce0202"
    }

    Button {
        id: button
        x: 270
        y: 273
        text: qsTr("Button")

        onClicked: {
            ZDWindow.openGLWindow();
        }
    }
}
