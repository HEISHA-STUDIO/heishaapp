/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


import QtQuick 2.3

import QGroundControl               1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Controls      1.0
import QGroundControl.Palette       1.0
import QGroundControl.Vehicle       1.0

Item {
    //property bool useLightColors - Must be passed in from loaded

    Timer {
        interval:   500  // 2Hz, same as real joystick rate    2
        running:    _activeVehicle //QGroundControl.settingsManager.appSettings.virtualJoystick.value && _activeVehicle
        repeat:     true
        onTriggered: {
            if (_activeVehicle) {
                _activeVehicle.virtualTabletJoystickValue(rightStick.xAxis, rightStick.yAxis, leftStick.xAxis, leftStick.yAxis)
            }
        }
    }



    JoystickThumbPad {
        id:                     leftStick
        anchors.leftMargin:     xPositionDelta
        anchors.bottomMargin:   -yPositionDelta
        anchors.left:           parent.left
        anchors.bottom:         parent.bottom
        width:                  140//parent.height
        height:                 140//parent.height
        yAxisThrottle:          true
        lightColors:            useLightColors
        visible:                false
    }

    JoystickThumbPad {
        id:                     rightStick
        anchors.rightMargin:    -xPositionDelta
        anchors.bottomMargin:   -yPositionDelta
        anchors.right:          parent.right
        anchors.bottom:         parent.bottom
        width:                  140//parent.height
        height:                 140//parent.height
        lightColors:            useLightColors
        visible:        true
    }
/*
    Grid {
        id:                      valuesGrid
     //   anchors.horizontalCenter: parent.verticalCenter
        anchors.verticalCenter:   parent.verticalCenter
        anchors.topMargin:     ScreenTools.defaultFontPixelWidth / 2
        anchors.top:           rightStick.bottom
        anchors.right:          parent.right

        columns:        2
        columnSpacing:  ScreenTools.defaultFontPixelWidth
        visible:        true

        QGCLabel {
            id:         value1
         //   visible:                    activeVehicle && !isNaN(activeVehicle.gps.hdop.value)&&_activeVehicle.systemDronelinkStatus
            color:                      mapPal.text
            text:                      rightStick.xAxis
        }

        QGCLabel {
            id:         value2
         //   visible:    activeVehicle && !isNaN(activeVehicle.gps.hdop.value)&&_activeVehicle.systemDronelinkStatus
            color:      mapPal.text
            text:       rightStick.yAxis
        }
    }
    */

}
