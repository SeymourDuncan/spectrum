import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import com.types.datamodel 1.0
import QtCharts 2.0
import "controls"

Item {
    property var context: null
    // сам объект контекста всегда один и тот же
    // onUpdateContext вызывается при переподключении
    // т.к. нужно обновить модели

    // по сути контекст должен полностью управлять вьюшкой
    signal updateContext(bool val);

    // TODO при переподключении нужно обновлять модель как бы
    onUpdateContext: {
        if (!context)
            return;
        if (val){
            cbSystem.model = context.GetSystemList()
            cbSystem.currentIndex = 0;
            cbSystem.activated(0);
        }
        else{
            // QMLFUN. Сналача нужно нулить модель, а только потом выставлять
            // индексы в -1. Потому что если надоборот, то сначала сделаем
            // индекс и текст -1 и '', а при зануление получится 0 и ''.
            // И если далее дать модель, то выйдет что первый элемент УЖЕ выбран,
            // и текст у него ''.
            // А вот еще:
            // Changing the model after initialization will reset currentIndex to 0.
            // Разгадал загадку!! Всю сука субботу разгадывал.
            // "Changing" не случается если элементы в модели не поменялись!!!
            cbSystem.model = null
            cbClass.model = null
            cbObject.model = null

            cbSystem.currentIndex = -1
            cbClass.currentIndex = -1
            cbObject.currentIndex = -1
        }

    }

    GroupBox{
        title: "Data view"
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
                    anchors.top: parent.top
                    anchors.topMargin: 7
                    columns: 2
                    rows: 3
                    spacing: 10

                    Label{
                        text: "System:"
                    }
                    // Система
                    ComboBox{
                        id: cbSystem
                        height: 25
                        onActivated:{
                            context.SelectSystem(index);                            
                        }
                    }
                    Label{
                        text: "Class:"
                    }
                    // Класс
                    ComboBox{
                        id: cbClass
                        height: 25
                        onActivated:{
                            context.SelectClass(index);
                        }

                        Connections{
                            target: cbSystem
                            onActivated:{
                                cbClass.currentIndex = -1
                                cbClass.model = context.GetClassList();
                                cbClass.activated(cbClass.currentIndex);
                            }
                        }

                    }
                    Label{
                        text: "Object:"
                    }

                    // Объект
                    ComboBox{
                        id: cbObject
                        height: 25
                        onActivated:{
                            context.SelectObject(index);
                        }
                        Connections{
                            target: cbClass
                            onActivated:{
                                cbObject.currentIndex = -1
                                cbObject.model = context.GetObjectList();
                                cbObject.activated(cbObject.index);
                            }
                        }
                    }
                }
            }

            // таблица со значениями
            GroupBox{
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

                    // поменялся выбранный объект
                    Connections{
                        target: cbObject
                        onActivated:{
                            vertView.model = context.GetSpectrumValuesModel();
                        }
                    }
                }
            }

            // графики
            GroupBox{
                id: gbCharts
//                title: implicitWidth + ' ' + implicitHeight
//                title: Layout.preferredWidth + ' ' + Layout.preferredHeight
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.columnSpan: 2
                Layout.column: 1
                Layout.row: 2

                // та же история что и у значений
//                Layout.preferredWidth:  50
//                Layout.preferredHeight: 0

//                Rectangle{
//                    color: "plum"
//                    anchors.fill: parent
//                }
                implicitHeight: 50;
                implicitWidth: 50;

                ChartView {
                    id: mainChart
//                    title: "Line"
                    // если пустой, то растягивается, но без шкалы
                    anchors.fill: parent
                    antialiasing: true
                    legend.visible: false

                    property real xMax: 300;
                    property real yMax: 300

                    ValueAxis {
                        id: axisX
                        min: 0
                        max: mainChart.xMax
                        tickCount: 5
                    }

                    ValueAxis {
                        id: axisY
                        min: 0
                        max: mainChart.yMax
                    }

                    LineSeries {
                        id: lineSeries
                        name: "Current spectrum"

                        axisX: axisX
                        axisY: axisY
                    }

                    Connections{
                        target: cbObject
                        onActivated:{
                            lineSeries.clear();
                            var lvalues = context.GetLVector();
                            var kvalues = context.GetKVector();

                            mainChart.legend.visible = lvalues.length > 0

                            var xmax = 0;
                            var ymax = 0;
                            for (var i = 0; i < lvalues.length; ++i){
//                                console.log(lvalues[i]);
//                                console.log(kvalues[i]);
                                lineSeries.append(lvalues[i], kvalues[i])
                                xmax = (xmax < lvalues[i]) ? lvalues[i] : xmax;
                                ymax = (ymax < kvalues[i]) ? kvalues[i] : ymax
                            }
                            mainChart.xMax = xmax + 0.1 * xmax ;
                            mainChart.yMax = ymax + 0.1 * ymax ;
                        }
                    }
                }
            }

        }
    }
}
