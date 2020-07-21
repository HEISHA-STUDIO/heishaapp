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
import QtQuick.Controls.Styles  1.4
import QtQuick.Dialogs          1.2
import QtLocation               5.3
import QtPositioning            5.3
import QtQuick.Layouts          1.2
import QtQuick.Window           2.2
import QtQml.Models             2.1

import QGroundControl               1.0
import QGroundControl.FlightDisplay 1.0
import QGroundControl.FlightMap     1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Controls      1.0
import QGroundControl.Palette       1.0
import QGroundControl.Vehicle       1.0
import QGroundControl.Controllers   1.0
import QGroundControl.FactSystem    1.0

/// Flight Display View
QGCView {
    id:             root
    viewPanel:      _panel

    QGCPalette { id: qgcPal; colorGroupEnabled: enabled }

    property alias  guidedController:   guidedActionsController

    property bool activeVehicleJoystickEnabled: _activeVehicle ? _activeVehicle.joystickEnabled : false

    property var    _planMasterController:  masterController
    property var    _missionController:     _planMasterController.missionController
    property var    _geoFenceController:    _planMasterController.geoFenceController
    property var    _rallyPointController:  _planMasterController.rallyPointController
    property var    _activeVehicle:         QGroundControl.multiVehicleManager.activeVehicle
    property bool   _mainIsMap:             QGroundControl.videoManager.hasVideo ? QGroundControl.loadBoolGlobalSetting(_mainIsMapKey,  true) : true
    property bool   _isPipVisible:          QGroundControl.videoManager.hasVideo ? QGroundControl.loadBoolGlobalSetting(_PIPVisibleKey, true) : false
    property bool   _useChecklist:          QGroundControl.settingsManager.appSettings.useChecklist.rawValue
    property real   _savedZoomLevel:        0
    property real   _margins:               ScreenTools.defaultFontPixelWidth / 2
    property real   _pipSize:               flightView.width * 0.2
    property alias  _guidedController:      guidedActionsController
 //   property alias  _altitudeSlider:        altitudeSlider

    property bool   chargePadShow :        false

    readonly property var       _dynamicCameras:        _activeVehicle ? _activeVehicle.dynamicCameras : null
    readonly property bool      _isCamera:              _dynamicCameras ? _dynamicCameras.cameras.count > 0 : false
    readonly property bool      isBackgroundDark:       _mainIsMap ? (_flightMap ? _flightMap.isSatelliteMap : true) : true
    readonly property real      _defaultRoll:           0
    readonly property real      _defaultPitch:          0
    readonly property real      _defaultHeading:        0
    readonly property real      _defaultAltitudeAMSL:   0
    readonly property real      _defaultGroundSpeed:    0
    readonly property real      _defaultAirSpeed:       0
    readonly property string    _mapName:               "FlightDisplayView"
    readonly property string    _showMapBackgroundKey:  "/showMapBackground"
    readonly property string    _mainIsMapKey:          "MainFlyWindowIsMap"
    readonly property string    _PIPVisibleKey:         "IsPIPVisible"

    function setStates() {
        QGroundControl.saveBoolGlobalSetting(_mainIsMapKey, _mainIsMap)
        if(_mainIsMap) {
            //-- Adjust Margins
            _flightMapContainer.state   = "fullMode"
            _flightVideo.state          = "pipMode"
            //-- Save/Restore Map Zoom Level
            if(_savedZoomLevel != 0)
                _flightMap.zoomLevel = _savedZoomLevel
            else
                _savedZoomLevel = _flightMap.zoomLevel
        } else {
            //-- Adjust Margins
            _flightMapContainer.state   = "pipMode"
            _flightVideo.state          = "fullMode"
            //-- Set Map Zoom Level
            _savedZoomLevel = _flightMap.zoomLevel
            _flightMap.zoomLevel = _savedZoomLevel - 3
        }
    }

    function setPipVisibility(state) {
        _isPipVisible = state;
        QGroundControl.saveBoolGlobalSetting(_PIPVisibleKey, state)
    }

    function isInstrumentRight() {
        if(QGroundControl.corePlugin.options.instrumentWidget) {
            if(QGroundControl.corePlugin.options.instrumentWidget.source.toString().length) {
                switch(QGroundControl.corePlugin.options.instrumentWidget.widgetPosition) {
                case CustomInstrumentWidget.POS_TOP_LEFT:
                case CustomInstrumentWidget.POS_BOTTOM_LEFT:
                case CustomInstrumentWidget.POS_CENTER_LEFT:
                    return false;
                }
            }
        }
        return true;
    }

    function chargePadChoose() {
       console.log("chargePadChoose")
            if(chargePadShow)
                chargePadShow = false
            else
                chargePadShow = true
    }

    function flightViewsetuserlogin(){
           userLogin.setuserlogin()
    }

/*
    function showFlightDisplayView(){
      //  var indicatorComponent = Qt.createComponent("QGCFlickableVerticalIndicator.qml")
      //  indicatorComponent.createObject(root)
       var component = Qt.createComponent("MainToolBar.qml");
             console.log("showFlightDisplayView")
           if (component.status === Component.Ready) {
         var bQml = component.createObject(root);
                bQml.chargePadChooseClicked.connect(chargePadChoose);// 实现两个qml组件之间的通信
                console.log("showFlightDisplayView111")
           }
          //  return _chargePadShow
     }

*/
    PlanMasterController {
        id:                     masterController
        Component.onCompleted:  start(true /* flyView */)
    }

    BuiltInPreFlightCheckModel {
        id: preFlightCheckModel
    }

    Connections {
        target:                     _missionController
        onResumeMissionReady:       guidedActionsController.confirmAction(guidedActionsController.actionResumeMissionReady)
        onResumeMissionUploadFail:  guidedActionsController.confirmAction(guidedActionsController.actionResumeMissionUploadFail)
    }

    Component.onCompleted: {
        setStates()
        if(QGroundControl.corePlugin.options.flyViewOverlay.toString().length) {
            flyViewOverlay.source = QGroundControl.corePlugin.options.flyViewOverlay
        }
    }

    // The following code is used to track vehicle states such that we prompt to remove mission from vehicle when mission completes

    property bool vehicleArmed:                 _activeVehicle ? _activeVehicle.armed : true // true here prevents pop up from showing during shutdown
    property bool vehicleWasArmed:              false
    property bool vehicleInMissionFlightMode:   _activeVehicle ? (_activeVehicle.flightMode === _activeVehicle.missionFlightMode) : false
    property bool promptForMissionRemove:       false

    onVehicleArmedChanged: {
        if (vehicleArmed) {
            if (!promptForMissionRemove) {
                promptForMissionRemove = vehicleInMissionFlightMode
                vehicleWasArmed = true
            }
        } else {
            if (promptForMissionRemove && (_missionController.containsItems || _geoFenceController.containsItems || _rallyPointController.containsItems)) {
                // ArduPilot has a strange bug which prevents mission clear from working at certain times, so we can't show this dialog
                if (!_activeVehicle.apmFirmware) {
                    root.showDialog(missionCompleteDialogComponent, qsTr("Flight Plan complete"), showDialogDefaultWidth, StandardButton.Close)
                }
            }
            promptForMissionRemove = false
        }
    }

    onVehicleInMissionFlightModeChanged: {
        if (!promptForMissionRemove && vehicleArmed) {
            promptForMissionRemove = true
        }
    }
  //  onChargePadChoose:{

   // }



    Component {
        id: missionCompleteDialogComponent

        QGCViewDialog {
            QGCFlickable {
                anchors.fill:   parent
                contentHeight:  column.height

                ColumnLayout {
                    id:                 column
                    anchors.margins:    _margins
                    anchors.left:       parent.left
                    anchors.right:      parent.right
                    spacing:            ScreenTools.defaultFontPixelHeight

                    QGCLabel {
                        Layout.fillWidth:       true
                        text:                   qsTr("%1 Images Taken").arg(_activeVehicle.cameraTriggerPoints.count)
                        horizontalAlignment:    Text.AlignHCenter
                        visible:                _activeVehicle.cameraTriggerPoints.count != 0
                    }

                    QGCButton {
                        Layout.fillWidth:   true
                        text:               qsTr("Remove plan from vehicle")
                        onClicked: {
                            _planMasterController.removeAllFromVehicle()
                            hideDialog()
                        }
                    }

                    QGCButton {
                        Layout.fillWidth:   true
                        Layout.alignment:   Qt.AlignHCenter
                        text:               qsTr("Leave plan on vehicle")
                        onClicked:          hideDialog()
                    }
                }
            }
        }
    }

    Window {
        id:             videoWindow
        width:          !_mainIsMap ? _panel.width  : _pipSize
        height:         !_mainIsMap ? _panel.height : _pipSize * (9/16)
        visible:        false

        Item {
            id:             videoItem
            anchors.fill:   parent
        }

        onClosing: {
            _flightVideo.state = "unpopup"
            videoWindow.visible = false
        }
    }

    /* This timer will startVideo again after the popup window appears and is loaded.
     * Such approach was the only one to avoid a crash for windows users
     */
    Timer {
      id: videoPopUpTimer
      interval: 2000;
      running: false;
      repeat: false
      onTriggered: {
          // If state is popup, the next one will be popup-finished
          if (_flightVideo.state ==  "popup") {
            _flightVideo.state = "popup-finished"
          }
          QGroundControl.videoManager.startVideo()
      }
    }

    QGCMapPalette { id: mapPal; lightColors: _mainIsMap ? _flightMap.isSatelliteMap : true }

    QGCViewPanel {
        id:             _panel
        anchors.fill:   parent

        //-- Map View
        //   For whatever reason, if FlightDisplayViewMap is the _panel item, changing
        //   width/height has no effect.
        Item {
            id: _flightMapContainer
            z:  _mainIsMap ? _panel.z + 1 : _panel.z + 2
            anchors.left:   _panel.left
            anchors.bottom: _panel.bottom
            visible:        _mainIsMap || _isPipVisible && !QGroundControl.videoManager.fullScreen
            width:          _mainIsMap ? _panel.width  : _pipSize
            height:         _mainIsMap ? _panel.height : _pipSize * (9/16)
            states: [
                State {
                    name:   "pipMode"
                    PropertyChanges {
                        target:             _flightMapContainer
                        anchors.margins:    ScreenTools.defaultFontPixelHeight
                    }
                },
                State {
                    name:   "fullMode"
                    PropertyChanges {
                        target:             _flightMapContainer
                        anchors.margins:    0
                    }
                }
            ]
            FlightDisplayViewMap {
                id:                         _flightMap
                anchors.fill:               parent
                planMasterController:       masterController
                guidedActionsController:    _guidedController
                flightWidgets:              flightDisplayViewWidgets
                rightPanelWidth:            ScreenTools.defaultFontPixelHeight * 9
                qgcView:                    root
                multiVehicleView:           !singleVehicleView.checked
                scaleState:                 (_mainIsMap && flyViewOverlay.item) ? (flyViewOverlay.item.scaleState ? flyViewOverlay.item.scaleState : "bottomMode") : "bottomMode"
            }
        }

        //-- Video View
        Item {
            id:             _flightVideo
            z:              _mainIsMap ? _panel.z + 2 : _panel.z + 1
            width:          !_mainIsMap ? _panel.width  : _pipSize
            height:         !_mainIsMap ? _panel.height : _pipSize * (9/16)
            anchors.left:   _panel.left
            anchors.bottom: _panel.bottom
            visible:        QGroundControl.videoManager.hasVideo && (!_mainIsMap || _isPipVisible)

            onParentChanged: {
                /* If video comes back from popup
                 * correct anchors.
                 * Such thing is not possible with ParentChange.
                 */
                if(parent == _panel) {
                    // Do anchors again after popup
                    anchors.left =       _panel.left
                    anchors.bottom =     _panel.bottom
                    anchors.margins =    ScreenTools.defaultFontPixelHeight
                }
            }

            states: [
                State {
                    name:   "pipMode"
                    PropertyChanges {
                        target: _flightVideo
                        anchors.margins: ScreenTools.defaultFontPixelHeight
                    }
                    PropertyChanges {
                        target: _flightVideoPipControl
                        inPopup: false
                    }
                },
                State {
                    name:   "fullMode"
                    PropertyChanges {
                        target: _flightVideo
                        anchors.margins:    0
                    }
                    PropertyChanges {
                        target: _flightVideoPipControl
                        inPopup: false
                    }
                },
                State {
                    name: "popup"
                    StateChangeScript {
                        script: {
                            // Stop video, restart it again with Timer
                            // Avoiding crashs if ParentChange is not yet done
                            QGroundControl.videoManager.stopVideo()
                            videoPopUpTimer.running = true
                        }
                    }
                    PropertyChanges {
                        target: _flightVideoPipControl
                        inPopup: true
                    }
                },
                State {
                    name: "popup-finished"
                    ParentChange {
                        target: _flightVideo
                        parent: videoItem
                        x: 0
                        y: 0
                        width: videoItem.width
                        height: videoItem.height
                    }
                },
                State {
                    name: "unpopup"
                    StateChangeScript {
                        script: {
                            QGroundControl.videoManager.stopVideo()
                            videoPopUpTimer.running = true
                        }
                    }
                    ParentChange {
                        target: _flightVideo
                        parent: _panel
                    }
                    PropertyChanges {
                        target: _flightVideoPipControl
                        inPopup: false
                    }
                }
            ]
            //-- Video Streaming
            FlightDisplayViewVideo {
                id:             videoStreaming
                anchors.fill:   parent
                visible:        QGroundControl.videoManager.isGStreamer
            }
            //-- UVC Video (USB Camera or Video Device)
            Loader {
                id:             cameraLoader
                anchors.fill:   parent
                visible:        !QGroundControl.videoManager.isGStreamer
                source:         QGroundControl.videoManager.uvcEnabled ? "qrc:/qml/FlightDisplayViewUVC.qml" : "qrc:/qml/FlightDisplayViewDummy.qml"
            }
        }

        QGCPipable {
            id:                 _flightVideoPipControl
            z:                  _flightVideo.z + 3
            width:              _pipSize
            height:             _pipSize * (9/16)
            anchors.left:       _panel.left
            anchors.bottom:     _panel.bottom
            anchors.margins:    ScreenTools.defaultFontPixelHeight
            visible:            QGroundControl.videoManager.hasVideo && !QGroundControl.videoManager.fullScreen && _flightVideo.state != "popup"
            isHidden:           !_isPipVisible
            isDark:             isBackgroundDark
            enablePopup:        _mainIsMap
            onActivated: {
                _mainIsMap = !_mainIsMap
                setStates()
            }
            onHideIt: {
                setPipVisibility(!state)
            }
            onPopup: {
                videoWindow.visible = true
                _flightVideo.state = "popup"
            }
            onNewWidth: {
                _pipSize = newWidth
            }
        }

        Row {
            id:                     singleMultiSelector
            anchors.topMargin:      ScreenTools.toolbarHeight + _margins
            anchors.rightMargin:    _margins
            anchors.right:          parent.right
            anchors.top:            parent.top
            spacing:                ScreenTools.defaultFontPixelWidth
            z:                      _panel.z + 4
            visible:                QGroundControl.multiVehicleManager.vehicles.count > 1

            ExclusiveGroup { id: multiVehicleSelectorGroup }

            QGCRadioButton {
                id:             singleVehicleView
                exclusiveGroup: multiVehicleSelectorGroup
                text:           qsTr("Single")
                checked:        true
                color:          mapPal.text
            }

            QGCRadioButton {
                exclusiveGroup: multiVehicleSelectorGroup
                text:           qsTr("Multi-Vehicle")
                color:          mapPal.text
            }
        }

        FlightDisplayViewWidgets {
            id:                 flightDisplayViewWidgets
            z:                  _panel.z + 4
            height:             ScreenTools.availableHeight - (singleMultiSelector.visible ? singleMultiSelector.height + _margins : 0)
            anchors.left:       parent.left
            anchors.right:      parent.right  //altitudeSlider.visible ? altitudeSlider.left : parent.right
            anchors.bottom:     parent.bottom
            qgcView:            root
            useLightColors:     isBackgroundDark
            missionController:  _missionController
            visible:            singleVehicleView.checked && !QGroundControl.videoManager.fullScreen
        }

        //-------------------------------------------------------------------------
        //-- Loader helper for plugins to overlay elements over the fly view
        Loader {
            id:                 flyViewOverlay
            z:                  flightDisplayViewWidgets.z + 1
            visible:            !QGroundControl.videoManager.fullScreen
            height:             ScreenTools.availableHeight
            anchors.left:       parent.left
            anchors.right:      parent.right// altitudeSlider.visible ? altitudeSlider.left : parent.right
            anchors.bottom:     parent.bottom

            property var qgcView: root
        }

        MultiVehicleList {
            anchors.margins:    _margins
            anchors.top:        singleMultiSelector.bottom
            anchors.right:      parent.right
            anchors.bottom:     parent.bottom
            width:              ScreenTools.defaultFontPixelWidth * 30
            visible:            !singleVehicleView.checked && !QGroundControl.videoManager.fullScreen
            z:                  _panel.z + 4
        }

        //-- Virtual Joystick
        Loader {
            id:                         virtualJoystickMultiTouch
            z:                          _panel.z + 5
            width:                      parent.width - ScreenTools.defaultFontPixelHeight * 4// - (_flightVideoPipControl.width / 2)
            height:                     Math.min(ScreenTools.availableHeight * 0.25, ScreenTools.defaultFontPixelWidth * 16)
            visible:                    !chargePadShow&&!QGroundControl.videoManager.fullScreen //&& !(_activeVehicle ? _activeVehicle.highLatencyLink : false)// (_virtualJoystick ? _virtualJoystick.value : false) && !QGroundControl.videoManager.fullScreen && !(_activeVehicle ? _activeVehicle.highLatencyLink : false)
            anchors.bottom:            parent.bottom// _flightVideoPipControl.top
            anchors.bottomMargin:       ScreenTools.defaultFontPixelHeight * 8
            anchors.horizontalCenter:   flightDisplayViewWidgets.horizontalCenter
            source:                     "qrc:/qml/VirtualJoystick.qml"
            active:                    true// !(_activeVehicle ? _activeVehicle.highLatencyLink : false) //(_virtualJoystick ? _virtualJoystick.value : false) && !(_activeVehicle ? _activeVehicle.highLatencyLink : false)

            property bool useLightColors: isBackgroundDark

            property Fact _virtualJoystick: QGroundControl.settingsManager.appSettings.virtualJoystick

        }
/*
        Row {
            id:                     singleMultiSelector
            anchors.topMargin:      ScreenTools.toolbarHeight + _margins
            anchors.rightMargin:    _margins
            anchors.right:          parent.right
            anchors.top:            parent.top
            spacing:                ScreenTools.defaultFontPixelWidth
            z:                      _panel.z + 4
            visible:                QGroundControl.multiVehicleManager.vehicles.count > 1


        }
*/
        Grid
        {
            id: chargepad
            rows:4
          //  anchors.centerIn: parent
            anchors.rightMargin:     _margins
            anchors.topMargin:       ScreenTools.toolbarHeight + (_margins * 2)
            anchors.right:           parent.right
            anchors.top:             parent.top
            z:                       _panel.z + 6
            spacing: 4
           // onChargePadChoose:    _chargePadShow=!_chargePadShow
            visible:               chargePadShow //showFlightDisplayView()? true : false

            ExclusiveGroup { id: mainChargePadGroup }

            ImageButton {
                source:              "/res/BarslockT3"
                logo:               false
                height:             ScreenTools.toolbarHeight
                exclusiveGroup:     mainChargePadGroup
                z:                  QGroundControl.zOrderTopMost+1
                onClicked:          {
                             QGroundControl.chargePad.blars_Lock()
                                    }
                visible:            true
            }

            ImageButton {
                source:              "/res/BarsunlockT3"
                logo:               false
                height:             ScreenTools.toolbarHeight
                exclusiveGroup:     mainChargePadGroup
                z:                  QGroundControl.zOrderTopMost+1
                onClicked:          {
                              QGroundControl.chargePad.blars_UNlock()
                                    }
                visible:            true
            }

            ImageButton {
                source:              "/res/PadchargeT3"
                logo:               false
                height:             ScreenTools.toolbarHeight
                exclusiveGroup:     mainChargePadGroup
                z:                  QGroundControl.zOrderTopMost+1
                onClicked:          {
                              QGroundControl.chargePad.charge()
                                    }
                visible:            true
            }

            ImageButton {
                source:              "/res/PadunchargeT3"
                logo:               false
                height:             ScreenTools.toolbarHeight
                exclusiveGroup:     mainChargePadGroup
                z:                  QGroundControl.zOrderTopMost+1
                onClicked:          {
                              QGroundControl.chargePad.uncharge()
                                    }
                visible:            true
            }

            ImageButton {
                source:              "/res/LockT3"
                logo:               false
                height:             ScreenTools.toolbarHeight
                exclusiveGroup:     mainChargePadGroup
                z:                  QGroundControl.zOrderTopMost+1
                onClicked:          {
                            QGroundControl.chargePad.drone()
                                    }
                visible:            true
            }

            ImageButton {
                source:              "/res/UnlockT3"
                logo:               false
                height:             ScreenTools.toolbarHeight
                exclusiveGroup:     mainChargePadGroup
                z:                  QGroundControl.zOrderTopMost+1
                onClicked:          {
                             QGroundControl.chargePad.undrone()
                                    }
                visible:            true
            }

            ImageButton {
                source:              "/res/RConT3"
                logo:               false
                height:             ScreenTools.toolbarHeight
                exclusiveGroup:     mainChargePadGroup
                z:                  QGroundControl.zOrderTopMost+1
                onClicked:          {
                              QGroundControl.chargePad.rc()
                                    }
                visible:            true
            }

            ImageButton {
                source:              "/res/RCoffT3"
                logo:               false
                height:             ScreenTools.toolbarHeight
                exclusiveGroup:     mainChargePadGroup
                z:                  QGroundControl.zOrderTopMost+1
                onClicked:          {
                              QGroundControl.chargePad.unrc()
                                    }
                visible:            true
            }

        }
/*
         Grid
         {
             id: chargepad
             rows:4
           //  anchors.centerIn: parent
             anchors.rightMargin:     _margins
             anchors.topMargin:       ScreenTools.toolbarHeight + (_margins * 2)
             anchors.right:           parent.right
             anchors.top:             parent.top
             z:                       _panel.z + 6
             spacing: 4
            // onChargePadChoose:    _chargePadShow=!_chargePadShow
             visible:               _chargePadShow //showFlightDisplayView()? true : false

             Button {
           //      id:             charge_blars_lock
                 text:           qsTr("Blars Lock")
                 width:           ScreenTools.defaultFontPixelWidth * 16
             //    color:          mapPal.text
                 onClicked:  {
                     QGroundControl.chargePad.blars_Lock()
                 }
             }
             Button {
         //        id:             charge_blars_unlock
                 text:           qsTr("Blars UnLock")
                 width:            ScreenTools.defaultFontPixelWidth * 16
               //   color:          mapPal.text
                  onClicked:  {
                     QGroundControl.chargePad.blars_UNlock()
                  }
             }
             Button {
           //      id:             charge_chargeon
                 text:           qsTr("ChargeON")
                 width:            ScreenTools.defaultFontPixelWidth * 16
              //   color:          mapPal.text
                 onClicked:  {
                     QGroundControl.chargePad.charge()
                 }
             }
           Button {
           //      id:             charge_uncharge
                 text:           qsTr("ChargeOFF")
                 width:          ScreenTools.defaultFontPixelWidth * 16
               //   color:          mapPal.text
                  onClicked:  {
                      QGroundControl.chargePad.uncharge()
                  }
             }

             Button {
            //     id:             charge_drone_on
                 text:           qsTr("DroneON")
                 width:          ScreenTools.defaultFontPixelWidth * 16
               //  color:          mapPal.text
                 onClicked:  {
                        QGroundControl.chargePad.drone()
                 }
             }
             Button {
              //   id:             charge_undrone
                 text:           qsTr("DroneOFF")
                 width:      ScreenTools.defaultFontPixelWidth * 16
              //    color:          mapPal.text
                  onClicked:  {
                         QGroundControl.chargePad.undrone()
                  }
             }

             Button {
           //      id:             charge_rc_on
                 text:           qsTr("RCON")
                 width:      ScreenTools.defaultFontPixelWidth * 16
              //   color:          mapPal.text
                 onClicked:  {
                           QGroundControl.chargePad.rc()
                 }
             }

             Button {
          //       id:             charge_unrc
                 text:           qsTr("RCOFF")
                 width:      ScreenTools.defaultFontPixelWidth * 16
              //   color:          mapPal.text
                 onClicked:  {
                          QGroundControl.chargePad.unrc()
                 }
             }

             Component.onCompleted:{

             }
         }
*/
//*davis
        ToolStrip {
            visible:            !QGroundControl.videoManager.fullScreen  //(_activeVehicle ? _activeVehicle.guidedModeSupported : true) && !QGroundControl.videoManager.fullScreen
          //  opacity:            0.6
            id:                 toolStrip
            anchors.leftMargin: isInstrumentRight() ? ScreenTools.defaultFontPixelWidth : undefined
            anchors.left:       isInstrumentRight() ? _panel.left : undefined
            anchors.rightMargin:isInstrumentRight() ? undefined : ScreenTools.defaultFontPixelWidth
            anchors.right:      isInstrumentRight() ? undefined : _panel.right
            anchors.topMargin:  ScreenTools.toolbarHeight + (_margins * 2)
            anchors.top:        _panel.top
            z:                  _panel.z + 4
         //daivs   title:              qsTr("Fly")
            maxHeight:          (_flightVideo.visible ? _flightVideo.y : parent.height) - toolStrip.y
          //  buttonVisible:      [ _useChecklist, _guidedController.showTakeoff || !_guidedController.showLand, _guidedController.showLand && !_guidedController.showTakeoff, true, true, true ]
          //  buttonEnabled:      [ _useChecklist && _activeVehicle, _guidedController.showTakeoff, _guidedController.showLand, _guidedController.showRTL, _guidedController.showPause, _anyActionAvailable ]


            buttonVisible:      [ true,  true, false, false ]
           // buttonEnabled:      [  _guidedController.showTakeoff ,   _guidedController.showTakeoff,   _guidedController.showPause,   _anyActionAvailable ]
            buttonEnabled:      [  _guidedController.showTakeoff&&(!_activeVehicle.systemFlyconditionStatus)&&(!_activeVehicle.systemFlyingStatus)  ,   _activeVehicle.systemFlyconditionStatus,   _guidedController.showPause,   _anyActionAvailable ]

            property bool _anyActionAvailable: _guidedController.showStartMission || _guidedController.showResumeMission || _guidedController.showChangeAlt || _guidedController.showLandAbort
            property var _actionModel: [
                {
                    title:      _guidedController.startMissionTitle,
                    text:       _guidedController.startMissionMessage,
                    action:     _guidedController.actionStartMission,
                    visible:    _guidedController.showStartMission
                },
                {
                    title:      _guidedController.continueMissionTitle,
                    text:       _guidedController.continueMissionMessage,
                    action:     _guidedController.actionContinueMission,
                    visible:    _guidedController.showContinueMission
                },
                {
                    title:      _guidedController.resumeMissionTitle,
                    text:       _guidedController.resumeMissionMessage,
                    action:     _guidedController.actionResumeMission,
                    visible:    _guidedController.showResumeMission
                },
                {
                    title:      _guidedController.changeAltTitle,
                    text:       _guidedController.changeAltMessage,
                    action:     _guidedController.actionChangeAlt,
                    visible:    _guidedController.showChangeAlt
                },
                {
                    title:      _guidedController.landAbortTitle,
                    text:       _guidedController.landAbortMessage,
                    action:     _guidedController.actionLandAbort,
                    visible:    _guidedController.showLandAbort
                }
            ]

            model: [
              /*  {
                    name:               "Checklist",
                    iconSource:         "/qmlimages/check.svg",
                    dropPanelComponent: checklistDropPanel
                },*/
                {
                    name:                   qsTr("Ready"),
                    iconSource:             "/qmlimages/MapSync.svg",
                    action:     _guidedController.actionReady
                },
                {
                    name:       _activeVehicle.systemFlyingStatus? (_activeVehicle.systemFlypauseStatus? qsTr("Pause") : qsTr("Action")):qsTr("takeoff"),// _guidedController.takeoffTitle,
                    iconSource:  _activeVehicle.systemFlyingStatus? (_activeVehicle.systemFlypauseStatus? "/res/pause-mission.svg" : "/res/action.svg"):"/res/takeoff.svg",
                    action:    _activeVehicle.systemFlyingStatus? (_activeVehicle.systemFlypauseStatus? _guidedController.actionPauseUse: _guidedController.actionContinueUse) :_guidedController.actionTakeoff
                },
           /*    {
                    name:       qsTr("Land"),//_guidedController.landTitle,
                    iconSource: "/res/land.svg",
                    action:     _guidedController.actionLand
                },*/
            //*    {
              //      name:       qsTr("RTL"),//_guidedController.rtlTitle,
              //      iconSource: "/res/rtl.svg",
                //   },*
              //      action:     _guidedController.actionRTL
                 {
                    name:      _guidedController.pauseTitle,// qsTr("Pause") ,//
                    iconSource: "/res/pause-mission.svg",
                    action:     _guidedController.actionPause
                 },
                 {
                    name:       qsTr("Action"),
                    iconSource: "/res/action.svg",
                    action:     -1
                 }
            ]

            onClicked: {
                guidedActionsController.closeAll()
                var action = model[index].action
                if (action === -1) {
                    guidedActionList.model   = _actionModel

                    guidedActionList.visible = true
                } else if (action === _guidedController.actionReady) {
                    console.log("_guidedController.actionReady")
                    _activeVehicle.flyReady()

                } else if (action === _guidedController.actionPauseUse) {
                    console.log("_guidedController.actionPauseUse")
                    _activeVehicle.flyPauseUse()

                } else if (action === _guidedController.actionContinueUse) {
                    console.log("_guidedController.actionContinueUse")
                    _activeVehicle.flyContinueUse()

                }else{
                    _guidedController.confirmAction(action)
                }
            }
        }

        GuidedActionsController {
            id:                 guidedActionsController
            missionController:  _missionController
            confirmDialog:      guidedActionConfirm
            actionList:         guidedActionList
            //altitudeSlider:     _altitudeSlider
            z:                  _flightVideoPipControl.z + 1
/*
            onShowStartMissionChanged: {
                if (showStartMission && !showResumeMission) {
                    confirmAction(actionStartMission)
                }
            }

            onShowContinueMissionChanged: {
                if (showContinueMission) {
                    confirmAction(actionContinueMission)
                }
            }

            onShowResumeMissionChanged: {
                if (showResumeMission) {
                    confirmAction(actionResumeMission)
                }
            }

            onShowLandAbortChanged: {
                if (showLandAbort) {
                    confirmAction(actionLandAbort)
                }
            }
*/
            /// Close all dialogs
            function closeAll() {
                mainWindow.enableToolbar()
                rootLoader.sourceComponent  = null
                guidedActionConfirm.visible = false
                guidedActionList.visible    = false
          //      altitudeSlider.visible      = false
            }
        }

        GuidedActionConfirm {
            id:                         guidedActionConfirm
            anchors.margins:            _margins
            anchors.bottom:             parent.bottom
            anchors.horizontalCenter:   parent.horizontalCenter
            guidedController:           _guidedController
            //altitudeSlider:             _altitudeSlider
        }

        UserLogin {
            id:                         userLogin
            anchors.margins:            _margins
            anchors.bottom:             parent.bottom
            anchors.horizontalCenter:   parent.horizontalCenter
            z :            _panel.z + 4
        //    visible:        true
        }


        GuidedActionList {
            id:                         guidedActionList
            anchors.margins:            _margins
            anchors.bottom:             parent.bottom
            anchors.horizontalCenter:   parent.horizontalCenter
            guidedController:           _guidedController
        }

        //-- Altitude slider
       /* GuidedAltitudeSlider {
            id:                 altitudeSlider
            anchors.margins:    _margins
            anchors.right:      parent.right
            anchors.topMargin:  ScreenTools.toolbarHeight + _margins
            anchors.top:        parent.top
            anchors.bottom:     parent.bottom
            z:                  _guidedController.z
            radius:             ScreenTools.defaultFontPixelWidth / 2
            width:              ScreenTools.defaultFontPixelWidth * 10
            color:              qgcPal.window
            visible:            false
        }
        */
    }


/*
             Button {
           //      id:             charge_blars_lock
                 text:           qsTr("Blars Lock")
                 width:           ScreenTools.defaultFontPixelWidth * 10
                 color:          mapPal.text
                 onClicked:  {

                 }
             }*/
/*
    QGCButton {
        id:             chargeblarslock
        text:            qsTr("Blars Lock")
        width:           ScreenTools.defaultFontPixelWidth * 20
      //  color:           qgcPal.window
        anchors.bottom:     parent.bottom
        anchors.right:      parent.right
        anchors.rightMargin:  ScreenTools.defaultFontPixelWidth *2
      //  anchors.centerIn: parent
         z:                       _panel.z + 2
         onClicked:  {

         }
    }
*/
    //-- Checklist GUI
    Component {
        id: checklistDropPanel

        PreFlightCheckList {
            model: preFlightCheckModel
        }
    } //Component
} //QGC View