import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import Eno

Item {
    id: root
    property bool running: false
    property var model: MyIntro

    function open() {
        root.running = true;
        fileDialog.open();
    }

    MyLoader {
        id: myLoader

        function openProject(filePath) {
            progressBar.visible = true
            root.model = MyIntro;
            init(MyProject, MyLoader.Type.Load, filePath);
            start();
        }

        onFinished: {
            if (result() === MyLoader.Result.Success)
                root.model = MyProject;
            progressBar.visible = false
            root.running = false;
        }
    }

    FileDialog {
        id: fileDialog
        nameFilters: ["Eno Project (*.eno)"]
        onAccepted: {
            myLoader.openProject(selectedFile)
        }
        onRejected: {
            root.running = false;
        }
    }

    Rectangle {
        id: progressBar
        visible: false
        anchors.fill: parent
        color: "#AAAAAAAA"

        BusyIndicator {
            anchors.centerIn: parent
            running: true
        }
    }
}
