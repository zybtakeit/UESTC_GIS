#include "QCompare.h"

QCompare::QCompare(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	iniUI();
	iniSignalSlots();

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

QCompare::~QCompare()
{
}

void QCompare::iniUI() {
	QWidget* w = new QWidget();
	this->setCentralWidget(w);

	action_addListWidgetItem = new QAction(tr("&添加图像"), this);

	//菜单栏设置
	menuBar = new QMenuBar();
	QMenu* menu_1 = new QMenu("图像");
	menu_1->addAction(action_addListWidgetItem);
	QMenu* menu_2 = new QMenu("其他功能");
	menuBar->addMenu(menu_1);

	VLayout1 = new QVBoxLayout();
	VLayout2 = new QVBoxLayout();
	VLayout2_1 = new QVBoxLayout();
	HLayout2_1 = new QHBoxLayout();
	HLayout = new QHBoxLayout(this);
	//HLayout2_1->setSizeConstraint(QLayout::SetMinimumSize);
	//VLayout1->setSizeConstraint(QLayout::SetFixedSize);
	button = new QPushButton();
	//侧边栏
	dockWidget = new QDockWidget();
	//树状TreeWidget
	treeWidget = new QTreeWidget();
	iniTree();
	//
	groupBox = new QGroupBox;
	//滑块
	spinBox = new QSpinBox;
	spinBox->setMaximum(100);
	spinBox->setMinimum(1);
	spinBox->setSingleStep(1);
	//spinBox->setFixedHeight(20);
	//滑动条
	slider = new QSlider;
	slider->setOrientation(Qt::Horizontal);
	slider->setMaximum(100);
	slider->setMinimum(1);
	slider->setSingleStep(1);
	//;
	label = new QLabel;
	HLayout2_1->addWidget(spinBox);
	HLayout2_1->addWidget(slider);
	groupBox->setLayout(HLayout2_1);
	groupBox->setFixedHeight(60);

	mapCanvas = new QgsMapCanvas();
	mapCanvas->enableAntiAliasing(true);
	mapCanvas->setCanvasColor(QColor(255, 255, 0));
	mapCanvas->setVisible(true);

	VLayout2->addWidget(groupBox);
	VLayout2->addWidget(mapCanvas);
	VLayout1->addWidget(treeWidget);
	HLayout->addLayout(VLayout1);
	HLayout->addLayout(VLayout2);
	w->setLayout(HLayout);
}

void QCompare::iniTree() {
	QString dataStr = "";
	treeWidget->clear();
	treeWidget->setColumnCount(2);
	treeWidget->setFixedWidth(300);
	QStringList labelList;
	labelList.append("图片");
	labelList.append("图片位置");
	treeWidget->setHeaderLabels(labelList);
}

void QCompare::iniSignalSlots() {
	//连接信号槽
	connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
	connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
}
//添加图像
void QCompare::on_action_addImage_triggered() {
	QString fileUrl = QFileDialog::getOpenFileName(this, "选择一个或多个文件", "", "Images(*.tif)");//多选文件
	if (fileUrl.isEmpty())
		return;
	QFileInfo FileInfo = QFileInfo(fileUrl);
	QString filename = FileInfo.fileName();
	QTreeWidgetItem* item = new QTreeWidgetItem(QCompare::itTopitem);
	item->setText(QCompare::colItem, filename);
	item->setText(QCompare::colItemType, fileUrl);
	item->setCheckState(colItem,Qt::Unchecked);
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
	treeWidget->addTopLevelItem(item);
}
//图像对比
void QCompare::on_action_compareImage_triggered() {
	if (treeWidget->topLevelItemCount() == 0)
		return;
	int checkNumber = 0;
	QVector<int> itemChecked; //存储选择的图像的序号
	QTreeWidgetItem* item;
	for (int i = 0; i < treeWidget->topLevelItemCount(); i++) {
		item = treeWidget->topLevelItem(i);
		if (item->checkState(0) == Qt::Checked)
			itemChecked.append(i);
			checkNumber++;
	}
	//如果选择了两张，就开始对比
	if (itemChecked.size() == 2) {  
		const char* Url_1 = (treeWidget->topLevelItem(itemChecked.at(0))->text(1)).toLatin1().data();
		const char* Url_2 = (treeWidget->topLevelItem(itemChecked.at(1))->text(1)).toLatin1().data();
		qDebug() << "开始对比";
	}
	//选择数量不为2的错误提示
	else {  
		QMessageBox::information(NULL, "错误提示", "对比时需要选择两张图像", QMessageBox::Ok);
	}
}

//删除选中图像
void QCompare::on_action_removeImage_triggered() {
	QTreeWidgetItem* item = treeWidget->currentItem();
	if (item == NULL)
		return;
	treeWidget->takeTopLevelItem(treeWidget->indexOfTopLevelItem(item));
	delete item;
}