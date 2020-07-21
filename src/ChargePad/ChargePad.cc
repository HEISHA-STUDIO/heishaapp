/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

//#include <QElapsedTimer>
#include <cmath>
#include <QDebug>
#include <QSignalSpy>

#include "MultiVehicleManager.h"
#include "FirmwarePlugin.h"
#include "MAVLinkProtocol.h"
#include "ChargePad.h"
#include "Vehicle.h"
#include "PositionManager.h"
#include "SettingsManager.h"
#include "AppSettings.h"

ChargePad::ChargePad(QGCApplication* app, QGCToolbox* toolbox)
    : QGCTool(app, toolbox)//, estimatation_capabilities(0)
{
     memset(&_chargePadReport, 0, sizeof(chargePadReport_s));
   // runTime.start();
    _gcsMotionReportTimer.setSingleShot(false);
}

void ChargePad::setToolbox(QGCToolbox* toolbox)
{
    QGCTool::setToolbox(toolbox);
 //   connect(&_gcsMotionReportTimer, &QTimer::timeout, this, &ChargePad::_sendGCSChargePadReport);
//    connect(toolbox->settingsManager()->appSettings()->followTarget(), &Fact::rawValueChanged, this, &ChargePad::_settingsChanged);
   // _currentMode = _toolbox->settingsManager()->appSettings()->followTarget()->rawValue().toUInt();
   // if(_currentMode == MODE_ALWAYS) {
  //      _enable();
   // }
}

void ChargePad::_settingsChanged(){



}
void ChargePad::blars_Lock(){
  qWarning() << "Blars_Lock:"  << endl;
    _chargePadReport.contorl_uint = 230;//MAV_CMD_PAD_LOCK;
    _sendGCSChargePadReport();
}

void ChargePad::blars_UNlock(){
    qWarning() << "blars_UNlock:"  << endl;
    _chargePadReport.contorl_uint =231;// MAV_CMD_PAD_UNLOCK;
    _sendGCSChargePadReport();
}

void ChargePad::charge(){
    qWarning() << "charge:"  << endl;
   _chargePadReport.contorl_uint = 228;//MAV_CMD_PAD_START_CHARGE;
   _sendGCSChargePadReport();
}

void ChargePad::uncharge(){
    qWarning() << "uncharge:"  << endl;
   _chargePadReport.contorl_uint = 229;//MAV_CMD_PAD_START_CHARGE;
   _sendGCSChargePadReport();
}

void ChargePad::drone(){
    qWarning() << "drone:"  << endl;
    _chargePadReport.contorl_uint =234;// MAV_CMD_PAD_TURN_ON_DRONE;
    _sendGCSChargePadReport();
}

void ChargePad::undrone(){
    qWarning() << "drone:"  << endl;
    _chargePadReport.contorl_uint =235;// MAV_CMD_PAD_TURN_ON_DRONE;
    _sendGCSChargePadReport();
}
void ChargePad::rc(){
    qWarning() << "rc:"  << endl;
    _chargePadReport.contorl_uint = 232;//MAV_CMD_PAD_TURN_ON_RC;
    _sendGCSChargePadReport();

}

void ChargePad::unrc(){
    qWarning() << "rc:"  << endl;
    _chargePadReport.contorl_uint = 233;//MAV_CMD_PAD_TURN_ON_RC;
    _sendGCSChargePadReport();
}


void ChargePad::_sendGCSChargePadReport()
{
//MAV_CMD_PAD_UNLOCK
    //-- Do we have any real data?
  //  if(_motionReport.lat_int == 0 && _motionReport.lon_int == 0 && _motionReport.alt == 0) {
   //     return;
   // }

    QmlObjectListModel & vehicles    = *_toolbox->multiVehicleManager()->vehicles();
    MAVLinkProtocol* mavlinkProtocol = _toolbox->mavlinkProtocol();
    mavlink_command_int_t chargepad_target;
    memset(&chargepad_target, 0, sizeof(chargepad_target));

    chargepad_target.command = _chargePadReport.contorl_uint;

    for (int i=0; i< vehicles.count(); i++) {
        Vehicle* vehicle = qobject_cast<Vehicle*>(vehicles[i]);
            mavlink_message_t message;
        /*    mavlink_msg_charege_pad_encode_chan(mavlinkProtocol->getSystemId(),
                                                mavlinkProtocol->getComponentId(),
                                                vehicle->priorityLink()->mavlinkChannel(),
                                                &message,
                                                &chargepad_target);
*/
         /*
            memset(&chargepad_target, 0, sizeof(chargepad_target));
            chargepad_target.target_system =     _id;
            chargepad_target.target_component =  queuedCommand.component;
            chargepad_target.command =           queuedCommand.command;
            chargepad_target.frame =             queuedCommand.frame;
            chargepad_target.param1 =            queuedCommand.rgParam[0];
            chargepad_target.param2 =            queuedCommand.rgParam[1];
            chargepad_target.param3 =            queuedCommand.rgParam[2];
            chargepad_target.param4 =            queuedCommand.rgParam[3];
            chargepad_target.x =                 queuedCommand.rgParam[4] * qPow(10.0, 7.0);
            chargepad_target.y =                 queuedCommand.rgParam[5] * qPow(10.0, 7.0);
            chargepad_target.z =                 queuedCommand.rgParam[6];*/
            mavlink_msg_command_int_encode_chan(mavlinkProtocol->getSystemId(),
                                                mavlinkProtocol->getComponentId(),
                                                vehicle->priorityLink()->mavlinkChannel(),
                                                &message,
                                                &chargepad_target);

            vehicle->sendMessageOnLink(vehicle->priorityLink(), message);
    }
}
/*
void FollowMe::followMeHandleManager(const QString&)
{
    //-- If we are to change based on current flight mode
    if(_currentMode == MODE_FOLLOWME) {
        QmlObjectListModel & vehicles = *_toolbox->multiVehicleManager()->vehicles();
        for (int i=0; i< vehicles.count(); i++) {
            Vehicle* vehicle = qobject_cast<Vehicle*>(vehicles[i]);
            if (vehicle->px4Firmware() && vehicle->flightMode().compare(FirmwarePlugin::px4FollowMeFlightMode, Qt::CaseInsensitive) == 0) {
                _enable();
                return;
            }
        }
        _disable();
    }
}

void FollowMe::_settingsChanged()
{
    uint32_t mode = _toolbox->settingsManager()->appSettings()->followTarget()->rawValue().toUInt();
    if(_currentMode != mode) {
        _currentMode = mode;
        switch (mode) {
        case MODE_NEVER:
            if(_gcsMotionReportTimer.isActive()) {
                _disable();
            }
            break;
        case MODE_ALWAYS:
            if(!_gcsMotionReportTimer.isActive()) {
                _enable();
            }
            break;
        case MODE_FOLLOWME:
            if(!_gcsMotionReportTimer.isActive()) {
                followMeHandleManager(QString());
            }
            break;
        default:
            break;
        }
    }
}

void FollowMe::_enable()
{
    connect(_toolbox->qgcPositionManager(),
            &QGCPositionManager::positionInfoUpdated,
            this,
            &FollowMe::_setGPSLocation);
    _gcsMotionReportTimer.setInterval(_toolbox->qgcPositionManager()->updateInterval());
    _gcsMotionReportTimer.start();
}

void FollowMe::_disable()
{
    disconnect(_toolbox->qgcPositionManager(),
               &QGCPositionManager::positionInfoUpdated,
               this,
               &FollowMe::_setGPSLocation);
    _gcsMotionReportTimer.stop();
}

void FollowMe::_setGPSLocation(QGeoPositionInfo geoPositionInfo)
{
    if (!geoPositionInfo.isValid()) {
        //-- Invalidate cached coordinates
        memset(&_motionReport, 0, sizeof(motionReport_s));
    } else {
        // get the current location coordinates

        QGeoCoordinate geoCoordinate = geoPositionInfo.coordinate();

        _motionReport.lat_int = geoCoordinate.latitude()  * 1e7;
        _motionReport.lon_int = geoCoordinate.longitude() * 1e7;
        _motionReport.alt     = geoCoordinate.altitude();

        estimatation_capabilities |= (1 << POS);

        // get the current eph and epv

        if(geoPositionInfo.hasAttribute(QGeoPositionInfo::HorizontalAccuracy) == true) {
            _motionReport.pos_std_dev[0] = _motionReport.pos_std_dev[1] = geoPositionInfo.attribute(QGeoPositionInfo::HorizontalAccuracy);
        }

        if(geoPositionInfo.hasAttribute(QGeoPositionInfo::VerticalAccuracy) == true) {
            _motionReport.pos_std_dev[2] = geoPositionInfo.attribute(QGeoPositionInfo::VerticalAccuracy);
        }                

        // calculate z velocity if it's available

        if(geoPositionInfo.hasAttribute(QGeoPositionInfo::VerticalSpeed)) {
            _motionReport.vz = geoPositionInfo.attribute(QGeoPositionInfo::VerticalSpeed);
        }

        // calculate x,y velocity if it's available

        if((geoPositionInfo.hasAttribute(QGeoPositionInfo::Direction)   == true) &&
           (geoPositionInfo.hasAttribute(QGeoPositionInfo::GroundSpeed) == true)) {

            estimatation_capabilities |= (1 << VEL);

            qreal direction = _degreesToRadian(geoPositionInfo.attribute(QGeoPositionInfo::Direction));
            qreal velocity  = geoPositionInfo.attribute(QGeoPositionInfo::GroundSpeed);

            _motionReport.vx = cos(direction)*velocity;
            _motionReport.vy = sin(direction)*velocity;

        } else {
            _motionReport.vx = 0.0f;
            _motionReport.vy = 0.0f;
        }
    }
}

void FollowMe::_sendGCSMotionReport()
{

    //-- Do we have any real data?
    if(_motionReport.lat_int == 0 && _motionReport.lon_int == 0 && _motionReport.alt == 0) {
        return;
    }

    QmlObjectListModel & vehicles    = *_toolbox->multiVehicleManager()->vehicles();
    MAVLinkProtocol* mavlinkProtocol = _toolbox->mavlinkProtocol();
    mavlink_follow_target_t follow_target;
    memset(&follow_target, 0, sizeof(follow_target));

    follow_target.timestamp = runTime.nsecsElapsed() * 1e-6;
    follow_target.est_capabilities = estimatation_capabilities;
    follow_target.position_cov[0] = _motionReport.pos_std_dev[0];
    follow_target.position_cov[2] = _motionReport.pos_std_dev[2];
    follow_target.alt = _motionReport.alt;
    follow_target.lat = _motionReport.lat_int;
    follow_target.lon = _motionReport.lon_int;
    follow_target.vel[0] = _motionReport.vx;
    follow_target.vel[1] = _motionReport.vy;

    for (int i=0; i< vehicles.count(); i++) {
        Vehicle* vehicle = qobject_cast<Vehicle*>(vehicles[i]);
        if(_currentMode || vehicle->flightMode().compare(FirmwarePlugin::px4FollowMeFlightMode, Qt::CaseInsensitive) == 0) {
            mavlink_message_t message;
            mavlink_msg_follow_target_encode_chan(mavlinkProtocol->getSystemId(),
                                                  mavlinkProtocol->getComponentId(),
                                                  vehicle->priorityLink()->mavlinkChannel(),
                                                  &message,
                                                  &follow_target);
            vehicle->sendMessageOnLink(vehicle->priorityLink(), message);
        }
    }
}

double FollowMe::_degreesToRadian(double deg)
{
    return deg * M_PI / 180.0;
}
*/
