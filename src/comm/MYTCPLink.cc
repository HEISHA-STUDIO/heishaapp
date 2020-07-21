/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#include <QTimer>
#include <QList>
#include <QDebug>
#include <QMutexLocker>
#include <iostream>
#include "MYTCPLink.h"
#include "LinkManager.h"
#include "QGC.h"
#include <QHostInfo>
#include <QSignalSpy>

#include "qmqtt_client.h"
#include "qmqtt_client_p.h"
#include <qmqtt.h>
#include <QCoreApplication>
#include "qmqtt_network_p.h"

#include "qmqtt_message.h"
#include "qmqtt_message_p.h"

#include "qmqtt_frame.h"
#include "qmqtt_socket_p.h"

#include <QLoggingCategory>
#include <QDataStream>
#include "SettingsManager.h"
#include "AppSettings.h"

const QHostAddress EXAMPLE_HOST = QHostAddress::LocalHost;
const quint16 EXAMPLE_PORT = 9002;
const QString EXAMPLE_TOPIC = "serial-1";
QString topicto ="changpad123";
QHostAddress hostto;
quint16 portto;

MyTCPLink::MyTCPLink(SharedLinkConfigurationPointer& config)
    : LinkInterface(config)
    , _mytcpConfig(qobject_cast<MyTCPConfiguration*>(config.data()))
    , _client(NULL)
    , _socketIsConnected(false)
{
    Q_ASSERT(_mytcpConfig);
    moveToThread(this);
}



MyTCPLink::~MyTCPLink()
{
    _disconnect();
    // Tell the thread to exit
    quit();
    // Wait for it to exit
    wait();
}


quint8 connect_first=0;
void MyTCPLink::run()
{
    _hardwareConnect();

    exec();
}

void MyTCPLink::_writeBytes(const QByteArray data)
{
#ifdef TCPLINK_READWRITE_DEBUG
    _writeDebugBytes(data);
#endif
   //   qWarning() << "mytcp _writeBytes"  <<  _topic_standard << _host << _port << endl;
       quint16 msgid = 0;
       quint8 header = 1;
       QString str_buf;
    if (_client) {
        QMQTT::Message message(_number, EXAMPLE_TOPIC,
                               QString("Number is %1").arg(_number).toUtf8());
       QString topic_pub = _topic_standard;
       QString topic_sub = _topic_standard;
       topic_pub.append("-2");
       topic_sub.append("-1");
       message.setTopic(topic_pub);
       message.setQos(0);

       message.setPayload(data);

        _client->publish(message);
     //   connect_first++;
     //   if(connect_first<2)
            _client->subscribe(topic_sub, 0);
         //  _client->subscribe("serial-1", 0);
      //  else
     //       connect_first=10;
       // QMQTT::Network::sendFrame(frame);
     //test  _socket->write(frame.data());
        _logOutputDataRate(data.size(), QDateTime::currentMSecsSinceEpoch());
    }
}

//============================
void MyTCPLink::write_host(QString address_temp ){
    _host=address_temp;

      //    port_temp ;
     // QString address_temp = _appSettings->allhost();
   //   QString topic_temp = _appSettings->alltopic();

}

//============================
void MyTCPLink::write_port(quint16   port_temp){
    _port=port_temp;
      //    port_temp ;
     // QString address_temp = _appSettings->allhost();
   //   QString topic_temp = _appSettings->alltopic();

}

//============================
void MyTCPLink::write_topic(QString topic_temp){
    _topic_standard=topic_temp;
      //    port_temp ;
     // QString address_temp = _appSettings->allhost();
   //   QString topic_temp = _appSettings->alltopic();

}

void MyTCPLink::_writeByteschangepad(const QByteArray data)
{
#ifdef TCPLINK_READWRITE_DEBUG
    _writeDebugBytes(data);
#endif
      qWarning() << "mytcp _writeBytes" << endl;
       quint16 msgid = 0;
       quint8 header = 1;
       QString str_buf;

    if (_client) {
        QMQTT::Message message(_number, EXAMPLE_TOPIC,
                               QString("Number is %1").arg(_number).toUtf8());
//        QMQTT::Message message(_number, EXAMPLE_TOPIC,
 //                                   QString("Number is %1").arg(_number).);
       QString topic_standard = _mytcpConfig->mytopic();
       QString topic_pub = topic_standard;
       QString topic_sub = topic_standard;
       topic_pub.append("-2");
       topic_sub.append("-1");


        message.setTopic("changepad");
      //  message.setTopic(topic_pub);
        message.setQos(0);

        message.setPayload(data);

        _client->publish(message);

        _logOutputDataRate(data.size(), QDateTime::currentMSecsSinceEpoch());
    }
}

/**
 * @brief Read a number of bytes from the interface.
 *
 * @param data Pointer to the data byte array to write the bytes to
 * @param maxLength The maximum number of bytes to write
 **/
void MyTCPLink::readBytes()
{
/*
      qWarning() << "mytcp readBytes" << endl;
    if (_socket) {

        qint64 byteCount = _socket->bytesAvailable();
        if (byteCount)
        {
            QByteArray buffer;
            QMQTT::Message message;
            buffer.resize(byteCount);
            _socket->read(buffer.data(), buffer.size());

            emit bytesReceived(this, buffer);
            _logInputDataRate(byteCount, QDateTime::currentMSecsSinceEpoch());
#ifdef TCPLINK_READWRITE_DEBUG
            writeDebugBytes(buffer.data(), buffer.size());
#endif
        }
    }
    */

  //  qWarning() << "mytcp readBytes" << endl;
  if (_client) {
         QMQTT::Message message;
        //   buffer;
     //    qWarning() << "mytcp readBytes" << endl;

        QByteArray buffer=_client->get_date_buf_out();
   //      qWarning() << "mytcp onNetworkReceived" << buffer << endl;
      //  buffer.resize(byteCount);
     //    _client->read(buffer.data(), buffer.size());

       emit bytesReceived(this, buffer);
   //   _logInputDataRate(byteCount, QDateTime::currentMSecsSinceEpoch());


  }

}


/**
 * @brief Disconnect the connection.
 *
 * @return True if connection has been disconnected, false if connection couldn't be disconnected.
 **/
void MyTCPLink::_disconnect(void)
{
    quit();
    wait();
    qWarning() << "mytcp _disconnect" << endl;
    if (_client) {

      //  _client = false;
        if(_socketIsConnected){
            qWarning() << "mytcp _disconnect1" << endl;
            _client->disconnectFromHost(); // Disconnect tcp
            qWarning() << "mytcp _disconnect2" << endl;
          //  _client->waitForDisconnected();
            qWarning() << "mytcp _disconnect3" << endl;
             _socketIsConnected = false;
        }
        _client->deleteLater(); // Make sure delete happens on correct thread
         qWarning() << "mytcp _disconnect4" << endl;
        _client = NULL;

        qWarning() << "mytcp _disconnect123" << endl;
    }
}


/**
 * @brief Connect the connection.
 *
 * @return True if connection has been established, false if connection couldn't be established.
 **/
bool MyTCPLink::_connect(void)
{
    if (isRunning())
    {
        quit();
        wait();
    }
    qWarning() << "mytcp _connect" << endl;
    start(HighPriority);
  //  ChangePad.SetMqttIP();
    return true;
}
/*
bool MyTCPLink::onconnected(void){

   // QMQTT::Client::onNetworkConnected();
    //  _client->onNetworkConnected();
      _client->subscribe(EXAMPLE_TOPIC, 0);
      _timer.start(1000);
          qWarning() << "mytcp subscribe" << endl;
}


bool MyTCPLink::disonconnected(void){
        _timer.stop();
        _client->disconnectFromHost();
         qWarning() << "mytcp disconnectFromHost" << endl;
}

bool MyTCPLink::onTimeout(void){
    QMQTT::Message message(_number, EXAMPLE_TOPIC,
                           QString("Number is %1").arg(_number).toUtf8());

    _client->publish(message);
    qWarning() << "mytcp publish(message)" << endl;
     _number++;
  //  if(_number >= 10)
  //  {
   //     _timer.stop();
   //     _socket->disconnectFromHost();
   // }

}

bool MyTCPLink::onSubscribed(const QString& topic){



}

bool MyTCPLink::onReceived(const QMQTT::Message& message){



}

*/

QMQTT::Client* MyTCPLink::getclient()
{
    return _client;
}
bool MyTCPLink::_hardwareConnect()
{
   // _network = QMQTT::NetworkInterface();
     qWarning() << "mytcp _hardwareConnect 1" << endl;
     Q_ASSERT(_client == NULL);

    //  _client =  new QMQTT::Client(QHostAddress("118.190.91.165"), 9002);
      _client =  new QMQTT::Client(QHostAddress(_host), _port);
    // hostto =_mytcpConfig->address();
  //   protto = _mytcpConfig->myport();
     connect_first=0;
  //  qWarning() << "mytcp _hardwareConnect 2" <<  _mytcpConfig->address() << _mytcpConfig->mytopic() << _mytcpConfig->myport() << endl;
     _client->connectToHost();
      QObject::connect(_client, &QMQTT::Client::readByte, this, &MyTCPLink::readBytes);
//-------------------------------
    _socketIsConnected = true;
  //  emit connected();
    return true;

}


void MyTCPLink::_socketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
     qWarning() << "MYTCP:" << "Link Error" ;
   //emit communicationError(tr("Link Error"), tr("Error on link %1. Error on socket: %2.").arg(getName()).arg(_socket->errorString()));
}

/**
 * @brief Check if connection is active.
 *
 * @return True if link is connected, false otherwise.
 **/
bool MyTCPLink::isConnected() const
{
  //   qWarning() << "mytcp isConnected" <<  _socketIsConnected << endl;
    return _socketIsConnected;
}

QString MyTCPLink::getName() const
{
 //   qWarning() << "mytcp getName" << endl;
    return _mytcpConfig->name();
}
qint64 MyTCPLink::getConnectionSpeed() const
{
    return 54000000; // 54 Mbit
}

qint64 MyTCPLink::getCurrentInDataRate() const
{
    return 0;
}

qint64 MyTCPLink::getCurrentOutDataRate() const
{
    return 0;
}

void MyTCPLink::waitForBytesWritten(int msecs)
{
    Q_ASSERT(_client);
    _client->waitForBytesWritten(msecs);
}

void MyTCPLink::waitForReadyRead(int msecs)
{
    Q_ASSERT(_client);
    _client->waitForReadyRead(msecs);

}


void MyTCPLink::_restartConnection()
{
    if(this->isConnected())
    {
        _disconnect();
        _connect();
    }
}

//--------------------------------------------------------------------------
//-- TCPConfiguration

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

MyTCPConfiguration::MyTCPConfiguration(const QString& name) : LinkConfiguration(name)
{
    _port    = QGC_MyTCP_PORT;
    _address = QHostAddress::Any;
    _topic    = QGC_MyTCP_TOPIC;
}

MyTCPConfiguration::MyTCPConfiguration(MyTCPConfiguration* source) : LinkConfiguration(source)
{
    _port    = source->myport();
    _address = source->address();
    _topic   = source->mytopic();
}

void MyTCPConfiguration::copyFrom(LinkConfiguration *source)
{
    LinkConfiguration::copyFrom(source);
    MyTCPConfiguration* usource = dynamic_cast<MyTCPConfiguration*>(source);
    Q_ASSERT(usource != NULL);
    _port    = usource->myport();
    _address = usource->address();
    _topic =  usource->mytopic();
}

void MyTCPConfiguration::mysetPort(quint16 myport)
{
   _port = myport;


 //   qWarning() << "MYTCP:"  << _port;
}

void MyTCPConfiguration::setAddress(const QHostAddress& address)
{
    _address = address;
}

void MyTCPConfiguration::setHost(const QString host)
{
    QString ipAdd = get_ip_address(host);
    if(ipAdd.isEmpty()) {
        qWarning() << "MYTCP:" << "Could not resolve host:" << host;
    } else {
        _address = ipAdd;
    }
}

void MyTCPConfiguration::mysetTopic(QString topic)
{
    _topic = topic;
     qWarning() << "MYTCP:"  << _topic;
}

void MyTCPConfiguration::saveSettings(QSettings& settings, const QString& root)
{
    settings.beginGroup(root);
    settings.setValue("port", (int)_port);
    settings.setValue("host", address().toString());
    settings.setValue("topic", (QString)_topic);
     qWarning() << "saveSettings MYTCP:topic"  << _topic  << _port;
    settings.endGroup();

}

void MyTCPConfiguration::loadSettings(QSettings& settings, const QString& root)
{
    settings.beginGroup(root);
    _port = (quint16)settings.value("port", QGC_MyTCP_PORT).toUInt();
    QString address = settings.value("host", _address.toString()).toString();
    _address = address;
    _topic = settings.value("topic", QGC_MyTCP_TOPIC).toString();
    settings.endGroup();
      qWarning() << "loadSettings MYTCP:topic"  << _topic  << _port;
}

void MyTCPConfiguration::updateSettings()
{
    if(_link) {
        MyTCPLink* ulink = dynamic_cast<MyTCPLink*>(_link);
        if(ulink) {
            ulink->_restartConnection();
        }
    }
}
