#pragma once
#include "i18n.h"
#include "data.h"
class DriveRing:public QWidget{
    double used;QColor accent;bool light;
public:
    DriveRing(double u,QColor a,bool l):used(qBound(0.,u,1.)),accent(a),light(l){setFixedSize(132,132);}
    void paintEvent(QPaintEvent*) override{
        QPainter p(this);p.setRenderHint(QPainter::Antialiasing);p.setPen(QPen(light?QColor("#dce1ea"):QColor("#39424f"),12));
        p.drawEllipse(QRectF(12,12,108,108));p.setPen(QPen(accent,12,Qt::SolidLine,Qt::RoundCap));p.drawArc(QRectF(12,12,108,108),90*16,-qRound(used*5760));
        p.setPen(light?QColor("#273343"):Qt::white);QFont f=p.font();f.setPixelSize(24);f.setBold(true);p.setFont(f);p.drawText(rect(),Qt::AlignCenter,QString::number(qRound(used*100))+"%");
    }
};
