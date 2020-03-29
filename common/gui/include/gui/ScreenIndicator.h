#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class Indicator;
}
QT_END_NAMESPACE

class Indicator : public QWidget
{
    using QWidget::QWidget;
    Q_OBJECT

public:
    Indicator(QWidget* parent = nullptr);
    void setAppearance(int index, const Qt::GlobalColor&);
    ~Indicator();

    Ui::Indicator* ui;
};
