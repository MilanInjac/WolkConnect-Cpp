#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	MainWindow(const std::string& firmwareVersion, QWidget *parent = 0);
	~MainWindow();

public slots:
	void setMessage(std::string message);

	void setToggle(bool state);

signals:
	void messageSet(bool success);
	void toggleSet(bool success, bool value);

private slots:
	void on_pushButton_toggled(bool checked);

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
