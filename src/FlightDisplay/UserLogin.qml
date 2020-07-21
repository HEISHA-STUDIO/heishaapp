/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick                  2.3
import QtQuick.Controls         1.2

import QGroundControl.FlightDisplay 1.0

import QGroundControl               1.0
import QGroundControl.Controls      1.0
import QGroundControl.Vehicle       1.0
import QGroundControl.MultiVehicleManager   1.0
import QGroundControl.Palette               1.0
import QGroundControl.Controllers           1.0
import QGroundControl.SettingsManager       1.0
import QGroundControl.ScreenTools           1.0

/// Altitude slider for guided change altitude command
Rectangle {
    id:             _root
  //   radius:             width / 2
    width:          ScreenTools.defaultFontPixelWidth*36
    height:         ScreenTools.defaultFontPixelHeight*12
    radius:         ScreenTools.defaultFontPixelHeight / 2
    color:           "black"//qgcPal.window
    border.color:     Qt.rgba(0,0,0,0.75)// qgcPal.windowShade
 //   border.width:   _emergencyAction ? 4 : 1
 //  z:              guidedController.z
     opacity:            0.8
     visible:      _userloginshow //  QGroundControl.settingsManager.appSettings.userloginshow //

      property bool _userloginshow:   false
      property int _firstColumn:      ScreenTools.defaultFontPixelWidth * 16 //ScreenTools.defaultFontPixelWidth * 12
      property int _secondColumn:     ScreenTools.defaultFontPixelWidth * 30
      property var    _activeVehicle:         QGroundControl.multiVehicleManager.activeVehicle

    QGCPalette { id: qgcPal }

    function setuserlogin(){
        console.log("setuserlogin")
                  _userloginshow =true
         QGroundControl.settingsManager.appSettings.userloginshow = true
    }
/*
    function saveSettings(){
           // console.log("Got message:")
            QGroundControl.settingsManager.appSettings.host = hostField.text
            QGroundControl.settingsManager.appSettings.port = parseInt(portField.text)
            QGroundControl.settingsManager.appSettings.topic = topicnameField.text
     }
    */
    function saveSettings(){
           // console.log("Got message:")
            QGroundControl.settingsManager.appSettings.usename = useNameField.text
            QGroundControl.settingsManager.appSettings.password = passwordField.text
     }

    Column {
        id:                 userlog
     //   anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing:            ScreenTools.defaultFontPixelHeight * 0.5
      //  anchors.margins:    ScreenTools.defaultFontPixelWidth
     //   anchors.top:        parent.top
     //   anchors.topMargin:  ScreenTools.defaultFontPixelHeight * 0.5
        anchors.bottomMargin:  ScreenTools.defaultFontPixelHeight * 0.5
        anchors.bottom:       useNameField.top
        opacity:              1

        QGCLabel {
            text:       qsTr("   HEISHA")
            font.pointSize: ScreenTools.largeFontPointSize
            color:     "white"
            opacity:            1
        //    width:      _firstColumn
      //      anchors.verticalCenter: parent.verticalCenter
        }

        QGCLabel {
            text:       qsTr("Command Center")
            color:      "white"
            opacity:            1
        //    width:      _firstColumn
     //       anchors.verticalCenter: parent.verticalCenter
        }

    }
/*
    Column {

        id:                 useloginmain
     //   anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing:            ScreenTools.defaultFontPixelHeight * 0.5
     //   anchors.margins:    ScreenTools.defaultFontPixelWidth
        anchors.bottomMargin:  ScreenTools.defaultFontPixelHeight * 0.5
        anchors.bottom:       hostField.top
        opacity:            1
            QGCTextField {
                id:         useNameField
               text:        QGroundControl.settingsManager.appSettings.usename
               width:      _firstColumn
             //   opacity:            1

            }

            QGCTextField {
                id:         passwordField
                text:       QGroundControl.settingsManager.appSettings.password
                width:      _firstColumn
              //  opacity:            1
              // anchors.verticalCenter: parent.verticalCenter


            }
    }
*/
    LineInput {
        id:         useNameField
    //    text:        QGroundControl.settingsManager.appSettings.host
      //  width:      _firstColumn
     //   opacity:            1
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin:  ScreenTools.defaultFontPixelHeight*0.5
        anchors.bottom:       passwordField.top
        width: _firstColumn//rect1.width * 0.8;
        height:  _check_start.height
        font_size: ScreenTools.defaultFontPixelHeight
        text :   QGroundControl.settingsManager.appSettings.usename
     //   anchors {horizontalCenter: rect1.horizontalCenter; top: rect1.top; topMargin: 8}
        hint: "usename"
   //     anchors.verticalCenter: parent.verticalCenter

    }

    LineInput {
         id:         passwordField
      //  text:        QGroundControl.settingsManager.appSettings.port
     //   width:      _firstColumn
      //  opacity:            1
   //     anchors.verticalCenter: parent.verticalCenter
         anchors.horizontalCenter: parent.horizontalCenter
         anchors.bottomMargin:  ScreenTools.defaultFontPixelHeight*0.5
         anchors.bottom:       _check_start.top
        width: _firstColumn //rect1.width * 0.8;
        height:  _check_start.height
        font_size:  ScreenTools.defaultFontPixelHeight
        text:       QGroundControl.settingsManager.appSettings.password

     //   anchors {horizontalCenter: rect1.horizontalCenter; top: rect1.top; topMargin: 8}
        hint: "password"

    }

    QGCButton {
        id :                    _check_start
        anchors.bottom:          parent.bottom
        anchors.bottomMargin:    ScreenTools.defaultFontPixelHeight
        anchors.horizontalCenter: parent.horizontalCenter
        width:                 _firstColumn
       // anchors.bottomMargin:  _verticalMargin*6
       // anchors.top:           parent.top
      //  color:                qgcPal.windowShade
        //border.color:         qgcPal.colorGreen
        text:                   "LOG IN"
        opacity:            1
         onClicked:     {
               saveSettings()
               QGroundControl.settingsManager.appSettings.getuserlogin()
               _userloginshow = false
           //  QGroundControl.settingsManager.appSettings.userloginshow = false
         }
    }

    QGCColoredImage {
        anchors.margins:    _cancel
        anchors.top:        parent.top
        anchors.right:      parent.right
        width:              ScreenTools.defaultFontPixelHeight
        height:             width
        sourceSize.height:  width
        source:             "/res/XDelete.svg"
        fillMode:           Image.PreserveAspectFit
        color:              "white"
        QGCMouseArea {
            fillItem:   parent
            onClicked: {
                //altitudeSlider.visible = false
                QGroundControl.settingsManager.appSettings.userloginshow = false
                _userloginshow = false
            }
        }
    }
    /*
    function saveSettings(){
           // console.log("Got message:")
            QGroundControl.settingsManager.appSettings.host = hostField.text
            QGroundControl.settingsManager.appSettings.port = parseInt(portField.text)
            QGroundControl.settingsManager.appSettings.topic = topicnameField.text
     }

    Column {
                id:                 useloginmain
             //   anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                spacing:            ScreenTools.defaultFontPixelHeight * 0.5
                anchors.margins:    ScreenTools.defaultFontPixelWidth
                anchors.top:        parent.top
                anchors.topMargin:  ScreenTools.defaultFontPixelHeight * 0.5
            //    anchors.bottomMargin:  ScreenTools.defaultFontPixelHeight * 0.5
             //   anchors.bottom:       _check_start.top

                Row {
                    spacing:        ScreenTools.defaultFontPixelHeight * 0.5
                    QGCLabel {
                        text:       qsTr("IP Address:")
                        width:      _firstColumn
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QGCTextField {
                        id:         hostField
                        text:        QGroundControl.settingsManager.appSettings.host
                        width:      _firstColumn
                        anchors.verticalCenter: parent.verticalCenter

                    }
                }
                Row {
                    spacing:        ScreenTools.defaultFontPixelHeight * 0.5
                    QGCLabel {
                        text:       qsTr("TCP Port:   ")
                        width:      _firstColumn
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QGCTextField {
                        id:         portField
                        text:       QGroundControl.settingsManager.appSettings.port      //  QGroundControl.settingsManager.videoSettings.rtspUrl
                        width:      _firstColumn
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Row {
                    spacing:        ScreenTools.defaultFontPixelHeight * 0.5
                    QGCLabel {
                        text:       qsTr("Device ID:   ")
                        width:      _firstColumn
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QGCTextField {
                        id:         topicnameField
                        text:        QGroundControl.settingsManager.appSettings.topic
                        width:      _firstColumn
                       // inputMethodHints: Qt.ImhFormattedNumbersOnly
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Component.onCompleted: {

                }
        }
     Row {
         id :                  _check_start
         anchors.bottom:          parent.bottom
         anchors.bottomMargin:    ScreenTools.defaultFontPixelHeight
         anchors.horizontalCenter: parent.horizontalCenter
       //  height:                _buttonHeight
         spacing:        ScreenTools.defaultFontPixelHeight*3
         QGCButton {
           //  width:                 buttonColumn.width
            // anchors.bottomMargin:  _verticalMargin*6
          // anchors.top:           parent.top
             text:                   "start"
              onClicked:     {
                   saveSettings()
                   QGroundControl.settingsManager.appSettings.sendip()

                   console.log("Got saveSettings:")
                   _userloginshow = false
              }
         }

         QGCButton {
            // width:                 buttonColumn.width
            // anchors.bottomMargin:  _verticalMargin*6
            // anchors.top:           parent.top
             text:                   "cancal"

              onClicked:     {
               //    QGroundControl.settingsManager.appSettings.sendip()
                   _userloginshow = false
              }
         }
     }
*/






}
