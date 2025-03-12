#include "iplineedit.h"
#include <QSizePolicy>
#include <QHBoxLayout>
#include <QLabel>
#include <QRegExp>
#include <QRegExpValidator>
#include <QEvent>
#include <QKeyEvent>
IPLIneEdit::IPLIneEdit(QWidget *parent)
    :QLineEdit(parent)
{
    QHBoxLayout *hboxlayout = new QHBoxLayout(this);
    hboxlayout->setContentsMargins(2,2,2,2);

    QRegExp regExp("(25[0-5]|2[0-4][0-9]|1?[0-9]{1,2})");

    QLabel *labelDot[3];
    for(int i = 0;i < IP_INPUT_SIZE;++i){
        m_lineEdit[i] =new QLineEdit(this);

        m_lineEdit[i]->setProperty("ip",true);//????

        m_lineEdit[i]->setFrame(false);

        m_lineEdit[i]->setMaxLength(3);

        m_lineEdit[i]->setAlignment(Qt::AlignCenter);

        m_lineEdit[i]->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

        m_lineEdit[i]->setValidator(new QRegExpValidator(regExp,this));

        m_lineEdit[i]->installEventFilter(this);

        hboxlayout->addWidget(m_lineEdit[i]);

        if(i < 3){
            labelDot[i] = new QLabel(this);
            labelDot[i]->setText(".");
            labelDot[i]->setFixedWidth(3);
            hboxlayout->addWidget(labelDot[i]);
        }
    }
    this->setReadOnly(true);
    m_lineEdit[0]->setFocus();
    m_lineEdit[0]->selectAll();
}
bool IPLIneEdit::eventFilter(QObject *obj, QEvent *event)
{
    if(children().contains(obj) && QEvent::KeyPress == event->type()){

        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent*>(event);
        QLineEdit *pCurrentEdit = qobject_cast<QLineEdit*>(obj);
        switch (keyEvent->key()) {
        case Qt::Key_0:
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:{
            QString strText = pCurrentEdit->text();
            if (strText.length() <= 3 && strText.toInt()*10 > 255) {
                int index = getIndex(pCurrentEdit);
                if(index != -1 && index != 3){
                    m_lineEdit[index + 1]->setFocus();
                    m_lineEdit[index + 1]->selectAll();
                }
            }
            return QLineEdit::eventFilter(obj,event);
        }
        break;
        case Qt::Key_Left:
        {
            if(!pCurrentEdit->cursorPosition()){
                int index = getIndex(pCurrentEdit);
                if(index != -1 && index != 0){
                    m_lineEdit[index - 1]->setFocus();
                    int length = m_lineEdit[index-1]->text().length();
                    m_lineEdit[index-1]->setCursorPosition(length?length:0);
                }
            }
            return QLineEdit::eventFilter(obj,event);
        }
        break;
        case Qt::Key_Right:
        {

            if(pCurrentEdit->cursorPosition() == pCurrentEdit->text().length()){
                int index = getIndex(pCurrentEdit);
                if(index != -1 && index != 3){
                    m_lineEdit[index + 1]->setFocus();
                    m_lineEdit[index+1]->setCursorPosition(0);
                }
            }
            return QLineEdit::eventFilter(obj,event);
        }
        break;
        case Qt::Key_Backspace:
        {

            if(!pCurrentEdit->cursorPosition()){
                int index = getIndex(pCurrentEdit);
                if(index != -1 && index != 0){
                    m_lineEdit[index - 1]->setFocus();
                    int length = m_lineEdit[index-1]->text().length();
                    m_lineEdit[index-1]->setCursorPosition(length?length:0);
                }
            }
            return QLineEdit::eventFilter(obj,event);
        }
        break;
        case Qt::Key_Period://点号"."
        {
                int index = getIndex(pCurrentEdit);
                if(index != -1 && index != 3){
                    m_lineEdit[index + 1]->setFocus();
                    m_lineEdit[index + 1]->selectAll();
                }
            return QLineEdit::eventFilter(obj,event);
        }
        break;
        }
    }
}

QString IPLIneEdit::text()
{
    QString IP ;
    for(int i = 0;i < IP_INPUT_SIZE;++i){
        IP.append(m_lineEdit[i]->text());
        if(i < 3){
            IP.append(".");
        }
    }
    return IP;
}
IPLIneEdit::~IPLIneEdit()
{

}

int IPLIneEdit::getIndex(QLineEdit *pLineEdit)
{
    int index = -1;
    for(int i = 0;i < IP_INPUT_SIZE;++i){
        if(pLineEdit == m_lineEdit[i]){
            index = i;
        }
    }
    return index;
}
