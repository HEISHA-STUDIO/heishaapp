/****************************************************************************
 *
 *   (c) 2009-2018 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


/// @file
///     @author Lorenz Meier <mavteam@student.ethz.ch>

#pragma once

#include <QList>
#include <QMultiMap>
#include <QMutex>

#include "LinkConfiguration.h"
#include "LinkInterface.h"
#include "QGCLoggingCategory.h"
#include "QGCToolbox.h"
#include "ProtocolInterface.h"
#include "MAVLinkProtocol.h"
#if !defined(__mobile__)
#include "LogReplayLink.h"
#include "UdpIODevice.h"
#endif
#include "QmlObjectListModel.h"

#ifndef NO_SERIAL_LINK
    #include "SerialLink.h"
#endif

#ifdef QT_DEBUG
    #include "MockLink.h"
#endif

#include <QTcpSocket>
#include "qmqtt_client.h"
#include "qmqtt_client_p.h"
#include <qmqtt.h>
#include <QCoreApplication>
#include <qmqtt_global.h>

#include "qmqtt_network_p.h"
Q_DECLARE_LOGGING_CATEGORY(LinkManagerLog)
Q_DECLARE_LOGGING_CATEGORY(LinkManagerVerboseLog)

class QGCApplication;
class UDPConfiguration;
class AutoConnectSettings;
class QMQTT::Client;

/// Manage communication links
///
/// The Link Manager organizes the physical Links. It can manage arbitrary
/// links and takes care of connecting them as well assigning the correct
/// protocol instance to transport the link data into the application.

class ChangePad : public QGCTool
{
    Q_OBJECT

    /// Unit Test has access to private constructor/destructor


public:
    ChangePad(QGCApplication* app, QGCToolbox* toolbox);
  //  ChangePad();
    ~ChangePad();
   // Q_PROPERTY(QmlObjectListModel*  ChangePads  READ _qmlChangePads                                           NOTIFY _qmlChangePadsChanged)

    Q_INVOKABLE void Close(void);
    Q_INVOKABLE void Open(void);
    Q_INVOKABLE void Charge(void);
    Q_INVOKABLE void Drone(void);
    Q_INVOKABLE void RC(void);
  //  Q_INVOKABLE void OTGO(void);
   // Q_INVOKABLE void OTGC(void);
  //  Q_INVOKABLE void Check(void);

    void SetMqttIP(QHostAddress& address,quint16 port);
signals:
    void _close();

private slots:


private:
    void senddate(QByteArray byte_buf);

     QMQTT::Client*     _client;
     quint16 _number;
};
