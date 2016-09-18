import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {      
    height: cellHeight * 2 + 18
    property alias model: gvTable.model

    property int cellWidth: 30
    property int cellHeight: 30

    property int count: gvTable.count

    property string headerText1
    property string headerText2

    Component {
        id: cellDelegate
        Item {
            width: cellWidth
            height: cellHeight * 2
            Column {
                 Rectangle {
                    border.width: 1
                    radius: 0
                    width: cellWidth
                    height: cellHeight
                    Text {
                        text: valueL
                        anchors.centerIn: parent
                    }
                 }
                 Rectangle {
                    border.width: 1
                    radius: 0
                    width: cellWidth
                    height: cellHeight
                    clip: true
                    Text {
                        text: valueK.toFixed(1)
                        clip: true
                        anchors.centerIn: parent
                    }
                 }
              }
          }
    }
    ScrollView {
        id: scrollV
        width: parent.width
        height: parent.height
        flickableItem.interactive: true

        ListView{
            id: gvTable            
            delegate: cellDelegate
            orientation: Qt.Horizontal
            snapMode: ListView.SnapOneItem
            anchors.fill: parent
            clip: true
            header: Column{
                id: headerCol
                Rectangle{
                    border.width: 1
                    radius: 2
                    width: cellWidth
                    height: cellHeight
                    color: "#73AED0"
                    Text {
                        text: headerText1
                        anchors.centerIn: parent
                    }
                }
                Rectangle{
                    border.width: 1
                    radius: 2
                    width: cellWidth
                    height: cellHeight
                    color: "#D1AED0"
                    Text{
                        text: headerText2
                        anchors.centerIn: parent
                    }

                }
            }

        }

//        style: ScrollViewStyle {
//            transientScrollBars: true
//        }

    }


}
