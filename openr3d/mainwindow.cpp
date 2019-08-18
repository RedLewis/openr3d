#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Create the GLWidget
    GLWidget* glWidget = new GLWidget(120, this);
    ui->rendererGridLayout->addWidget(glWidget);
    QObject::connect(glWidget, SIGNAL(frameRateUpdate(int)),
                     ui->ioRendererRealTimeFrameratesLcdNumber, SLOT(display(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    /*QMessageBox::StandardButton resBtn = QMessageBox::question( this, this->windowTitle(),
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }*/
    (void)event;
    std::cout << "Closing window." << std::endl;
}
