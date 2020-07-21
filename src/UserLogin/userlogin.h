
#ifndef UPDATE_H
#define UPDATE_H
 
#include <QtGlobal>
#include <QApplication>
//网络相关头文件
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
//JSON相关头文件
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QFile>
#include <QIODevice>
#include <QFileInfo>
#include <QProgressBar>

#include <QTemporaryFile>

class userlogin : public  QObject
{
    Q_OBJECT
 
public:
    explicit userlogin(QObject *parent = 0);
    ~userlogin();
     void installApk(const QString &filePath);  //更新自己



     int parse_UpdateJSON(QString str);      //解析数据函数的声明
     void DownLoadUpdateFiles(void);

      void on_LoginButton_clicked();

      int get_port();
      QString get_topic();
      QString get_ip();

      QString _usenamelonin ;
      QString _passwordlonin;

public slots:
     void replyFinished(QNetworkReply *reply);   //网络数据接收完成槽函数的声明

signals:
    //finished(QNetworkReply *reply);
     void netipchange(void);
private:
       QFile *file;
       QTemporaryFile *tmpFile;
       QFileInfo fileInfo;

    //   QNetworkAccessManager *accessManager;
       QNetworkAccessManager *manager;     //定义网络请求对象
       QString CurVerison = "V1.2";    //定义当前软件的版本号
       QNetworkRequest request;
       QNetworkReply *reply;

       bool httpRequestAbort; //请求停止的标志位,false表示允许请求
       quint8 manager_time;

       QString _ip ;
       int _port ;
       QString _topic;



    
 

};
 
#endif // MAINWINDOW_H
