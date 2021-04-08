#pragma once
#include <qwidget.h>
#include <qpainterpath.h>
#include <qpainter.h>
#include <QRandomGenerator>
#include <qevent.h>
#include <qimage.h>
#include <qtransform.h>

#include "enums.h"
#include "helper_calc.cpp"

#include "global_variables.h"

#include <windows.h>
#include <numbers>

#define HITBOX

class RenderArea :
    public QWidget
{
    Q_OBJECT

public:

    explicit RenderArea(QWidget* parent = 0);

public slots:
    void setNumberOfPlayers(int num);
    void setCurrentMap(int num);
    void setShowNextRing(bool state);
    void setShowFirstRing(bool state);
    void setShowDebugMap(bool state);
    void setCurrentDebugColor(QColor clr);


protected:
    void paintEvent(QPaintEvent* event)override;
    void mousePressEvent(QMouseEvent* event)override;
    void mouseReleaseEvent(QMouseEvent* event)override;

private:
    int playersNum=1;
    QPixmap pixmap;
    bool bShowNextRing;
    bool bShowFirstRing;
    bool bShowDebugMap;
    QColor debugColor;

    QImage* debugMapImage;
    void drawDebugMapPoint(int x, int y, float value,float maxmin);
 

    struct PlayerInfo {
        QPainterPath QPath;
        QRectF HitBox;
        float pathAngle;
    };

    QRectF makeHitboxFromQPainterPath(QPainterPath PP);

    QVector<PlayerInfo*> vectorOfPlayers;
    PlayerInfo* createPlayerInfo();
    PlayerInfo* pSelectedPlayer = nullptr;

    QPoint calculateNextRing(QPoint point, int current_ring_radius, int next_ring_radius);
    

#ifdef DEBUG
  
#endif
    
};

