import QtQuick 2.0

Rectangle{
    property alias text: txtIn.text
    property bool isPassword: false;   

    implicitWidth: 150;
    implicitHeight: 21;
    border.width: 1

    onIsPasswordChanged: {
        txtIn.echoMode = isPassword ?
                    TextInput.Password:
                    TextInput.Normal
    }
    TextInput{
        id: txtIn
        selectByMouse: true
        width: parent.width-16
        anchors.centerIn: parent
        clip: true;
        cursorVisible: false        
    }
    MouseArea{
        anchors.fill: parent
        hoverEnabled : true
        propagateComposedEvents: true

        onEntered: {
                if(txtIn.focus){
                    cursorShape = Qt.IBeamCursor
                }
            }
        onExited: {
                    cursorShape = Qt.ArrowCursor
            }
        onPressed: {
            cursorShape = Qt.IBeamCursor
            mouse.accepted = false
        }

    }
}
