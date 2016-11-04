import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import "loader"

ApplicationWindow {
    id: mainWindow
    objectName: "mainWindow"
    visible: true
    width: 700
    height: 700
    title: "Spectrum"

    // global var for some dyn created components
    property var component;
    property var loader;

    function finishLoaderCreation(){
        if (component.status == Component.Ready) {
               loader = component.createObject(mainWindow);
               if (loader == null) {
                   console.log("Error creating object");
               }
               else{
                   loader.modality = Qt.WindowModal;
                   loader.show();
               }
           } else if (component.status == Component.Error) {
               console.log("Error loading component:", component.errorString());
        }
    }

    SpectrumView{
        id: spectrView

        anchors.fill: parent
        width: 500
        height: 400

        Component.onCompleted: {
            context = DataContainerContext
        }

        Connections{
            target: DBConnector
            onConnectionStatusChanged : spectrView.updateContext(val)
        }
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

    Action{
        id: loaderAct
        text: "Loader"
        iconSource: "/resources/icons/loadSpectrums.png"
        onTriggered:
        {
            component = Qt.createComponent("loader/loaderWindow.qml", Component.Asynchronous);
            if (component.status == Component.Ready)
                finishLoaderCreation();
            else
                component.statusChanged.connect(finishLoaderCreation);
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
                action: loaderAct
            }
        }
    }

    toolBar: ToolBar {
        RowLayout {
            spacing: 5
            ToolButton {
                action: connectDBAct
            }
            ToolButton {
                action: loaderAct
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
