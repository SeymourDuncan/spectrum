import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import "../controls"

Window {
    id: mainWind
    title: "Spectrum loader"
    width: 600
    height: 400    

    //property var context: nil;

    function fillFilesModel(list){
       for (var key in list){
            filesModel.append(
                {
                    filename: list[key]
                }
                );
       }
    }

    FileDialog{
        id: fileDialog
        title: "Please choose a directory with csv files"
        folder: shortcuts.home
        selectMultiple: true
        nameFilters: ["CSV (*.csv)"]
        selectedNameFilter: "CSV (*.csv)"
        onAccepted: {
            fillFilesModel(fileUrls)
            filesView.visible = true;
        }
        visible: false;
    }

    ListModel{
        id: filesModel
    }

    GroupBox{
        id: filesGb
        title: "File List"
        width: parent.width  * 0.6
        height: parent.height - 60
        anchors.left: parent.left
        anchors.leftMargin: 10

        ListView{
            Component {
                id: highlight
                Rectangle {
                    width: filesView.width;
                    height: 20
                    y: filesView.currentItem.y;
                    color: "#DDD";
                    Behavior on y { SpringAnimation { spring: 2; damping: 0.1 } }
                }
            }

            id: filesView
            anchors.fill: parent
            focus: true
            visible: false;
            model: filesModel

            delegate:
                Text {
                    id: txt
                    text: filename
                    anchors.left: filesView.left
                    anchors.leftMargin: 4
                    clip: true
                    width: filesView.width
                    height: 20

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            filesView.currentIndex = index
                        }
                    }
                }


            highlight: highlight
            highlightFollowsCurrentItem: false
        }
    }


    Button{
        id: startLoad
        text: "Load to DB"
        width: 150
        height: 50
        anchors{
            left: filesGb.right
            top: filesGb.top
            leftMargin: 50
            topMargin: 50


        }

        action: Action{
            id: loadAct
            onTriggered: {
                var files = [];
                for (var i = 0; i < filesModel.count; ++i){
                    files.push(filesModel.get(i).filename);
                }
                console.log(files);
                console.log(DataLoader);
                DataLoader.LoadFromCsvFiles(files);
            }
        }
    }
    Rectangle{
        id: listBnts
        anchors.top: filesGb.bottom;
        height: 50
        width: filesGb.width * 0.9
        anchors.horizontalCenter: filesGb.horizontalCenter
        Button{
            id: addBtn
            text: "Add"
            width: parent.width * 0.3
            height: 35
            anchors.topMargin: 5
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.03
            implicitWidth: 50

            action: Action{
                id: selFolder;
                onTriggered:{
                    fileDialog.open();
                }
            }
        }
        Button{
            id: clearBtn
            text: "Clear"
            width: parent.width * 0.3
            height: 35
            anchors.topMargin: 5
            anchors.top: parent.top
            anchors.left: addBtn.right
            anchors.leftMargin: parent.width * 0.03
            implicitWidth: 50

            action: Action{
                id: clrFileList;
                onTriggered:{
                    filesModel.clear();
                }
            }
        }
        Button{
            id: delBtn
            text: "Delete"
            width: parent.width * 0.3
            height: 35
            anchors.topMargin: 5
            anchors.top: parent.top
            anchors.left: clearBtn.right
            anchors.leftMargin: parent.width * 0.03
            implicitWidth: 50

            action: Action{
                id: delSelFile;
                onTriggered:{
                    filesModel.remove(filesView.currentIndex);
                }
            }
        }
    }
}
