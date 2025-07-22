#pragma once

#ifndef FASTCODE_H
#define FASTCODE_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class FastCode;
}
QT_END_NAMESPACE

class FastCode : public QMainWindow
{
    Q_OBJECT

public:
    FastCode(QWidget *parent = nullptr);
    ~FastCode();

private slots:
    void handleOpenAction();
    void handleSaveAsAction();
    void handleSaveAction();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QString currentFilePath;
    bool isModified = false;

    Ui::FastCode *ui;

    void saveFile(const QString &filePath);
};
#endif // FASTCODE_H
