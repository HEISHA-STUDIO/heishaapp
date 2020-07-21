/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "AppSettings.h"
#include "QGCPalette.h"
#include "QGCApplication.h"

#include "SettingsManager.h"
#include "userlogin.h"

#include <QQmlEngine>
#include <QtQml>
#include <QStandardPaths>

const char* AppSettings::name =                                         "App";
const char* AppSettings::settingsGroup =                                ""; // settings are in root group

const char* AppSettings::offlineEditingFirmwareTypeSettingsName =       "OfflineEditingFirmwareType";
const char* AppSettings::offlineEditingVehicleTypeSettingsName =        "OfflineEditingVehicleType";
const char* AppSettings::offlineEditingCruiseSpeedSettingsName =        "OfflineEditingCruiseSpeed";
const char* AppSettings::offlineEditingHoverSpeedSettingsName =         "OfflineEditingHoverSpeed";
const char* AppSettings::offlineEditingAscentSpeedSettingsName =        "OfflineEditingAscentSpeed";
const char* AppSettings::offlineEditingDescentSpeedSettingsName =       "OfflineEditingDescentSpeed";
const char* AppSettings::batteryPercentRemainingAnnounceSettingsName =  "batteryPercentRemainingAnnounce";
const char* AppSettings::defaultMissionItemAltitudeSettingsName =       "DefaultMissionItemAltitude";
const char* AppSettings::telemetrySaveName =                            "PromptFLightDataSave";
const char* AppSettings::telemetrySaveNotArmedName =                    "PromptFLightDataSaveNotArmed";
const char* AppSettings::audioMutedName =                               "AudioMuted";
const char* AppSettings::virtualJoystickName =                          "VirtualTabletJoystick";
const char* AppSettings::appFontPointSizeName =                         "BaseDeviceFontPointSize";
const char* AppSettings::indoorPaletteName =                            "StyleIsDark";
const char* AppSettings::showLargeCompassName =                         "ShowLargeCompass";
const char* AppSettings::savePathName =                                 "SavePath";
const char* AppSettings::autoLoadMissionsName =                         "AutoLoadMissions";
const char* AppSettings::useChecklistName =                             "UseChecklist";
const char* AppSettings::mapboxTokenName =                              "MapboxToken";
const char* AppSettings::esriTokenName =                                "EsriToken";
const char* AppSettings::defaultFirmwareTypeName =                      "DefaultFirmwareType";
const char* AppSettings::gstDebugName =                                 "GstreamerDebugLevel";
const char* AppSettings::followTargetName =                             "FollowTarget";

const char* AppSettings::parameterFileExtension =   "params";
const char* AppSettings::planFileExtension =        "plan";
const char* AppSettings::missionFileExtension =     "mission";
const char* AppSettings::waypointsFileExtension =   "waypoints";
const char* AppSettings::fenceFileExtension =       "fence";
const char* AppSettings::rallyPointFileExtension =  "rally";
const char* AppSettings::telemetryFileExtension =   "tlog";
const char* AppSettings::kmlFileExtension =         "kml";
const char* AppSettings::logFileExtension =         "ulg";

const char* AppSettings::parameterDirectory =       "Parameters";
const char* AppSettings::telemetryDirectory =       "Telemetry";
const char* AppSettings::missionDirectory =         "Missions";
const char* AppSettings::logDirectory =             "Logs";
const char* AppSettings::videoDirectory =           "Video";
const char* AppSettings::crashDirectory =           "CrashLogs";

const char* AppSettings::_userloginKey=              "Userlogin";
const char* AppSettings::_ipAdressSettingsKey =    "IpAdress";
const char* AppSettings::_tcpPortSettingsKey =     "TcpPort";
const char* AppSettings::_deviceIDSettingsKey =    "DeviceID";
const char* AppSettings::_usenameSettingsKey =     "Usename";
const char* AppSettings::_passwordSettingsKey =    "Password";

AppSettings::AppSettings(QObject* parent)
    : SettingsGroup                         (name, settingsGroup, parent)
    , _offlineEditingFirmwareTypeFact       (NULL)
    , _offlineEditingVehicleTypeFact        (NULL)
    , _offlineEditingCruiseSpeedFact        (NULL)
    , _offlineEditingHoverSpeedFact         (NULL)
    , _offlineEditingAscentSpeedFact        (NULL)
    , _offlineEditingDescentSpeedFact       (NULL)
    , _batteryPercentRemainingAnnounceFact  (NULL)
    , _defaultMissionItemAltitudeFact       (NULL)
    , _telemetrySaveFact                    (NULL)
    , _telemetrySaveNotArmedFact            (NULL)
    , _audioMutedFact                       (NULL)
    , _virtualJoystickFact                  (NULL)
    , _appFontPointSizeFact                 (NULL)
    , _indoorPaletteFact                    (NULL)
    , _showLargeCompassFact                 (NULL)
    , _savePathFact                         (NULL)
    , _autoLoadMissionsFact                 (NULL)
    , _useChecklistFact                     (NULL)
    , _mapboxTokenFact                      (NULL)
    , _esriTokenFact                        (NULL)
    , _defaultFirmwareTypeFact              (NULL)
    , _gstDebugFact                         (NULL)
    , _followTargetFact                     (NULL)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    qmlRegisterUncreatableType<AppSettings>("QGroundControl.SettingsManager", 1, 0, "AppSettings", "Reference only");
    QGCPalette::setGlobalTheme(indoorPalette()->rawValue().toBool() ? QGCPalette::Dark : QGCPalette::Light);

    // Instantiate savePath so we can check for override and setup default path if needed

    SettingsFact* savePathFact = qobject_cast<SettingsFact*>(savePath());
    QString appName = qgcApp()->applicationName();
    if (savePathFact->rawValue().toString().isEmpty() && _nameToMetaDataMap[savePathName]->rawDefaultValue().toString().isEmpty()) {
#ifdef __mobile__
#ifdef __ios__
        QDir rootDir = QDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
#else
        QDir rootDir = QDir(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation));
#endif
        savePathFact->setVisible(false);
#else
        QDir rootDir = QDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
#endif
        savePathFact->setRawValue(rootDir.filePath(appName));
    }

    connect(savePathFact, &Fact::rawValueChanged, this, &AppSettings::savePathsChanged);
    connect(savePathFact, &Fact::rawValueChanged, this, &AppSettings::_checkSavePathDirectories);

       connect(this, &AppSettings::allhostChanged, this, &AppSettings::allsetHost1);




  //   connect(this, &AppSettings::allportChanged, this, &AppSettings::allsetPort);
   //  connect(this, &AppSettings::alltopicChanged, this, &AppSettings:);
   //  connect(_videoSettings->tcpUrl(),        &Fact::rawValueChanged, this, &VideoManager::_tcpUrlChanged);

    _checkSavePathDirectories();
    _userlogin = new userlogin();
     loaduserlogin();
     connect(_userlogin, &userlogin::netipchange, this, &AppSettings::netsendip);
}

void AppSettings::allsetHost1(void){

    qWarning() << "allsetHost1:"  << _alladdress << _alltopic <<_allport;

}
void AppSettings::_checkSavePathDirectories(void)
{
    QDir savePathDir(savePath()->rawValue().toString());
    if (!savePathDir.exists()) {
        QDir().mkpath(savePathDir.absolutePath());
    }
    if (savePathDir.exists()) {
        savePathDir.mkdir(parameterDirectory);
        savePathDir.mkdir(telemetryDirectory);
        savePathDir.mkdir(missionDirectory);
        savePathDir.mkdir(logDirectory);
        savePathDir.mkdir(videoDirectory);
        savePathDir.mkdir(crashDirectory);
    }
}

Fact* AppSettings::offlineEditingFirmwareType(void)
{
    if (!_offlineEditingFirmwareTypeFact) {
        _offlineEditingFirmwareTypeFact = _createSettingsFact(offlineEditingFirmwareTypeSettingsName);
    }

    return _offlineEditingFirmwareTypeFact;
}

Fact* AppSettings::offlineEditingVehicleType(void)
{
    if (!_offlineEditingVehicleTypeFact) {
        _offlineEditingVehicleTypeFact = _createSettingsFact(offlineEditingVehicleTypeSettingsName);
    }

    return _offlineEditingVehicleTypeFact;
}

Fact* AppSettings::offlineEditingCruiseSpeed(void)
{
    if (!_offlineEditingCruiseSpeedFact) {
        _offlineEditingCruiseSpeedFact = _createSettingsFact(offlineEditingCruiseSpeedSettingsName);
    }
    return _offlineEditingCruiseSpeedFact;
}

Fact* AppSettings::offlineEditingHoverSpeed(void)
{
    if (!_offlineEditingHoverSpeedFact) {
        _offlineEditingHoverSpeedFact = _createSettingsFact(offlineEditingHoverSpeedSettingsName);
    }
    return _offlineEditingHoverSpeedFact;
}

Fact* AppSettings::offlineEditingAscentSpeed(void)
{
    if (!_offlineEditingAscentSpeedFact) {
        _offlineEditingAscentSpeedFact = _createSettingsFact(offlineEditingAscentSpeedSettingsName);
    }
    return _offlineEditingAscentSpeedFact;
}

Fact* AppSettings::offlineEditingDescentSpeed(void)
{
    if (!_offlineEditingDescentSpeedFact) {
        _offlineEditingDescentSpeedFact = _createSettingsFact(offlineEditingDescentSpeedSettingsName);
    }
    return _offlineEditingDescentSpeedFact;
}

Fact* AppSettings::batteryPercentRemainingAnnounce(void)
{
    if (!_batteryPercentRemainingAnnounceFact) {
        _batteryPercentRemainingAnnounceFact = _createSettingsFact(batteryPercentRemainingAnnounceSettingsName);
    }

    return _batteryPercentRemainingAnnounceFact;
}

Fact* AppSettings::defaultMissionItemAltitude(void)
{
    if (!_defaultMissionItemAltitudeFact) {
        _defaultMissionItemAltitudeFact = _createSettingsFact(defaultMissionItemAltitudeSettingsName);
    }

    return _defaultMissionItemAltitudeFact;
}

Fact* AppSettings::telemetrySave(void)
{
    if (!_telemetrySaveFact) {
        _telemetrySaveFact = _createSettingsFact(telemetrySaveName);
    }

    return _telemetrySaveFact;
}

Fact* AppSettings::telemetrySaveNotArmed(void)
{
    if (!_telemetrySaveNotArmedFact) {
        _telemetrySaveNotArmedFact = _createSettingsFact(telemetrySaveNotArmedName);
    }

    return _telemetrySaveNotArmedFact;
}

Fact* AppSettings::audioMuted(void)
{
    if (!_audioMutedFact) {
        _audioMutedFact = _createSettingsFact(audioMutedName);
    }

    return _audioMutedFact;
}

Fact* AppSettings::useChecklist(void)
{
    if (!_useChecklistFact) {
        _useChecklistFact = _createSettingsFact(useChecklistName);
    }

    return _useChecklistFact;
}

Fact* AppSettings::appFontPointSize(void)
{
    if (!_appFontPointSizeFact) {
        _appFontPointSizeFact = _createSettingsFact(appFontPointSizeName);
    }

    return _appFontPointSizeFact;
}

Fact* AppSettings::virtualJoystick(void)
{
    if (!_virtualJoystickFact) {
        _virtualJoystickFact = _createSettingsFact(virtualJoystickName);
    }
  //  _virtualJoystickFact->rawValueChanged(true);
    return _virtualJoystickFact;
}



Fact* AppSettings::gstDebug(void)
{
    if (!_gstDebugFact) {
        _gstDebugFact = _createSettingsFact(gstDebugName);
    }

    return _gstDebugFact;
}

Fact* AppSettings::indoorPalette(void)
{
    if (!_indoorPaletteFact) {
        _indoorPaletteFact = _createSettingsFact(indoorPaletteName);
        connect(_indoorPaletteFact, &Fact::rawValueChanged, this, &AppSettings::_indoorPaletteChanged);
    }

    return _indoorPaletteFact;
}

void AppSettings::_indoorPaletteChanged(void)
{
    qgcApp()->_loadCurrentStyleSheet();
    QGCPalette::setGlobalTheme(indoorPalette()->rawValue().toBool() ? QGCPalette::Dark : QGCPalette::Light);
}

Fact* AppSettings::showLargeCompass(void)
{
    if (!_showLargeCompassFact) {
        _showLargeCompassFact = _createSettingsFact(showLargeCompassName);
    }

    return _showLargeCompassFact;
}

Fact* AppSettings::savePath(void)
{
    if (!_savePathFact) {
        _savePathFact = _createSettingsFact(savePathName);
    }

    return _savePathFact;
}

QString AppSettings::missionSavePath(void)
{
    QString path = savePath()->rawValue().toString();
    if (!path.isEmpty() && QDir(path).exists()) {
        QDir dir(path);
        return dir.filePath(missionDirectory);
    }

    return QString();
}

QString AppSettings::parameterSavePath(void)
{
    QString path = savePath()->rawValue().toString();
    if (!path.isEmpty() && QDir(path).exists()) {
        QDir dir(path);
        return dir.filePath(parameterDirectory);
    }

    return QString();
}

QString AppSettings::telemetrySavePath(void)
{
    QString path = savePath()->rawValue().toString();
    if (!path.isEmpty() && QDir(path).exists()) {
        QDir dir(path);
        return dir.filePath(telemetryDirectory);
    }

    return QString();
}

QString AppSettings::logSavePath(void)
{
    QString path = savePath()->rawValue().toString();
    if (!path.isEmpty() && QDir(path).exists()) {
        QDir dir(path);
        return dir.filePath(logDirectory);
    }

    return QString();
}

QString AppSettings::videoSavePath(void)
{
    QString path = savePath()->rawValue().toString();
    if (!path.isEmpty() && QDir(path).exists()) {
        QDir dir(path);
        return dir.filePath(videoDirectory);
    }

    return QString();
}

QString AppSettings::crashSavePath(void)
{
    QString path = savePath()->rawValue().toString();
    if (!path.isEmpty() && QDir(path).exists()) {
        QDir dir(path);
        return dir.filePath(crashDirectory);
    }

    return QString();
}

Fact* AppSettings::autoLoadMissions(void)
{
    if (!_autoLoadMissionsFact) {
        _autoLoadMissionsFact = _createSettingsFact(autoLoadMissionsName);
    }

    return _autoLoadMissionsFact;
}

Fact* AppSettings::mapboxToken(void)
{
    if (!_mapboxTokenFact) {
        _mapboxTokenFact = _createSettingsFact(mapboxTokenName);
    }

    return _mapboxTokenFact;
}

Fact* AppSettings::esriToken(void)
{
    if (!_esriTokenFact) {
        _esriTokenFact = _createSettingsFact(esriTokenName);
    }

    return _esriTokenFact;
}

MAV_AUTOPILOT AppSettings::offlineEditingFirmwareTypeFromFirmwareType(MAV_AUTOPILOT firmwareType)
{
    if (firmwareType != MAV_AUTOPILOT_PX4 && firmwareType != MAV_AUTOPILOT_ARDUPILOTMEGA) {
        firmwareType = MAV_AUTOPILOT_GENERIC;
    }
    return firmwareType;
}

MAV_TYPE AppSettings::offlineEditingVehicleTypeFromVehicleType(MAV_TYPE vehicleType)
{
    if (QGCMAVLink::isRover(vehicleType)) {
        return MAV_TYPE_GROUND_ROVER;
    } else if (QGCMAVLink::isSub(vehicleType)) {
        return MAV_TYPE_SUBMARINE;
    } else if (QGCMAVLink::isVTOL(vehicleType)) {
        return MAV_TYPE_VTOL_QUADROTOR;
    } else if (QGCMAVLink::isFixedWing(vehicleType)) {
        return MAV_TYPE_FIXED_WING;
    } else {
        return MAV_TYPE_QUADROTOR;
    }
}

Fact* AppSettings::defaultFirmwareType(void)
{
    if (!_defaultFirmwareTypeFact) {
        _defaultFirmwareTypeFact = _createSettingsFact(defaultFirmwareTypeName);
    }

    return _defaultFirmwareTypeFact;
}

Fact* AppSettings::followTarget(void)
{
    if (!_followTargetFact) {
        _followTargetFact = _createSettingsFact(followTargetName);
    }

    return _followTargetFact;
}


static bool is_ip(const QString& address)
{
    int a,b,c,d;
    if (sscanf(address.toStdString().c_str(), "%d.%d.%d.%d", &a, &b, &c, &d) != 4
            && strcmp("::1", address.toStdString().c_str())) {
        return false;
    } else {
        return true;
    }
}


static QString get_ip_address(const QString& address)
{
    if(is_ip(address))
        return address;
    // Need to look it up
    QHostInfo info = QHostInfo::fromName(address);
    if (info.error() == QHostInfo::NoError)
    {
        QList<QHostAddress> hostAddresses = info.addresses();
        for (int i = 0; i < hostAddresses.size(); i++)
        {
            // Exclude all IPv6 addresses
            if (!hostAddresses.at(i).toString().contains(":"))
            {
                return hostAddresses.at(i).toString();
            }
        }
    }
    return {};
}

void AppSettings::allsetPort(quint16 myport)
{
   _allport = myport;
    //g_port =_allport;


    qWarning() << "MYTCP:"  << _allport;
}

void AppSettings::setAddress(const QHostAddress& address)
{
    _alladdress = address;
   //  g_address = _alladdress;
}

void AppSettings::allsetHost(const QString host)
{
    QString ipAdd = get_ip_address(host);
    if(ipAdd.isEmpty()) {
        qWarning() << "MYTCP:" << "Could not resolve host:" << host;
    } else {
        _alladdress = ipAdd;
    //    g_address = _alladdress;
    }
}

void  AppSettings::setusename (QString usename )
{
     _usename=usename;
}


void  AppSettings::setpassword(QString password)
{
      _password=password;
}

void  AppSettings::setuserloginshow(bool userloginshow)
{
     _userloginshow=userloginshow;
}



void AppSettings::allsetTopic(QString topic)
{
    _alltopic = topic;
    //g_topic =_alltopic;
     qWarning() << "MYTCP:"  << _alltopic;
}

void AppSettings::sendip(void){
    saveuserlogin();
    qWarning() << "sendip:"  << _alladdress << _alltopic <<_allport;
    emit send_com_ip();
    emit send_camera_ip();
}
void AppSettings::getuserlogin(void){
        qWarning() << "getuserlogin:" ;
        _userlogin->_usenamelonin=_usename;
        _userlogin->_passwordlonin=_password;
        _userlogin->on_LoginButton_clicked();

}
void AppSettings::netsendip(void)
{
        _alltopic = _userlogin->get_topic() ;
        _allport = _userlogin->get_port() ;
        _alladdress =QHostAddress(_userlogin->get_ip());
    //     _usename=usename;
        _userloginshow = false;
           qWarning() << "sendip:"  << _alladdress << _alltopic <<_allport << _userloginshow;
        saveuserlogin();
       // setuserloginshow(true);

        emit send_com_ip();
        emit send_camera_ip();
}

void AppSettings::saveuserlogin(void){
      QSettings   settings;
     _deviceID = _alltopic;
     _tcpPort = _allport;
     _ipAdress = _alladdress.toString();
     settings.beginGroup(_userloginKey);
     settings.setValue(_ipAdressSettingsKey, _ipAdress);
     settings.setValue(_tcpPortSettingsKey,  _tcpPort);
     settings.setValue(_deviceIDSettingsKey, _deviceID);
     settings.setValue(_usenameSettingsKey,  _usename);
     settings.setValue(_passwordSettingsKey, _password);
     settings.endGroup();
}

void AppSettings::loaduserlogin(void){
     QSettings   settings;
     settings.beginGroup(_userloginKey);
     _ipAdress = settings.value(_ipAdressSettingsKey, "118.190.91.165").toString();
     _tcpPort = settings.value(_tcpPortSettingsKey, 9002).toInt();
     _deviceID = settings.value(_deviceIDSettingsKey, "2d0358e55cdc1ce8").toString();
     _usename = settings.value(_usenameSettingsKey, "heisha1").toString();
     _password = settings.value(_passwordSettingsKey, "111111").toString();

      settings.endGroup();
     _alltopic = _deviceID ;
     _allport = _tcpPort ;
     _alladdress =QHostAddress(_ipAdress);
}





