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

	void setTransLateSpeed(qreal speeed);	//ƽ���ٶ�
	qreal translateSpeed() const;

	void setZoomDelta(qreal delta);			//���ŵ�����
	qreal zoomDelta() const;

	void setScale();
	qreal getScale();

	void getSceneRect(int& SceneX, int& SceneY, int& SceneWidth, int& SceneHeight);
	void updateTempScene();

	QGraphicsScene* Temparea;
	//QGraphicsPixmapItem* TempImageItem;

protected:
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;	//������¼�
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;	//����ƶ��¼�
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;//����ͷ��¼�
	void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;		//�������¼�
public slots:
	void zoomIn();					//�Ŵ�
	void zoomOut();					//��С
	void zoom(float scaleFactor);	//����
	void translate(QPointF delta);	//ƽ��
	void on_timer_timeout();
private:
	Qt::MouseButton m_translateButton;	//ƽ�ư�ť
	qreal m_tanslateSpeed;				//ƽ���ٶ�
	qreal m_zoomDelta;					//���ŵ�����
	bool m_bMouseTranslate;				//ƽ�Ʊ�ʶ
	QPoint m_lastMousePos;				//�������µ�λ��
	qreal m_scale;						//����ֵ
	bool ifWheel;
	QTimer* fTimer;

signals:
	void mouseMovePoint(QPoint point);
	void mouseClicked(QPoint point);
	void mouseRelease();
	void mouseWheel();


};