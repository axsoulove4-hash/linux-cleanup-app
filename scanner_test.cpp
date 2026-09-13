#include "data.h"
int main(int argc,char **argv){
    QCoreApplication app(argc,argv);
    QTemporaryDir tmp;
    if(!tmp.isValid())return 1;
    const QString base=tmp.path();
    QStringList roots{base+"/cache","",base+"/state",base+"/cache/thumbnails",base+"/trash",base+"/temp"};
    auto file=[&](QString path,int size,int age){
        QDir().mkpath(QFileInfo(path).absolutePath());
        QFile f(path);if(!f.open(QIODevice::WriteOnly))return false;
        if(f.write(QByteArray(size,'x'))!=size)return false;
        if(!f.flush())return false;
        return f.setFileTime(QDateTime::currentDateTime().addDays(-age),QFileDevice::FileModificationTime);
    };
    if(!file(roots[0]+"/old",20,30)||!file(roots[0]+"/recent",50,0)||!file(roots[3]+"/thumb",30,30)||
       !file(roots[2]+"/old.log",40,30)||!file(roots[2]+"/unrelated",70,30)||!file(roots[4]+"/trashed",60,0)||
       !file(roots[5]+"/temp",80,30)||!file(roots[0]+"/excluded/item",90,30))return 2;
#ifndef Q_OS_WIN
    if(!QFile::link(roots[4],roots[0]+"/symlink-dir"))return 3;
#endif
    auto data=scanLocal(roots,14,{roots[0]+"/excluded"},base,60,false);
    if(data.sizes!=QVector<qint64>{20,-1,40,30,60,80})return 4;
    if(data.files.size()!=5)return 5;
    for(const auto &f:data.files)if(f.path.contains("symlink")||f.path.contains("excluded"))return 6;
    if(data.large.size()!=3)return 7;
    if(data.large[0].size!=80)return 8;
    auto find=[&](QString path){for(auto f:data.files)if(f.path==path)return f;return FileRow{};};
    auto cached=find(roots[0]+"/old");
    if(removeScannedFile(cached,roots[2])!=Removal::Changed||!QFile::exists(cached.path))return 9;
    if(removeScannedFile(cached,roots[0])!=Removal::Deleted||QFile::exists(cached.path))return 10;
    if(!QFile::exists(roots[0]+"/recent"))return 11;
    auto log=find(roots[2]+"/old.log");
    if(!file(log.path,41,0))return 12;
    if(removeScannedFile(log,roots[2])!=Removal::Changed||!QFile::exists(log.path))return 13;
    auto trash=find(roots[4]+"/trashed");
    if(removeScannedFile(trash,roots[4])!=Removal::Deleted||QFile::exists(trash.path))return 14;
    auto temp=find(roots[5]+"/temp");
    if(removeScannedFile(temp,roots[5])!=Removal::Deleted||QFile::exists(temp.path))return 15;
    return 0;
}
