#include "UESTC_GIS.h"

UESTC_GIS::UESTC_GIS(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	iniUI();
	iniSlots();
}

void UESTC_GIS::iniUI() {
	VLayout = new QVBoxLayout();
	VLayout->setMargin(5); //设置layout的左右边距
	VLayout->setSpacing(5); //设置layout的上下边距
	HLayout = new QHBoxLayout();
	HLayout_Page1 = new QHBoxLayout();
	HLayout_Page2 = new QHBoxLayout();
	HLayout_Page3 = new QHBoxLayout();

	//显示模块
	mapCanvas = new QgsMapCanvas();
	mapCanvas->enableAntiAliasing(true);
	mapCanvas->setCanvasColor(QColor(255, 255, 0));
	mapCanvas->setVisible(true);
	pixel = mapCanvas->getCoordinateTransform();
	//拖动模块
	toolPan = new QgsMapToolPan(mapCanvas);

	//action设置
	action_openImage = new QAction(tr("&打开图像"), this);
	action_coordTrans = new QAction(tr("&坐标转换"), this);
	action_compareIamge = new QAction(tr("&图像比较"), this);
	action_panMode = new QAction(tr("&拖动"), this);
	action_panMode->setCheckable(true); 
	action_addListWidgetItem = new QAction(tr("&添加图像"), this);
	action_deleteListWidgetItem = new QAction(tr("&删除图像"), this);
	action_clearImage = new QAction(tr("&清空图像"), this);
	action_clearListWidget = new QAction(tr("&清空列表"), this);

	//菜单栏设置
	menuBar = new QMenuBar();
	QMenu* menu_1 = new QMenu("图像");
	menu_1->addAction(action_openImage);
	menu_1->addAction(action_addListWidgetItem);
	menu_1->addAction(action_deleteListWidgetItem);
	menu_1->addAction(action_clearImage);
	menu_1->addAction(action_clearListWidget);
	QMenu* menu_2 = new QMenu("其他功能");
	menu_2->addAction(action_coordTrans);
	menu_2->addAction(action_compareIamge);
	menuBar->addMenu(menu_1);
	menuBar->addMenu(menu_2);

	//工具栏第一页Page1
	QWidget* Page1 = new QWidget;
	toolBar_1 = new QToolBar();
	HLayout_Page1->addWidget(toolBar_1);
	toolBar_1->addAction(action_openImage);
	toolBar_1->addAction(action_addListWidgetItem);
	toolBar_1->addAction(action_deleteListWidgetItem);
	toolBar_1->addAction(action_clearImage);
	toolBar_1->addAction(action_clearListWidget);
	Page1->setLayout(HLayout_Page1);
	//工具栏第二页Page2
	QWidget* Page2 = new QWidget;
	toolBar_2 = new QToolBar();
	HLayout_Page2->addWidget(toolBar_2);
	toolBar_2->addAction(action_coordTrans);
	toolBar_2->addAction(action_compareIamge);
	Page2->setLayout(HLayout_Page2);
	//工具栏第三页Page3
	QWidget* Page3 = new QWidget;
	toolBar_3 = new QToolBar();
	HLayout_Page3->addWidget(toolBar_3);
	toolBar_3->addAction(action_panMode);
	Page3->setLayout(HLayout_Page3);
	//工具栏栏设置
	tabWidget = new QTabWidget();
	tabWidget->addTab(Page1, "图像操作");
	tabWidget->addTab(Page2, "其他功能");
	tabWidget->addTab(Page3, "查看图像");
	tabWidget->setFixedHeight(100);

	//底部状态栏
	statusBar = new QStatusBar();
	lineEdit_1 = new QLineEdit("X坐标为:");
	lineEdit_1->setFixedWidth(400);
	lineEdit_2 = new QLineEdit("Y坐标为:");
	lineEdit_2->setFixedWidth(400);
	statusBar->addWidget(lineEdit_1);
	statusBar->addWidget(lineEdit_2);

	//列表组建设置
	listWidget = new QListWidget();
	listWidget->setFixedWidth(200);
	listWidget->clear();

	//布局设置
	HLayout->addWidget(listWidget);
	HLayout->addWidget(mapCanvas);
	VLayout->addWidget(menuBar);
	VLayout->addWidget(tabWidget);
	VLayout->addLayout(HLayout);
	VLayout->addWidget(statusBar);
	UESTC_GIS::setLayout(VLayout);
}

void UESTC_GIS::iniSlots() {
	connect(action_panMode, SIGNAL(toggled(bool)), this, SLOT(panMode(bool)));
	connect(action_openImage, SIGNAL(triggered()), this, SLOT(openImage()));
	connect(action_addListWidgetItem, SIGNAL(triggered()), this, SLOT(addListWidgetItem()));
	connect(action_deleteListWidgetItem, SIGNAL(triggered()), this, SLOT(deleteListWidgetItem()));
	connect(action_clearListWidget, SIGNAL(triggered()), this, SLOT(clearListWidget()));
	connect(action_clearImage, SIGNAL(triggered()), this, SLOT(clearImage()));
	connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(listWidgetItemDoubleClicked(QListWidgetItem*)));
	connect(mapCanvas, SIGNAL(xyCoordinates(const QgsPointXY)), this, SLOT(mouseMove(const QgsPointXY)));
	connect(action_coordTrans, SIGNAL(triggered()), this, SLOT(transCoordination()));
	connect(action_compareIamge, SIGNAL(triggered()), this, SLOT(compareImages()));
}

void UESTC_GIS::transCoordination() {
	CSatCoordTrans sat;
	sat.Set_Parameters_ByGUI();
}

void UESTC_GIS::compareImages() {
	QCompare* window = new QCompare(this);
	window->show();
}

void UESTC_GIS::addListWidgetItem() {
	QString fileUrl = QFileDialog::getOpenFileName(this, "选择一个或多个文件", "", "Images(*.tif)");//多选文件
	if (fileUrl.isEmpty())
		return;
	QFileInfo FileInfo = QFileInfo(fileUrl);
	QString basename = FileInfo.fileName();
	QListWidgetItem* item = new QListWidgetItem();
	item->setText(basename);
	item->setData(1, fileUrl);
	listWidget->addItem(item);
}

void UESTC_GIS::deleteListWidgetItem() {
	int row = listWidget->currentRow();
	QListWidgetItem* item = listWidget->takeItem(row);
	delete item;
}

void UESTC_GIS::clearListWidget() {
	listWidget->clear();
}

void UESTC_GIS::clearImage() {
	qDeleteAll(mapCanvasLayerSet);
	mapCanvasLayerSet.clear();
	mapCanvas->refresh();
}

void UESTC_GIS::listWidgetItemDoubleClicked(QListWidgetItem* item) {
	QString fileUrl = (item->data(1)).toString();
	QString basename = item->text();
	changeRasterLayer(fileUrl, basename);
}

void UESTC_GIS::openImage() {
	QString fileUrl = QFileDialog::getOpenFileName(this, "选择一个或多个文件", "", "Images(*.tif)");//多选文件
	if (fileUrl.isEmpty())
		return;
	QFileInfo FileInfo = QFileInfo(fileUrl);
	QString basename = FileInfo.fileName();
	QListWidgetItem* item = new QListWidgetItem();
	item->setText(basename);
	item->setData(1, fileUrl);
	listWidget->addItem(item);
	changeRasterLayer(fileUrl, basename);
}

void UESTC_GIS::changeRasterLayer(QString url, QString basename) {
	QgsRasterLayer* rasterLayer = new QgsRasterLayer(url, basename, "gdal", QgsRasterLayer::LayerOptions());
	if (!rasterLayer->isValid())
	{
		QMessageBox::critical(this, "error", "layer is invalid");
		return;
	}
	if (mapCanvasLayerSet.length() == 0) {
		mapCanvasLayerSet.append(rasterLayer);
	}
	else {
		qDeleteAll(mapCanvasLayerSet); 
		mapCanvasLayerSet.clear(); 
		mapCanvasLayerSet.append(rasterLayer);
	}
	QgsRectangle rec = rasterLayer->extent();
	mapCanvas->setExtent(rasterLayer->extent());
	mapCanvas->setLayers(mapCanvasLayerSet);
	mapCanvas->setVisible(true);
	mapCanvas->freeze(false);
	mapCanvas->setVisible(true);
	mapCanvas->freeze(false); 
	mapCanvas->refresh();
}

void UESTC_GIS::mouseMove(const QgsPointXY& p) {
	QgsPointXY a = pixel->toMapCoordinates(p.x(),p.y());
	lineEdit_1->setText("X坐标为:" + QString::number(a.x(),'d',10));
	lineEdit_2->setText("Y坐标为:" + QString::number(a.y(), 'd', 10));
	//lineEdit_1->setText("X坐标为:" + QString::number(p.x()));
	//lineEdit_2->setText("Y坐标为:" + QString::number(p.y()));
}

void UESTC_GIS::panMode(bool checked) {
	if (checked) {
		mapCanvas->setMapTool(toolPan);
	}
	else {
		mapCanvas->unsetMapTool(toolPan);
	}
}
