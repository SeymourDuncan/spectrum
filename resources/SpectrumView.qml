import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import "controls"

Item {
    GroupBox{
        title: "Data view"
//        title: height + ':' + implicitHeight
//        title: width + ':' + implicitWidth
        anchors.fill: parent
        clip: true
        GridLayout{
            /* Если использовать Layout.fillHeight или fillWidth, то
            implicit размеры компонента будут основой для определения пропорций
            */

            id: mainGrid
            anchors.fill: parent
            anchors.topMargin: 5
            rows: 2
            columns: 2

            // комбобоксы
            GroupBox{
                id: layers
                title: "Object selection"
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                Layout.row: 1
                Layout.column: 1
                // хак. Через bottomMargin не вышло
                Component.onCompleted: {
                    implicitHeight += 7;
                }
                Grid{
                    //Component.onCompleted: console.log(implicitHeight)
                    anchors.top: parent.top
                    anchors.topMargin: 7
                    columns: 2
                    rows: 3
                    spacing: 10
                    Label{
                        text: "System:"
                    }
                    ComboBox{
                        id: cbSystem
                        height: 25
                        model: ListModel {
                                  id: model1
                                  ListElement { text: "Banana";}
                                  ListElement { text: "Apple"; }
                                  ListElement { text: "Coconut";}
                        }
                    }
                    Label{
                        text: "Class:"
                    }
                    ComboBox{
                        id: cbClass
                        height: 25
                        model: ListModel {
                                  id: model2
                                  ListElement { text: "Banana";}
                                  ListElement { text: "Apple"; }
                                  ListElement { text: "Coconut";}
                        }
                    }
                    Label{
                        text: "Object:"
                    }
                    ComboBox{
                        id: cbObject
                        height: 25
                        model: ListModel {
                                  id: model3
                                  ListElement { text: "Banana";}
                                  ListElement { text: "Apple"; }
                                  ListElement { text: "Coconut";}
                        }
                    }
                }
            }

            // таблица со значениями
            GroupBox{
//                title: height + ':' + implicitHeight
                title: "Values table"
                Layout.fillWidth: true
                Layout.columnSpan: 1
                Layout.rowSpan: 1
                Layout.row: 1
                Layout.column: 2
                // если не поставить руками, то implicitWidth будет забинжена с width
                // при растяжении implicitWidth увеличится, а при сжатии не сузится, потому что
                // implicitWidth - предпочитаемая толщина, что-то вроде min:
                // http://doc.qt.io/qt-5/qtquicklayouts-overview.html. Пиздец кароче
                implicitWidth: 50

                // иначе хз как.
                height: layers.height
                VerticalView{
                    id: vertView
                    anchors.topMargin: 5
                    headerText1: "X"
                    headerText2: "Y"
                    anchors.fill: parent
                }
            }

            // графики
            GroupBox{
                id: gbCharts
//                title: height + ':' + implicitHeight
                title: "Charts"

                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.columnSpan: 2
                Layout.column: 1
                Layout.row: 2

                // та же история что и у значений
                implicitWidth: 50

                Rectangle{
                    color: "plum"
                    anchors.fill: parent
                }
            }

        }
    }
}
