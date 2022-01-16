#ifndef TEMPGRAPH_H
#define TEMPGRAPH_H

#include <QWidget>

namespace Ui {
class tempgraph;
}

class tempgraph : public QWidget
{
    Q_OBJECT

public:
    explicit tempgraph(QWidget *parent = nullptr);
    ~tempgraph();

private:
    Ui::tempgraph *ui;
};

#endif // TEMPGRAPH_H
