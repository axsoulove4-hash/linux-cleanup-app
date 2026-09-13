#pragma once
#include <QtWidgets>
#include <QtConcurrent>
#ifdef Q_OS_WIN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif
#include <algorithm>

static bool redirectedPath(const QFileInfo &info) {
#ifdef Q_OS_WIN
    DWORD attr=GetFileAttributesW(reinterpret_cast<LPCWSTR>(info.absoluteFilePath().utf16()));
    return attr==INVALID_FILE_ATTRIBUTES||(attr&FILE_ATTRIBUTE_REPARSE_POINT);
#else
    return info.isSymLink();
#endif
}
static bool userFile(const QString &path) {
#ifdef Q_OS_WIN
    QFileInfo f(path);
    QString home=QFileInfo(QDir::homePath()).canonicalFilePath();
    if(home.isEmpty()||!f.canonicalFilePath().startsWith(home+"/",Qt::CaseInsensitive))return false;
    // Reject junctions, symlinks and cloud placeholders, including ancestors.
    for(QString current=f.absoluteFilePath();!current.isEmpty();){
        if(redirectedPath(QFileInfo(current)))return false;
        QString parent=QFileInfo(current).absolutePath();if(parent==current)break;current=parent;
    }
    return f.isFile();
#else
    struct stat st{};return lstat(QFile::encodeName(path).constData(),&st)==0&&S_ISREG(st.st_mode)&&st.st_uid==getuid();
#endif
}

struct FileRow { QString path; qint64 size; QDateTime modified; int category; };
enum class Removal { Deleted, Changed, Failed };
static Removal removeScannedFile(const FileRow &file,const QString &allowedRoot) {
    if(allowedRoot.isEmpty()||file.category<0||file.category==1||file.category>5)return Removal::Changed;
    QFileInfo root(allowedRoot),info(file.path);
    if(redirectedPath(root)||redirectedPath(info)||!root.isDir()||!info.isFile()||!userFile(file.path))return Removal::Changed;
    const QString canonical=root.canonicalFilePath();
    if(canonical.isEmpty()||!info.canonicalFilePath().startsWith(canonical+"/")||
       info.size()!=file.size||info.lastModified()!=file.modified)return Removal::Changed;
    // Recheck every parent to reject replaced directories and junctions.
    for(QString parent=info.absolutePath();;){
        if(redirectedPath(QFileInfo(parent)))return Removal::Changed;
        if(parent==root.absoluteFilePath())break;
        QString next=QFileInfo(parent).absolutePath();if(parent==next)return Removal::Changed;parent=next;
    }
    return QFile::remove(file.path)?Removal::Deleted:Removal::Failed;
}
struct ScanData {
    QVector<FileRow> files, large;
    QVector<qint64> sizes=QVector<qint64>(6,0);
    QString packages, packageStatus;
    int skipped=0;
};
static QString bytes(qint64 n) {
    if(n<0)return "Indisponible";
    if(n<1024)return QString::number(n)+" o";
    QStringList u{"Ko","Mo","Go","To"};double v=n;
    for(auto unit:u){v/=1024;if(v<1024||unit=="To")return QLocale().toString(v,'f',1)+" "+unit;}
    return {};
}
static QString commandOutput(const QString &cmd,const QStringList &args,bool &ok) {
    QProcess p;p.setProcessEnvironment(QProcessEnvironment::systemEnvironment());p.start(cmd,args);
    if(!p.waitForStarted(2000)){ok=false;return p.errorString();}
    if(!p.waitForFinished(15000)){p.kill();p.waitForFinished();ok=false;return "Délai dépassé";}
    ok=p.exitStatus()==QProcess::NormalExit&&p.exitCode()==0;
    return QString::fromUtf8(ok?p.readAllStandardOutput():p.readAllStandardError());
}
static ScanData scanLocal(QStringList roots,int days,QStringList exclusions,QString largeRoot,qint64 threshold,bool queryPackages=true) {
    ScanData out;const auto now=QDateTime::currentDateTime();
    auto excluded=[exclusions](const QString &path){for(auto x:exclusions){
#ifdef Q_OS_WIN
        if(path.compare(x,Qt::CaseInsensitive)==0||path.startsWith(x+"/",Qt::CaseInsensitive))return true;
#else
        if(path==x||path.startsWith(x+"/"))return true;
#endif
    }return false;};
    auto walk=[&](const QString &root,auto visitor){
        if(root.isEmpty())return;
        QStringList pending{root};
        while(!pending.isEmpty()){
            QString current=pending.takeLast();
            QFileInfo di(current);if(redirectedPath(di)||excluded(current))continue;
            if(!di.exists())continue;
            if(!di.isReadable()){out.skipped++;continue;}
            const auto children=QDir(current).entryInfoList(QDir::AllEntries|QDir::Hidden|QDir::System|QDir::NoDotAndDotDot,QDir::NoSort);
            for(const auto &f:children){
                if(redirectedPath(f)||excluded(f.absoluteFilePath()))continue;
                if(f.isDir())pending<<f.absoluteFilePath();
                else if(f.isFile()){if(f.isReadable())visitor(f);else out.skipped++;}
            }
        }
    };
    for(int i=0;i<6;i++){
        if(i==1)continue;
        if(roots[i].isEmpty()){out.sizes[i]=-1;continue;}
        walk(roots[i],[&](const QFileInfo &f){
            QString path=f.absoluteFilePath();
            if(i==0&&(path==roots[3]||path.startsWith(roots[3]+"/")))return;
            if((i==0||i==2||i==3||i==5)&&f.lastModified().daysTo(now)<days)return;
            if(i==2&&!path.endsWith(".log"))return;
            if(!userFile(path))return;
            out.files.push_back({path,f.size(),f.lastModified(),i});out.sizes[i]+=f.size();
        });
    }
    bool ok=false;
#ifdef Q_OS_WIN
    out.packageStatus="Non pris en charge";
    out.packages="Windows : gestion des applications dans Paramètres Windows. Aucune estimation de paquets inutilisés.";
#else
    if(!queryPackages){out.packageStatus="Non interrogé";}
    else if(!QStandardPaths::findExecutable("dnf").isEmpty()){
        out.packages=commandOutput("dnf",{"--cacheonly","repoquery","--installed","--unneeded","--queryformat","%{name}\\n"},ok);
        out.packageStatus=ok?(out.packages.trimmed().isEmpty()?"Aucun orphelin DNF":"Suggestions DNF"):"DNF indisponible";
    }else if(!QStandardPaths::findExecutable("apt-get").isEmpty()){
        QString raw=commandOutput("apt-get",{"--simulate","autoremove"},ok);
        if(ok){for(auto l:raw.split('\n'))if(l.startsWith("Remv "))out.packages+=l+"\n";}
        else out.packages=raw;
        out.packageStatus=ok?(out.packages.isEmpty()?"Aucun orphelin APT":"Suggestions APT"):"APT indisponible";
    }else if(!QStandardPaths::findExecutable("pacman").isEmpty()){
        out.packages=commandOutput("pacman",{"-Qdtq"},ok);
        out.packageStatus=ok?"Suggestions pacman":"Aucun résultat / indisponible";
    }else{
        out.packageStatus="Non pris en charge";
        out.packages="Détection des paquets orphelins disponible avec DNF, APT ou pacman. Aucun résultat inventé.";
    }
#endif
    out.sizes[1]=-1;
    walk(largeRoot,[&](const QFileInfo &f){if(f.size()>=threshold)out.large.push_back({f.absoluteFilePath(),f.size(),f.lastModified(),-1});});
    std::sort(out.large.begin(),out.large.end(),[](const auto&a,const auto&b){return a.size>b.size;});
    if(out.large.size()>500)out.large.resize(500);
    return out;
}
