#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator() = default;

private slots:
    void digitClicked();
    void operatorClicked();
    void equalClicked();
    void clearClicked();

private:
    QLineEdit *display;

    double pendingOperand;
    QString pendingOperator;
    bool waitingForOperand;

    void calculate(double rightOperand, const QString &pendingOperator);
};

#endif // CALCULATOR_H