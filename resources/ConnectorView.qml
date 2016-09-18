import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Dialogs 1.2
import "controls"

Dialog{
    id: connectorView
    objectName: "connectorView"
    width: 350
    height: 250
    visible: false
    title: "Connection to DB..."

    property alias host: tiHost.text
    property alias port: tiPort.text
    property alias dbName: tiDB.text
    property alias user: tiUser.text
    property alias password: tiPassword.text

    Grid{
        id: contentArea
        anchors.centerIn: parent
        rows: 5
        columns: 2
        spacing: 10

        Label{
            text: "Host:"
        }
        TextItemBorder{
            id: tiHost
            objectName: "tiHost"
            onTextChanged: {
                connectorView.host = tiHost.text
            }
        }

        Label{
            text: "Port:"
        }

        TextItemBorder{
            id: tiPort
            objectName: "tiPort"
            onTextChanged: {
                connectorView.port = tiPort.text;
            }
        }

        Label{
            text: "Database:"
        }

        TextItemBorder{
            id: tiDB
            objectName: "tiDB"
            onTextChanged: {
                connectorView.dbName = tiDB.text
            }
        }

        Label{
            text: "User:"
        }
        TextItemBorder{
            id: tiUser
            objectName: "tiUser"
            onTextChanged: {
                connectorView.user = tiUser.text
            }
        }
        Label{
            text: "Password:"
        }
        TextItemBorder{
            id: tiPassword
            objectName: "tiPassword"
            isPassword: true;
            onTextChanged: {
                connectorView.password = tiPassword.text;
            }
        }
   }

    standardButtons: StandardButton.Ok | StandardButton.Cancel
}


