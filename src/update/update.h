
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

class update : public  QObject
{
    Q_OBJECT
 
public:
    explicit update(QObject *parent = 0);
    ~update();
     void installApk(const QString &filePath);  //更新自己



     int parse_UpdateJSON(QString str);      //解析数据函数的声明
     void DownLoadUpdateFiles(void);

      void on_pushButton_clicked();

public slots:
     void replyFinished(QNetworkReply *reply);   //网络数据接收完成槽函数的声明

signals:
    //finished(QNetworkReply *reply);
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

    
 

};
 
#endif // MAINWINDOW_H
