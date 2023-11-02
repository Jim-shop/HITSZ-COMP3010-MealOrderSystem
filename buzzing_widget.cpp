#include "buzzing_widget.h"

void BuzzingWidget::closeEvent(QCloseEvent *event)
{
    emit quit();
}
