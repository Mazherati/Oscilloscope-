#include "tempgraph.h"
#include "ui_tempgraph.h"

tempgraph::tempgraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tempgraph)
{
    ui->setupUi(this);
}

tempgraph::~tempgraph()
{
    delete ui;
}
