#ifndef SETTINGS_H
#define SETTINGS_H

#include <QFrame>

namespace Ui {
class Settings;
}

class Settings : public QFrame
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private slots:
    void on_pushButton_clicked();

signals:
    void sendParams(double,double,int,int,bool);

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
