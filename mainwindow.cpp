#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
#include <QVector3D>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->offsetXSpinBox, SIGNAL(valueChanged(double)),
            ui->glWidget, SLOT(setOffsetX(double)));
    connect(ui->offsetYSpinBox, SIGNAL(valueChanged(double)),
            ui->glWidget, SLOT(setOffsetY(double)));
    connect(ui->chooseColorButton, SIGNAL(clicked()),
            this, SLOT(chooseColor()));
    connect(ui->frustumScaleSpinBox, SIGNAL(valueChanged(double)),
            ui->glWidget, SLOT(setFrustrumScale(double)));
    connect(ui->zNearSpinBox, SIGNAL(valueChanged(double)),
            ui->glWidget, SLOT(setZNear(double)));
    connect(ui->zFarSpinBox, SIGNAL(valueChanged(double)),
            ui->glWidget, SLOT(setZFar(double)));
    connect(ui->eyePointXSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(setEyePoint()));
    connect(ui->eyePointYSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(setEyePoint()));
    connect(ui->eyePointZSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(setEyePoint()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chooseColor()
{
    QColorDialog dialog(this);
    dialog.setCurrentColor(ui->glWidget->color());

    if (dialog.exec() == QDialog::Rejected)
        return;

    ui->glWidget->setColor(dialog.currentColor());
}

void MainWindow::setEyePoint()
{
    ui->glWidget->setEyePoint(QVector3D(ui->eyePointXSpinBox->value(),
                                        ui->eyePointYSpinBox->value(),
                                        ui->eyePointZSpinBox->value()));
}
