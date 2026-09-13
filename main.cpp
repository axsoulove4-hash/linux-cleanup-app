#include <QtWidgets>
#include <QtConcurrent>
#include <cmath>
#include "data.h"
#include "i18n.h"
#include "drivecard.h"

// All geometry uses the reference's 1536 × 1024 design coordinates.
// Icons are resolution-independent paths, never font-dependent emoji.
static QColor ink("#b9c3d3");
static void icon(QPainter &p, QString type, QRectF box, QColor color=ink) {
    p.save(); p.translate(box.topLeft()); p.scale(box.width()/32,box.height()/32);
    p.setRenderHint(QPainter::Antialiasing); p.setPen(QPen(color,2.1,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin)); p.setBrush(Qt::NoBrush);
    QPainterPath a;
    if(type=="leaf") {
        QLinearGradient g(0,32,28,0); g.setColorAt(0,QColor("#84c94d"));g.setColorAt(1,QColor("#42b77b"));
        p.setBrush(g);p.setPen(Qt::NoPen);a.moveTo(5,27);a.cubicTo(-2,10,17,7,29,1);a.cubicTo(33,21,22,32,5,27);p.drawPath(a);
        p.setPen(QPen(QColor("#285f3d"),1.7,Qt::SolidLine,Qt::RoundCap));a=QPainterPath();a.moveTo(2,33);a.cubicTo(6,22,13,15,23,9);p.drawPath(a);
    } else if(type=="home") {
        a.moveTo(3,14);a.lineTo(16,3);a.lineTo(29,14);a.moveTo(6,12);a.lineTo(6,28);a.lineTo(12,28);a.lineTo(12,19);a.lineTo(20,19);a.lineTo(20,28);a.lineTo(26,28);a.lineTo(26,12);p.drawPath(a);
    } else if(type=="broom") {
        p.drawLine(25,3,13,19);p.setBrush(color);a.moveTo(10,15);a.lineTo(20,22);a.lineTo(12,30);a.lineTo(2,23);a.closeSubpath();p.drawPath(a);p.setPen(QPen(QColor("#20252b"),1.5));p.drawLine(7,25,11,20);p.drawLine(11,28,15,23);
    } else if(type=="apps") {
        p.setBrush(color);for(int x:{4,18})for(int y:{4,18})p.drawRoundedRect(QRectF(x,y,9,9),2,2);
    } else if(type=="disk") {
        a.moveTo(7,4);a.lineTo(25,4);a.lineTo(29,25);a.lineTo(27,28);a.lineTo(5,28);a.lineTo(3,25);a.closeSubpath();p.drawPath(a);p.drawLine(5,21,27,21);p.drawPoint(23,25);
    } else if(type=="folder") {
        a.moveTo(3,27);a.lineTo(3,7);a.lineTo(12,7);a.lineTo(15,11);a.lineTo(27,11);a.lineTo(28,15);p.drawPath(a);p.setBrush(color);a=QPainterPath();a.moveTo(4,27);a.lineTo(8,15);a.lineTo(30,15);a.lineTo(26,27);a.closeSubpath();p.drawPath(a);
    } else if(type=="settings") {
        for(int i=0;i<8;i++){p.save();p.translate(16,16);p.rotate(i*45);p.drawRect(QRectF(-2,-14,4,6));p.restore();}p.setBrush(QColor("#20252b"));p.drawEllipse(QPointF(16,16),10,10);p.drawEllipse(QPointF(16,16),4,4);
    } else if(type=="cache") {
        p.setBrush(color);p.drawRoundedRect(QRectF(4,8,24,19),4,4);p.drawEllipse(QRectF(4,3,24,11));p.setPen(QPen(QColor("#20252b"),1.4));p.drawArc(QRectF(3,9,26,11),180*16,180*16);p.drawArc(QRectF(3,17,26,9),180*16,180*16);
    } else if(type=="package") {
        p.setBrush(color);p.drawRoundedRect(QRectF(4,12,24,17),2,2);p.drawRect(QRectF(2,8,28,6));p.setPen(QPen(QColor("#20252b"),3));p.drawLine(12,15,20,15);
    } else if(type=="log") {
        p.setBrush(color);a.moveTo(7,2);a.lineTo(21,2);a.lineTo(27,8);a.lineTo(27,29);a.lineTo(7,29);a.closeSubpath();p.drawPath(a);p.setPen(QPen(QColor("#20252b"),1.6));for(int y:{14,19,24})p.drawLine(11,y,22,y);p.drawLine(20,3,20,9);p.drawLine(20,9,26,9);
    } else if(type=="image") {
        p.setBrush(color);p.drawRoundedRect(QRectF(3,4,26,24),3,3);p.setPen(Qt::NoPen);p.setBrush(QColor("#20252b"));p.drawEllipse(QPointF(11,11),2.5,2.5);a.moveTo(5,25);a.lineTo(13,16);a.lineTo(18,21);a.lineTo(23,14);a.lineTo(28,25);a.closeSubpath();p.drawPath(a);
    } else if(type=="trash") {
        p.setBrush(color);p.drawRoundedRect(QRectF(7,8,19,22),3,3);p.drawLine(3,6,29,6);p.drawLine(12,2,20,2);p.setPen(QPen(QColor("#20252b"),1.6));for(int x:{12,17,22})p.drawLine(x,12,x,25);
    } else if(type=="download") {
        p.setBrush(color);a.moveTo(12,2);a.lineTo(20,2);a.lineTo(20,14);a.lineTo(26,14);a.lineTo(16,24);a.lineTo(6,14);a.lineTo(12,14);a.closeSubpath();p.drawPath(a);p.setBrush(Qt::NoBrush);p.drawLine(4,24,4,29);p.drawLine(4,29,28,29);p.drawLine(28,29,28,24);
    } else if(type=="refresh") {
        p.drawArc(QRectF(5,5,22,22),30*16,145*16);p.drawArc(QRectF(5,5,22,22),210*16,145*16);p.drawLine(27,4,27,12);p.drawLine(27,12,20,10);p.drawLine(5,28,5,20);p.drawLine(5,20,12,22);
    } else if(type=="list") {
        for(int y:{7,16,25}) {p.drawPoint(4,y);p.drawLine(11,y,28,y);}
    } else if(type=="moon") {
        p.drawEllipse(QRectF(5,4,23,23));a.moveTo(16,5);a.cubicTo(10,18,22,21,27,16);p.drawPath(a);
    } else if(type=="clock" || type=="info") {
        p.drawEllipse(QRectF(4,4,24,24));if(type=="clock"){p.drawLine(16,8,16,17);p.drawLine(16,17,22,20);}else{p.drawPoint(16,10);p.drawLine(16,15,16,23);}
    }
    p.restore();
}

struct Entry {QString title,amount,description,type,risk,path; bool checked;};
class Window: public QWidget {
    QVector<Entry> entries{
        {"Caches utilisateur","…","Caches anciens du compte.","cache","Recréable",QDir::homePath()+"/.cache",true},
        {"Paquets inutilisés","…","Suggestions du gestionnaire.","package","À vérifier","Gestionnaire de paquets — aperçu uniquement",false},
        {"Fichiers de logs","…","Journaux utilisateur anciens.","log","À vérifier",QDir::homePath()+"/.local/state",false},
        {"Miniatures","…","Vignettes d’images et de vidéos.","image","Recréable",QDir::homePath()+"/.cache/thumbnails",true},
        {"Corbeille","…","Fichiers présents dans la corbeille\nutilisateur.","trash","À vérifier",QDir::homePath()+"/.local/share/Trash/files",false},
        {"Fichiers temporaires","…","Fichiers temporaires divers.","download","À vérifier","/tmp",false}
    };
    QStringList pages{"Tableau de bord","Nettoyage","Applications","Stockage","Éléments volumineux","Paramètres"};
    QStringList symbols{"home","broom","apps","disk","folder","settings"};
    int active=0; bool busy=false; QString recover="…", diskTitle="Volume utilisateur",used="…",free="…";
    double diskRatio=0; QString timestamp="Analyse au démarrage…";
    ScanData data;
    QSettings prefs{"ClariSweep","LinuxCleanup"};
    QWidget *page=nullptr;
    QColor accent;
    bool light=false;
    void text(QPainter &p,QRectF r,QString s,int size=16,QColor c=ink,bool bold=false,int flags=Qt::AlignLeft|Qt::AlignVCenter) {
        if(light&&(c==ink||c==QColor(Qt::white)))c=QColor("#263343");
        p.setPen(c);p.setFont(QFont("Noto Sans",size,-1));QFont f=p.font();f.setPixelSize(size);f.setWeight(bold?QFont::DemiBold:QFont::Normal);p.setFont(f);p.drawText(r,flags,ui(s));
    }
    void panel(QPainter &p,QRectF r,QColor bg=QColor("#20262c"),QColor border=QColor("#343b44"),int radius=13) {
        if(light&&(bg==QColor("#20262c")||bg==QColor("#22282e"))){bg=QColor("#ffffff");border=QColor("#cbd2dd");}
        p.setPen(QPen(border,1));p.setBrush(bg);p.drawRoundedRect(r,radius,radius);
    }
    void button(QPainter &p,QRectF r,QString title,QString symbol="",bool primary=false) {
        panel(p,r,primary?accent:QColor("#22282e"),primary?accent.lighter():QColor("#39424d"),12);
        if(!symbol.isEmpty())icon(p,symbol,QRectF(r.x()+18,r.center().y()-13,26,26),primary?Qt::white:ink);
        text(p,r.adjusted(symbol.isEmpty()?0:48,0,0,0),title,primary?23:14,Qt::white,primary,Qt::AlignCenter);
    }
    QRectF card(int i) const {return QRectF(329+(i%3)*399,535+(i/3)*181,381,163);}
    void details(int i) {
#ifdef Q_OS_WIN
        if(i==4){QProcess::startDetached("explorer.exe",{"shell:RecycleBinFolder"});return;}
        if(i==1){QDesktopServices::openUrl(QUrl("ms-settings:appsfeatures"));return;}
        if(i==2||i==3){QMessageBox::information(this,ui("Non pris en charge"),ui("Géré par Windows. Aucun nettoyage automatique."));return;}
#endif
        if(i==1){QMessageBox::information(this,data.packageStatus,data.packages.isEmpty()?data.packageStatus:data.packages);return;}
        showFiles(i,true);
    }
    void analyze() {
        if(busy)return;busy=true;timestamp="Analyse locale en cours…";update();
        auto *watcher=new QFutureWatcher<ScanData>(this);
        QStringList paths;for(const auto &e:entries)paths<<e.path;
        connect(watcher,&QFutureWatcherBase::finished,this,[this,watcher]{
            data=watcher->result();qint64 total=0;
            for(int i=0;i<entries.size();i++){entries[i].amount=i==1?"Voir détails":format(data.sizes[i]);if(data.sizes[i]>0)total+=data.sizes[i];}
            recover=format(total);busy=false;
            QStorageInfo info(QDir::homePath());diskTitle="Volume utilisateur";diskRatio=info.bytesTotal()>0?double(info.bytesTotal()-info.bytesAvailable())/info.bytesTotal():0;
            used=format(info.bytesTotal()-info.bytesAvailable())+" utilisés";free=format(info.bytesAvailable())+" libres";
            timestamp="Analyse : "+QTime::currentTime().toString("HH:mm")+" · "+QString::number(data.skipped)+" inaccessible(s)";
            watcher->deleteLater();rebuildPage();update();
        });
        int days=prefs.value("days",14).toInt();
        QStringList excludes=prefs.value("exclusions").toString().split('\n',Qt::SkipEmptyParts);
        for(auto &s:excludes)s=QDir::cleanPath(s.trimmed());
        QString root=prefs.value("largeRoot",QDir::homePath()).toString();
        qint64 threshold=prefs.value("threshold",100).toLongLong()*1048576;
        watcher->setFuture(QtConcurrent::run([paths,days,excludes,root,threshold]{return scanLocal(paths,days,excludes,root,threshold);}));
    }
    static QString format(qint64 n) {return ui(bytes(n));}
    qint64 selectedBytes() const {qint64 total=0;for(int i=0;i<6;i++)if(entries[i].checked&&data.sizes[i]>0)total+=data.sizes[i];return total;}
    QTableWidget *table(QStringList headers,QWidget *parent=nullptr) {
        auto *t=new QTableWidget(0,headers.size(),parent);t->setHorizontalHeaderLabels(headers);
        t->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        t->horizontalHeader()->setSectionResizeMode(headers.size()-1,QHeaderView::Stretch);
        t->verticalHeader()->hide();t->setSelectionBehavior(QAbstractItemView::SelectRows);
        t->setEditTriggers(QAbstractItemView::NoEditTriggers);t->setAlternatingRowColors(true);return t;
    }
    void row(QTableWidget *t,QStringList cells) {
        int r=t->rowCount();t->insertRow(r);for(int i=0;i<cells.size();i++)t->setItem(r,i,new QTableWidgetItem(i==cells.size()-1?cells[i]:ui(cells[i])));
    }
    void showFiles(int category,bool allowClean) {
        if(busy){QMessageBox::information(this,"Analyse","Analyse en cours. Attends résultats.");return;}
        QDialog dialog(this);dialog.setWindowTitle("Fichiers mesurés — aperçu");dialog.resize(1050,650);
        auto *layout=new QVBoxLayout(&dialog);
        auto *note=new QLabel(ui("Cochez les fichiers à supprimer définitivement. Fermez les applications concernées. Les fichiers modifiés depuis analyse seront ignorés."));
        note->setWordWrap(true);layout->addWidget(note);
        auto *t=table({"Taille","Catégorie","Chemin"});layout->addWidget(t);
        QVector<FileRow> chosen;
        for(const auto &f:data.files)if(category>=0?f.category==category:entries[f.category].checked){
            chosen<<f;row(t,{format(f.size),entries[f.category].title,f.path});
            auto *item=t->item(t->rowCount()-1,0);item->setFlags(item->flags()|Qt::ItemIsUserCheckable);
            item->setCheckState(category<0||entries[f.category].checked?Qt::Checked:Qt::Unchecked);
        }
        auto *close=new QPushButton("Fermer");connect(close,&QPushButton::clicked,&dialog,&QDialog::reject);layout->addWidget(close);
        if(allowClean){
            auto *clean=new QPushButton;
            auto updateSelection=[=]{int count=0;qint64 size=0;for(int i=0;i<chosen.size();i++)if(t->item(i,0)->checkState()==Qt::Checked){count++;size+=chosen[i].size;}
                clean->setText(ui("Supprimer")+" "+QString::number(count)+" "+ui("fichiers")+" · "+format(size));clean->setEnabled(count>0);};
            connect(t,&QTableWidget::itemChanged,&dialog,[updateSelection]{updateSelection();});updateSelection();
            layout->addWidget(clean);
            connect(clean,&QPushButton::clicked,&dialog,[&,chosen]{
                QVector<FileRow> selected;for(int i=0;i<chosen.size();i++)if(t->item(i,0)->checkState()==Qt::Checked)selected<<chosen[i];
                if(selected.isEmpty())return;
                if(QMessageBox::warning(&dialog,ui("Suppression définitive"),QString::number(selected.size())+" "+ui("fichiers seront supprimés sans passer par corbeille. Continuer ?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)!=QMessageBox::Yes)return;
                int removed=0,skipped=0,failed=0;qint64 removedSize=0;QStringList errors;
                for(const auto &f:selected){
                    auto result=removeScannedFile(f,entries[f.category].path);
                    if(result==Removal::Deleted){removed++;removedSize+=f.size;}
                    else {if(result==Removal::Changed)skipped++;else failed++;errors<<f.path;}
                }
                QMessageBox result(&dialog);result.setWindowTitle(ui("Résultat"));
                result.setText(ui("Supprimés")+" : "+QString::number(removed)+" ("+format(removedSize)+")\n"+ui("Ignorés")+" : "+QString::number(skipped)+"\n"+ui("Échecs")+" : "+QString::number(failed));
                if(!errors.isEmpty())result.setDetailedText(errors.join('\n'));result.exec();
                dialog.accept();analyze();
            });
        }
        localizeWidgets(&dialog);dialog.exec();
    }
    void applyTheme(){
        english=prefs.value("language","fr").toString()=="en";
        QLocale::setDefault(QLocale(english?QLocale::English:QLocale::French));
        light=prefs.value("light",false).toBool();accent=QColor(prefs.value("accent","#5967f0").toString());
        if(!accent.isValid())accent=QColor("#5967f0");
        QString bg=light?"#f3f4f6":"#20262c",fg=light?"#202833":"#e5ebf5",alt=light?"#e6e9ef":"#272f38";
        setStyleSheet(QString("QDialog,QWidget#content{background:%1;color:%2;} QLabel,QCheckBox{color:%2;} QTableWidget,QLineEdit,QPlainTextEdit,QSpinBox,QComboBox{background:%1;color:%2;border:1px solid #647080;padding:6px;selection-background-color:%4;alternate-background-color:%3;} QHeaderView::section{background:%3;color:%2;padding:9px;border:0;} QPushButton{background:%4;color:white;border:0;border-radius:7px;padding:10px 16px;} QScrollArea{border:0;}").arg(bg,fg,alt,accent.name()));
        setStyleSheet(styleSheet()+QString("QListWidget{background:%1;color:%2;border:0;outline:0;font-size:14px;} QListWidget::item{background:%3;border:1px solid %3;border-radius:12px;padding:10px;} QListWidget::item:selected{border:2px solid %4;background:%3;} QFrame#driveCard{background:%3;border-radius:16px;} QScrollArea>QWidget>QWidget{background:%1;}").arg(bg,fg,alt,accent.name()));
        update();
    }
    QIcon appIcon(QString key,QString desktopPath){
        QIcon found=QDir::isAbsolutePath(key)?QIcon(key):QIcon::fromTheme(key);
        if(!found.isNull())return found;
        QStringList dirs{QFileInfo(desktopPath).absolutePath()+"/../icons",QDir::homePath()+"/.local/share/icons","/var/lib/flatpak/exports/share/icons",QDir::homePath()+"/.local/share/flatpak/exports/share/icons"};
        if(!key.isEmpty())for(auto dir:dirs){QDirIterator it(dir,{key+".png",key+".svg",key+".xpm"},QDir::Files,QDirIterator::Subdirectories);if(it.hasNext())return QIcon(it.next());}
        QPixmap pix(64,64);pix.fill(Qt::transparent);QPainter p(&pix);icon(p,"apps",{12,12,40,40},accent);return QIcon(pix);
    }
    void rebuildPage(){
        if(page){delete page;page=nullptr;}if(active==0)return;
        page=new QWidget(this);page->setObjectName("content");
        auto *layout=new QVBoxLayout(page);layout->setContentsMargins(20,16,20,20);layout->setSpacing(12);
        auto *refresh=new QPushButton(busy?"Analyse en cours…":"Actualiser les données");refresh->setEnabled(!busy);
        connect(refresh,&QPushButton::clicked,this,[this]{analyze();rebuildPage();});layout->addWidget(refresh);
        if(active==1){
            layout->addWidget(new QLabel("Mesures utilisateur · cache / miniatures anciens · paquets : suggestions seulement"));
            auto *t=table({"Catégorie","Mesuré","Action disponible","Emplacement"});layout->addWidget(t);
            for(int i=0;i<6;i++)row(t,{entries[i].title,entries[i].amount,(i!=1&&!entries[i].path.isEmpty())?"Supprimer après confirmation":"Consultation",entries[i].path});
            connect(t,&QTableWidget::cellDoubleClicked,this,[this](int r,int){details(r);});
            auto *view=new QPushButton("Aperçu complet / nettoyage des catégories cochées au tableau de bord");
            connect(view,&QPushButton::clicked,this,[this]{showFiles(-1,true);});layout->addWidget(view);
            layout->addWidget(new QLabel(ui("Double-clic catégorie : cochez les fichiers puis confirmez leur suppression.")));
        }else if(active==2){
            auto *summary=new QLabel;layout->addWidget(summary);
            auto *search=new QLineEdit;search->setPlaceholderText("Rechercher une application…");layout->addWidget(search);
            auto *list=new QListWidget;list->setViewMode(QListView::IconMode);list->setResizeMode(QListView::Adjust);list->setMovement(QListView::Static);
            list->setIconSize({48,48});list->setGridSize({225,125});list->setWordWrap(true);list->setSpacing(9);layout->addWidget(list);
            QStringList roots=QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);
#ifdef Q_OS_WIN
            roots={QDir::fromNativeSeparators(qEnvironmentVariable("APPDATA"))+"/Microsoft/Windows/Start Menu/Programs",
                   QDir::fromNativeSeparators(qEnvironmentVariable("ProgramData"))+"/Microsoft/Windows/Start Menu/Programs"};
            QFileIconProvider provider;QSet<QString> seen;
            for(const auto &root:roots){QDirIterator it(root,{"*.lnk"},QDir::Files,QDirIterator::Subdirectories);
                while(it.hasNext()){QFileInfo f(it.next());QString name=f.completeBaseName();if(seen.contains(name.toLower()))continue;seen.insert(name.toLower());
                    QString source=ui(f.absoluteFilePath().startsWith(QDir::homePath(),Qt::CaseInsensitive)?"Installée pour vous":"Installée sur cet ordinateur");
                    auto *item=new QListWidgetItem(provider.icon(f),name+"\n"+source,list);item->setSizeHint({215,116});
                    item->setData(Qt::UserRole,f.absoluteFilePath());item->setData(Qt::UserRole+1,name);item->setData(Qt::UserRole+2,source);
                }}
#else
            roots<<"/var/lib/flatpak/exports/share/applications"<<QDir::homePath()+"/.local/share/flatpak/exports/share/applications";
            QSet<QString> seen;
            for(auto root:roots){QDirIterator it(root,{"*.desktop"},QDir::Files,QDirIterator::Subdirectories);
                while(it.hasNext()){QString path=it.next(),id=QDir(root).relativeFilePath(path);if(seen.contains(id))continue;seen.insert(id);
                    QSettings f(path,QSettings::IniFormat);f.beginGroup("Desktop Entry");
                    if(f.value("Hidden",false).toBool()||f.value("NoDisplay",false).toBool())continue;
                    auto name=f.value(english?"Name[en]":"Name[fr]",f.value("Name")).toString();if(name.isEmpty())continue;
                    QString source=path.contains("flatpak")?"Flatpak":ui(path.startsWith(QDir::homePath())?"Installée pour vous":"Installée sur cet ordinateur");
                    auto *item=new QListWidgetItem(appIcon(f.value("Icon").toString(),path),name+"\n"+source,list);
                    item->setSizeHint({215,116});
                    item->setData(Qt::UserRole,path);item->setData(Qt::UserRole+1,name);item->setData(Qt::UserRole+2,source);
                    item->setToolTip(name+"\n"+f.value(english?"Comment":"Comment[fr]",f.value("Comment")).toString());
                }}
#endif
            summary->setText(QString::number(list->count())+" "+ui("applications avec un raccourci. Les composants techniques ne sont pas affichés."));
            list->sortItems(Qt::AscendingOrder);
            connect(search,&QLineEdit::textChanged,list,[list](QString s){for(int r=0;r<list->count();r++)list->item(r)->setHidden(!list->item(r)->text().contains(s,Qt::CaseInsensitive));});
            connect(list,&QListWidget::itemDoubleClicked,this,[this](QListWidgetItem *i){QMessageBox::information(this,ui("Informations sur l’application"),ui("Nom")+": "+i->data(Qt::UserRole+1).toString()+"\n"+ui("Installation")+": "+i->data(Qt::UserRole+2).toString());});
        }else if(active==3){
            layout->addWidget(new QLabel("Espace occupé et disponible. Les montages techniques sont masqués."));
            auto *scroll=new QScrollArea;scroll->setWidgetResizable(true);auto *body=new QWidget;auto *cards=new QVBoxLayout(body);
            scroll->setWidget(body);layout->addWidget(scroll);QSet<QByteArray> seen;int count=0;
            auto volumes=QStorageInfo::mountedVolumes();std::sort(volumes.begin(),volumes.end(),[](const auto&a,const auto&b){return a.rootPath().size()<b.rootPath().size();});
            for(const auto &s:volumes){
                if(!s.isValid()||!s.isReady()||s.bytesTotal()<=0)continue;
#ifdef Q_OS_WIN
                QByteArray key=s.rootPath().toUtf8();if(seen.contains(key))continue;
#else
                QByteArray key=s.device();
                if(!s.device().startsWith("/dev/")||s.device().startsWith("/dev/loop")||seen.contains(key))continue;
#endif
                if(s.rootPath().startsWith("/boot")||s.rootPath().startsWith("/run/credentials"))continue;
                seen.insert(key);count++;
                auto *card=new QFrame;card->setObjectName("driveCard");auto *h=new QHBoxLayout(card);h->setContentsMargins(22,20,22,20);
                h->addWidget(new DriveRing(double(s.bytesTotal()-s.bytesAvailable())/s.bytesTotal(),accent,light));
                auto *copy=new QVBoxLayout;QString name=s.name();
                if(name.isEmpty())name=ui(s.rootPath()=="/"?"Disque principal":"Disque supplémentaire");
#ifdef Q_OS_WIN
                name=(s.name().isEmpty()?ui("Disque"):s.name())+" ("+QDir::toNativeSeparators(s.rootPath())+")";
#endif
                auto *title=new QLabel(name);title->setStyleSheet("font-size:22px;font-weight:600");copy->addWidget(title);
                auto *available=new QLabel(format(s.bytesAvailable())+" "+ui("disponibles sur")+" "+format(s.bytesTotal()));available->setStyleSheet("font-size:18px");copy->addWidget(available);
                copy->addWidget(new QLabel(ui("Occupé")+" : "+format(s.bytesTotal()-s.bytesAvailable())+"    ·    "+ui("Disponible")+" : "+format(s.bytesAvailable())));
                h->addLayout(copy,1);auto *open=new QPushButton(ui("Ouvrir ce disque"));h->addWidget(open);
                connect(open,&QPushButton::clicked,this,[s]{QDesktopServices::openUrl(QUrl::fromLocalFile(s.rootPath()));});
                card->setToolTip(QString::fromUtf8(s.device())+" · "+s.rootPath());cards->addWidget(card);
            }
            cards->addWidget(new QLabel(count?QString::number(count)+" "+ui("disque(s) détecté(s)"):ui("Aucun disque accessible détecté.")));cards->addStretch();
        }else if(active==4){
            auto *summary=new QLabel;layout->addWidget(summary);
            auto *filters=new QHBoxLayout;auto *search=new QLineEdit;search->setPlaceholderText("Rechercher un fichier…");filters->addWidget(search,1);
            auto *kind=new QComboBox;for(auto s:{"Tous les fichiers","Images","Vidéos","Documents","Autres"})kind->addItem(ui(s));filters->addWidget(kind);layout->addLayout(filters);
            auto *list=new QListWidget;list->setIconSize({40,40});list->setSpacing(5);layout->addWidget(list);
            QFileIconProvider provider;QMimeDatabase mime;
            for(const auto &f:data.large){
                QFileInfo fi(f.path);QString type=mime.mimeTypeForFile(fi,QMimeDatabase::MatchExtension).name();
                int group=type.startsWith("image/")?1:type.startsWith("video/")?2:(type.startsWith("text/")||type.contains("pdf")||type.contains("document"))?3:4;
                QString folder=QDir::homePath()==fi.absolutePath()?QString("~"):fi.absolutePath();folder.replace(QDir::homePath()+"/","~/");
                auto *item=new QListWidgetItem(provider.icon(fi),fi.fileName()+"    ·    "+format(f.size)+"\n"+folder,list);item->setSizeHint({0,76});
                item->setData(Qt::UserRole,f.path);item->setData(Qt::UserRole+1,group);item->setData(Qt::UserRole+2,f.modified);item->setData(Qt::UserRole+3,f.size);item->setToolTip(f.path);
            }
            auto updateFilter=[=]{int count=0;qint64 total=0;for(int i=0;i<list->count();i++){auto *item=list->item(i);bool show=item->text().contains(search->text(),Qt::CaseInsensitive)&&(kind->currentIndex()==0||kind->currentIndex()==item->data(Qt::UserRole+1).toInt());item->setHidden(!show);if(show){count++;total+=item->data(Qt::UserRole+3).toLongLong();}}summary->setText(QString::number(count)+" "+ui("fichiers affichés")+" · "+format(total)+" · "+ui("Les plus gros en premier"));};
            connect(search,&QLineEdit::textChanged,list,[updateFilter]{updateFilter();});connect(kind,&QComboBox::currentIndexChanged,list,[updateFilter]{updateFilter();});updateFilter();
            auto *details=new QLabel(ui("Sélectionnez un fichier pour voir ses détails."));details->setWordWrap(true);layout->addWidget(details);
            auto *open=new QPushButton(ui("Ouvrir le dossier"));open->setEnabled(false);layout->addWidget(open);
            connect(list,&QListWidget::currentItemChanged,this,[details,open](QListWidgetItem *item){open->setEnabled(item);if(item)details->setText(ui("Modifié")+" : "+QLocale().toString(item->data(Qt::UserRole+2).toDateTime(),QLocale::ShortFormat));});
            connect(open,&QPushButton::clicked,this,[list]{if(auto *item=list->currentItem())QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(item->data(Qt::UserRole).toString()).absolutePath()));});
        }else{
            auto *form=new QFormLayout;layout->addLayout(form);
            auto *language=new QComboBox;language->addItem("Français","fr");language->addItem("English","en");language->setCurrentIndex(english?1:0);form->addRow("Langue",language);
            auto *theme=new QCheckBox("Mode clair");theme->setChecked(light);form->addRow("Apparence",theme);
            auto *color=new QPushButton("Choisir couleur");form->addRow("Accent",color);
            connect(theme,&QCheckBox::toggled,this,[this](bool v){prefs.setValue("light",v);applyTheme();});
            connect(color,&QPushButton::clicked,this,[this]{auto c=QColorDialog::getColor(accent,this);if(c.isValid()){prefs.setValue("accent",c.name());applyTheme();}});
            auto *autoScan=new QCheckBox("Analyser au lancement");autoScan->setChecked(prefs.value("auto",true).toBool());form->addRow("Démarrage",autoScan);
            auto *days=new QSpinBox;days->setRange(1,365);days->setValue(prefs.value("days",14).toInt());days->setSuffix(ui(" jours"));form->addRow("Âge minimum caches, logs, /tmp",days);
            auto *threshold=new QSpinBox;threshold->setRange(1,1048576);threshold->setValue(prefs.value("threshold",100).toInt());threshold->setSuffix(ui(" Mo"));form->addRow("Seuil fichiers volumineux",threshold);
            auto *root=new QLineEdit(prefs.value("largeRoot",QDir::homePath()).toString());form->addRow("Dossier fichiers volumineux",root);
            auto *excluded=new QPlainTextEdit(prefs.value("exclusions").toString());excluded->setPlaceholderText("Un chemin absolu par ligne");form->addRow("Exclusions de toutes les analyses",excluded);
            auto *save=new QPushButton("Enregistrer et réanalyser");layout->addWidget(save);
            connect(save,&QPushButton::clicked,this,[=,this]{
                if(!QFileInfo(root->text()).isDir()||!QDir::isAbsolutePath(root->text())){QMessageBox::warning(this,"Dossier","Indique dossier absolu existant.");return;}
                for(auto s:excluded->toPlainText().split('\n',Qt::SkipEmptyParts))if(!QDir::isAbsolutePath(s.trimmed())){QMessageBox::warning(this,"Exclusions","Utilise chemins absolus.");return;}
                prefs.setValue("auto",autoScan->isChecked());prefs.setValue("days",days->value());prefs.setValue("threshold",threshold->value());prefs.setValue("largeRoot",root->text());prefs.setValue("exclusions",excluded->toPlainText());prefs.sync();analyze();
                prefs.setValue("language",language->currentData().toString());prefs.sync();applyTheme();
                QTimer::singleShot(0,this,[this]{rebuildPage();});
            });
            layout->addWidget(new QLabel("Confirmation toujours obligatoire. Aucun démarrage root ni nettoyage automatique."));
            layout->addStretch();
        }
        localizeWidgets(page);positionPage();page->show();
    }
    void positionPage(){if(page)page->setGeometry(qRound(329.*width()/1536),qRound(139.*height()/1024),qRound(1182.*width()/1536),qRound(840.*height()/1024));}
public:
    void verify(){
        if(busy){QTimer::singleShot(250,this,[this]{verify();});return;}
        resize(1536,1024);
        QJsonObject report{{"distribution",QSysInfo::prettyProductName()},{"kernel",QSysInfo::kernelVersion()},{"files",data.files.size()},{"largeFiles",data.large.size()},{"packages",data.packageStatus},{"skipped",data.skipped}};
        QJsonArray sizes;for(auto n:data.sizes)sizes.append(n);report["bytes"]=sizes;
        for(int i=0;i<6;i++){
            active=i;rebuildPage();
            grab().save(QCoreApplication::applicationDirPath()+QString("/page-%1.png").arg(i));
            if(i==2||i==4){auto *list=page->findChild<QListWidget*>();report[QString("items-%1").arg(i)]=list?list->count():-1;}
            if(i==3)report["drives"]=page->findChildren<QFrame*>("driveCard").size();
        }
        english=true;QLocale::setDefault(QLocale(QLocale::English));
        for(int i=0;i<6;i++){active=i;rebuildPage();grab().save(QCoreApplication::applicationDirPath()+QString("/page-en-%1.png").arg(i));}
        QFile reportFile(QCoreApplication::applicationDirPath()+"/verification.json");if(reportFile.open(QIODevice::WriteOnly))reportFile.write(QJsonDocument(report).toJson());
        QCoreApplication::quit();
    }
    Window(){setWindowTitle("Linux Cleanup");resize(1382,922);setMinimumSize(1000,667);setMouseTracking(true);
        QPixmap logo(128,128);logo.fill(Qt::transparent);QPainter lp(&logo);icon(lp,"leaf",QRectF(12,8,100,104));lp.end();setWindowIcon(QIcon(logo));
        for(auto &e:entries)e.amount="…";
        entries[0].title="Caches utilisateur";entries[0].description="Caches anciens du compte utilisateur.";
        QString cache=QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation);
        entries[0].path=cache;entries[3].path=cache+"/thumbnails";
        entries[4].path=QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)+"/Trash/files";
        QString state=qEnvironmentVariable("XDG_STATE_HOME");if(!state.startsWith('/'))state=QDir::homePath()+"/.local/state";entries[2].path=state;
        entries[2].description="Fichiers .log anciens du compte.";
        entries[1].description="Suggestions DNF / APT / pacman.\nAucune suppression automatique.";
        entries[5].description="Fichiers /tmp anciens appartenant\nà votre utilisateur.";
#ifdef Q_OS_WIN
        const QString local=QDir::fromNativeSeparators(qEnvironmentVariable("LOCALAPPDATA"));
        entries[0].path=local.isEmpty()?QString():local+"/Microsoft/Windows/INetCache";
        entries[0].description="Cache Internet Windows du compte.";
        entries[1].description="Ouvrir les applications Windows.";
        for(int i:{1,2,3,4}){entries[i].path.clear();entries[i].checked=false;entries[i].amount="Non pris en charge";}
        entries[2].description="Journaux gérés par Windows.";
        entries[3].description="Miniatures gérées par Windows.";
        entries[4].description="Ouvrir la corbeille Windows.";
        entries[5].path=QDir::tempPath();entries[5].description="Fichiers temporaires anciens du compte.";
#endif
        applyTheme();
        QStorageInfo disk(QDir::homePath());diskRatio=disk.bytesTotal()>0?double(disk.bytesTotal()-disk.bytesAvailable())/disk.bytesTotal():0;
        used=format(disk.bytesTotal()-disk.bytesAvailable())+" utilisés";free=format(disk.bytesAvailable())+" libres";
        if(prefs.value("auto",true).toBool())QTimer::singleShot(0,this,[this]{analyze();});
        else {timestamp="Analyse automatique désactivée";recover="—";}
    }
protected:
    void resizeEvent(QResizeEvent *e) override {QWidget::resizeEvent(e);positionPage();}
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);p.setRenderHint(QPainter::Antialiasing);p.scale(width()/1536.,height()/1024.);
        p.fillRect(QRectF(0,0,1536,1024),QColor(light?"#f3f4f6":"#191e22"));p.fillRect(QRectF(0,0,304,1024),QColor(light?"#e7eaf0":"#171c20"));p.setPen(QColor("#647080"));p.drawLine(304,0,304,1024);
        icon(p,"leaf",{32,40,54,69});text(p,{109,34,190,34},"Linux Cleanup",23,Qt::white,true);text(p,{109,72,190,45},"Un système plus propre,\nune meilleure expérience",14);
        for(int i=0;i<6;i++){QRectF r(14,143+i*65,277,60);if(active==i){QLinearGradient g(r.topLeft(),r.topRight());g.setColorAt(0,accent);g.setColorAt(1,accent.darker(160));p.setBrush(g);p.setPen(accent);p.drawRoundedRect(r,10,10);}icon(p,symbols[i],{32.,158.+i*65,29,30},active==i?Qt::white:ink);text(p,{83.,148.+i*65,212,49},pages[i],17,active==i?QColor("#fefefe"):ink,active==i);}
        icon(p,"leaf",{31,869,46,49});text(p,{96,868,195,47},"Linux propre\nDonnées locales",14);text(p,{29,962,240,30},"v0.3.0 · "+QString::number(data.files.size())+" fichiers mesurés",13);
        text(p,{329,42,650,48},active==0?"Bonjour !":pages[active],35,Qt::white,true);
        text(p,{329,91,750,35},busy?"Analyse locale en cours…":"Mesures locales · fichiers utilisateur accessibles",20);
        QIcon distro=QIcon::fromTheme("distributor-logo");
        if(!distro.isNull())distro.paint(&p,QRect(1070,72,44,44));else icon(p,"disk",{1070,72,44,44});
        text(p,{1128,67,254,27},QSysInfo::prettyProductName().section('(',0,0).trimmed(),15);text(p,{1128,95,254,27},QSysInfo::kernelVersion(),14);
        icon(p,"moon",{1411,80,28,28});icon(p,"settings",{1474,80,27,27});
        if(active!=0)return;
        panel(p,{329,139,636,291});text(p,{350,151,550,36},"Fichiers candidats · tailles logiques",20,Qt::white,true);
        p.setPen(QPen(QColor("#606b7d"),21));p.setBrush(Qt::NoBrush);p.drawEllipse(QRectF(395,215,176,180));
        QStringList colors{"#618dff","#9254cf","#c45487","#ffa642","#778395","#58c5ab"};
        qint64 sum=0;for(auto n:data.sizes)sum+=qMax<qint64>(0,n);int start=90*16;
        for(int i=0;i<6;i++)if(sum>0&&data.sizes[i]>0){int span=-qRound(5760.*data.sizes[i]/sum);p.setPen(QPen(QColor(colors[i]),21));p.drawArc(QRectF(395,215,176,180),start,span);start+=span;}
        text(p,{396,267,174,49},recover,30,Qt::white,true,Qt::AlignCenter);text(p,{396,313,174,33},"mesurés",16,ink,false,Qt::AlignCenter);
        QStringList names{"Cache","Paquets","Logs","Miniatures","Corbeille","Temporaires"};
        for(int i=0;i<6;i++){p.setPen(Qt::NoPen);p.setBrush(QColor(colors[i]));p.drawEllipse(QRectF(641,216+i*32,15,15));text(p,{673.,208.+i*32,155,31},names[i],15);text(p,{828.,208.+i*32,115,31},entries[i].amount,14,Qt::white,true,Qt::AlignRight|Qt::AlignVCenter);}
        panel(p,{985,139,526,170});text(p,{1006,151,460,36},"Votre stockage",20,Qt::white,true);icon(p,"disk",{1014,212,48,47});text(p,{1092,197,380,28},diskTitle,16,Qt::white,true);
        p.setPen(Qt::NoPen);p.setBrush(QColor("#383f48"));p.drawRoundedRect(QRectF(1092,233,397,20),9,9);p.setBrush(QColor("#6089ff"));p.drawRoundedRect(QRectF(1092,233,397*diskRatio,20),9,9);text(p,{1092,263,258,27},used,16);text(p,{1330,263,159,27},free,16,ink,false,Qt::AlignRight|Qt::AlignVCenter);
        panel(p,{985,327,526,103},QColor("#1d2d2a"),QColor("#29463c"));p.setPen(Qt::NoPen);p.setBrush(QColor("#48c58a"));p.drawEllipse(QRectF(1014,353,48,50));p.setPen(QPen(Qt::white,4,Qt::SolidLine,Qt::RoundCap));p.drawLine(1028,378,1035,385);p.drawLine(1035,385,1048,371);text(p,{1083,345,410,35},"Analyse en lecture seule",17,QColor("#9affd6"),true);text(p,{1083,380,400,28},"Vos fichiers restent sous votre contrôle.",14);
        text(p,{329,451,520,38},"Analyse rapide",27,Qt::white,true);text(p,{329,491,635,30},"Sélectionnez ce que vous souhaitez nettoyer.",16);icon(p,"clock",{997,478,24,24});text(p,{1030,475,271,31},timestamp,13);button(p,{1304,465,207,50},busy?"Analyse…":"Analyser à nouveau","refresh");
        for(int i=0;i<6;i++){
            auto r=card(i);auto &e=entries[i];panel(p,r);QRectF cb(r.x()+19,r.y()+21,21,21);panel(p,cb,e.checked?QColor("#70aaff"):QColor("#22282e"),QColor("#607080"),3);
            if(e.checked){p.setPen(QPen(QColor("#101820"),2.4,Qt::SolidLine,Qt::RoundCap));p.drawLine(cb.topLeft()+QPointF(4,11),cb.topLeft()+QPointF(9,16));p.drawLine(cb.topLeft()+QPointF(9,16),cb.topLeft()+QPointF(17,6));}
            icon(p,e.type,{r.x()+63,r.y()+20,32,34});text(p,{r.x()+119,r.y()+16,220,26},e.title,14,Qt::white,true);text(p,{r.x()+119,r.y()+40,210,26},e.amount,16);
            QColor risk=e.risk=="Recréable"?QColor("#87f9b0"):QColor("#ffb647");panel(p,{r.right()-112,r.y()+122,94,25},e.risk=="Recréable"?QColor("#223e30"):QColor("#3a3021"),e.risk=="Recréable"?QColor("#29653f"):QColor("#705127"),12);text(p,{r.right()-112,r.y()+122,94,25},e.risk,12,risk,false,Qt::AlignCenter);
            text(p,{r.x()+119,r.y()+67,250,48},e.description,12,ink,false,Qt::AlignLeft|Qt::AlignTop);
            button(p,{r.x()+119,r.y()+119,126,30},"Voir les détails ⌄");
        }
        button(p,{633,910,514,63},busy?"Analyse en cours…":"Vérifier "+format(selectedBytes())+"  ›","broom",true);button(p,{1235,916,276,58},"Voir un aperçu des fichiers","list");
        icon(p,"info",{329,980,25,25});text(p,{363,978,1100,32},"Suppression définitive des fichiers cochés, uniquement après confirmation.",14);
    }
    void mousePressEvent(QMouseEvent *ev) override {
        QPointF pt(ev->position().x()*1536/width(),ev->position().y()*1024/height());
        if(QRectF(1400,70,50,50).contains(pt)){prefs.setValue("light",!light);applyTheme();return;}
        if(QRectF(1460,70,50,50).contains(pt)){active=5;rebuildPage();update();return;}
        for(int i=0;i<6;i++)if(QRectF(14,143+i*65,277,60).contains(pt)){active=i;rebuildPage();update();return;}
        if(active!=0)return;
        for(int i=0;i<6;i++){
            if(QRectF(card(i).x()+19,card(i).y()+21,25,25).contains(pt)){entries[i].checked=!entries[i].checked;update();return;}
            if(QRectF(card(i).x()+119,card(i).y()+119,126,30).contains(pt)){details(i);return;}}
        if(QRectF(1304,465,207,50).contains(pt)){analyze();return;}
        if(QRectF(633,910,878,64).contains(pt)){
            showFiles(-1,pt.x()<1148);
        }
    }
};

int main(int argc,char **argv){
    QApplication app(argc,argv);app.setStyle("Fusion");
    QGuiApplication::setDesktopFileName("org.clarisweep.LinuxCleanup");
    QCoreApplication::setApplicationName("Linux Cleanup");
    LanguageDialogs languageDialogs;app.installEventFilter(&languageDialogs);
    app.setStyleSheet("QMessageBox{background:#20262c} QMessageBox QLabel{color:#e5ebf5;font-size:14px} QPushButton{background:#303b56;color:white;border:1px solid #566382;border-radius:7px;padding:9px 18px}");
    Window w;w.show();
    if(app.arguments().contains("--verify"))QTimer::singleShot(500,&w,[&]{w.verify();});
    if(app.arguments().contains("--screenshot"))QTimer::singleShot(200,&w,[&]{w.resize(1536,1024);w.grab().save(QCoreApplication::applicationDirPath()+"/preview.png");app.quit();});
    return app.exec();
}
