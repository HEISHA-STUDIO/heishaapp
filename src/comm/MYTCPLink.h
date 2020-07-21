/****************************************************************************
 *
 *   (c) 2009-2018 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


/// @file
///     @brief TCP link type for SITL support
///
///     @author Don Gagne <don@thegagnes.com>

#pragma once

#include <QString>
#include <QList>
#include <QMap>
#include <QMutex>
#include <QHostAddress>
#include <LinkInterface.h>
#include "QGCConfig.h"
#include "LinkManager.h"

// Even though QAbstractSocket::SocketError is used in a signal by Qt, Qt doesn't declare it as a meta type.
// This in turn causes debug output to be kicked out about not being able to queue the signal. We declare it
// as a meta type to silence that.
#include <QMetaType>
#include <QTcpSocket>
#include "qmqtt_client.h"
#include "qmqtt_client_p.h"
#include <qmqtt.h>
#include <QCoreApplication>
#include <qmqtt_global.h>

#include "qmqtt_network_p.h"
#include "changepad.h"

//#define TCPLINK_READWRITE_DEBUG   // Use to debug data reads/writes



class MyTCPLinkTest;
class AppSettings;

#define QGC_MyTCP_PORT 9002
#define QGC_MyTCP_TOPIC   "serial"

class MyTCPConfiguration : public LinkConfiguration
{
    Q_OBJECT
     friend class QMQTT::Client;
     friend class SettingsManager;

public:

    Q_PROPERTY(quint16  port    READ myport    WRITE mysetPort    NOTIFY myportChanged)
    Q_PROPERTY(QString  host    READ host    WRITE setHost    NOTIFY hostChanged)
    Q_PROPERTY(QString  topic   READ mytopic   WRITE mysetTopic   NOTIFY mytopicChanged)

 //   Q_PROPERTY(QString  topic   READ topic  WRITE setTopic  NOTIFY topicChanged)


    /*!
     * @brief Regular constructor
     *
     * @param[in] name Configuration (user friendly) name
     */
    MyTCPConfiguration(const QString& name);

    /*!
     * @brief Copy contructor
     *
     * When manipulating data, you create a copy of the configuration, edit it
     * and then transfer its content to the original (using copyFrom() below). Use this
     * contructor to create an editable copy.
     *
     * @param[in] source Original configuration
     */
    MyTCPConfiguration(MyTCPConfiguration* source);
    /*!
     * @brief The TCP port
     *
     * @return Port number
     */
    quint16 myport   () { return _port; }

    /*!
     * @brief Set the TCP port
     *
     * @param[in] port Port number
     */
    void mysetPort   (quint16 myport);

    /*!
     * @brief The host address
     *
     * @return Host address
     */
    const QHostAddress& address   () { return _address; }
    const QString       host      () { return _address.toString(); }

     QString    mytopic      () { return _topic; }

    /*!
     * @brief Set the host address
     *
     * @param[in] address Host address
     */
    void setAddress (const QHostAddress& address);
    void setHost    (const QString host);

    void mysetTopic    (QString topic);

    /// From LinkConfiguration
    LinkType    type            () { return LinkConfiguration::TypeMyTcp; }
    void        copyFrom        (LinkConfiguration* source);
    bool        isHighLatencyAllowed () { return true; }
    void        loadSettings    (QSettings& settings, const QString& root);
    void        saveSettings    (QSettings& settings, const QString& root);
    void        updateSettings  ();
    QString     settingsURL     () { return   "MyTcpSettings.qml"; }
    QString     settingsTitle   () { return tr("MyTCP Link Settings"); }

signals:
    void  myportChanged();
    void  hostChanged();
    void  mytopicChanged();

private:
    QHostAddress _address;
    quint16     _port;
    QString     _topic;
};


class MyTCPLink : public LinkInterface
{
    Q_OBJECT

    friend class MyTCPLinkTest;
    friend class MyTCPConfiguration;
    friend class LinkManager;
    friend class QMQTT::Client;
    friend class QMQTT::Network;

public:
    QMQTT::Client* getSocket(void) { return _client; }

    void signalBytesWritten(void);

    // LinkInterface methods
    virtual QString getName(void) const;
    virtual bool    isConnected(void) const;
    virtual void    requestReset(void) {};




    // Extensive statistics for scientific purposes
    qint64 getConnectionSpeed() const;
    qint64 getCurrentInDataRate() const;
    qint64 getCurrentOutDataRate() const;

    // These are left unimplemented in order to cause linker errors which indicate incorrect usage of
    // connect/disconnect on link directly. All connect/disconnect calls should be made through LinkManager.
    bool connect(void);
    bool disconnect(void);
 QMQTT::Client* getclient();


private slots:
    // From LinkInterface
    void _writeBytes(const QByteArray data);
    void  _writeByteschangepad(const QByteArray data);

    void write_host(QString address_temp);
    //============================
    void write_port(quint16   port_temp);

    //============================
    void write_topic(QString topic_temp);

public slots:
    void waitForBytesWritten(int msecs);
    void waitForReadyRead(int msecs);

protected slots:
    void _socketError(QAbstractSocket::SocketError socketError);

    // From LinkInterface
    virtual void readBytes(void);

protected:
    // From LinkInterface->QThread
    virtual void run(void);

private:
    // Links are only created/destroyed by LinkManager so constructor/destructor is not public
    MyTCPLink(SharedLinkConfigurationPointer& config);
    ~MyTCPLink();

    // From LinkInterface
    virtual bool _connect(void);
    virtual void _disconnect(void);

    bool onconnected(void);
    bool disonconnected(void);
    bool onTimeout(void);
    bool onSubscribed( QString& topic);
    bool onReceived(const QMQTT::Message& message);

    bool _hardwareConnect();
    void _restartConnection();

#ifdef TCPLINK_READWRITE_DEBUG
    void _writeDebugBytes(const QByteArray data);
#endif

    MyTCPConfiguration* _mytcpConfig;
  //  QMQTT::Client*       _socket;
    QMQTT::Client*       _client;
    bool              _socketIsConnected;
    QMQTT::NetworkInterface* _network;
    AppSettings*   _appSettings;


    quint64 _bitsSentTotal;
    quint64 _bitsSentCurrent;
    quint64 _bitsSentMax;
    quint64 _bitsReceivedTotal;
    quint64 _bitsReceivedCurrent;
    quint64 _bitsReceivedMax;
    quint64 _connectionStartTime;
    QMutex  _statisticsMutex;
    QTimer _timer;
    QTimer _pingResponseTimer;
    quint16 _number;

    QString _topic_standard;
    QString _host;
    quint16 _port;


};












