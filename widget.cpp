#include "widget.h"
#include "ui_widget.h"
#include <QVBoxLayout>
#include <QDebug>

#include <QFileDialog>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{

    QStringList s;
    ui->setupUi(this);
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_NC = new NCshow(this);
    layout->addWidget(m_NC);
    layout->addWidget(ui->pushButton);
    vector<axis> aa;
}

Widget::~Widget()
{
    delete ui;
}


NCshow::NCshow(QWidget *parent)
    :QGraphicsView(parent)
{
    m_psence = new QGraphicsScene();
    this->setScene(m_psence);
    m_psence->setBackgroundBrush(QBrush(QColor("#A9D0F5")));

    QPen pen;   // 定义一个画笔，设置画笔颜色和宽度
    pen.setColor(QColor(255, 0, 0));
    pen.setWidthF(0.1);

    m_pline = new QGraphicsPathItem();
    m_pline->setPen(pen);

    m_psence->addItem(m_pline);
    this->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    m_poinut.setX(0);
    m_poinut.setY(0);

    //隐藏水平/竖直滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //设置场景范围
    setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);
    updataNC();

    zoom = 1;
}

void NCshow::updataNC()
{
    m_pline->setPath(path);

    double h1 = max[0]-min[0];
    double h2 = max[1]-min[1];
    if(h1>h2)
    {
        zoom = width()/h1;
    }
    else
    {
        zoom = height()/h2;
    }

    m_pline->setFlag(QGraphicsItem::ItemIsMovable);
    QMatrix q;
    q.setMatrix(zoom,this->matrix().m12(),this->matrix().m21(),zoom,this->matrix().dx(),this->matrix().dy());
    this->setMatrix(q,false);
    centerOn(0,0);
    m_poinut.setX(0);
    m_poinut.setY(0);
}

void NCshow::wheelEvent(QWheelEvent *event)
{
    if(event->delta()>0)
    {
        this->scale(0.9,0.9);

    }
    else
    {
        this->scale(1.1,1.1);

    }
}

void NCshow::mousePressEvent(QMouseEvent *event)
{
    _mouseLBtnDown = event->pos();//mapToScene(event->pos().x(),event->pos().y())-m_poinut;

    QWidget::mousePressEvent(event);
}

void NCshow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {

        QPointF ptNow = mapToScene(event->pos());
        QPointF movePt = ptNow - mapToScene(_mouseLBtnDown)+m_poinut;

        //qDebug()<<"当前点："<<ptNow.x()<< " "<<ptNow.y();
        //qDebug()<<movePt.x()<<"  "<<movePt.y();

        //根据鼠标当前的点作为定位点
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        QPoint nowCenter(-movePt.x(),  -movePt.y());
        centerOn((nowCenter));
        setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    }
    QWidget::mouseMoveEvent(event);
}

void NCshow::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF ptNow = mapToScene(event->pos());
    QPointF movePt = ptNow - mapToScene(_mouseLBtnDown)+m_poinut;

    m_poinut = movePt;
    //qDebug()<<"sssss:"<<movePt.x()<<"  "<<movePt.y();
    QWidget::mouseReleaseEvent(event);
}

void NCshow::mouseDoubleClickEvent(QMouseEvent *event)
{

    QMatrix q;
    q.setMatrix(zoom,this->matrix().m12(),this->matrix().m21(),zoom,this->matrix().dx(),this->matrix().dy());
    this->setMatrix(q,false);
    centerOn(0,0);
    m_poinut.setX(0);
    m_poinut.setY(0);
    QWidget::mouseDoubleClickEvent(event);
}

void Widget::on_pushButton_clicked()
{
    axisvec.clear();
    QString str =QFileDialog::getOpenFileName(NULL,"open","./","tr(*.nc *.NC)");

    ifstream infile;
    infile.open(str.toStdString().c_str(),ios::in);

    char buffer[256];
    double doux,douy;
    char o;
    if(infile.is_open())
    {
        qDebug()<<"打开成功";
        while (!infile.eof())
        {
            infile.getline(buffer,256);
            if(buffer[0] == 'G'&&buffer[1] == '0' &&buffer[2]=='1'&&buffer[3] == 'X')
            {
                stringstream sstr(buffer);
                sstr>>o>>o>>o>>o>>doux>>o>>douy;
                axis as;
                as.x = doux;
                as.y = douy;

                axisvec.push_back(as);
            }
            if(buffer[0] == 'X')
            {
                stringstream sstr(buffer);
                sstr>>o>>doux>>o>>douy;
                axis as;
                as.x = doux;
                as.y = douy;
                axisvec.push_back(as);
            }
        }
        m_NC->setNC(axisvec);
    }
}
