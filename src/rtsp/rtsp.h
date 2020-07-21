#pragma once
#include <QTimer>
#include <QList>
#include <QDebug>
#include <QMutexLocker>
#include <iostream>

#include <QMap>
#include <QMutex>
#include <QHostAddress>
#include <QMetaType>
#include <QTcpSocket>
#include "qmqtt_client.h"
#include "qmqtt_client_p.h"
#include <qmqtt.h>
#include <QCoreApplication>
#include <qmqtt_global.h>
#include "qmqtt_network_p.h"
#include "rtsp_demo.h"
#include <QThread>

#include <QUdpSocket>

class RTSPLink: public QThread
{
    Q_OBJECT

    friend class QMQTT::Client;
    friend class QMQTT::Network;
public:
    QMQTT::Client* getSocket(void) { return _client; }

    bool connect(void);
    bool disconnect(void);
    virtual bool    isConnected(void) const;


    RTSPLink();
    ~RTSPLink();
 //  void rt_run(void);
    bool rtsp_new_demo (quint16 port);

    QString _host;
    qint32  _port;
    QString _image_top;

private slots:
    // From LinkInterface


public slots:
    void waitForBytesWritten(int msecs);
    void waitForReadyRead(int msecs);
    void rt_run(void);

protected slots:
    void _socketError(QAbstractSocket::SocketError socketError);

    // From LinkInterface
    virtual void readBytes(void);
    virtual void readBytes123(void);
    virtual void writeBytes(void);

signals:
    void rtsp_run(void);
    void rtsp_writeBytes_run(void);
protected:


private:
    // Links are only created/destroyed by LinkManager so constructor/destructor is not public


    virtual bool _connect(void);
    virtual void _disconnect(void);

    bool onconnected(void);
    bool disonconnected(void);
    bool onTimeout(void);
    bool onSubscribed(const QString& topic);
    bool onReceived(const QMQTT::Message& message);

    bool _hardwareConnect();
   void _restartConnection();


  //  QMQTT::Client*       _socket;
    QMQTT::Client*       _client;
    bool              _socketIsConnected;
    QMQTT::NetworkInterface* _network;

    QUdpSocket*      _socket;
    bool           _connectState;

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



};

