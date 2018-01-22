#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(const std::string& firmwareVersion, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	ui->label->setText(firmwareVersion.c_str());

	//setFixedSize(800,600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMessage(std::string message)
{
	ui->pushButton->setText(message.c_str());
	emit messageSet(true);
}

void MainWindow::setToggle(bool tog)
{
	ui->pushButton->setChecked(tog);
	//emit toggleSet(true);
}

void MainWindow::on_pushButton_toggled(bool checked)
{
	emit toggleSet(true, checked);
}
