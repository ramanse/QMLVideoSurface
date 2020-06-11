import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import Entertainment 1.0
Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    ListView {
        id: mediaList
        anchors.fill: parent
        highlightFollowsCurrentItem: true
        highlightRangeMode: ListView.StrictlyEnforceRange
        highlightMoveDuration: 1000
        highlightMoveVelocity: -1
        model: entertainment.currentAudioSource === 5?entertainment.currentPlayList:entertainment.currentStationsList
        delegate: Item{
            width: parent.width
            height: 100
            Image{
                anchors.left: parent.left
                anchors.top: parent.top
                width:100
                height: 100
                source: "file:///D:/GSEP/UI-Simu/projects/SimuMediaCovers/"+coverHash
            }

            Text {
                anchors.centerIn: parent
                text:  title + ", " + album +"\n" +   artist + ", " + playTime}
            MouseArea{
                anchors.fill: parent
                onClicked: {mediaList.currentIndex = index
                    if(index !== entertainment.curentListIndex && index >= 0)
                         entertainment.updatePlayListwithICIndex(index);}
            }
        }

    }
    HuEntertainment{
       id: entertainment
        onCurrentListIndexChanged:{
            mediaList.currentIndex = entertainment.curentListIndex
        }
//        onCurrentAudioSourceChanged:{
//            if(entertainment.currentAudioSource === 5)
//                mediaList.model = entertainment.currentPlayList
//            else
//                mediaList.model = entertainment.currentStationsList

//        }
    }
    Button {
        id: btn
        text: entertainment.currentAudioSource === 5?"Media":"Radio"
        onClicked: entertainment.currentAudioSource === 5?entertainment.updateAudioSource(0):
                                                             entertainment.updateAudioSource(5)
    }
    Text {
        anchors.left: btn.right
        anchors.top: btn.top
        text:  "current play time" + entertainment.currentPlayTime
    }

    SimuVideo{
        width: 200
        height: 200
        anchors.right: parent.right
        anchors.top: parent.top
    }


}
