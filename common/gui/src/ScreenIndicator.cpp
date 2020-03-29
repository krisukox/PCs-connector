#include "ui_ScreenIndicator.h"

#include <QDebug>
#include <QLabel>
#include <QString>
#include <string>
#include "gui/ScreenIndicator.h"

Indicator::Indicator(QWidget* parent) : QWidget{parent}, ui{new Ui::Indicator}
{
    ui->setupUi(this);
}

void Indicator::setAppearance(int index, const Qt::GlobalColor& color)
{
    QLabel* label = ui->label;
    label->setText(QString::fromStdString(std::to_string(index)));
    QPalette palette = label->palette();
    palette.setColor(label->foregroundRole(), color);
    label->setPalette(palette);
}

Indicator::~Indicator()
{
    delete ui;
}
