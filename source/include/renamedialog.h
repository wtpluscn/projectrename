#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QCloseEvent>
#include <QStringList>
#include <QFileInfo>

namespace Ui {
	class RenameDialog;
}

class QCheckBox;
class QLineEdit;
class QGroupBox;
typedef struct
{
	QGroupBox* gbUse;
	QLineEdit* leSrc;
	QLineEdit* leTar;
	QCheckBox* ckCase;
	QCheckBox* ckUpLow;
}RenameItem_t;

typedef struct
{
	bool bUse;
	QString strSrc;
	QString strTar;
	bool bCase;
	bool bUpLow;
}RenameData_t;

#define RENAME_CONTENT_NUM	5
#define RENAME_ITEM_NUM	(RENAME_CONTENT_NUM+2)
#define SUFFIX_CACHE_NUM 5

class RenameDialog : public QDialog
{
	Q_OBJECT

public:
	explicit RenameDialog(QWidget *parent = 0);
	~RenameDialog();
public:
private:
	Ui::RenameDialog *ui;
public:
	virtual void closeEvent(QCloseEvent *event);
private:
	RenameItem_t  m_item[RENAME_ITEM_NUM];
	RenameData_t  m_data[RENAME_ITEM_NUM];
	RenameData_t* m_pDRData;	//dir rename
	RenameData_t* m_pFRData;	//file rename
	int			  m_nPgCount;
	int			  m_nPgMax;
	QStringList   m_suffixList;
	QString       m_baseDir;
	void Init();
public:
	void LoadSetting();
	void SaveSetting();
public slots:
	void onClkBtnScan();
	void onClkBtnExecute();
	void onClkBtnHelp();
	void onClkBtnPathSelect();
	void onFirstInit();
	void onClkBtnRenameSelectAll();
	void onClkBtnRenameReverse();
	void onClkBtnReplaceSelectAll();
	void onClkBtnReplaceReverse();
	void onClkGbUse();
	void onClkCkCase();
private:
	void CalcDir(QString strDir);
	void CalcFile(QString strDir);
	void ScanDir(QString strDir);
	void ScanDirFile(QString strDir);
	void ScanFile(const QFileInfo& fileInfo);
	void Item2Data();
	void Log(QString strLog, bool bError=false);
	void ClearLog();
	void ReplaceContent(QString strFile);
	bool Rename(QString strOldFile, QString strNewFile);
};

#endif // RENAMEDIALOG_H
