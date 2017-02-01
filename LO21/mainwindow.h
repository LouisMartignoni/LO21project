#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QDebug>
#include <QtGui>
#include <QTableWidget>
#include <QString>
#include <QMediaPlayer>
#include <QKeyEvent>
#include <QEvent>
#include <Qt>
#include <QShortcut>
#include "Controleur.hpp"
#include "ComputerException.h"
#include "Expression.hpp"
#include "Pile.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QLineEdit* message;
    QLineEdit* ligne_de_commande;
    QTableWidget* vuePile;
    bool expression = false;
    bool programme = false;
    QShortcut* shortcut1;
    QShortcut* shortcut2;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool eventFilter(QObject *target, QEvent *event);

private slots:
    void on_button0_clicked();
    void on_button1_clicked();
    void on_button2_clicked();
    void on_button3_clicked();
    void on_button4_clicked();
    void on_button5_clicked();
    void on_button6_clicked();
    void on_button7_clicked();
    void on_button8_clicked();
    void on_button9_clicked();
    void on_buttonPoint_clicked();
    void on_buttonDivision_clicked();
    void on_buttonMultiplication_clicked();
    void on_buttonSoustraction_clicked();
    void on_buttonAddition_clicked();
    void on_buttonNEG_clicked();
    void on_buttonSpace_clicked();
    void on_buttonSuppr_clicked();

    void on_Pile_activated(const QString &arg1);
    void on_Boucle_activated(const QString &arg1);
    void on_Trigo_activated(const QString &arg1);
    void on_Autre_activated(const QString &arg1);
    void on_Complexe_activated(const QString &arg1);
    void on_Logique_activated(const QString &arg1);
    void on_Expression_Programme_Variable_activated(const QString &arg1);

    void on_createExpression_clicked();
    void on_beginProgramme_clicked();
    void on_endProgramme_clicked();

    void on_Undo_clicked();

    void on_Redo_clicked();

private:
    Ui::MainWindow *ui;
    void initPile();
    bool analyse(const QString & str, int option);

public slots:
    void refresh();
    void on_buttonEnter_clicked();

signals:
  void operation();
};

#endif // MAINWINDOW_H
