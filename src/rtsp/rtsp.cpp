#include <QTimer>
#include <QList>
#include <QDebug>
#include <QMutexLocker>
#include <iostream>
#include <QNetworkProxy>
#include "QGC.h"
#include <QHostInfo>
#include <QSignalSpy>
#include "LinkManager.h"
#include <QTcpSocket>
#include "qmqtt_client.h"
#include "qmqtt_client_p.h"
#include <qmqtt.h>
#include <QCoreApplication>
#include <qmqtt_global.h>
#include "qmqtt_network_p.h"
#include "rtsp.h"
#include <QHostInfo>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include "rtsp_demo.h"
#include "comm.h"
//#include <android/log.h>
#include "rtsp_utils.h""
#define HOST "118.190.91.165".



#define PORT  9002
#define KEEP_ALIVE 60

#define MAX_BLOCK_SIZE  1448  //2896 //488   1448  //


//struct myserver *ms = NULL;
uint8_t    tempbuffer[1024*1024];

const QHostAddress EXAMPLE_HOST =QHostAddress("118.190.91.165");
const quint16 EXAMPLE_PORT = 9002;
const QString EXAMPLE_TOPIC = "image";

rtsp_demo_handle g_rtsplive = NULL;
rtsp_session_handle session= NULL;

int g_s32Quit=1;

int curindex=-1;
int fpart = 0;
int nsize=0;
unsigned  int buffer_lenth,count=0,count1000=0;


typedef struct SENDData
{
    unsigned int index;
    unsigned int part;
    unsigned int npart;
    unsigned int len;
    uint8_t     data[MAX_BLOCK_SIZE];
    uint64_t    pts;
}SENDData;

RTSPLink::RTSPLink()
    : QThread(0)
    , _client(NULL)
   // ,_socket(NULL)
    , _socketIsConnected(false)
{
  //  Q_ASSERT(_mytcpConfig);
     moveToThread(this);
     QObject::connect(this,&RTSPLink::rtsp_run,this,&RTSPLink::rt_run);
     qWarning() << "RTSPLink RTSPLink" << endl;
   // rt_run();
}

RTSPLink::~RTSPLink()
{
    _disconnect();
    // Tell the thread to exit
    quit();
    // Wait for it to exit
    wait();

}

void RTSPLink::rt_run()
{
   //   start(HighPriority);
     qWarning() << "RTSPLink run" << g_rtsplive << endl;
     if(!g_rtsplive){
         rtsp_new_demo(1554);
    }

    _hardwareConnect();
     exec();
}

void RTSPLink::writeBytes()
{
#ifdef TCPLINK_READWRITE_DEBUG
    _writeDebugBytes(data);
#endif
       quint16 msgid = 0;
       quint8 header = 1;
       QString str_buf;
    if (_client) {
         qWarning() << "mytcp _writeBytes" << endl;
    //     _client->connectToHost();
         _client->subscribe(_image_top, 0);
    }
}

void RTSPLink::readBytes123(){
      count++;
      if(count>=1000){
          count=0;
          count1000++;
           qWarning() << count1000 << endl;
      }
      return;
}

/**
 * @brief Read a number of bytes from the interface.
 *
 * @param data Pointer to the data byte array to write the bytes to
 * @param maxLength The maximum number of bytes to write
 **/
void RTSPLink::readBytes()
{
  //  qWarning() << "RTSPLink readBytes" << endl;
  if (_client) {
         QMQTT::Message message;
        //   buffer;

     //    qWarning() << "RTSPLink readBytes" << endl;

         QByteArray buffer=_client->get_date_buf_out();
      //   SENDData senddata;
     //    qWarning() << "RTSPLink onNetworkReceived" << buffer << endl;

         char tmp[512];
         if(g_rtsplive)
         {

          // if(strstr(message->topic,"image")&&message->payloadlen)
          //  {
              //   printf("image -------- -----------\n",message->payload);
              SENDData senddata;
              memcpy(&senddata,(struct SENDData *)(buffer.data()),sizeof(senddata));
           //      memcpy(&senddata,(struct SENDData *)buffer,sizeof(senddata));
               //  if(senddata.part == 0){
           //   if(senddata.npart%1000)
           //          qWarning() << "pts" << senddata.npart << senddata.index << endl;
              //  memcpy(tempbuffer , &senddata.data , senddata.len);
              //   rtsp_sever_tx_video(g_rtsplive,session,senddata.data,senddata.len,senddata.pts);
          /*
              while((rsize = recv(fd, (char *)udpdata, UDP_PLAYLOAD_SIZE, 0)) >= 0)
              {
                                //   printf("--------%d--------%d-------%lld---------\n",rsize ,udpdata->len,udpdata->pts);
                     if(curindex != udpdata->index)
                     {
                         curindex=udpdata->index;
                                       if(fpart!=0){
                            fpart=0;
                                      //    uint64_t pts = current_timestamp();
                            rtsp_sever_tx_video(g_rtsplive,session,tempbuffer,nsize,udpdata->pts);
                                          nsize=0;
                                       }

                         memcpy(tempbuffer+nsize , udpdata->data , udpdata->len);
                         nsize+=udpdata->len;
                                    fpart ++;
                     }
                     else
                     {
                          memcpy(tempbuffer+nsize , udpdata->data , udpdata->len);
                          nsize+=udpdata->len;
                     }

               }

           */

                     if(curindex != senddata.index){
                       //   QDateTime current_date_time =QDateTime::currentDateTime();
                       //   QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
                   //         qWarning() << "RTSPLink g_rtsplive1" << endl;
                         curindex=senddata.index;

                         rtsp_sever_tx_video(g_rtsplive,session,tempbuffer,nsize,senddata.pts);
                      //   rtsp_sever_tx_video(g_rtsplive,session,tempbuffer,nsize,0);
                         nsize=0;
                         fpart=0;
                          memcpy(tempbuffer+nsize , &senddata.data , senddata.len);
                       //  memcpy(tempbuffer+fpart*MAX_BLOCK_SIZE , &senddata.data , senddata.len);
                         nsize+=senddata.len;
                         fpart ++;

                     }else{

                         //   memcpy(tempbuffer+fpart*MAX_BLOCK_SIZE , &senddata.data , senddata.len);
                         memcpy(tempbuffer+nsize , &senddata.data , senddata.len);
                         nsize+=senddata.len;
                         fpart ++;
                     }

                     return;



/*
                         if((senddata.data[0]==0x00)&&(senddata.data[1]==0x00)&&(senddata.data[2]==0x00)&&(senddata.data[3]==0x01)){
                              rtsp_sever_tx_video(g_rtsplive,session,tempbuffer,nsize,senddata.pts);
                              nsize=0;
                              fpart=0;
                              memcpy(tempbuffer+fpart*MAX_BLOCK_SIZE , &senddata.data , senddata.len);
                              nsize+=senddata.len;
                              fpart ++;

                         }else{
                             memcpy(tempbuffer+fpart*MAX_BLOCK_SIZE , &senddata.data , senddata.len);
                             nsize+=senddata.len;
                             fpart ++;

                         }*/

                        //   return;
             //   }
             /*  if(senddata.part == 1)
                {
                        rtsp_sever_tx_video(g_rtsplive,session,senddata.data,senddata.len,senddata.pts);
                    //    qWarning() << "RTSPLink0 " << senddata.data[0] << senddata.data[1] << senddata.data[2] << senddata.data[3] << senddata.data[4]<< endl;
                        curindex=-1;
                        nsize=0;
                        fpart=0;
                        return;
                 }

                 if(senddata.part > 1)
                 {
                       if(curindex == -1)
                          curindex=senddata.index;

                       if(curindex == senddata.index)
                       {
                           if(fpart == senddata.npart)
                           {
                              // memcpy(tempbuffer+senddata.npart*MAX_BLOCK_SIZE , &senddata.data , senddata.len);
                               memcpy(tempbuffer+nsize , &senddata.data , senddata.len);
                               nsize+=senddata.len;
                               fpart ++;

                           }
                           if(senddata.npart == senddata.part-1)
                           {
                               //  qWarning() << "RTSPLink1 " << tempbuffer[0] << tempbuffer[1] << tempbuffer[2] << tempbuffer[3] << tempbuffer[4]<< endl;

                                 rtsp_sever_tx_video(g_rtsplive,session,tempbuffer,nsize,senddata.pts);

                                        //     printf(" multi package:---len: %d --pts: %ld-------   \n",nsize,senddata.pts);

                                             curindex=-1;
                                             nsize=0;
                                             fpart=0;
                                             return;
                           }
                       }
                    }
                 */

         }





      //  buffer.resize(byteCount);
     //    _client->read(buffer.data(), buffer.size());

   //   emit bytesReceived(this, buffer);
   //   _logInputDataRate(byteCount, QDateTime::currentMSecsSinceEpoch());


  }

}


/**
 * @brief Disconnect the connection.
 *
 * @return True if connection has been disconnected, false if connection couldn't be disconnected.
 **/
void RTSPLink::_disconnect(void)
{
     quit();
     wait();
    qWarning() << "mytcp _disconnect1" << _client << endl;
    if (_client) {
         qWarning() << "mytcp _disconnect2" << endl;
         _socketIsConnected = false;
      //  _client = false;
        qWarning() << "mytcp _disconnect1" << endl;
        _client->disconnectFromHost(); // Disconnect tcp
        qWarning() << "mytcp _disconnect2" << endl;
        _client->waitForDisconnected();
        qWarning() << "mytcp _disconnect3" << endl;
        _client->deleteLater(); // Make sure delete happens on correct thread
         qWarning() << "mytcp _disconnect4" << endl;
        _client = NULL;
      //  qWarning() << "mytcp _disconnect123" << endl;
    }
}


/**
 * @brief Connect the connection.
 *
 * @return True if connection has been established, false if connection couldn't be established.
 **/
bool RTSPLink::_connect(void)
{
    if (isRunning())
    {
       quit();
        wait();
    }
   start(HighPriority);
    return true;
}


bool RTSPLink::_hardwareConnect()
{
   // _network = QMQTT::NetworkInterface();
     qWarning() << "RTSPLink _hardwareConnect 1" << endl;
     if (_client) {
          qWarning() << "mytcp _disconnect2" << endl;
          _socketIsConnected = false;
         _client->disconnectFromHost(); // Disconnect tcp
         _client->waitForDisconnected();
         _client->deleteLater(); // Make sure delete happens on correct thread
         _client = NULL;
     }
      Q_ASSERT(_client == NULL);
     _client =  new QMQTT::Client(QHostAddress(_host), 9002);
     qWarning() << "_client _hardwareConnect OK?" << _client << endl;
     count=0;
     count1000=0;
    _client->connectToHost();
 //   QObject::connect(_client, &QMQTT::Client::writeBytes, this, &RTSPLink::writeBytes);
    QObject::connect(this , &RTSPLink::rtsp_writeBytes_run, this, &RTSPLink::writeBytes);
    QObject::connect(_client, &QMQTT::Client::readByte, this, &RTSPLink::readBytes);

  //   writeBytes();
    _socketIsConnected = true;

  //  emit connected();
      _client->subscribe(_image_top, 0);
      _client->subscribe(_image_top, 0);

      qWarning() << "+__host" << _host << _image_top << endl;
    return true;

}
//===================================
bool RTSPLink::rtsp_new_demo (quint16 port)
{
    g_rtsplive = create_rtsp_demo(1554);
    if(g_rtsplive){
          qWarning() << "RTSPLink _hardwareConnect b" << g_rtsplive << endl;
    }
    session= create_rtsp_session(g_rtsplive,"/live.sdp");
  //  session= create_rtsp_session(g_rtsplive,"/video.sdp");
}

//======================================


void RTSPLink::_socketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
     qWarning() << "MYTCP:" << "Link Error" ;
   //emit communicationError(tr("Link Error"), tr("Error on link %1. Error on socket: %2.").arg(getName()).arg(_socket->errorString()));
}

bool RTSPLink::isConnected() const
{
     qWarning() << "RTSPLink isConnected" <<  _socketIsConnected << endl;
    return _socketIsConnected;
}

void RTSPLink::waitForBytesWritten(int msecs)
{
    Q_ASSERT(_client);
    _client->waitForBytesWritten(msecs);
}

void RTSPLink::waitForReadyRead(int msecs)
{
    Q_ASSERT(_client);
    _client->waitForReadyRead(msecs);

}

void RTSPLink::_restartConnection()
{
    if(this->isConnected())
    {
        _disconnect();
        _connect();
    }
}

