#include "renamedialog.h"
#include "ui_RenameDialog.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QFileDialog>
#include <QGroupBox>
#include <QTemporaryFile>
#include <QTextStream>

enum
{
	TABLE_COL_NAME = 0,
	TABLE_COL_RENAME,
	TABLE_COL_REPLACE,
	TABLE_COL_NEWNAME,
	TABLE_COL_LOG,
};

RenameDialog::RenameDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::RenameDialog)
{
	ui->setupUi(this);

	Qt::WindowFlags flags = windowFlags();
	flags |= Qt::WindowMinMaxButtonsHint;
	setWindowFlags(flags);
	setWindowIcon(QIcon(":rename.png"));
	showMaximized();

	connect(ui->btnPathSelect, SIGNAL(clicked()), this, SLOT(onClkBtnPathSelect()));
	connect(ui->btnScan, SIGNAL(clicked()), this, SLOT(onClkBtnScan()));
	connect(ui->btnExecute, SIGNAL(clicked()), this, SLOT(onClkBtnExecute()));
	connect(ui->btnHelp, SIGNAL(clicked()), this, SLOT(onClkBtnHelp()));
	connect(ui->btnRenameSelect, SIGNAL(clicked()), this, SLOT(onClkBtnRenameSelectAll()));
	connect(ui->btnRenameReverse, SIGNAL(clicked()), this, SLOT(onClkBtnRenameReverse()));
	connect(ui->btnReplaceSelect, SIGNAL(clicked()), this, SLOT(onClkBtnReplaceSelectAll()));
	connect(ui->btnReplaceReverse, SIGNAL(clicked()), this, SLOT(onClkBtnReplaceReverse()));	
	Init();

	QTimer::singleShot(0, this, SLOT(onFirstInit()));
}

RenameDialog::~RenameDialog()
{
	delete ui;
}

void RenameDialog::Init()
{
	int nCount = 0;
	m_item[nCount].gbUse = ui->gbDirRename;
	m_item[nCount].leSrc = ui->leDirNameSrc;
	m_item[nCount].leTar = ui->leDirNameTar;
	m_item[nCount].ckCase = ui->ckDirNameCase;
	m_item[nCount].ckUpLow = ui->ckDirNameUpLow;
	nCount++;

	m_item[nCount].gbUse = ui->gbFileRename;
	m_item[nCount].leSrc = ui->leFileNameSrc;
	m_item[nCount].leTar = ui->leFileNameTar;
	m_item[nCount].ckCase = ui->ckFileNameCase;
	m_item[nCount].ckUpLow = ui->ckFileNameUpLow;
	nCount++;

	m_item[nCount].gbUse = ui->gbContent1;
	m_item[nCount].leSrc = ui->leContentSrc1;
	m_item[nCount].leTar = ui->leContentTar1;
	m_item[nCount].ckCase = ui->ckContentCase1;
	m_item[nCount].ckUpLow = ui->ckContentUpLow1;
	nCount++;

	m_item[nCount].gbUse = ui->gbContent2;
	m_item[nCount].leSrc = ui->leContentSrc2;
	m_item[nCount].leTar = ui->leContentTar2;
	m_item[nCount].ckCase = ui->ckContentCase2;
	m_item[nCount].ckUpLow = ui->ckContentUpLow2;
	nCount++;

	m_item[nCount].gbUse = ui->gbContent3;
	m_item[nCount].leSrc = ui->leContentSrc3;
	m_item[nCount].leTar = ui->leContentTar3;
	m_item[nCount].ckCase = ui->ckContentCase3;
	m_item[nCount].ckUpLow = ui->ckContentUpLow3;
	nCount++;

	m_item[nCount].gbUse = ui->gbContent4;
	m_item[nCount].leSrc = ui->leContentSrc4;
	m_item[nCount].leTar = ui->leContentTar4;
	m_item[nCount].ckCase = ui->ckContentCase4;
	m_item[nCount].ckUpLow = ui->ckContentUpLow4;
	nCount++;

	m_item[nCount].gbUse = ui->gbContent5;
	m_item[nCount].leSrc = ui->leContentSrc5;
	m_item[nCount].leTar = ui->leContentTar5;
	m_item[nCount].ckCase = ui->ckContentCase5;
	m_item[nCount].ckUpLow = ui->ckContentUpLow5;
	nCount++;

	m_pDRData = m_data + 0;
	m_pFRData = m_data + 1;

	for (int i = 0; i < RENAME_ITEM_NUM; i++)
	{
		connect(m_item[i].gbUse, SIGNAL(clicked()), this, SLOT(onClkGbUse()));
		connect(m_item[i].ckCase, SIGNAL(clicked()), this, SLOT(onClkCkCase()));
	}
	
	QStringList lst;
	lst << tr("File") << tr("To Rename") << tr("To Replace Content")  << tr("New Name");
	ui->tableFile->setColumnCount(lst.count());
	ui->tableFile->setHorizontalHeaderLabels(lst);
	ui->tableFile->setColumnWidth(0, 400);
	ui->tableFile->setSelectionBehavior(QTableWidget::SelectRows);

	ui->pgBar->setMaximum(100);
	ui->pgBar->setValue(0);
}

void RenameDialog::onFirstInit()
{
	LoadSetting();
}

void RenameDialog::LoadSetting()
{
	QSettings settings("cn.wtplus", "ProjectRename");
	ui->lePath->setText(settings.value("Path").toString());

	for (int i = 0; i < RENAME_ITEM_NUM; i++)
	{
		m_item[i].gbUse->setChecked(settings.value(QString("Item%1_Use").arg(i)).toBool());
		m_item[i].leSrc->setText(settings.value(QString("Item%1_Src").arg(i)).toString());
		m_item[i].leTar->setText(settings.value(QString("Item%1_Tar").arg(i)).toString());
		m_item[i].ckCase->setChecked(settings.value(QString("Item%1_Case").arg(i)).toBool());
		if(m_item[i].ckCase->isChecked())
		{
			m_item[i].ckUpLow->setChecked(settings.value(QString("Item%1_UpLow").arg(i)).toBool());			
		}
		else
		{
			m_item[i].ckUpLow->setEnabled(false);					
		}
	}

	m_suffixList = settings.value("Suffix").toStringList();
	int nCount = m_suffixList.count();
	for (int i = nCount - 1; i >= 0; i--)
	{
		if (!m_suffixList[i].isEmpty())
		{
			ui->cbSuffix->addItem(m_suffixList[i]);
		}
	}

	QStringList lstPre;
	lstPre << "*.cpp;*.cxx;*.h;*.hxx;*.pro;*.ui;*.pri"
		<< "*.sln;*.vcxproj.filters;*.vcxproj;*.props;*.rc;*.def;*.rc2"
		<< "Makefile;Makefile.vc";
	
	nCount = lstPre.count();
	for (int i = 0; i < nCount; i++)
	{
		int nIndex = ui->cbSuffix->findText(lstPre[i]);
		if (nIndex < 0)
		{
			ui->cbSuffix->addItem(lstPre[i]);
		}
	}
}

void RenameDialog::SaveSetting()
{
	QSettings settings("cn.wtplus", "ProjectRename");
	settings.setValue("Path", ui->lePath->text());

	for (int i = 0; i < RENAME_ITEM_NUM; i++)
	{
		settings.setValue(QString("Item%1_Use").arg(i), m_item[i].gbUse->isChecked());
		settings.setValue(QString("Item%1_Src").arg(i), m_item[i].leSrc->text());
		settings.setValue(QString("Item%1_Tar").arg(i), m_item[i].leTar->text());
		settings.setValue(QString("Item%1_Case").arg(i), m_item[i].ckCase->isChecked());
		settings.setValue(QString("Item%1_UpLow").arg(i), m_item[i].ckUpLow->isChecked());
	}

	settings.setValue(QString("Suffix"), m_suffixList);
}

void RenameDialog::onClkBtnRenameSelectAll()
{
	int nCount = ui->tableFile->rowCount();
	for (int i = 0; i < nCount; i++)
	{
		QTableWidgetItem* item = ui->tableFile->item(i, TABLE_COL_RENAME);
		if (item && (item->flags() |= Qt::ItemIsUserCheckable))
		{
			item->setCheckState(Qt::Checked);
		}
	}
}

void RenameDialog::onClkBtnRenameReverse()
{
	int nCount = ui->tableFile->rowCount();
	for (int i = 0; i < nCount; i++)
	{
		QTableWidgetItem* item = ui->tableFile->item(i, TABLE_COL_RENAME);
		if (item && (item->flags() |= Qt::ItemIsUserCheckable))
		{
			if (item->checkState() == Qt::Checked)
			{
				item->setCheckState(Qt::Unchecked);
			}
			else
			{
				item->setCheckState(Qt::Checked);
			}
		}
	}
}

void RenameDialog::onClkBtnReplaceSelectAll()
{
	int nCount = ui->tableFile->rowCount();
	for (int i = 0; i < nCount; i++)
	{
		QTableWidgetItem* item = ui->tableFile->item(i, TABLE_COL_REPLACE);
		if (item && (item->flags() |= Qt::ItemIsUserCheckable))
		{
			item->setCheckState(Qt::Checked);
		}
	}
}

void RenameDialog::onClkBtnReplaceReverse()
{
	int nCount = ui->tableFile->rowCount();
	for (int i = 0; i < nCount; i++)
	{
		QTableWidgetItem* item = ui->tableFile->item(i, TABLE_COL_REPLACE);
		if (item && (item->flags() |= Qt::ItemIsUserCheckable))
		{
			if (item->checkState() == Qt::Checked)
			{
				item->setCheckState(Qt::Unchecked);
			}
			else
			{
				item->setCheckState(Qt::Checked);
			}
		}
	}
}

void RenameDialog::onClkBtnScan()
{
	if (ui->lePath->text().isEmpty())
	{
		QMessageBox::warning(this, tr("warning"), tr("path is empty"), QMessageBox::Ok);
		return;
	}

	QDir dir(ui->lePath->text());
	if (!dir.exists())
	{
		QMessageBox::warning(this, tr("warning"), tr("path is not exist"), QMessageBox::Ok);
		return;
	}

	ClearLog();
	Item2Data();

	bool bUse = false;
	for (int i = 0; i < RENAME_ITEM_NUM; i++)
	{
		if (m_data[i].bUse)
		{
			bUse = true;
			break;
		}
	}
	if (!bUse)
	{
		QMessageBox::warning(this, tr("warning"), tr("there is not rename option"), QMessageBox::Ok);
		return;
	}

	QString strCurSuffix = ui->cbSuffix->currentText();

	int nIndex = ui->cbSuffix->findText(strCurSuffix);
	if (nIndex < 0)
	{
		ui->cbSuffix->insertItem(-1, strCurSuffix);
	}

	m_suffixList.removeOne(strCurSuffix);
	m_suffixList << strCurSuffix;
	if (m_suffixList.count() > SUFFIX_CACHE_NUM)
	{
		m_suffixList.removeFirst();
	}

	Log(tr("Scan Start!"));

	m_nPgMax = 0;
	m_nPgCount = 0;
	if (m_pDRData->bUse)
	{
		CalcDir(ui->lePath->text());
	}
	else
	{
		CalcFile(ui->lePath->text());
	}
	ui->pgBar->setMaximum(m_nPgMax);
	ui->pgBar->setValue(m_nPgCount);
	

	ui->tableFile->setRowCount(0);
	m_baseDir = ui->lePath->text();

	if (m_pDRData->bUse)
	{
		ScanDir(m_baseDir);
		CalcDir(ui->lePath->text());
	}
	else
	{
		ScanDirFile(m_baseDir);
	}


	Log(tr("Scan End!"));
}

void RenameDialog::CalcDir(QString strDir)
{
	QDir dir(strDir);
	QFileInfoList lstDir = dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);

	m_nPgMax += lstDir.count();

	QFileInfo dirInfo;
	foreach(dirInfo, lstDir)
	{
		CalcDir(dirInfo.absoluteFilePath());
	}
}

void RenameDialog::CalcFile(QString strDir)
{
	QDir dir(strDir, ui->cbSuffix->currentText());

	m_nPgMax += dir.entryInfoList(QDir::Files).count();

	QFileInfoList lstDir = dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);
	QFileInfo dirInfo;
	foreach(dirInfo, lstDir)
	{
		CalcFile(dirInfo.absoluteFilePath());
	}
}

void RenameDialog::ScanDir(QString strDir)
{
	QDir dir(strDir);
	QFileInfoList lstDir = dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);
	QFileInfo dirInfo;
	foreach(dirInfo, lstDir)
	{
		QString strName = dirInfo.fileName();
		if (strName.contains(m_pDRData->strSrc, (m_pDRData->bCase&&(!m_pDRData->bUpLow)) ? Qt::CaseSensitive : Qt::CaseInsensitive))
		{
			int nCount = ui->tableFile->rowCount();
			ui->tableFile->insertRow(nCount);

			QTableWidgetItem* item = new QTableWidgetItem(dirInfo.absoluteFilePath());
			ui->tableFile->setItem(nCount, TABLE_COL_NAME, item);

			item = new QTableWidgetItem(tr("Rename"), Qt::ItemIsUserCheckable);
			item->setCheckState(Qt::Checked);
			ui->tableFile->setItem(nCount, TABLE_COL_RENAME, item);

			strName = strName.replace(m_pDRData->strSrc, m_pDRData->strTar, m_pDRData->bCase ? Qt::CaseSensitive : Qt::CaseInsensitive);
			if(m_pDRData->bCase && m_pDRData->bUpLow)
			{
				strName = strName.replace(m_pDRData->strSrc.toUpper(), m_pDRData->strTar.toUpper(), Qt::CaseSensitive);
				strName = strName.replace(m_pDRData->strSrc.toLower(), m_pDRData->strTar.toLower(), Qt::CaseSensitive);				
			}

			item = new QTableWidgetItem(QString("%1/%2").arg(dirInfo.absolutePath()).arg(strName));
			ui->tableFile->setItem(nCount, TABLE_COL_NEWNAME, item);
		}

		ScanDir(dirInfo.absoluteFilePath());
		ui->pgBar->setValue(++m_nPgCount);
	}
}

void RenameDialog::ScanDirFile(QString strDir)
{
	QDir dir(strDir, ui->cbSuffix->currentText());

	QFileInfoList lstFile = dir.entryInfoList(QDir::Files);
	QFileInfo fileInfo;
	foreach(fileInfo, lstFile)
	{
		ScanFile(fileInfo.absoluteFilePath());
		ui->pgBar->setValue(++m_nPgCount);
	}

	QFileInfoList lstDir = dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot);
	QFileInfo dirInfo;
	foreach(dirInfo, lstDir)
	{
		ScanDirFile(dirInfo.absoluteFilePath());
	}
}

void RenameDialog::ScanFile(const QFileInfo& fileInfo)
{
	bool bRename = false;
	if (m_pFRData->bUse)
	{
		QString strCbName = fileInfo.completeBaseName();
		if (strCbName.contains(m_pFRData->strSrc, (m_pFRData->bCase&&(!m_pFRData->bUpLow)) ? Qt::CaseSensitive : Qt::CaseInsensitive))
		{
			bRename = true;

			int nCount = ui->tableFile->rowCount();
			ui->tableFile->insertRow(nCount);
			
			QTableWidgetItem* item = new QTableWidgetItem(fileInfo.absoluteFilePath());
			ui->tableFile->setItem(nCount, TABLE_COL_NAME, item);

			item = new QTableWidgetItem(tr("Rename"), Qt::ItemIsUserCheckable);
			item->setCheckState(Qt::Checked);
			ui->tableFile->setItem(nCount, TABLE_COL_RENAME, item);

			strCbName = strCbName.replace(m_pFRData->strSrc, m_pFRData->strTar, m_pFRData->bCase ? Qt::CaseSensitive : Qt::CaseInsensitive);
			if(m_pFRData->bCase && m_pFRData->bUpLow)
			{
				strCbName = strCbName.replace(m_pFRData->strSrc.toUpper(), m_pFRData->strTar.toUpper(), Qt::CaseSensitive);
				strCbName = strCbName.replace(m_pFRData->strSrc.toLower(), m_pFRData->strTar.toLower(), Qt::CaseSensitive);				
			}

			item = new QTableWidgetItem(QString("%1/%2.%3").arg(fileInfo.absolutePath()).arg(strCbName).arg(fileInfo.suffix()));
			ui->tableFile->setItem(nCount, TABLE_COL_NEWNAME, item);
		}
	}

	QString strFile = fileInfo.absoluteFilePath();
	QFile fp(strFile);
	if (!fp.open(QIODevice::ReadOnly))
	{
		Log(tr("Open file %1 failed").arg(strFile));
		return;
	}

	bool bFind = false;
	QString strLine;
	QTextStream stream(&fp);
	while (!stream.atEnd())
	{
		strLine = stream.readLine();
		for (int i = 2; i < RENAME_ITEM_NUM; i++)	//from 1
		{
			if (m_data[i].bUse)
			{
				if (strLine.contains(m_data[i].strSrc, (m_data[i].bCase&&(!m_data[i].bUpLow)) ? Qt::CaseSensitive : Qt::CaseInsensitive))
				{
					bFind = true;
					break;
				}
			}
		}
		if (bFind)
		{
			break;
		}
	}
	
	if (!bFind)
	{
		return;
	}

	int nCount = ui->tableFile->rowCount();
	if (!bRename)
	{
		ui->tableFile->insertRow(nCount);

		QTableWidgetItem* item = new QTableWidgetItem(fileInfo.absoluteFilePath(), Qt::ItemIsUserCheckable);
		ui->tableFile->setItem(nCount, TABLE_COL_NAME, item);

		item = new QTableWidgetItem(tr("Replace"), Qt::ItemIsUserCheckable);
		item->setCheckState(Qt::Checked);
		ui->tableFile->setItem(nCount, TABLE_COL_REPLACE, item);
	}
	else
	{
		QTableWidgetItem* item = new QTableWidgetItem(tr("Replace"), Qt::ItemIsUserCheckable);
		item->setCheckState(Qt::Checked);
		ui->tableFile->setItem(nCount - 1, TABLE_COL_REPLACE, item);
	}
}

void RenameDialog::closeEvent(QCloseEvent *event)
{
	SaveSetting();

	event->accept();
}

void RenameDialog::onClkBtnPathSelect()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		ui->lePath->text(),
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);

	if (!dir.isEmpty())
	{
		ui->lePath->setText(dir);
	}
}

void RenameDialog::onClkBtnExecute()
{
	int nRowCount = ui->tableFile->rowCount();
	if (nRowCount <= 0)
	{
		QMessageBox::warning(this, tr("warning"), tr("there is no file need to operate; please change the option and scan"), QMessageBox::Ok);
		return;
	}

	m_nPgMax = 0;
	for (int i = 0; i < nRowCount; i++)
	{
		QTableWidgetItem* itemRename = ui->tableFile->item(i, TABLE_COL_RENAME);
		QTableWidgetItem* itemReplace = ui->tableFile->item(i, TABLE_COL_REPLACE);
		if (itemRename && itemRename->checkState() == Qt::Checked)
		{
			m_nPgMax++;
		}
		if(itemReplace && itemReplace->checkState() == Qt::Checked)
		{
			m_nPgMax++;
		}
	}
	if (m_nPgMax == 0)
	{
		QMessageBox::warning(this, tr("warning"), tr("there is no file need to operate; please select it"), QMessageBox::Ok);
		return;
	}

	Log(tr("Execute start"));
	ui->pgBar->setMaximum(m_nPgMax);
	ui->pgBar->setValue(0);
	m_nPgCount = 0;

	if (m_pDRData->bUse)
	{
		for (int i = nRowCount-1; i >= 0; i--)
		{
			QTableWidgetItem* item = ui->tableFile->item(i, TABLE_COL_RENAME);
			if (item && item->checkState() == Qt::Checked)
			{
				bool bSucc = Rename(ui->tableFile->item(i, TABLE_COL_NAME)->text(), ui->tableFile->item(i, TABLE_COL_NEWNAME)->text());
				if (bSucc)
				{
					Log(tr("Rename file %1 succeed").arg(ui->tableFile->item(i, TABLE_COL_NAME)->text()));
				}
				else
				{
					Log(tr("Rename file %1 failed").arg(ui->tableFile->item(i, TABLE_COL_NAME)->text()), true);
				}
				ui->pgBar->setValue(++m_nPgCount);
			}
		}
	}
	else
	{
		for (int i = 0; i < nRowCount; i++)
		{
			QTableWidgetItem* item = ui->tableFile->item(i, TABLE_COL_RENAME);
			if (item && item->checkState() == Qt::Checked)
			{
				bool bSucc = Rename(ui->tableFile->item(i, TABLE_COL_NAME)->text(), ui->tableFile->item(i, TABLE_COL_NEWNAME)->text());
				if (bSucc)
				{
					Log(tr("Rename file %1 succeed").arg(ui->tableFile->item(i, TABLE_COL_NAME)->text()));
				}
				else
				{
					Log(tr("Rename file %1 failed").arg(ui->tableFile->item(i, TABLE_COL_NAME)->text()), true);
				}
				ui->pgBar->setValue(++m_nPgCount);
			}
		}

		for (int i = 0; i < nRowCount; i++)
		{
			QTableWidgetItem* item = ui->tableFile->item(i, TABLE_COL_REPLACE);
			if (item && item->checkState() == Qt::Checked)
			{
				QTableWidgetItem* renameItem = ui->tableFile->item(i, TABLE_COL_RENAME);
				if (renameItem && renameItem->checkState() == Qt::Checked)
				{
					ReplaceContent(ui->tableFile->item(i, TABLE_COL_NEWNAME)->text());
				}
				else
				{
					ReplaceContent(ui->tableFile->item(i, TABLE_COL_NAME)->text());
				}
				ui->pgBar->setValue(++m_nPgCount);
			}
		}
	}


	Log(tr("Execute end"));
}

void RenameDialog::onClkBtnHelp()
{
	QMessageBox::about(NULL, tr("Help"), 
	"Rename V0.2.1 \n"
	"by WtPlus.cn"
	);
}

void RenameDialog::Item2Data()
{
	for (int i = 0; i < RENAME_ITEM_NUM; i++)
	{
		m_data[i].bUse = m_item[i].gbUse->isChecked();
		m_data[i].bCase = m_item[i].ckCase->isChecked();
		m_data[i].bUpLow = m_item[i].ckUpLow->isChecked();
		m_data[i].strSrc = m_item[i].leSrc->text();
		m_data[i].strTar = m_item[i].leTar->text();

		if (m_data[i].strSrc.isEmpty())
		{
			m_data[i].bUse = false;
		}
	}
}

void RenameDialog::Log(QString strLog, bool bError)
{
	ui->teLog->moveCursor(QTextCursor::End);
	if (bError)
	{
		ui->teLog->insertHtml(QString("<font color='red'>%1</font>\n").arg(strLog));
	}
	else
	{
		ui->teLog->insertPlainText(strLog);
		ui->teLog->insertPlainText("\n");
	}
}

void RenameDialog::ClearLog()
{
	ui->teLog->clear();
}

bool RenameDialog::Rename(QString strOldFile, QString strNewFile)
{
	QDir dir;
	return dir.rename(strOldFile, strNewFile);
}

void RenameDialog::ReplaceContent(QString strFile)
{
	QFileInfo fileInfo(strFile);
	QFile fp(strFile);
	if (!fp.open(QIODevice::ReadWrite))
	{
		Log(tr("Replace file %1 failed, error: Open fail").arg(strFile), true);
		return;
	}

	QTemporaryFile tmpFile;
	if (!tmpFile.open())
	{
		fp.close();
		Log(tr("Replace file %1 failed, error: Open temporary file fail").arg(strFile), true);
		return;
	}
	tmpFile.setAutoRemove(false);
	QTextStream tmpStream(&tmpFile);

	QString strLine;
	QTextStream stream(&fp);
	if (fileInfo.completeSuffix() == "vcxproj.filters"
		|| fileInfo.completeSuffix() == "vcxproj"
		|| fileInfo.completeSuffix() == "props")
	{
		stream.setCodec("UTF-8");
		tmpStream.setCodec("UTF-8");
		tmpStream.setGenerateByteOrderMark(true);
	}
	while (!stream.atEnd())
	{
		strLine = stream.readLine();
		for (int i = 2; i < RENAME_ITEM_NUM; i++)	//from 1
		{
			if (m_data[i].bUse)
			{
				strLine = strLine.replace(m_data[i].strSrc, m_data[i].strTar, m_data[i].bCase ? Qt::CaseSensitive : Qt::CaseInsensitive);
				
				if(m_data[i].bCase && m_data[i].bUpLow)
				{
					strLine = strLine.replace(m_data[i].strSrc.toUpper(), m_data[i].strTar.toUpper(), Qt::CaseSensitive);
					strLine = strLine.replace(m_data[i].strSrc.toLower(), m_data[i].strTar.toLower(), Qt::CaseSensitive);
				}
			}
		}
		tmpStream << strLine;
		tmpStream << "\n";
		//tmpFile.write(strLine.toLocal8Bit());
	}	

	fp.close();
	fp.remove();
	if (!tmpFile.rename(strFile))
	{
		Log(tr("Replace file %1 failed, error: rename fail").arg(strFile), true);
	}
	tmpFile.close();
	Log(tr("Replace file %1 succeed").arg(strFile));
}

void RenameDialog::onClkGbUse()
{
	QGroupBox* gb = (QGroupBox*)sender();
	if (!gb->isChecked())
	{
		return;
	}

	if (sender() == m_item[0].gbUse)
	{
		for (int i = 1; i < RENAME_ITEM_NUM; i++)
		{
			m_item[i].gbUse->setChecked(false);
		}
	}
	else
	{
		m_item[0].gbUse->setChecked(false);
	}
}
void RenameDialog::onClkCkCase()
{
	QCheckBox* ckCase = (QCheckBox*)sender();
	
	for (int i = 0; i < RENAME_ITEM_NUM; i++)
	{
		if(sender() == m_item[i].ckCase)
		{
			if(ckCase->isChecked())
			{
				m_item[i].ckUpLow->setEnabled(true);
			}
			else
			{
				m_item[i].ckUpLow->setEnabled(false);
				m_item[i].ckUpLow->setChecked(false);				
			}
		}
	}
}