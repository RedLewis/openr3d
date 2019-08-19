#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Create the GLWidget
    GLWidget* glWidget = new GLWidget();
    glWidget->setFocusPolicy(Qt::StrongFocus);
    ui->centralWidget->layout()->addWidget(glWidget);

    //Create a fps indicator label on GLWidget
    QLabel* fpsIndicator = new QLabel("fpsIndicator");
    fpsIndicator->setStyleSheet("QLabel { color : white; background-color: rgba(0, 0, 0, 0%); }");
    fpsIndicator->setAlignment(Qt::AlignBottom);
    fpsIndicator->setAlignment(Qt::AlignRight);
    fpsIndicator->setFocusPolicy(Qt::NoFocus); //Avoid fpsIndicator blocking keyboard events
    fpsIndicator->setAttribute(Qt::WA_TransparentForMouseEvents, true); //Avoid fpsIndicator blocking mouse events
    QObject::connect(glWidget, SIGNAL(frameRateUpdate(int)), fpsIndicator, SLOT(setNum(int)));
    glWidget->setLayout(new QGridLayout);
    glWidget->layout()->addWidget(fpsIndicator);
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
