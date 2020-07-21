/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#ifndef AppSettings_H
#define AppSettings_H

#include "SettingsGroup.h"
#include "QGCMAVLink.h"
#include <QHostAddress>
#include "userlogin.h"

class userlogin;

class AppSettings : public SettingsGroup
{
    Q_OBJECT
    
public:
    AppSettings(QObject* parent = NULL);

    Q_PROPERTY(Fact* offlineEditingFirmwareType         READ offlineEditingFirmwareType         CONSTANT)
    Q_PROPERTY(Fact* offlineEditingVehicleType          READ offlineEditingVehicleType          CONSTANT)
    Q_PROPERTY(Fact* offlineEditingCruiseSpeed          READ offlineEditingCruiseSpeed          CONSTANT)
    Q_PROPERTY(Fact* offlineEditingHoverSpeed           READ offlineEditingHoverSpeed           CONSTANT)
    Q_PROPERTY(Fact* offlineEditingAscentSpeed          READ offlineEditingAscentSpeed          CONSTANT)
    Q_PROPERTY(Fact* offlineEditingDescentSpeed         READ offlineEditingDescentSpeed         CONSTANT)
    Q_PROPERTY(Fact* batteryPercentRemainingAnnounce    READ batteryPercentRemainingAnnounce    CONSTANT)
    Q_PROPERTY(Fact* defaultMissionItemAltitude         READ defaultMissionItemAltitude         CONSTANT)
    Q_PROPERTY(Fact* telemetrySave                      READ telemetrySave                      CONSTANT)
    Q_PROPERTY(Fact* telemetrySaveNotArmed              READ telemetrySaveNotArmed              CONSTANT)
    Q_PROPERTY(Fact* audioMuted                         READ audioMuted                         CONSTANT)
    Q_PROPERTY(Fact* virtualJoystick                    READ virtualJoystick                    CONSTANT)
    Q_PROPERTY(Fact* appFontPointSize                   READ appFontPointSize                   CONSTANT)
    Q_PROPERTY(Fact* indoorPalette                      READ indoorPalette                      CONSTANT)
    Q_PROPERTY(Fact* showLargeCompass                   READ showLargeCompass                   CONSTANT)
    Q_PROPERTY(Fact* savePath                           READ savePath                           CONSTANT)
    Q_PROPERTY(Fact* autoLoadMissions                   READ autoLoadMissions                   CONSTANT)
    Q_PROPERTY(Fact* useChecklist                       READ useChecklist                       CONSTANT)
    Q_PROPERTY(Fact* mapboxToken                        READ mapboxToken                        CONSTANT)
    Q_PROPERTY(Fact* esriToken                          READ esriToken                          CONSTANT)
    Q_PROPERTY(Fact* defaultFirmwareType                READ defaultFirmwareType                CONSTANT)
    Q_PROPERTY(Fact* gstDebug                           READ gstDebug                           CONSTANT)
    Q_PROPERTY(Fact* followTarget                       READ followTarget                       CONSTANT)

    Q_PROPERTY(QString missionSavePath      READ missionSavePath    NOTIFY savePathsChanged)
    Q_PROPERTY(QString parameterSavePath    READ parameterSavePath  NOTIFY savePathsChanged)
    Q_PROPERTY(QString telemetrySavePath    READ telemetrySavePath  NOTIFY savePathsChanged)
    Q_PROPERTY(QString logSavePath          READ logSavePath        NOTIFY savePathsChanged)
    Q_PROPERTY(QString videoSavePath        READ videoSavePath      NOTIFY savePathsChanged)
    Q_PROPERTY(QString crashSavePath        READ crashSavePath      NOTIFY savePathsChanged)

    Q_PROPERTY(QString planFileExtension        MEMBER planFileExtension        CONSTANT)
    Q_PROPERTY(QString missionFileExtension     MEMBER missionFileExtension     CONSTANT)
    Q_PROPERTY(QString waypointsFileExtension   MEMBER waypointsFileExtension   CONSTANT)
    Q_PROPERTY(QString parameterFileExtension   MEMBER parameterFileExtension   CONSTANT)
    Q_PROPERTY(QString telemetryFileExtension   MEMBER telemetryFileExtension   CONSTANT)
    Q_PROPERTY(QString kmlFileExtension         MEMBER kmlFileExtension         CONSTANT)
    Q_PROPERTY(QString logFileExtension         MEMBER logFileExtension         CONSTANT)




    Q_PROPERTY(quint16  port    READ allport    WRITE allsetPort    NOTIFY allportChanged)
    Q_PROPERTY(QString  host    READ allhost    WRITE allsetHost    NOTIFY allhostChanged)
    Q_PROPERTY(QString  topic   READ alltopic   WRITE allsetTopic   NOTIFY alltopicChanged)

    Q_PROPERTY(QString  usename    READ usename    WRITE setusename    NOTIFY usenameChanged)
    Q_PROPERTY(QString  password   READ password   WRITE setpassword   NOTIFY passwordChanged)
    Q_PROPERTY(bool  userloginshow   READ userloginshow   WRITE setuserloginshow   NOTIFY userloginshowChanged)



    Fact* offlineEditingFirmwareType        (void);
    Fact* offlineEditingVehicleType         (void);
    Fact* offlineEditingCruiseSpeed         (void);
    Fact* offlineEditingHoverSpeed          (void);
    Fact* offlineEditingAscentSpeed         (void);
    Fact* offlineEditingDescentSpeed        (void);
    Fact* batteryPercentRemainingAnnounce   (void);
    Fact* defaultMissionItemAltitude        (void);
    Fact* telemetrySave                     (void);
    Fact* telemetrySaveNotArmed             (void);
    Fact* audioMuted                        (void);
    Fact* virtualJoystick                   (void);
    Fact* appFontPointSize                  (void);
    Fact* indoorPalette                     (void);
    Fact* showLargeCompass                  (void);
    Fact* savePath                          (void);
    Fact* autoLoadMissions                  (void);
    Fact* useChecklist                      (void);
    Fact* mapboxToken                       (void);
    Fact* esriToken                         (void);
    Fact* defaultFirmwareType               (void);
    Fact* gstDebug                          (void);
    Fact* followTarget                      (void);

    Q_INVOKABLE void sendip(void);
    Q_INVOKABLE void getuserlogin(void);


    QString missionSavePath     (void);
    QString parameterSavePath   (void);
    QString telemetrySavePath   (void);
    QString logSavePath         (void);
    QString videoSavePath         (void);
    QString crashSavePath         (void);

    static MAV_AUTOPILOT offlineEditingFirmwareTypeFromFirmwareType(MAV_AUTOPILOT firmwareType);
    static MAV_TYPE offlineEditingVehicleTypeFromVehicleType(MAV_TYPE vehicleType);

    static const char* name;
    static const char* settingsGroup;

    static const char* offlineEditingFirmwareTypeSettingsName;
    static const char* offlineEditingVehicleTypeSettingsName;
    static const char* offlineEditingCruiseSpeedSettingsName;
    static const char* offlineEditingHoverSpeedSettingsName;
    static const char* offlineEditingAscentSpeedSettingsName;
    static const char* offlineEditingDescentSpeedSettingsName;
    static const char* batteryPercentRemainingAnnounceSettingsName;
    static const char* defaultMissionItemAltitudeSettingsName;
    static const char* telemetrySaveName;
    static const char* telemetrySaveNotArmedName;
    static const char* audioMutedName;
    static const char* virtualJoystickName;
    static const char* appFontPointSizeName;
    static const char* indoorPaletteName;
    static const char* showLargeCompassName;
    static const char* savePathName;
    static const char* autoLoadMissionsName;
    static const char* useChecklistName;
    static const char* mapboxTokenName;
    static const char* esriTokenName;
    static const char* defaultFirmwareTypeName;
    static const char* gstDebugName;
    static const char* followTargetName;

    // Application wide file extensions
    static const char* parameterFileExtension;
    static const char* planFileExtension;
    static const char* missionFileExtension;
    static const char* waypointsFileExtension;
    static const char* fenceFileExtension;
    static const char* rallyPointFileExtension;
    static const char* telemetryFileExtension;
    static const char* kmlFileExtension;
    static const char* logFileExtension;

    // Child directories of savePath for specific file types
    static const char* parameterDirectory;
    static const char* telemetryDirectory;
    static const char* missionDirectory;
    static const char* logDirectory;
    static const char* videoDirectory;
    static const char* crashDirectory;


    quint16 allport   () {  return _allport; }
    const QHostAddress& address   () {   return _alladdress; }
    const QString       allhost      () {  return _alladdress.toString(); }

     QString    alltopic      () { return _alltopic; }

     QHostAddress _alladdress = QHostAddress("118.190.91.165") ;//QHostAddress("192.168.124.200") ; //
     quint16     _allport  = 9002;
     QString     _alltopic =  "2d0358e55cdc1ce8"; // "d92c5b2831f237fa"; //"60f33b97637dcd30";//


     void allsetPort   (quint16 allport);

     void setAddress (const QHostAddress& address);
     void allsetHost    (const QString host);

     void allsetTopic    (QString topic);

     void  setusename (QString usename ) ;
     void  setpassword(QString password);
     void  setuserloginshow(bool userloginshow);

     QString  usename()  { return _usename ;}
     QString  password() {return _password;}
     bool    userloginshow() {return _userloginshow;}




signals:
    void savePathsChanged(void);


    void  allportChanged();
    void  allhostChanged();
    void  alltopicChanged();
    void  usenameChanged();
    void  passwordChanged();
    void  userloginshowChanged();

    void send_com_ip(void);
    void send_camera_ip(void);

private slots:
    void _indoorPaletteChanged(void);
    void _checkSavePathDirectories(void);

    void allsetHost1(void);
    void netsendip(void);





private:
    SettingsFact* _offlineEditingFirmwareTypeFact;
    SettingsFact* _offlineEditingVehicleTypeFact;
    SettingsFact* _offlineEditingCruiseSpeedFact;
    SettingsFact* _offlineEditingHoverSpeedFact;
    SettingsFact* _offlineEditingAscentSpeedFact;
    SettingsFact* _offlineEditingDescentSpeedFact;
    SettingsFact* _batteryPercentRemainingAnnounceFact;
    SettingsFact* _defaultMissionItemAltitudeFact;
    SettingsFact* _telemetrySaveFact;
    SettingsFact* _telemetrySaveNotArmedFact;
    SettingsFact* _audioMutedFact;
    SettingsFact* _virtualJoystickFact;
    SettingsFact* _appFontPointSizeFact;
    SettingsFact* _indoorPaletteFact;
    SettingsFact* _showLargeCompassFact;
    SettingsFact* _savePathFact;
    SettingsFact* _autoLoadMissionsFact;
    SettingsFact* _useChecklistFact;
    SettingsFact* _mapboxTokenFact;
    SettingsFact* _esriTokenFact;
    SettingsFact* _defaultFirmwareTypeFact;
    SettingsFact* _gstDebugFact;
    SettingsFact* _followTargetFact;


    static const char* _ipAdressSettingsKey;
    static const char* _tcpPortSettingsKey;
    static const char* _deviceIDSettingsKey;
    static const char* _usenameSettingsKey;
    static const char* _passwordSettingsKey;
    static const char* _userloginKey;

    QString _usename;
    QString _password;
    bool _userloginshow;
    QString _ipAdress;
    int    _tcpPort;
    QString _deviceID;
    userlogin* _userlogin;

    void saveuserlogin(void);
    void loaduserlogin(void);
};

#endif
