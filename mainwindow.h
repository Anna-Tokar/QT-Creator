#pragma once
#include "TableDialog.h"

#include <QtWidgets>
#include <QMainWindow>
#include <QApplication>
#include <QtGui>
#include <QMap>
#include <QPair>

class QLabel;
class QLineEdit;
class QPushButton;

typedef QPair<int, int> IntPair;
typedef QMap<QString, IntPair> BookEl;

class StudentBook : public QWidget {
    Q_OBJECT

public:
    StudentBook(QWidget *parent = 0);
     enum Mode { NavigationMode, AddingMode, EditingMode };

public slots: // Слот - функция, которая реагирует на особый сигнал.
    void addStudent();
    void submitStudent();
    void cancel();
    void next();
    void previous();
    void editStudent();
    void removeStudent();
    void saveToFile();
    void loadFromFile();
    void tableStudent();

private:
    void updateInterface(Mode mode);

    // Поля для ввода данных
    QLineEdit *nameLine;
    QLineEdit *courseLine;
    QLineEdit *groupLine;

    QPushButton *addButton;
    QPushButton *submitButton;
    QPushButton *cancelButton;
    QPushButton *nextButton;
    QPushButton *previousButton;
    QPushButton *editButton;
    QPushButton *removeButton;
    QPushButton *openButton;
    QPushButton *saveButton;
    QPushButton *tableButton;

    BookEl student; // контейнер для хранения студентов

    QString oldName; // Имя
    IntPair oldInf; // Информация

    Mode currentMode; // Режим работы приложения
    TableDialoge* table; // Вывод в виде таблицы
};
