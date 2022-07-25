#include "mainwindow.h"
#include<QDebug>

StudentBook::StudentBook(QWidget *parent) : QWidget(parent) {

    QLabel *nameLabel = new QLabel(tr("Имя:"));
    nameLine = new QLineEdit;
    nameLine->setReadOnly(true);

    QLabel *courseLabel = new QLabel(tr("Курс:"));
    courseLine = new QLineEdit;
    courseLine->setReadOnly(true);

    QLabel *groupLabel = new QLabel(tr("Группа:"));
    groupLine = new QLineEdit;
    groupLine->setReadOnly(true);

    editButton = new QPushButton(tr("&Редактирование"));
    editButton->setEnabled(false);

    removeButton = new QPushButton(tr("&Удаление"));
    removeButton->setEnabled(false);

    // Изначальное состояние кнопок
    addButton = new QPushButton(tr("&Добавление"));
    addButton->show();

    submitButton = new QPushButton(tr("&OK"));
    submitButton->hide();

    cancelButton = new QPushButton(tr("&Отмена"));
    cancelButton->hide();

    nextButton = new QPushButton(tr("&Следующий"));
    nextButton->setEnabled(false);

    previousButton = new QPushButton(tr("&Предыдущий"));
    previousButton->setEnabled(false);

    openButton = new QPushButton(tr("&Открыть"));
    openButton->show();

    saveButton = new QPushButton(tr("&Сохранить"));
    saveButton->show();

    table = new TableDialoge;
    tableButton = new QPushButton(tr("&Таблица"));
    tableButton->show();

    // Привязываем кнопки (Сигналы и слоты)
    connect(addButton, SIGNAL(clicked()), this, SLOT(addStudent()));
    connect(submitButton, SIGNAL(clicked()), this, SLOT(submitStudent()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(nextButton, SIGNAL(clicked()), this, SLOT(next()));
    connect(previousButton, SIGNAL(clicked()), this, SLOT(previous()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editStudent()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeStudent()));
    connect(openButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveToFile()));
    connect(tableButton, SIGNAL(clicked()), this, SLOT(tableStudent()));

    // Иннициализируем QGridLayout  и распологаем все компоненты
    // выравнивание виджетов по сетке
    QGridLayout  *gridLayout = new QGridLayout;
    gridLayout->addWidget(nameLabel, 0, 0);
    gridLayout->addWidget(nameLine, 0, 1, 1, 3);
    gridLayout->addWidget(courseLabel, 1, 0);
    gridLayout->addWidget(courseLine, 1, 1);
    gridLayout->addWidget(groupLabel, 2, 0);
    gridLayout->addWidget(groupLine, 2, 1);

    // Распологаем кнопки
    // добавления
    gridLayout->addWidget(addButton, 0, 4);
    gridLayout->addWidget(submitButton);
    gridLayout->addWidget(cancelButton);

    // Навигация
    gridLayout->addWidget(previousButton, 3, 0);
    gridLayout->addWidget(nextButton, 3, 1);

    // Изменение
    gridLayout->addWidget(editButton, 1, 4);
    gridLayout->addWidget(removeButton, 2, 4);
    gridLayout->addWidget(openButton, 3, 4);
    gridLayout->addWidget(saveButton, 4, 4);
    gridLayout->addWidget(tableButton, 5, 4);

    // выводим Layout
    setLayout(gridLayout);

    setWindowTitle(tr("Студенческая книга"));

    resize( 400, 200 );
}


// Добавление
void StudentBook::addStudent() {
    oldName = nameLine->text();
    oldInf.first = courseLine->text().toInt();
    oldInf.second = groupLine->text().toInt();

    nameLine->clear();
    courseLine->clear();
    groupLine->clear();

    updateInterface(AddingMode);
}

// Отмена
void StudentBook::cancel() {
    // Ставим старые значения
    nameLine->setText(oldName);
    courseLine->setText(QString::number(oldInf.first));
    groupLine->setText(QString::number(oldInf.second));

    updateInterface(NavigationMode); // Мод навигации
}

// ok
void StudentBook::submitStudent() {
    // Считываем данные
    QString name = nameLine->text();
    IntPair Inf = qMakePair(courseLine->text().toInt(), groupLine->text().toInt());

    // Проверяем что поля заполнены
    if (name == "" || Inf.first == 0 ||  Inf.second == 0) {
        QMessageBox::information(this, tr("Пустое Поле"), tr("Пожалуйста, введите имя, курс и группу."));
        return;
    }
    if (currentMode == AddingMode) { // Если это добавление
    if (!student.contains(name)) {
        student.insert(name, Inf);
        QMessageBox::information(this, tr("Добавление Успешно"), tr("\"%1\" был добавлен ").arg(name));
    } else {
        QMessageBox::information(this, tr("Добавление Неудачно"), tr("Извините, \"%1\" уже есть в вашем файле.").arg(name));
        return;
           }
    }
    else if (currentMode == EditingMode) { // Если изменение
        if (!student.contains(name)) {
            QMessageBox::information(this, tr("Редактирование прошло успешно"),tr("\"%1\" был отредактирован ").arg(oldName));
            student.remove(oldName);
            student.insert(name, Inf);
        }
        else if (oldInf != Inf) {
            QMessageBox::information(this, tr("Удаление прошло успешно"), tr("\"%1\" был отредактирован n").arg(name));
            student[name] = Inf;
        }
        else {
            QMessageBox::information(this, tr("Редактирование не удалось"), tr("Извините, \"%1\" уже есть в вашем файле.").arg(name));
            return;
        }
    }
    updateInterface(EditingMode); // Включаем все отключенные кнопки
}


// Обновление работы интерфейса
void StudentBook::updateInterface(Mode mode) {
// Функция обновляет текущий режим согласно переданного в нее аргумента mode, присваивая его переменной currentMode перед проверкой значения.
    currentMode = mode;

    switch (currentMode) {
        case AddingMode:
        case EditingMode:
            nameLine->setReadOnly(false); //разрешаем менять текст
            nameLine->setFocus(Qt::OtherFocusReason);
            courseLine->setReadOnly(false);
            groupLine->setReadOnly(false);

            addButton->setEnabled(false);
            editButton->hide();
            removeButton->hide();

            nextButton->setEnabled(false);
            previousButton->setEnabled(false);

            submitButton->show();
            cancelButton->show();

            break;

        case NavigationMode:
            if (student.isEmpty()) {
             nameLine->clear();
             courseLine->clear();
             groupLine->clear();
            }

            nameLine->setReadOnly(true);
            courseLine->setReadOnly(true);
            groupLine->setReadOnly(true);
            addButton->setEnabled(true);

            int size = student.size();
            editButton->show();
            removeButton->show();
            editButton->setEnabled(size >= 1);
            removeButton->setEnabled(size >= 1);
            nextButton->setEnabled(size > 1);
            previousButton->setEnabled(size > 1);

            submitButton->hide();
            cancelButton->hide();
            break;
     }
 }


// Следующий
void StudentBook::next() {
    QString name = nameLine->text();
    BookEl::iterator i = student.find(name);

    if (i != student.end()) i++;
    if (i == student.end()) i = student.begin();

    nameLine->setText(i.key());
    courseLine->setText(QString::number(i.value().first));
    groupLine->setText(QString::number(i.value().second));
}

// Предыдущий
void StudentBook::previous() {
    QString name = nameLine->text();
    BookEl::iterator i = student.find(name);

    if (i == student.end()) {
        nameLine->clear();
        courseLine->clear();
        groupLine->clear();
        return;
    }

    if (i == student.begin()) i = student.end();

    i--;
    nameLine->setText(i.key());
    courseLine->setText(QString::number(i.value().first));
    groupLine->setText(QString::number(i.value().second));
}

// Редактирование
void StudentBook::editStudent() {
    oldName = nameLine->text();
    oldInf.first = courseLine->text().toInt();
    oldInf.second = groupLine->text().toInt();

    updateInterface(EditingMode);
}

// Удаление
void StudentBook::removeStudent() {
    QString name = nameLine->text();

    if (student.contains(name)) {
        int button = QMessageBox::question(this,tr("Подтвердите удаление"), tr("Вы уверены, что хотите удалить \"%1\"?").arg(name), QMessageBox::Yes | QMessageBox::No);

        if (button == QMessageBox::Yes) {
            previous();
            student.remove(name);
            QMessageBox::information(this, tr("Удаление прошло успешно"), tr("\"%1\" был удален ").arg(name));
        }
    }

    updateInterface(NavigationMode);
}

// Сохранение
void StudentBook::saveToFile() {
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Student Book"), "",
            tr("Student Book (*.abk);;All Files (*)"));

    if (fileName.isEmpty()) return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Не удается открыть файл"), file.errorString());
            return;
        }

    QDataStream out(&file);
    out << student;
    }
}

// Загрузка из файла
void StudentBook::loadFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Student Book"), "",
            tr("Student Book (*.abk);;All Files (*)"));

        if (fileName.isEmpty()) return;
        else {
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(this, tr("Не удается открыть файл"),file.errorString());
                return;
            }
        QDataStream in(&file);
        student.empty();   // очистка существующих студентов
        in >> student;
        if (student.isEmpty()) QMessageBox::information(this, tr("No contacts in file"), tr("Файл, который вы пытаетесь открыть, не содержит ничего."));
        else {
            BookEl::iterator i = student.begin();
            nameLine->setText(i.key());
            courseLine->setText(QString::number(i.value().first));
            groupLine->setText(QString::number(i.value().second));
        }
    }
    updateInterface(NavigationMode);
}

// Данные в виде таблицы
void StudentBook::tableStudent() {
    table->show();
    table->createT(student);
    this->setWindowTitle("Таблица Студентов");
}

