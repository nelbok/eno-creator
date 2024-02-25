import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import Eno

Item {
    id: root
    property bool running: false
    property var model: MyIntro.scene.objects

    function open() {
        root.running = true;
        fileDialog.open();
    }

    MyLoader {
        id: myLoader

        function openProject(filePath) {
            progressBar.visible = true
            root.model = MyIntro.scene.objects;
            init(MyProject, MyLoader.Type.Load, filePath);
            start();
        }

        onFinished: {
            if (result() === MyLoader.Result.Success)
                root.model = MyProject.scene.objects;
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
