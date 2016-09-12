import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: mainWindow
    objectName: "mainWindow"
    visible: true
    width: 700
    height: 700
    title: "Spectrum"

    SpectrumView{
//        anchors.fill: parent
        width: 500
        height: 400
    }

    style: ApplicationWindowStyle {
           background: BorderImage {
               border { left: 20; top: 20; right: 20; bottom: 20 }
           }
       }

    Action{
        id: connectDBAct
        text: "Connect"
        iconSource: "/resources/icons/connect-your-database.png"
        onTriggered:
            connectorView.visible = true;            
    }

    menuBar: MenuBar{
        Menu{
            id: dbMenu
            objectName: "dbMenu"
            title: "Database"

            MenuItem {
                action: connectDBAct
            }
        }
    }

    toolBar: ToolBar {
        RowLayout {
            spacing: 5
            ToolButton {
                action: connectDBAct
            }
        }
    }

    statusBar: StatusBar{
        id: statusBar
        property alias text: sbLabel.text

        RowLayout{
            anchors.fill: parent
            spacing: 5
            Label {
                id: sbLabel
                text: "Start working with connect to Database"
            }
        }       
    }

    Connections{
        target: DBConnector
        onConnectionStatusChanged : {
            if (val){
                statusBar.text = "Connected"
            }
            else{
                statusBar.text = "Connection failed"
            }
        }
    }

    ConnectorView{
        id: connectorView
        objectName: "connectorView"
        visible: false;

        function setValuesToDef(){
            host = DBConnector.Host;
            port = DBConnector.Port;
            dbName = DBConnector.Database;
            user = DBConnector.User;
            password = DBConnector.Password;
        }

        onAccepted: {
            DBConnector.Host = host;
            DBConnector.Port = port;
            DBConnector.Database = dbName;
            DBConnector.User = user;
            DBConnector.Password = password;
            DBConnector.tryConnect();
        }

        onVisibleChanged: {
            if (visible){
                setValuesToDef();
            }
        }
    }

    BusyIndicator{
        id: mainBzIndicator
        anchors.centerIn: parent
        running: false
    }
    Connections{
        target: ApplicationContext
        onStartBusy:
        {
            mainBzIndicator.running = true
        }
        onFinishBusy: {
            mainBzIndicator.running = false
        }
    }

}
