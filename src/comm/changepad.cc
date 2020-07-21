/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include <QList>
#include <QApplication>
#include <QDebug>
#include <QSignalSpy>

#ifndef NO_SERIAL_LINK
#include "QGCSerialPortInfo.h"
#endif

#include "changepad.h"
#include "QGCApplication.h"
#include "UDPLink.h"
#include "TCPLink.h"
#include "MYTCPLink.h"
#include "SettingsManager.h"
#ifdef QGC_ENABLE_BLUETOOTH
#include "BluetoothLink.h"
#endif
#include "MYTCPLink.h"
#ifndef __mobile__
#include "GPSManager.h"
#include "PositionManager.h"
#endif

QGC_LOGGING_CATEGORY(ChangePadLog, "ChangePadLog")
QGC_LOGGING_CATEGORY(ChangePadVerboseLog, "ChangePadVerboseLog")
extern QString topicto;
//const char* ChangePad::_defaultUPDLinkName =       "UDP Link (AutoConnect)";

//const int ChangePad::_autoconnectUpdateTimerMSecs =   1000;
/*
#ifdef Q_OS_WIN
// Have to manually let the bootloader go by on Windows to get a working connect
const int LinkManager::_autoconnectConnectDelayMSecs =  6000;
#else
const int ChangePad::_autoconnectConnectDelayMSecs =  1000;
#endif
*/


ChangePad::ChangePad(QGCApplication* app, QGCToolbox* toolbox)
    : QGCTool(app, toolbox)
   // , _configUpdateSuspended(false)
  //  , _configurationsLoaded(false)
   //  , _connectionsSuspended(false)
  // , _mavlinkChannelsUsedBitMask(1)    // We never use channel 0 to avoid sequence numbering problems
 //    , _autoConnectSettings(nullptr)
  //  , _mavlinkProtocol(nullptr)
{
   qmlRegisterUncreatableType<ChangePad>         ("QGroundControl", 1, 0, "ChangePad",         "Reference only");
   // qmlRegisterUncreatableType<LinkConfiguration>   ("QGroundControl", 1, 0, "LinkConfiguration",   "Reference only");
   // qmlRegisterUncreatableType<LinkInterface>       ("QGroundControl", 1, 0, "LinkInterface",       "Reference only");

 //   Q_ASSERT(_client == NULL);
 // const QHostAddress& host = QHostAddress("87.236.103.96");//QHostAddress::LocalHost,
  // const QHostAddress& host = QHostAddress("139.199.24.138");//QHostAddress::LocalHost,
   const QHostAddress& host = QHostAddress("118.190.91.165");//QHostAddress::LocalHost,
  //   const QHostAddress& host = QHostAddress("192.168.124.19");//QHostAddress::LocalHost,
 //   const QHostAddress& host = QHostAddress("51.38.230.217");//QHostAddress::LocalHost,
   const quint16 port = 9002;
 //    _client =  new QMQTT::Client(_mytcpConfig->address(), _mytcpConfig->port(),_network);
     _client =  new QMQTT::Client(host, port);
     _client->connectToHost();
  //   QByteArray byte_buf;
  //   byte_buf="start";
 //    senddate(byte_buf);
   //  QObject::connect(_client, &QMQTT::Client::readByte, this, &MyTCPLink::readBytes);


  //  QObject::connect(this, &QMQTT::Client::readByte, MyTCPLink::_client, &MyTCPLink::_writeByteschangepad);
}


/*
ChangePad::ChangePad()
{

}
*/
 ChangePad::~ChangePad()
{

}

 void ChangePad::SetMqttIP(QHostAddress& host,quint16 port){
     Q_ASSERT(_client == NULL);
      //   host = QHostAddress("139.199.24.138");//QHostAddress::LocalHost,.
          host = QHostAddress("118.190.91.165");//QHostAddress::LocalHost,
         port = 9002;
      //    _client =  new QMQTT::Client(_mytcpConfig->address(), _mytcpConfig->port(),_network);
          _client =  new QMQTT::Client(host, port);
          _client->connectToHost();
          QByteArray byte_buf;
          byte_buf="start";
          senddate(byte_buf);
 }

void ChangePad::senddate(QByteArray byte_buf){
    QMQTT::Message message(_number, "changepad",
                           QString("Number is %1").arg(_number).toUtf8());



   QString topic_standard  = topicto;
   qWarning() << topic_standard << endl;
 // QString topic_pub = topic_standard;
 // QString topic_sub = topic_standard;
 // topic_pub.append("-2");
 // topic_sub.append("-1");
  // message.setTopic("changepad");
   message.setTopic(topic_standard);
   message.setQos(0);

   message.setPayload(byte_buf);

   _client->publish(message);

 }



void ChangePad::Close(){

    qWarning() << "Close" << endl;
    QByteArray byte_buf;
    byte_buf="Close";
    senddate(byte_buf);
}

void ChangePad::Open( ){

qWarning() << "Open" << endl;
QByteArray byte_buf;
byte_buf="Open";
senddate(byte_buf);
}

void ChangePad::Charge( ){
qWarning() << "Charge" << endl;
QByteArray byte_buf;
byte_buf="Charge";
senddate(byte_buf);



}

void ChangePad::Drone(){

qWarning() << "Drone" << endl;
QByteArray byte_buf;
byte_buf="Drone";
senddate(byte_buf);
}

void ChangePad::RC(){

qWarning() << "RC" << endl;
QByteArray byte_buf;
byte_buf="RC";
senddate(byte_buf);
}
/*
void ChangePad::OTGO(){

qWarning() << "OTGO" << endl;
QByteArray byte_buf;
byte_buf="OTGO";
senddate(byte_buf);
}
void ChangePad::OTGC(){
qWarning() << "OTGC" << endl;
QByteArray byte_buf;
byte_buf="OTGC";
senddate(byte_buf);

}
void ChangePad::Check(){
qWarning() << "Check" << endl;
QByteArray byte_buf;
byte_buf="Check";
senddate(byte_buf);

}
*/
