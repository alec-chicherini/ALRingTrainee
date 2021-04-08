#pragma once
#include <Qapplication>
#include <QPushButton>
#include <qpainter.h>
#include <QLAbel>
#include <QSpinBox>
#include <Qwidget.h>
#include <qgridlayout.h>
#include <qcombobox.h>
#include <qcheckbox.h>

#include "RenderArea.h"

#include "enums.h"
#include "global_variables.h"

class window : public QWidget
{

private:
Q_OBJECT
RenderArea* renderArea;
QLabel* PlayersNumber;
QSpinBox* PlayersNumberBox;
QLabel* MapName;
QComboBox* MapNameBox;
QCheckBox* ShowNextRingBox;
QCheckBox* ShowFirstRingBox;
QCheckBox* ShowDebugMapBox;
QSpinBox *R_DebugMapBox;
QSpinBox *G_DebugMapBox;
QSpinBox *B_DebugMapBox;
QLabel* R_DebugMap;
QLabel* G_DebugMap;
QLabel* B_DebugMap;
public:
	 explicit window(QWidget* parent = 0);

signals:

private slots:
	void numberPlayersChanged();
	void mapChanged();
	void debugColorChanged();


};

