#include "RenderArea.h"
void RenderArea::setNumberOfPlayers(int num) {
    if (num > this->playersNum) {
        auto n =num - this->playersNum;
       do
        {
            vectorOfPlayers.push_back(createPlayerInfo());
       } while (--n);
    }

    else if (num < this->playersNum) {
        auto n = this->playersNum - num;
        do
        {
          delete vectorOfPlayers.front();
          vectorOfPlayers.pop_front();
        } while (--n);
    }

	this->playersNum = num;

	repaint();
}

RenderArea::RenderArea(QWidget* parent) :QWidget(parent)
{
    this->setFixedSize(MAP_SIZE_X_Y, MAP_SIZE_X_Y);
	playersNum = 1;
	auto result_of_map_loading  = pixmap.load("res//olympus.png");
    if (!result_of_map_loading)exit(1);

    vectorOfPlayers.push_back(createPlayerInfo());

    bShowNextRing = false;
    bShowFirstRing = false;
    bShowDebugMap = false;

    debugMapImage = new QImage(MAP_SIZE_X_Y, MAP_SIZE_X_Y, QImage::Format_ARGB32);
    debugMapImage->fill(QColor(0, 0, 0, 0));

    debugColor = QColor(0, 0, 0);

}

void RenderArea::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.drawPixmap(QRect(QRect(0, 0, width() - 1, height() - 1)),pixmap);

    QPoint result;

    if(bShowDebugMap||bShowNextRing)
    result = calculateNextRing(QPoint(400, 400), 400, 300);

   if (bShowDebugMap) 
    {
        painter.drawImage(0, 0, *debugMapImage);
    }

    for (auto& p : vectorOfPlayers) {
       
        painter.fillPath(p->QPath,QBrush(Qt::green,Qt::SolidPattern));

#ifdef HITBOX
        painter.setPen(QPen(Qt::red));
        painter.drawRect(p->HitBox);
#endif
    }
    if (bShowNextRing)
    {
        painter.setPen(QPen(Qt::red));
        painter.drawEllipse(result, 300, 300);
        painter.setPen(QPen(Qt::white));
        painter.drawEllipse(result, STEP, STEP);
        
    }
    if (bShowFirstRing)
    {
        painter.setPen(QPen(Qt::red));
        painter.drawEllipse(QPointF(400,400), 400,400);
        
    
    }
    painter.setPen(QPen(Qt::black));
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

RenderArea::PlayerInfo* RenderArea::createPlayerInfo()
{
    PlayerInfo* plrInfo = new PlayerInfo;

    QPainterPath trianglePath;

    constexpr int gap = 30;

    auto pos = QPointF(QRandomGenerator::global()->bounded(gap, width() - gap),
        QRandomGenerator::global()->bounded(gap, height() - gap));

    trianglePath.moveTo(pos);
    trianglePath.lineTo(pos.x() + 20.0, pos.y() + 0.0);
    trianglePath.lineTo(pos.x() + 10.0, pos.y() + 30.0);
    trianglePath.lineTo(pos.x() + 0.0, pos.y() + 0.0);
    trianglePath.closeSubpath();

    plrInfo->QPath = std::move(trianglePath);
    plrInfo->HitBox = makeHitboxFromQPainterPath(plrInfo->QPath);
    plrInfo->pathAngle = 270;

    return plrInfo;
}

void RenderArea::mousePressEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        for (auto& v : vectorOfPlayers)
        {
            if (event->pos().x() > v->HitBox.x() &&
                event->pos().y() > v->HitBox.y() &&
                event->pos().x() < (v->HitBox.x() + v->HitBox.width()) &&
                event->pos().y() < (v->HitBox.y() + v->HitBox.height()))pSelectedPlayer=v;
        }
    }
}

void RenderArea::mouseReleaseEvent(QMouseEvent* event) {

    if ((event->button() == Qt::LeftButton) && pSelectedPlayer) {
    
        auto current_possition_x = pSelectedPlayer->QPath.currentPosition().x();
        auto current_possition_y = pSelectedPlayer->QPath.currentPosition().y();

        float PI = std::numbers::pi_v<float>;

        auto angle = 180.0-(180.0 / PI)*  atan2f(
                           ( pSelectedPlayer->QPath.currentPosition().y()- event->pos().y() ),
                           ( pSelectedPlayer->QPath.currentPosition().x()- event->pos().x() ));

        if (angle < 0.0)angle += 360;
       

        //OutputDebugStringA(std::to_string(angle).c_str());
        //OutputDebugStringA("-");
        //OutputDebugStringA(std::to_string(pSelectedPlayer->pathAngle).c_str());
        //OutputDebugStringA("=");
        //OutputDebugStringA(std::to_string(angle - pSelectedPlayer->pathAngle).c_str());
        //OutputDebugStringA("\n");

        pSelectedPlayer->QPath = QTransform()
            .translate(-1 * current_possition_x, -1 * current_possition_y)
            .map(pSelectedPlayer->QPath);

        pSelectedPlayer->QPath = QTransform()
            .rotate(-(angle-pSelectedPlayer->pathAngle), Qt::ZAxis)
            .map(pSelectedPlayer->QPath);
        pSelectedPlayer->pathAngle = angle;

        pSelectedPlayer->QPath = QTransform()
            .translate(current_possition_x, current_possition_y)
            .map(pSelectedPlayer->QPath);

        pSelectedPlayer->QPath = QTransform()
            .translate(event->pos().x() - pSelectedPlayer->QPath.currentPosition().x(), event->pos().y() - pSelectedPlayer->QPath.currentPosition().y())
            .map(pSelectedPlayer->QPath);

        pSelectedPlayer->HitBox = makeHitboxFromQPainterPath(pSelectedPlayer->QPath);

        pSelectedPlayer = nullptr;
        
        repaint();
    };

}

void RenderArea::setCurrentMap(int map)
{
    if (map == static_cast<int>(ALMaps::olympus))
    {
        auto result_of_map_loading = pixmap.load("res//olympus.png");
       // if (!result_of_map_loading)exit(1);
    }
    else if (map == static_cast<int>(ALMaps::kings_canyon))
    {
        auto result_of_map_loading = pixmap.load("res//kings_canyon.png");
       // if (!result_of_map_loading)exit(1);
    }
    else if (map == static_cast<int>(ALMaps::worlds_edge))
    {
        auto result_of_map_loading = pixmap.load("res//worlds_edge.png");
       // if (!result_of_map_loading)exit(1);
    }

    repaint();

}

void RenderArea::setShowNextRing(bool state)
{
    this->bShowNextRing = state;
    repaint();
}

void  RenderArea::setShowFirstRing(bool state)
{
    this->bShowFirstRing = state;
    repaint();
}

QPoint RenderArea::calculateNextRing(QPoint point, int current_ring_radius, int next_ring_radius)
{
 
    float maxminValue = 0.0f;
    QPoint result;

    float  resultvec[ARR_SIZE][ARR_SIZE];
    for (int x = 0; x < ARR_SIZE; x++)
        for (int y = 0; y < ARR_SIZE; y++)
        {
            resultvec[x][y] = FLT_MAX;
            
        };

    for (int x = 0; x < ARR_SIZE; x++)
        for (int y = 0; y < ARR_SIZE; y++) 
        {
            auto x_coord = x * STEP;
            auto y_coord = y * STEP;

            if (distance_between(x_coord, y_coord, point.x(), point.y()) > (current_ring_radius-next_ring_radius)) continue;
            else {
                for (auto& v : vectorOfPlayers) 
                {
                    auto distance = distance_between(x_coord, y_coord, v->HitBox.x(), v->HitBox.y());
                    if (distance < resultvec[x][y])resultvec[x][y] = distance;
                }

                if (resultvec[x][y] > maxminValue) { maxminValue = resultvec[x][y]; result = QPoint(x_coord, y_coord); }

                }
        }

    for (int x = 0; x < ARR_SIZE; x++)
        for (int y = 0; y < ARR_SIZE; y++)
        {
            auto x_coord = x * STEP;
            auto y_coord = y * STEP;

            if (distance_between(x_coord, y_coord, point.x(), point.y()) > (current_ring_radius - next_ring_radius))
            {
                drawDebugMapPoint(x_coord, y_coord, 0, 0);
            }
            else 
            {
                drawDebugMapPoint(x_coord, y_coord, resultvec[x][y], maxminValue);
            }
        }

    return result;
};

void RenderArea::setShowDebugMap(bool state)
{
    this->bShowDebugMap = state;
    repaint();
}

void RenderArea::setCurrentDebugColor(QColor clr)
{
    debugColor = clr;
    repaint();
}

void RenderArea::drawDebugMapPoint(int x, int y, float value, float maxmin)
{
    QColor clr = debugColor;
    int alpha_value = std::lround(float(255) * value / maxmin);
    clr.setAlpha(alpha_value);
   
    for (int i = 0; i < STEP; i++) {
        for (int j = 0; j < STEP; j++)
        {
            debugMapImage->setPixelColor(x + i,y + j, clr);
        }

    }

}

QRectF RenderArea::makeHitboxFromQPainterPath(QPainterPath PP)
{
    return PP.boundingRect();
};