#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>


#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
#include <QWheelEvent>
#include <QMouseEvent>
#include <vector>
#include <fstream>
#include <sstream>
#include <QPainterPath>

typedef struct axis
{
    double x;
    double y;
} axis;

using namespace  std;
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class NCshow;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    NCshow *m_NC;
    vector<axis> axisvec;

};


class NCshow :public QGraphicsView
{
     Q_OBJECT
public:
    NCshow(QWidget *parent = NULL);
    void setNC(vector<axis> a)
    {
        axisvec = a;
        path.clear();
        min[0] = a[0].x;
        min[1] = a[0].y;
        max[0] = a[0].x;
        max[1] = a[0].y;
        for (unsigned long long i =0;i<a.size();i++)
        {
            min[0]>a[i].x?min[0] = a[i].x:min[0];
            min[1]>a[i].y?min[1] = a[i].y:min[1];
            max[0]<a[i].x?max[0] = a[i].x:max[0];
            max[1]<a[i].y?max[1] = a[i].y:max[1];
            path.lineTo(a[i].x,a[i].y);
        }
        updataNC();
    }
    void updataNC();
private:
    QGraphicsScene *m_psence;
    QGraphicsPathItem  *m_pline;
    QPainterPath path;

    QPointF m_poinut;
    QPoint m_poinut2;
    QPoint _mouseLBtnDown;

    vector<axis> axisvec;
    double zoom;

    double max[2];
    double min[2];
protected:
    void wheelEvent(QWheelEvent *event)override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};


#endif // WIDGET_H
