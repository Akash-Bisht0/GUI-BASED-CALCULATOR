#include "calculator.h"

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent), pendingOperand(0.0), waitingForOperand(true)
{
    // Configure main window and display
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setWindowTitle("Qt C++ Calculator");
    setFixedSize(300, 400);

    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(15);

    // Set display styling
    QFont font = display->font();
    font.setPointSize(20);
    display->setFont(font);

    QGridLayout *layout = new QGridLayout(centralWidget);
    layout->addWidget(display, 0, 0, 1, 4);

    // Button layout definition
    const QString buttons[5][4] = {
        {"C", "", "", "/"},
        {"7", "8", "9", "*"},
        {"4", "5", "6", "-"},
        {"1", "2", "3", "+"},
        {"0", ".", "=", ""}
    };

    // Create buttons dynamically
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 4; ++col) {
            QString text = buttons[row][col];
            if (text.isEmpty()) continue;

            QPushButton *btn = new QPushButton(text);
            btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            btn->setFont(font);

            // Connect signal to appropriate slots based on button character
            if (text >= "0" && text <= "9" || text == ".") {
                connect(btn, &QPushButton::clicked, this, &Calculator::digitClicked);
            } else if (text == "+" || text == "-" || text == "*" || text == "/") {
                connect(btn, &QPushButton::clicked, this, &Calculator::operatorClicked);
            } else if (text == "=") {
                connect(btn, &QPushButton::clicked, this, &Calculator::equalClicked);
            } else if (text == "C") {
                connect(btn, &QPushButton::clicked, this, &Calculator::clearClicked);
            }

            if (text == "=") {
                layout->addWidget(btn, row + 1, col, 1, 2); // Span '=' button across 2 columns
                col++; // Skip next grid cell
            } else {
                layout->addWidget(btn, row + 1, col);
            }
        }
    }
}

void Calculator::digitClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    QString digitValue = clickedButton->text();

    if (display->text() == "0" && digitValue != ".") {
        display->clear();
    }

    if (waitingForOperand) {
        display->clear();
        waitingForOperand = false;
    }

    // Prevent entering multiple decimal points
    if (digitValue == "." && display->text().contains(".")) {
        return;
    }

    display->setText(display->text() + digitValue);
}

void Calculator::operatorClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    QString clickedOperator = clickedButton->text();
    double currentOperand = display->text().toDouble();

    if (!pendingOperator.isEmpty() && !waitingForOperand) {
        calculate(currentOperand, pendingOperator);
        display->setText(QString::number(pendingOperand) + " " + clickedOperator);
    } else {
        pendingOperand = currentOperand;
        display->setText(QString::number(pendingOperand) + " " + clickedOperator);
    }

    pendingOperator = clickedOperator;
    waitingForOperand = true;
}

void Calculator::equalClicked()
{
    if (pendingOperator.isEmpty()) return;

    // Extract the operand after the operator string display
    QString currentText = display->text();
    QStringList parts = currentText.split(" ");

    double rightOperand = parts.last().toDouble();

    calculate(rightOperand, pendingOperator);
    display->setText(QString::number(pendingOperand));

    pendingOperator.clear();
    waitingForOperand = true;
}

void Calculator::clearClicked()
{
    pendingOperand = 0.0;
    pendingOperator.clear();
    display->setText("0");
    waitingForOperand = true;
}

void Calculator::calculate(double rightOperand, const QString &pendingOperator)
{
    if (pendingOperator == "+") {
        pendingOperand += rightOperand;
    } else if (pendingOperator == "-") {
        pendingOperand -= rightOperand;
    } else if (pendingOperator == "*") {
        pendingOperand *= rightOperand;
    } else if (pendingOperator == "/") {
        if (rightOperand != 0.0) {
            pendingOperand /= rightOperand;
        } else {
            display->setText("Error");
            pendingOperand = 0.0;
            pendingOperator.isEmpty();
            waitingForOperand = true;
            return;
        }
    }
}