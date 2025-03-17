#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QMutex>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QGraphicsWidget>

#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QLineEdit;
class QHBoxLayout;
class QVBoxLayout;
QT_END_NAMESPACE

class CustomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomWidget(QWidget *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);
};

/////////////////////////////////////////////////////////
/// \brief The CustomMoveWidget class
/// 可移动窗口
class CustomMoveWidget : public CustomWidget {
    Q_OBJECT
public:
    explicit CustomMoveWidget(QWidget *parent = 0);
    ~CustomMoveWidget();

protected:
    QPoint mousePoint;
    bool m_mousePressed;

    QGraphicsScene * scene;
    QGraphicsView  * view;
    QWidget *contentWidget;
    QSize originalSize;      // 原始尺寸
    QGraphicsProxyWidget *proxy;

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
// public:
//     explicit CustomMoveWidget(QWidget *parent = nullptr)
//         : CustomWidget(parent),
//         graphicsScene(new QGraphicsScene(this)),
//         graphicsView(new QGraphicsView(graphicsScene, this)),
//         graphicsWidget(new QGraphicsWidget()) {

//         // 设置布局
//         QVBoxLayout *layout = new QVBoxLayout(this);
//         layout->addWidget(graphicsView);
//         setLayout(layout);

//         // 添加 QGraphicsWidget 到场景
//         graphicsScene->addItem(graphicsWidget);

//         // 设置初始参数
//         isMoving = false;
//         scaleFactor = 1.0;
//         setMouseTracking(true);

//         // 设置背景色
//         graphicsWidget->setStyleSheet("background-color: lightblue;");
//         graphicsWidget->setGeometry(0, 0, 200, 200);
//     }

// protected:
//     void mousePressEvent(QMouseEvent *event) override {
//         if (graphicsWidget->geometry().contains(event->pos())) {
//             isMoving = true;
//             lastMousePos = event->pos();
//         }
//     }

//     void mouseMoveEvent(QMouseEvent *event) override {
//         if (isMoving) {
//             QPointF delta = event->pos() - lastMousePos;
//             lastMousePos = event->pos();
//             moveBy(delta.x(), delta.y());
//         }
//     }

//     void mouseReleaseEvent(QMouseEvent *event) override {
//         Q_UNUSED(event);
//         isMoving = false;
//     }

//     void wheelEvent(QWheelEvent *event) override {
//         // 缩放
//         if (event->angleDelta().y() > 0) {
//             scaleBy(1.1);  // 放大
//         } else {
//             scaleBy(0.9);  // 缩小
//         }
//     }

// private:
//     QGraphicsScene *graphicsScene;
//     QGraphicsView *graphicsView;
//     QGraphicsWidget *graphicsWidget;
//     bool isMoving;
//     QPointF lastMousePos;
//     qreal scaleFactor;

//     void moveBy(qreal dx, qreal dy) {
//         // 移动 QGraphicsWidget
//         QRectF rect = graphicsWidget->geometry();
//         rect.translate(dx, dy);
//         graphicsWidget->setGeometry(rect);
//     }

//     void scaleBy(qreal factor) {
//         // 等比例缩放 QGraphicsWidget
//         scaleFactor *= factor;

//         QTransform transform;
//         transform.scale(scaleFactor, scaleFactor);
//         graphicsWidget->setTransform(transform);
//     }
};



///////////////////////////////////////////////////////////////////////
/// \brief The CustomDialog class
///
class CustomDialog : public QDialog {
    Q_OBJECT
public:
    explicit CustomDialog(QWidget *parent = 0);
    ~CustomDialog();

protected:
    QPoint mousePoint;
    bool m_mousePressed;
    QSize m_nNormalSize;

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
};

///////////////////////////////////////////////////////////////////////////
/// \brief The CBaseDialog class
///
class CBaseDialog : public CustomDialog {
    Q_OBJECT
public:
    explicit CBaseDialog(QWidget *parent = 0);
    ~CBaseDialog();

    void SetWinIcon(QPixmap pixmap);
    void SetWinTitle(const QString &text);

private:
    QWidget         *widgetWinTitle;
    QLabel          *labelWinIcon;
    QLabel          *labelWinTitle;
    QPushButton     *btnWinMin;
    QPushButton     *btnWinClose;

protected:
    QWidget         *widgetBody;
};

///////////////////////////////////////////////////////////////////////////
/// \brief The CMessageBox class
/// 弹出信息框类
class CMessageBox : public CBaseDialog
{
    Q_OBJECT


public:
    typedef enum {
        E_Information =  0x01,
        E_Warning,
        E_Question,
        E_MSGTYPE_Error,
    } E_MSGTYPE;

public:
    explicit CMessageBox(QWidget *parent = 0);
    ~CMessageBox();

    // 显示消息
    void ShowMessage(const QString &content, const quint8 &msgType = CMessageBox::E_Information, const QString &title = "");

    void StartTimer();

    static int Infomation(QWidget *parent, const QString &content, const QString &title = "提示");
    static int Question(QWidget *parent, const QString &content, const QString &title = "询问");
    static int Warning(QWidget *parent, const QString &content, const QString &title = "告警");
protected:

private:
    QLabel      *labelIcon;
    QLabel      *labelMsgContent;

    QPushButton *btnOk;
    QPushButton *btnCancel;

    QTimer      *m_timer;
    int         m_nTimerCnt;
public slots:
    void SltTimerOut();
};


// -------------------弹出输入框类 --------------------------//
class CInputDialog : public CBaseDialog
{
    Q_OBJECT

public:
    explicit CInputDialog(QWidget *parent = 0);
    ~CInputDialog();

    static QString GetInputText(QWidget *parent,
                                const QString &text = "",
                                const QString &title = "输入",
                                QLineEdit::EchoMode mode = QLineEdit::Normal);

    QString GetText() const;
    void SetInputText(const QString &text);

    void SetEchoMode(QLineEdit::EchoMode mode);
private:
    static CInputDialog *self;

    QLabel    *labelText;
    QLineEdit *lineEditInput;

    QPushButton *btnOk;
    QPushButton *btnCancel;

};

#endif // CUSTOMWIDGET_H
