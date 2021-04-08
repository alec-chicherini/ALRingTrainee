#include "window.h"

window::window(QWidget* parent) :QWidget(parent) {
	this->setFixedSize(1100, 825);

	renderArea = new RenderArea;

	PlayersNumber = new QLabel(this);
	PlayersNumber->setText(tr("&Players number:"));
	PlayersNumberBox = new QSpinBox(this);
	PlayersNumberBox->setRange(1, 60);
	PlayersNumberBox->setValue(1);
	PlayersNumber->setBuddy(PlayersNumberBox);

	MapNameBox = new QComboBox;
	MapNameBox->addItem("Olympus", static_cast<int>(ALMaps::olympus));
	MapNameBox->addItem("Worlds edge", static_cast<int>(ALMaps::worlds_edge));
	MapNameBox->addItem("Kings Canyon", static_cast<int>(ALMaps::kings_canyon));
	MapName = new QLabel("&Map:", this);
	MapName->setBuddy(MapNameBox);

	ShowNextRingBox = new QCheckBox("&Show next ring");
	ShowFirstRingBox= new QCheckBox("&Show first ring");
	ShowNextRingBox->setChecked(false);
	ShowFirstRingBox->setChecked(false);

	ShowDebugMapBox = new QCheckBox("&Show debug map");
	ShowDebugMapBox->setChecked(false);

	R_DebugMapBox = new QSpinBox(this); R_DebugMapBox->setRange(0, 255); R_DebugMapBox->setValue(0);
	G_DebugMapBox = new QSpinBox(this); G_DebugMapBox->setRange(0, 255); G_DebugMapBox->setValue(0);
	B_DebugMapBox = new QSpinBox(this); B_DebugMapBox->setRange(0, 255); B_DebugMapBox->setValue(0);
	R_DebugMap = new QLabel("& R"); R_DebugMap->setBuddy(R_DebugMapBox);
	G_DebugMap = new QLabel("& G"); G_DebugMap->setBuddy(G_DebugMapBox);
	B_DebugMap = new QLabel("& B"); B_DebugMap->setBuddy(B_DebugMapBox);

	connect(PlayersNumberBox, &QSpinBox::valueChanged, this, &window::numberPlayersChanged);
	connect(MapNameBox, QOverload<int>::of(&QComboBox::activated), this, &window::mapChanged);
	connect(ShowNextRingBox, &QAbstractButton::toggled, renderArea, &RenderArea::setShowNextRing);
	connect(ShowFirstRingBox, &QAbstractButton::toggled, renderArea, &RenderArea::setShowFirstRing);
	connect(ShowDebugMapBox, &QAbstractButton::toggled, renderArea, &RenderArea::setShowDebugMap);
	connect(R_DebugMapBox, &QSpinBox::valueChanged, this, &window::debugColorChanged);
	connect(G_DebugMapBox, &QSpinBox::valueChanged, this, &window::debugColorChanged);
	connect(B_DebugMapBox, &QSpinBox::valueChanged, this, &window::debugColorChanged);

	QGridLayout* mainLayout = new QGridLayout;

	mainLayout->setColumnMinimumWidth(0, 50);
	mainLayout->setColumnMinimumWidth(1, 150);
	
	mainLayout->setColumnStretch(0, 1);
	mainLayout->setColumnStretch(1, 1);
	mainLayout->setColumnStretch(2, 10);

	mainLayout->addWidget(PlayersNumber, 0, 0);
	mainLayout->addWidget(PlayersNumberBox, 0, 1);

	mainLayout->addWidget(MapName, 1, 0);
	mainLayout->addWidget(MapNameBox, 1, 1);

	mainLayout->addWidget(ShowNextRingBox, 9, 0);
	mainLayout->addWidget(ShowFirstRingBox, 9, 1);
	mainLayout->addWidget(renderArea, 0, 7, 10, 1,Qt::AlignTop);

	mainLayout->addWidget(ShowDebugMapBox, 8, 0);

	QGridLayout* RGBLayout = new QGridLayout;
	RGBLayout->setSpacing(0);
	RGBLayout->addWidget(R_DebugMap, 0, 0);
	RGBLayout->addWidget(R_DebugMapBox, 0, 1);
	RGBLayout->addWidget(G_DebugMap, 0, 2);
	RGBLayout->addWidget(G_DebugMapBox, 0, 3);
	RGBLayout->addWidget(B_DebugMap, 0, 4);
	RGBLayout->addWidget(B_DebugMapBox, 0, 5);
	
	mainLayout->addLayout(RGBLayout, 8, 1);

	setLayout(mainLayout);

	setWindowTitle("AL zone traine");


};

void window::numberPlayersChanged() {

	renderArea->setNumberOfPlayers(PlayersNumberBox->value());
}

void window::mapChanged()
{
	renderArea->setCurrentMap(MapNameBox->currentIndex());
}

void window::debugColorChanged()
{
	renderArea->setCurrentDebugColor(QColor(R_DebugMapBox->value(), G_DebugMapBox->value(), B_DebugMapBox->value()));
}

