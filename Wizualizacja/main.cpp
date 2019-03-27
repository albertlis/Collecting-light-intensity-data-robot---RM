#include "mainwindow.h"
#include <QApplication>
#include <QtDataVisualization>
#include <Q3DBars>

using namespace QtDataVisualization;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Q3DBars bars;
    constexpr uint8_t rows{50};
    constexpr uint8_t columns{50};
    bars.setFlags(bars.flags() ^ Qt::FramelessWindowHint);
    bars.rowAxis()->setRange(0, rows - 1);
    bars.columnAxis()->setRange(0, columns - 1);
    QBar3DSeries *series = new QBar3DSeries;
    for(auto columns1{0}; columns1 < columns; ++columns1)
    {
        QBarDataRow *data = new QBarDataRow;
        for(auto i{0}; i < rows; ++i)
        {
            *data << static_cast<float>(i* columns1);
        }
        series->dataProxy()->addRow(data);
        delete data;
        data = nullptr;
    }
    bars.addSeries(series);
    bars.show();

    return a.exec();
}
