#include "update.h"

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

#include <QDir>


//#include "QGCMessageBox.h"
update::update(QObject  *parent)
{
    qWarning() << "update" << endl;
  //  installApk("/data/local/tmp/blacksand.apk");
    manager = new QNetworkAccessManager(this);          //新建QNetworkAccessManager对象
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//关联信号和槽
    manager_time=0;
}
 
update::~update()
{
   
}
 
void update::on_pushButton_clicked()
{
   // QString filename = "http://139.199.24.138:2020/heisha/HEISHA.apk";
   // installApk(filename);

  //  QUrl url("<a target=_blank href="http://mm.newhua.com/down/Samsung_USB_Drivers_for_Mobile_Phones_1.5.9.0.zip"> ");
             //http://mm.newhua.com/down/Samsung_USB_Drivers_for_Mobile_Phones_1.5.9.0.zip</a>");

  //  QUrl url(QStringLiteral("http://139.199.24.138:2020/heisha/blacksand.apk"));

 //   fileInfo = url.path();
 //   file = new QFile(fileInfo.fileName());
 //   file->open(QIODevice::WriteOnly);//只读方式打开文件
  //  accessManager = new QNetworkAccessManager(this);

 //   file = new QFile("/data/local/tmp/heishaversion.json");

    QNetworkRequest quest;
    quest.setUrl(QUrl("http://139.199.24.138:2020/heisha/heishaversion.json")); //包含最新版本软件的下载地址
   //  quest.setUrl(QUrl("http://139.199.24.138:2020/heisha/blacksand.txt")); //包含最新版本软件的下载地址
   //  quest.setUrl(QUrl("http://139.199.24.138:2020/heisha/blacksand.apk")); //包含最新版本软件的下载地址
   //  http://139.199.24.138:2020/heisha/blacksand.txt
    quest.setHeader(QNetworkRequest::UserAgentHeader,"RT-Thread ART");
    quest.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/apk; charset=utf-8"));

    manager->get(quest);    //发送get网络请求
     qWarning() << "on_pushButton_clicked" << endl;
}
 
void update::installApk(const QString &filePath)
{
    QAndroidJniObject jFilePath = QAndroidJniObject::fromString(filePath);
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject::callStaticMethod<void>(
                "org/mavlink/qgroundcontrol/QGCActivity",
                "installApk",
                "(Ljava/lang/String;Lorg/qtproject/qt5/android/bindings/QtActivity;)V",
                jFilePath.object<jstring>(),
                activity.object<jobject>()
                );
    qWarning() << "installApk" << endl;
}

/*
void update::getFilename()
{
    QAndroidJniObject jFilePath = QAndroidJniObject::fromString(filePath);
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject::callStaticMethod<void>(
                "org/mavlink/qgroundcontrol/QGCActivity",
                "getFilename",
                "(Ljava/lang/String;Lorg/qtproject/qt5/android/bindings/QtActivity;)V",
                jFilePath.object<jstring>(),
                activity.object<jobject>()
                );
    qWarning() << "getFilename" << endl;
}
*/
/*
void update::on_btn_chkUpdate_clicked()
{
    QNetworkRequest quest;
    quest.setUrl(QUrl("https://www.demo.com/software_update.json")); //包含最新版本软件的下载地址 k-
    quest.setHeader(QNetworkRequest::UserAgentHeader,"RT-Thread ART");
    manager->get(quest);    //发送get网络请求
}
*/
void update::replyFinished(QNetworkReply *reply)
{

   //  qWarning() << "str123455" << endl;
   //  system("touch /data/local/tmp/tmp.apk");
   //  system("chmod 777 tmp.apk");
    // QFile file("/data/local/tmp/123.apk");
    if(manager_time==0){
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
    }else{

        QFile file("/storage/emulated/0/user_temp.apk");
        if(!file.open(QIODevice::WriteOnly ))
        {
        //    QMessageBox::critical(this, "错误", "文件打开失败，信息未写入", "确定");
                qWarning() << "replyFinished err" << endl;
            return;
        }

        file.write(reply->readAll());
        file.close();
         qDebug() << "文件保存成功!";
        installApk("/storage/emulated/0/user_temp.apk");
       /*
          QFile file("/users/qt/user_temp.apk");
          QTemporaryFile::createNativeFile(file); // Returns 0
          if(!file.open(QIODevice::WriteOnly ))
          {
          //    QMessageBox::critical(this, "错误", "文件打开失败，信息未写入", "确定");
                  qWarning() << "replyFinished err" << endl;
              return;
          }

          file.write(reply->readAll());
          file.close();
           qDebug() << "文件保存成功!";
          installApk("/storage/emulated/0/user_temp.apk");

          */
/*
             QFile file("/storage/emulated/0/user_temp.apk");
             if(!file.open(QIODevice::WriteOnly ))
             {
             //    QMessageBox::critical(this, "错误", "文件打开失败，信息未写入", "确定");
                     qWarning() << "replyFinished err" << endl;
                 return;
             }

             file.write(reply->readAll());
             file.close();
              qDebug() << "文件保存成功!";
             installApk("/storage/emulated/0/user_temp.apk");
*/
    }

    //   QFile file("tmp.apk");

    //  QFile file("123.apk");
   //  QFile file("/data/local/tmp/heishaversion.json");
   //  QFile file("/mnt/user/heishaversion.json");
/*
    QString tmpname = QDir::homePath() + QDir::separator() + QCoreApplication::applicationName() + "_XXXXXX." + "txt";
       QTemporaryFile tmpFile(tmpname);

        tmpFile.setAutoRemove(false);
        if(tmpFile.open()){
            qDebug() << __FUNCTION__ << tmpFile.fileTemplate();
            qDebug() << __FUNCTION__ << tmpFile.fileName();
        }

        QFile f(tmpFile.fileName());
        bool ok = QTemporaryFile::createNativeFile(f);//如果文件存在，返回 false
        qDebug() << __FUNCTION__ << ok;
*/
/*
     QTemporaryFile tmpFile();

     QString tmpname = tmpFile.fileName();
      qWarning() << tmpname << endl;
*/

   /*QByteArray str;

     while(1){
              str.clear();
             str = reply->read(4096);//读取接收到的数据
              qWarning() << str.length() << endl;
            if(str.length()<=0){
                   qWarning() << "str_end" << endl;
                break;
            }
            QTextStream out(&file);
            out << str;     //输出到文件
              qWarning() << str << endl;
    }
     file.close();   //关闭文件
     qDebug() << "文件保存成功!";*/

 /*
  QFile file(":/db/mapping/joystick/gamecontrollerdb.txt");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
      qWarning() << "Couldn't load GameController mapping database.";
      return;
  }

  QTextStream s(&file);

  while (!s.atEnd()) {
      SDL_GameControllerAddMapping(s.readLine().toStdString().c_str());
  }

  */
/*
     QString str = reply->readAll();//读取接收到的数据
   qWarning() << str << endl;
 //    parse_UpdateJSON(str);
      qWarning() << "replyFinished" << endl;
     //文件保存到本地
    QString fileName = QCoreApplication::applicationDirPath();

    qWarning() << fileName << endl;

  // QDir("/data/local/tmp/heishaversion123.json");
 //  QFile file("/data/local/tmp/heishaversion.json");
 //  QFile file(fileName+"/"+"heishaversion.json");
 //   QFile file(fileName+"/"+"heishaversion123.apk");
     QFile file("/data/local/tmp/123.apk");



    // if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) //append 内容追加在文件后面
    // if(!file.open(QIODevice::WriteOnly | QIODevice::Text ))
     if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
     {
     //    QMessageBox::critical(this, "错误", "文件打开失败，信息未写入", "确定");
             qWarning() << "replyFinished err" << endl;
         return;
     }
     QTextStream out(&file);
     out << str;     //输出到文件
     file.close();   //关闭文件
     qDebug() << "文件保存成功!";
    // QString fileName = QCoreApplication::applicationDirPath();
*/
   //  qWarning() << fileName << endl;
    // installApk("/data/local/tmp/123.apk");
   //   installApk("/storage/emulated/0/user_temp.apk");
     //  installApk("tmpname");

   //  installApk("tmp.apk");


/*
    QFile file("software_update.json");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) //append 内容追加在文件后面
    {
        QMessageBox::critical(this, "错误", "文件打开失败，信息未写入", "确定");
        return;
    }
    QTextStream out(&file);
    out << str;     //输出到文件
    file.close();   //关闭文件
    qDebug() << "文件保存成功!";
//    file.remove();
//    qDebug() << "文件已经删除";
*/
    reply->deleteLater();               //销毁请求对象
}


int update::parse_UpdateJSON(QString str)
{
    //    QMessageBox msgBox;
    QJsonParseError err_rpt;
    QJsonDocument  root_Doc = QJsonDocument::fromJson(str.toUtf8(),&err_rpt);//字符串格式化为JSON
  //    installApk("http://139.199.24.138:2020/heisha/HEISHA.apk");


    if(err_rpt.error != QJsonParseError::NoError)
    {
          qWarning() << "root格式错误";
     //    QMessageBox::critical(this, "检查失败", "服务器地址错误或JSON格式错误!");
       //    QMessageBox::critical(this, "check fail", "server err or JSON err!");

        //   QMessageBox::critical()
      //     QMessageBox.critical(this, "check fail", "server err or JSON err!");
      //    msgBox.critical(this, "check fail", "server err or JSON err!");
        return -1;
    }
        qWarning() << "parse_UpdateJSON" << endl;
    if(root_Doc.isObject())
    {
        QJsonObject  root_Obj = root_Doc.object();   //创建JSON对象，不是字符串
        QJsonObject PulseValue = root_Obj.value("PulseSensor").toObject();
        QString Verison = PulseValue.value("LatestVerison").toString();  //V1.0
        QString Url = PulseValue.value("Url").toString();        //https://wcc-blog.oss-cn-beijing.aliyuncs.com/QtUpdate/uFun_Pulse_v1.0.exe
        QString UpdateTime = PulseValue.value("UpdateTime").toString();
        QString ReleaseNote = PulseValue.value("ReleaseNote").toString();
         qWarning() << Verison << CurVerison << endl;
        //davis
        if(Verison > CurVerison)
        {
            manager_time=1;
            QString warningStr =  "检测到新版本!\n版本号：" + Verison + "\n" + "更新时间：" + UpdateTime + "\n" + "更新说明：" + ReleaseNote;
            QNetworkRequest questapk;
                qWarning() << warningStr << endl;
         //   questapk.setUrl(QUrl("http://139.199.24.138:2020/heisha/blacksandtest.apk")); //包含最新版本软件的下载地址
            questapk.setUrl(QUrl(Url)); //包含最新版本软件的下载地址
            questapk.setHeader(QNetworkRequest::UserAgentHeader,"RT-Thread ART");
            questapk.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/apk; charset=utf-8"));

            manager->get(questapk);    //发送get网络请求
         //davis    int ret = QMessageBox::warning(this, "检查更新",  warningStr, "去下载", "不更新");
          //  if(ret == 0)    //点击更新
        //    {
         //davis        QDesktopServices::openUrl(QUrl(Url));

        //   }
            //QDesktopServices::openUrl(QUrl(Url));
         //     installApk("http://139.199.24.138:2020/heisha/HEISHA.apk");
        }
        //davis  else
        //davis     QMessageBox::information(this, "检查更新", "当前已经是最新版本!");

    }
    return 0;
}


/*
void update::DownLoadUpdateFiles()
{
    QString strServer="http://139.199.24.138:2020/heisha/HEISHA.apk";//需要下载的文件存储位置
    QString strCurrentDir=QDir::currentPath();//当前程序运行路径

       //放置下载的文件的路径

       QString strPlaceDir=strCurrentDir+"/download/";
       QDir directory(strPlaceDir);//如果路径不存在，则创建
       if(!directory.exists())directory.mkpath(strPlaceDir);

    //   QString strFileDirServer=strServer+m_listFileDir.at(i)+"/"+m_listFileName.at(i);//文件在服务器中的存储位置

 //      CHttpDownloadFile *http=new CHttpDownloadFile(strServer,"",strPlaceDir,this);//调用下载文件的类

    //   http->DownLoadFile();
   //    while(!http->m_bIsFinished)
   //    {


      //     if(http->m_nTotal==-1)
      //     {
      //          m_progDownload->setValue(1);
      //     }
       //    else
       //    {
        //        m_progDownload->setValue(100*http->m_nReceived/http->m_nTotal);
        //   }
         //  QCoreApplication::processEvents();

    //   }
       //将下载好的文件复制到主目录中，先删除原先的文件

       QString strLocalFileName=strCurrentDir;

       if(QFile::exists(strLocalFileName))QFile::remove(strLocalFileName);

       QDir directory1(strCurrentDir);//如果路径不存在，则创建

       if(!directory1.exists())directory1.mkpath(strCurrentDir);

       QFile::copy(strPlaceDir,strLocalFileName);

}
*/

