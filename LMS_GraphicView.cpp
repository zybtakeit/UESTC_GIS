#include "LMS_GraphicView.h"

LMS_GraphicView::LMS_GraphicView(QWidget*parent)
	: QGraphicsView(parent),
	m_translateButton(Qt::LeftButton),
	m_scale(1.0),
	m_zoomDelta(0.1),
	m_tanslateSpeed(1.0),
	m_bMouseTranslate(false),
	ifWheel(true)//初始化对象
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);	//取消滚动条
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setCursor(Qt::PointingHandCursor);						//设置光标
	setRenderHint(QPainter::Antialiasing);
	setAlignment(Qt::AlignLeft | Qt::AlignTop);
	centerOn(0, 0);
	Temparea = new QGraphicsScene(this);
	//TempImageItem = NULL;
	fTimer = new QTimer(this);
	fTimer->stop();
	fTimer->setInterval(1000);
	QObject::connect(fTimer, SIGNAL(timeout()), this, SLOT(on_timer_timeout()));
}

void  LMS_GraphicView::on_timer_timeout()
{
	emit mouseWheel();
	fTimer->stop();
}

void LMS_GraphicView::setTransLateSpeed(qreal speed)			//设置平移速度
{
	Q_ASSERT_X(speed >= 0.0 && speed <= 2.0,
		"LMS_GraphicView::setTransLateSpeed", "Speed should be in range[0.0,2.0]");
	m_tanslateSpeed = speed;
}
qreal LMS_GraphicView::translateSpeed() const				//返回平移速度
{
	return m_tanslateSpeed;
}
void LMS_GraphicView::setZoomDelta(qreal delta)				//设置缩放比例
{
	Q_ASSERT_X(delta >= 0.0 && delta <= 1.0,
		"LMS_GraphicView::setZoomDelta", "Delta should be in range[0.0,1.0]");
	m_zoomDelta = delta;
}
qreal LMS_GraphicView::zoomDelta()const						//返回缩放比例
{
	return m_zoomDelta;
}
void LMS_GraphicView::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == m_translateButton)
	{
		QPoint point_1 = event->pos();
		emit mouseClicked(point_1);
		QPointF point = mapToScene(event->pos());

		/*Temparea->clear();
		QPixmap NewPixmap(this->sceneRect().width(), this->sceneRect().height());
		QPainter painter(&NewPixmap);
		painter.setRenderHint(QPainter::Antialiasing);
		this->render(&painter);
		TempImageItem = new QGraphicsPixmapItem(NewPixmap);
		TempImageItem->setPos(this->sceneRect().x() , this->sceneRect().y());
		TempImageItem->setZValue(0);
		Temparea->addItem(TempImageItem);
		Temparea->update();*/
		this->setScene(Temparea);
		m_bMouseTranslate = true;						//平移标识（真）
		m_lastMousePos = event->pos();
	}
	QGraphicsView::mousePressEvent(event);
}
void LMS_GraphicView::mouseMoveEvent(QMouseEvent* event)
{
	QPoint point = event->pos();
	QPointF scenePos = this->mapToScene(point);
	emit mouseMovePoint(point);
	if (m_bMouseTranslate)
	{

		QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(m_lastMousePos);//移动距离
	   /* translate(mouseDelta);*/													//平移
		this->setSceneRect(this->sceneRect().x() - mouseDelta.x(),
			this->sceneRect().y() - mouseDelta.y(),
			this->sceneRect().width(),
			this->sceneRect().height());
		this->update();
	}
	m_lastMousePos = event->pos();
	QGraphicsView::mouseMoveEvent(event);
}
void LMS_GraphicView::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == m_translateButton)
	{
		m_bMouseTranslate = false;
		emit mouseRelease();
	}
	QGraphicsView::mouseReleaseEvent(event);
}
void LMS_GraphicView::wheelEvent(QWheelEvent* event)
{
	this->setScene(Temparea);
	fTimer->start();
	QPoint prev_viewPos = event->pos();
	QPointF prev_scenePos = this->mapToScene(prev_viewPos);

	QPointF scrollAmount = event->angleDelta();
	scrollAmount.y() > 0 ? zoomIn() : zoomOut();

	this->setSceneRect(this->mapToScene(this->rect()).boundingRect());
	QPointF scenePos = this->mapToScene(prev_viewPos);
	QPointF disPointF = scenePos - prev_scenePos;
	this->setSceneRect(this->sceneRect().x() - disPointF.x(),
		this->sceneRect().y() - disPointF.y(),
		this->sceneRect().width(),
		this->sceneRect().height());
	this->update();
	QGraphicsView::wheelEvent(event);
}


void LMS_GraphicView::zoomIn()		//放大
{
	zoom(1 + m_zoomDelta);
}
void LMS_GraphicView::zoomOut()		//缩小
{
	zoom(1 - m_zoomDelta);
}
void LMS_GraphicView::zoom(float scaleFactor)//缩放函数
{
	qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor>100)
	{
		return;
	}
	scale(scaleFactor, scaleFactor);
	m_scale *= scaleFactor;
}
void LMS_GraphicView::translate(QPointF delta)//平移函数
{
	delta *= m_scale;
	delta *= m_tanslateSpeed;

	this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	QPoint newCenter(this->viewport()->rect().width() / 2 - delta.x(), this->viewport()->rect().height() / 2 - delta.y());
	this->centerOn(mapToScene(newCenter));
	this->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}

qreal LMS_GraphicView::getScale()
{
	return m_scale;
}

void LMS_GraphicView::setScale()
{
}


void LMS_GraphicView::getSceneRect(int& SceneX, int& SceneY, int& SceneWidth, int& SceneHeight)
{
	SceneX = this->sceneRect().x();
	SceneY = this->sceneRect().y();
	SceneWidth = this->sceneRect().width();
	SceneHeight = this->sceneRect().height();
}

void LMS_GraphicView::updateTempScene()
{
	Temparea->clear();
	QPixmap NewPixmap(this->sceneRect().width(), this->sceneRect().height());
	QPainter painter(&NewPixmap);
	painter.setRenderHint(QPainter::Antialiasing);
	this->render(&painter);
	QGraphicsPixmapItem* TempImageItem = new QGraphicsPixmapItem(NewPixmap);
	TempImageItem->setPos(this->sceneRect().x(), this->sceneRect().y());
	TempImageItem->setZValue(0);
	Temparea->addItem(TempImageItem);
	Temparea->update();
}