#include "userlogin.h"

#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QtAndroid>
#include <QDebug>
#include <QMessageBox>

#include <QTimer>
#include <QList>
#include <QDebug>
#include <QMutexLocker>
#include <QHostInfo>
#include <QMessageBox>
#include <QClipboard>

#include <iostream>
#include <Eigen/Eigen>

#include <QDir>

#include <QFile>
#include <QIODevice>
#include <QFileInfo>
#include <QProgressBar>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDir>


//#include "QGCMessageBox.h"
userlogin::userlogin(QObject  *parent)
{
    qWarning() << "update" << endl;
  //  installApk("/data/local/tmp/blacksand.apk");
    manager = new QNetworkAccessManager(this);          //新建QNetworkAccessManager对象
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//关联信号和槽
    manager_time=0;
}
 
userlogin::~userlogin()
{
   
}

void userlogin::on_LoginButton_clicked()
{

   // QFile file(":/json/userlogin.json");
    QFile file(QStringLiteral(":/json/UserLogin.json"));
     //QFile file(QStringLiteral("../qgroundcontrol/scr/User"));

    if (!file.exists()) {
          qWarning() << "file.exists" << endl;
           return;
    }

  //  QFile file("userlogin12.json");
    if(!file.open(QIODevice::ReadOnly))
    {
    //    QMessageBox::critical(this, "错误", "文件打开失败，信息未写入", "确定");
            qWarning() << "userlogin.json err" << endl;
         return;
    }
     QString str2 =_passwordlonin;
     QString str4 =_usenamelonin;

     QString str1 = "{" ;
     str1.append("\n");
     str1.append("    ");
     str1.append("\"");
     str1.append("username");
     str1.append("\"" );
     str1.append(":" );
     str1.append("   ");
     str1.append("\"");
     str1.append(str4);
     str1.append("\"" );
     str1.append(",");
     str1.append("\n");
     str1.append("    ");
     str1.append("\"");
     str1.append("password");
     str1.append("\"");
     str1.append(":");
     str1.append("   ");
     str1.append("\"");
     str1.append(str2);
     str1.append("\"");
     str1.append("\n");
     str1.append("}");
     str1.append("\n");
/*
     QJsonParseError err_rpt;
     QJsonDocument  root_Doc = QJsonDocument::fromJson(str1.toUtf8(),&err_rpt);//字符串格式化为JSON

     if(err_rpt.error != QJsonParseError::NoError)
     {
           qWarning() << "str1 root格式错误";

     }

      QString str5 = root_Doc.toJson();
      qWarning() << str5.toUtf8() << endl;

*/

     QString str = file.readAll();


    QNetworkRequest quest;
    quest.setUrl(QUrl("http://118.190.91.165:65530/usercfg")); //包含最新版本软件的下载地址

    quest.setHeader(QNetworkRequest::UserAgentHeader,"RT-Thread ART");

    quest.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
        //发送请求
   //  manager->post(request,ui->editPostData->toPlainText().toUtf8());

     manager->post(quest,str1.toUtf8());

     qWarning() << str1.toUtf8() << endl;

     qWarning() << "on_LoginButton_clicked" << endl;
}


void userlogin::replyFinished(QNetworkReply *reply)
{
    QFile file("heishaversion.json");
    if(!file.open(QIODevice::WriteOnly ))
    {
    //    QMessageBox::critical(this, "错误", "文件打开失败，信息未写入", "确定");
            qWarning() << "replyFinished err" << endl;
        return;
    }
    QString str = reply->readAll();//读取接收到的数据
     qWarning() << str << endl;
    parse_UpdateJSON(str);
    reply->deleteLater();               //销毁请求对象
}


int userlogin::parse_UpdateJSON(QString str)
{
    //    QMessageBox msgBox;
    QJsonParseError err_rpt;
    QJsonDocument  root_Doc = QJsonDocument::fromJson(str.toUtf8(),&err_rpt);//字符串格式化为JSON

    if(err_rpt.error != QJsonParseError::NoError)
    {
          qWarning() << "root格式错误";
        return -1;
    }
        qWarning() << "parse_UpdateJSON" << endl;
    if(root_Doc.isObject())
    {
        QJsonObject  root_Obj = root_Doc.object();   //创建JSON对象，不是字符串
     //   QJsonObject PulseValue = root_Obj.value("PulseSensor").toObject();
     //   QString Verison = PulseValue.value("LatestVerison").toString();  //V1.0
    //    QString Url = PulseValue.value("Url").toString();        //https://wcc-blog.oss-cn-beijing.aliyuncs.com/QtUpdate/uFun_Pulse_v1.0.exe
    //    QString UpdateTime = PulseValue.value("UpdateTime").toString();
   //     QString ReleaseNote = PulseValue.value("ReleaseNote").toString();
        _ip = root_Obj.value("ip").toString();
        _port = root_Obj.value("port").toInt();
        _topic = root_Obj.value("topic").toString();
        emit netipchange();
      //   root_Obj.remove("ip");
       // root_Obj.insert("ip","118.191.90.165");




    }
    return 0;
}


int userlogin::get_port(){
      return _port;
}

QString userlogin::get_topic(){

     return  _topic;
}


QString userlogin::get_ip(){
     return  _ip;
}



