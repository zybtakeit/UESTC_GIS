#pragma once

#include <qgraphicsview.h>
#include<qevent.h>
#include<qrect.h>
#include<qscrollbar.h>
#include <qtimer.h>
#include<qgraphicsitem.h>
class LMS_GraphicView : public QGraphicsView
{
	Q_OBJECT

public:
	explicit LMS_GraphicView(QWidget* parent = nullptr);
	/*LMS_GraphicView(QWidget*parent);
	~LMS_GraphicView();*/

	void setTransLateSpeed(qreal speeed);	//平移速度
	qreal translateSpeed() const;

	void setZoomDelta(qreal delta);			//缩放的增量
	qreal zoomDelta() const;

	void setScale();
	qreal getScale();

	void getSceneRect(int& SceneX, int& SceneY, int& SceneWidth, int& SceneHeight);
	void updateTempScene();

	QGraphicsScene* Temparea;
	//QGraphicsPixmapItem* TempImageItem;

protected:
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;	//鼠标点击事件
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;	//鼠标移动事件
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;//鼠标释放事件
	void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;		//鼠标滚轮事件
public slots:
	void zoomIn();					//放大
	void zoomOut();					//缩小
	void zoom(float scaleFactor);	//缩放
	void translate(QPointF delta);	//平移
	void on_timer_timeout();
private:
	Qt::MouseButton m_translateButton;	//平移按钮
	qreal m_tanslateSpeed;				//平移速度
	qreal m_zoomDelta;					//缩放的增量
	bool m_bMouseTranslate;				//平移标识
	QPoint m_lastMousePos;				//鼠标最后按下的位置
	qreal m_scale;						//缩放值
	bool ifWheel;
	QTimer* fTimer;

signals:
	void mouseMovePoint(QPoint point);
	void mouseClicked(QPoint point);
	void mouseRelease();
	void mouseWheel();


};