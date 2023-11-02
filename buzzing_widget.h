#ifndef BUZZINGWIDGET_H
#define BUZZINGWIDGET_H

#include <QWidget>

class BuzzingWidget : public QWidget
{
    Q_OBJECT

public:
    using QWidget::QWidget;

private:
    void closeEvent(QCloseEvent *event) override;

signals:
    void quit();
};

#endif // BUZZINGWIDGET_H
