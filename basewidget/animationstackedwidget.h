#ifndef ANIMATIONSTACKEDWIDGET_H
#define ANIMATIONSTACKEDWIDGET_H

#include <QStackedWidget>

class AnimationStackedWidget : public QStackedWidget
{
    Q_OBJECT
    Q_PROPERTY(float iRotateVal READ rotateVal WRITE setRotateVal)
public:
    AnimationStackedWidget(QWidget *parent = 0);

    ~AnimationStackedWidget();

    void animation(int pageIndex);

    float rotateVal();
    void setRotateVal(float val);
    int startVal();
    void setStartVal(int val);
    int endVal();
    void setEndVal(int val);
private:
    float iRotateVal;
    int m_startVal;
    int m_endVal;
    int m_nextPageIndex;
private slots:
    void onValueChange();
    void onFinished();
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // ANIMATIONSTACKEDWIDGET_H
