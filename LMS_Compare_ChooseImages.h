#pragma once

#include <QWidget>
#include "ui_LMS_Compare_ChooseImages.h"

class LMS_Compare_ChooseImages : public QWidget
{
	Q_OBJECT

public:
	LMS_Compare_ChooseImages(QWidget *parent = Q_NULLPTR);
	~LMS_Compare_ChooseImages();

private:
	Ui::LMS_Compare_ChooseImages ui;
};
