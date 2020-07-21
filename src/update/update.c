#include "update.h"
 
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QtAndroid>
#include <QDebug>
 
update::update() 
{
    
}
 
update::~update()
{
   
}
 
void update::on_pushButton_clicked()
{
    QString filename = "/storage/emulated/0/shichun/android-build-debug.apk";
    installApk(filename);
}
 
void update::installApk(const QString &filePath)
{
    QAndroidJniObject jFilePath = QAndroidJniObject::fromString(filePath);
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject::callStaticMethod<void>(
                "my/demo/myJava/mmActivity",
                "installApk",
                "(Ljava/lang/String;Lorg/qtproject/qt5/android/bindings/QtActivity;)V",
                jFilePath.object<jstring>(),
                activity.object<jobject>()
                );
 
 
}

