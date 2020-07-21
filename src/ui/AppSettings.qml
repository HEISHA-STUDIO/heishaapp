/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


import QtQuick          2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts  1.2

import QGroundControl               1.0
import QGroundControl.Palette       1.0
import QGroundControl.Controls      1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.MultiVehicleManager   1.0


Rectangle {
    property var  _activeVehicle:       QGroundControl.multiVehicleManager.activeVehicle

    id:     settingsView
    color:  qgcPal.window
    z:      QGroundControl.zOrderTopMost

    signal    ipconnectbutton

    readonly property real _defaultTextHeight:  ScreenTools.defaultFontPixelHeight
    readonly property real _defaultTextWidth:   ScreenTools.defaultFontPixelWidth
    readonly property real _horizontalMargin:   _defaultTextWidth / 2
    readonly property real _verticalMargin:     _defaultTextHeight / 2
    readonly property real _buttonHeight:       ScreenTools.isTinyScreen ? ScreenTools.defaultFontPixelHeight * 3 : ScreenTools.defaultFontPixelHeight * 2

    property bool _first: true

    QGCPalette { id: qgcPal }

    Component.onCompleted: {
        //-- Default Settings
        __rightPanel.source = QGroundControl.corePlugin.settingsPages[QGroundControl.corePlugin.defaultSettings].url
    }

    QGCFlickable {
        id:                 buttonList
        width:              buttonColumn.width
        anchors.topMargin:  _verticalMargin*2
        anchors.top:        parent.top
        anchors.bottomMargin:  _verticalMargin
        anchors.bottom:     parent.bottom
        anchors.leftMargin: _horizontalMargin
        anchors.left:       parent.left
        contentHeight:      buttonColumn.height + _verticalMargin
        flickableDirection: Flickable.VerticalFlick
        clip:               true

        ExclusiveGroup { id: panelActionGroup }

        ColumnLayout {
            id:         buttonColumn
            spacing:    _verticalMargin

            property real _maxButtonWidth: 0

            QGCLabel {
                Layout.fillWidth:       true
                text:                   qsTr("Application Settings")
                wrapMode:               Text.WordWrap
                horizontalAlignment:    Text.AlignHCenter
                visible:                !ScreenTools.isShortScreen
            }

            Repeater {
                model:  QGroundControl.corePlugin.settingsPages
                QGCButton {
                    height:             _buttonHeight
                    text:               modelData.title
                    exclusiveGroup:     panelActionGroup
                    Layout.fillWidth:   true
                    enabled:            _activeVehicle
               //     visible:            false

                    onClicked: {
                        if(__rightPanel.source !== modelData.url) {
                            __rightPanel.source = modelData.url
                        }

                     //   checked = true
                    }
/*
                    Component.onCompleted: {
                        if(_first) {
                            _first = false
                            checked = true
                        }
                    }
*/
                }
            }
        }




    }

    QGCButton {
        id :              _check_start
        width:              buttonColumn.width
        anchors.bottomMargin:  _verticalMargin*6
     // anchors.top:        parent.top
        anchors.bottom:     parent.bottom
        anchors.leftMargin: _horizontalMargin
        anchors.left:       parent.left
        height:             _buttonHeight
        text:               "start"
    //    onClicked: {

    //    }
         onClicked:     {
              __rightPanel.item.saveSettings()
             QGroundControl.settingsManager.appSettings.sendip()
        //     console.log("Got saveSettings:")

         }
     //    Component.onCompleted: {

     //   }
    }


    Rectangle {
        id:                     divider
        anchors.topMargin:      _verticalMargin
        anchors.bottomMargin:   _verticalMargin
        anchors.leftMargin:     _horizontalMargin
        anchors.left:           buttonList.right
        anchors.top:            parent.top
        anchors.bottom:         parent.bottom
        width:                  1
        color:                  qgcPal.windowShade
    }


    QGCFlickable {
        id:                 buttonList1
        width:              buttonColumn.width
        anchors.topMargin:  _verticalMargin
        anchors.top:        parent.top
        anchors.bottom:     parent.bottom
        anchors.rightMargin: _horizontalMargin
        anchors.right:       parent.right
        contentHeight:      buttonColumn.height + _verticalMargin
        flickableDirection: Flickable.VerticalFlick
        clip:               true

        ExclusiveGroup { id: panelActionGroup1 }

        ColumnLayout {
            id:         buttonColumn1
            spacing:    _verticalMargin

            property real _maxButtonWidth: 0

            QGCLabel {
                Layout.fillWidth:       true
                text:                   qsTr("Application Settings")
                wrapMode:               Text.WordWrap
                horizontalAlignment:    Text.AlignHCenter
                visible:                !ScreenTools.isShortScreen
            }

              //  spacing: _verticalMargin


/*
            Repeater {
                model:  QGroundControl.corePlugin.settingsPages
                QGCButton {
                    height:             _buttonHeight
                    text:               modelData.title
                    exclusiveGroup:     panelActionGroup1
                    Layout.fillWidth:   true

                    onClicked: {
                        if(__rightPanel.source !== modelData.url) {
                            __rightPanel.source = modelData.url
                        }
                        checked = true
                    }

                    Component.onCompleted: {
                        if(_first) {
                            _first = false
                            checked = true
                        }
                    }
                }
            }
            */
        }

    }


    //-- Panel Contents
    Loader {
        id:                     __rightPanel
        anchors.leftMargin:     _horizontalMargin
        anchors.rightMargin:    _horizontalMargin
        anchors.topMargin:      _verticalMargin
        anchors.bottomMargin:   _verticalMargin
        anchors.left:           divider.right
        anchors.right:          parent.right
        anchors.top:            parent.top
        anchors.bottom:         parent.bottom
    }
}

