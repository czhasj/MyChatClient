#include "animationstackedwidget.h"
#include <QPropertyAnimation>
#include <QPainter>
#include <QTransform>
AnimationStackedWidget::AnimationStackedWidget(QWidget *parent)
    :QStackedWidget(parent)
{
    m_nextPageIndex = 0;
}
AnimationStackedWidget::~AnimationStackedWidget(){

}

void AnimationStackedWidget::animation(int pageIndex)
{
    m_nextPageIndex = pageIndex;

    int offsetX = frameRect().width();
    int offsetY = frameRect().height();
    widget(pageIndex)->setGeometry(0,0,offsetX,offsetY);

    QPropertyAnimation *animation = new QPropertyAnimation(this,"iRotateVal");
    animation->setDuration(700);
    animation->setEasingCurve(QEasingCurve::Linear);
    animation->setStartValue(m_startVal);
    animation->setEndValue(m_endVal);

    connect(animation,&QPropertyAnimation::valueChanged,this,&AnimationStackedWidget::onValueChange);
    connect(animation,&QPropertyAnimation::finished,this,&AnimationStackedWidget::onFinished);

    currentWidget()->hide();

    animation->start();
}



float AnimationStackedWidget::rotateVal()
{
    return iRotateVal;
}

void AnimationStackedWidget::setRotateVal(float val)
{
    iRotateVal = val;
}

int AnimationStackedWidget::startVal()
{
    return m_startVal;
}

void AnimationStackedWidget::setStartVal(int val)
{
    m_startVal = val;
}

int AnimationStackedWidget::endVal()
{
    return m_endVal;
}

void AnimationStackedWidget::setEndVal(int val)
{
    m_endVal = val;
}

void AnimationStackedWidget::onValueChange()
{
    repaint();
}

void AnimationStackedWidget::onFinished()
{
    widget(m_nextPageIndex)->show();
    widget(m_nextPageIndex)->raise();

    setCurrentWidget(widget(m_nextPageIndex));
    repaint();
}

void AnimationStackedWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
        //旋转
    if(iRotateVal > 0 && iRotateVal <= 90){
        QPixmap pix(currentWidget()->size());
        currentWidget()->render(&pix);
        QTransform transfrom;
        transfrom.translate(width()/2,0);
        transfrom.rotate(iRotateVal,Qt::YAxis);
        p.setTransform(transfrom);
        p.drawPixmap(-1*width()/2,0,pix);
    }else if(iRotateVal > 90 && iRotateVal <= 180){
        QPixmap pix(widget(m_nextPageIndex)->size());
        widget(m_nextPageIndex)->render(&pix);
        QTransform transfrom;
        transfrom.translate(width()/2,0);
        transfrom.rotate(iRotateVal+180,Qt::YAxis);
        p.setTransform(transfrom);
        p.drawPixmap(-1*width()/2,0,pix);
    }else if(iRotateVal > -90 && iRotateVal <= 0){
        QPixmap pix(currentWidget()->size());
        currentWidget()->render(&pix);
        QTransform transfrom;
        transfrom.translate(width()/2,0);
        transfrom.rotate(iRotateVal,Qt::YAxis);
        p.setTransform(transfrom);
        p.drawPixmap(-1*width()/2,0,pix);
    }else if(iRotateVal >= -180 && iRotateVal <= -90){
        QPixmap pix(widget(m_nextPageIndex)->size());
        widget(m_nextPageIndex)->render(&pix);
        QTransform transfrom;
        transfrom.translate(width()/2,0);
        transfrom.rotate(iRotateVal-180,Qt::YAxis);
        p.setTransform(transfrom);
        p.drawPixmap(-1*width()/2,0,pix);
    }else{
        QWidget::paintEvent(event);
    }
}
