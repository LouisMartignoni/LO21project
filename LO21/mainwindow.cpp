#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("UTComputer");

    Pile& pile = Pile::getInstance();

    message = ui->lineEdit_2;
    message->setReadOnly(true);

    ligne_de_commande = ui->lineEdit;
    ligne_de_commande->setFocus(Qt::OtherFocusReason);
    ligne_de_commande->installEventFilter(this);

    vuePile = ui->tableWidget;
    initPile();

    shortcut1 = new QShortcut(QKeySequence("Ctrl+Z"),this);
    shortcut2 = new QShortcut(QKeySequence("Ctrl+Y"),this);

    //connect(shortcut1,SIGNAL(activated()),this,SLOT(on_Undo_clicked()));
    //connect(shortcut2,SIGNAL(activated()),this,SLOT(on_Redo_clicked()));

    connect(&pile, SIGNAL(modificationEtat()), this, SLOT(refresh()));
    connect(this, SIGNAL(operation()), this, SLOT(on_buttonEnter_clicked()));
}

void MainWindow::initPile(){
    Pile& pile = Pile::getInstance();
    pile.setMaxAffiche(5);
    vuePile->setRowCount(pile.getMaxAffiche());
    vuePile->setColumnCount(1);

    QStringList nombres;
    for(unsigned int i=pile.getMaxAffiche(); i>0; i--){
        QString str = QString::number(i);
        str+=" : ";
        nombres << str;
        vuePile->setItem(i - 1, 0, new QTableWidgetItem(""));
    }
    vuePile->setVerticalHeaderLabels(nombres);
    vuePile->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

bool MainWindow::eventFilter(QObject *target, QEvent *event){
    if (target == ligne_de_commande){
        if (event->type() == QEvent::KeyRelease) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if(keyEvent->key() == Qt::Key_Apostrophe)
                expression = analyse(ligne_de_commande->text(),1);
            else if (keyEvent->key() == Qt::Key_BracketLeft || keyEvent->key() == Qt::Key_BracketRight)
                programme = analyse(ligne_de_commande->text(),0);
            else if (keyEvent->key() == Qt::Key_Plus || keyEvent->key() == Qt::Key_Minus ||
                    keyEvent->key() == Qt::Key_Dollar || keyEvent->key() == Qt::Key_Slash ||
                    keyEvent->key() == Qt::Key_Asterisk || keyEvent->key() == Qt::Key_division ||
                    keyEvent->key() == Qt::Key_Less || keyEvent->key() == Qt::Key_multiply) {
                if(!expression && !programme)
                    emit operation();
            }
            else if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
                emit operation();
            else
                keyEvent->ignore();
            return false;
        }

        else if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if(keyEvent->key() == Qt::Key_Left)
                return true;
            else
                keyEvent->ignore();
            return false;
        }
        else{
            event->ignore();
            return false;
        }
    }
    return QMainWindow::eventFilter(target, event);
}


void MainWindow::refresh(){
    Pile& pile = Pile::getInstance();
    message->setText(pile.getMessage());

    for(unsigned int i=0; i<pile.getMaxAffiche(); i++) {
        vuePile->setItem(i, 0, new QTableWidgetItem(""));
    }
    unsigned int nb =0;
    for(deque<Litterale*>::const_iterator it = pile.getIteratorBegin(); it!=pile.getIteratorEnd(); nb++, ++it)
        if(pile.getSize()<=pile.getMaxAffiche())
            vuePile->setItem(pile.getMaxAffiche()-nb-1, 0, new QTableWidgetItem(QString::fromStdString((*it)->toString())));
        else
            vuePile->setItem(pile.getSize()-nb-1, 0, new QTableWidgetItem(QString::fromStdString((*it)->toString())));
}


void MainWindow::on_buttonEnter_clicked() {
    Controleur& controleur = Controleur::getInstance();
    Pile& pile = Pile::getInstance();
    pile.setMessage("");
    QString c = ligne_de_commande->text();
    try {
        controleur.traitementRPN(c.toStdString());        
        ligne_de_commande->clear();
    } catch(ComputerException c) {
        pile.setMessage(c.getInfo());
        ligne_de_commande->setText(c.getRemaining());
    }
    emit pile.modificationEtat();
}


MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::on_button0_clicked(){ligne_de_commande->setText(ligne_de_commande->text()+"0");}
void MainWindow::on_button1_clicked(){ligne_de_commande->setText(ligne_de_commande->text()+"1");}
void MainWindow::on_button2_clicked(){ligne_de_commande->setText(ligne_de_commande->text()+"2");}
void MainWindow::on_button3_clicked(){ligne_de_commande->setText(ligne_de_commande->text()+"3");}
void MainWindow::on_button4_clicked(){ligne_de_commande->setText(ligne_de_commande->text()+"4");}
void MainWindow::on_button5_clicked(){ligne_de_commande->setText(ligne_de_commande->text()+"5");}
void MainWindow::on_button6_clicked(){ligne_de_commande->setText(ligne_de_commande->text()+"6");}
void MainWindow::on_button7_clicked(){ligne_de_commande->setText(ligne_de_commande->text()+"7");}
void MainWindow::on_button8_clicked(){ligne_de_commande->setText(ligne_de_commande->text()+"8");}
void MainWindow::on_button9_clicked(){ligne_de_commande->setText(ligne_de_commande->text()+"9");}
void MainWindow::on_buttonPoint_clicked(){ligne_de_commande->setText(ligne_de_commande->text()+".");}
void MainWindow::on_buttonSpace_clicked(){ligne_de_commande->setText(ligne_de_commande->text()+" ");}
void MainWindow::on_createExpression_clicked(){
    ligne_de_commande->setText(ligne_de_commande->text()+"'");
    expression = analyse(ligne_de_commande->text(),1);
}
void MainWindow::on_beginProgramme_clicked(){
    ligne_de_commande->setText(ligne_de_commande->text()+"[");
    programme = analyse(ligne_de_commande->text(),0);
}
void MainWindow::on_endProgramme_clicked(){
    ligne_de_commande->setText(ligne_de_commande->text()+"]");
    programme = analyse(ligne_de_commande->text(),0);
}

bool MainWindow::analyse(const QString& str, int option){
    if (option){
        message->setText(QString::number(str.count(QRegExp("'"))%2));
        return (str.count(QRegExp("'"))%2)==1;
    }
    else{
        message->setText(QString::number(str.count('[')-str.count(']')));
        return (str.count('[')-str.count(']'))!=0;
    }
}

void MainWindow::on_buttonSuppr_clicked(){ligne_de_commande->backspace();}

void MainWindow::on_buttonDivision_clicked(){
    ligne_de_commande->setText(ligne_de_commande->text()+"/");
    if(!expression && !programme)
        emit operation();
}
void MainWindow::on_buttonMultiplication_clicked(){
    ligne_de_commande->setText(ligne_de_commande->text()+"*");
    if(!expression && !programme)
        emit operation();
}
void MainWindow::on_buttonNEG_clicked(){
    ligne_de_commande->setText(ligne_de_commande->text()+"NEG");
    if(!expression && !programme)
        emit operation();
}
void MainWindow::on_buttonSoustraction_clicked(){
    ligne_de_commande->setText(ligne_de_commande->text()+"-");
    if(!expression && !programme)
        emit operation();
}
void MainWindow::on_buttonAddition_clicked(){
    ligne_de_commande->setText(ligne_de_commande->text()+"+");
    if(!expression && !programme)
        emit operation();
}
void MainWindow::on_Pile_activated(const QString &arg){
    ligne_de_commande->setText(ligne_de_commande->text()+arg);
    if(!expression && !programme)
        emit operation();
}
void MainWindow::on_Boucle_activated(const QString &arg){
    ligne_de_commande->setText(ligne_de_commande->text()+arg);
    if(!expression && !programme)
        emit operation();
}
void MainWindow::on_Trigo_activated(const QString &arg){
    ligne_de_commande->setText(ligne_de_commande->text()+arg);
    if(!expression && !programme)
        emit operation();
}
void MainWindow::on_Autre_activated(const QString &arg){
    ligne_de_commande->setText(ligne_de_commande->text()+arg);
    if(!expression && !programme)
        emit operation();
}
void MainWindow::on_Complexe_activated(const QString &arg){
    ligne_de_commande->setText(ligne_de_commande->text()+arg);
    if(!expression && !programme)
        emit operation();
}
void MainWindow::on_Logique_activated(const QString &arg){
    ligne_de_commande->setText(ligne_de_commande->text()+arg);
    if(!expression && !programme)
        emit operation();
}
void MainWindow::on_Expression_Programme_Variable_activated(const QString &arg){
    ligne_de_commande->setText(ligne_de_commande->text()+arg);
    if(!expression && !programme)
        emit operation();
}

void MainWindow::on_Undo_clicked(){
    ligne_de_commande->setText(ligne_de_commande->text()+"UNDO");
    if(!expression && !programme)
        emit operation();
}

void MainWindow::on_Redo_clicked(){
    ligne_de_commande->setText(ligne_de_commande->text()+"REDO");
    if(!expression && !programme)
        emit operation();
}
