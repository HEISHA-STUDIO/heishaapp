import QtQuick 2.3
import QGroundControl.ScreenTools           1.0

FocusScope {
    id: wrapper

    property alias text: input.text
    property alias hint: hint.text
    property alias prefix: prefix.text
    property int font_size: 18

    signal accepted

    Rectangle {
        anchors.fill: parent
        border.color: "#707070"
        color: "white"
        radius: 4

        Text {
            id: hint
            anchors { fill: parent; leftMargin: 14 }
            verticalAlignment: Text.AlignVCenter
            text: "Enter word"
            font.pixelSize: font_size
            color: "#707070"
            opacity: input.length ? 0 : 1
        }

        Text {
            id: prefix
            anchors { left: parent.left; leftMargin: 14; verticalCenter: parent.verticalCenter }
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: font_size
            color: "#707070"
            opacity: !hint.opacity
        }

        TextInput {
            id: input
            focus: true
            anchors { left: prefix.right; right: parent.right; top: parent.top; bottom: parent.bottom }
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: font_size
            //color: "#707070"
            color: "black"
            onAccepted: wrapper.accepted()
        }
    }
}
