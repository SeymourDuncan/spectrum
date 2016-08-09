import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3


ApplicationWindow {
    id: mainWindow
    objectName: "mainWindow"
    visible: true
    width: 500
    height: 700
    title: "Spectrum"


    style: ApplicationWindowStyle {
           background: BorderImage {
               border { left: 20; top: 20; right: 20; bottom: 20 }
           }
       }

    Action{
        id: connectDBAct
        text: "Connect"
        iconSource: "icons/connect.png"
        onTriggered:
            connectorView.visible = true;            
    }

    Action{
        id: disconnectDBAct
        text: "Disconnect"
        enabled: false
        onTriggered: {
            ConnectionData.doDisconnect();
            disconnectDBAct.enabled = false;
        }
    }
    Connections{
        target: connectorView
        onConnected: {
            disconnectDBAct.enabled = true;
        }
    }

    menuBar: MenuBar{
        Menu{
            id: dbMenu
            objectName: "dbMenu"
            title: "Database"

            MenuItem {
                action: connectDBAct
            }
            MenuItem {
                action: disconnectDBAct
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

    ConnectorView{
        id: connectorView
        objectName: "connectorView"
        visible: false;

        signal connected();

        function setValuesToDef(){
            host = ConnectionData.Host;
            port = ConnectionData.Port;
            dbName = ConnectionData.Database;
            user = ConnectionData.User;
            password = ConnectionData.Password;
        }

        onAccepted: {
            ConnectionData.Host = host;
            ConnectionData.Port = port;
            ConnectionData.Database = dbName;
            ConnectionData.User = user;
            ConnectionData.Password = password;

            if (ConnectionData.doConnect()){
                connected();
                statusBar.text = "Connected"
            }
            else{
                statusBar.text = "Connection failed"
            }
        }

        onVisibleChanged: {
            if (visible){
                setValuesToDef();
            }
        }
    }

}
