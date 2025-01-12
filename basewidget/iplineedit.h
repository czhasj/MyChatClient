#ifndef IPLINEEDIT_H
#define IPLINEEDIT_H

#include <QLineEdit>
#include <QWidget>
#define IP_INPUT_SIZE 4
class IPLIneEdit : public QLineEdit
{
public:
    IPLIneEdit(QWidget *parent = 0);
    ~IPLIneEdit();
protected:

private:
    QLineEdit *m_lineEdit[IP_INPUT_SIZE];
    int getIndex(QLineEdit *pLineEdit);
    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    QString text();
};



#endif // IPLINEEDIT_H
