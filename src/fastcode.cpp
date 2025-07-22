#include "fastcode.h"
#include "./ui_fastcode.h"

FastCode::FastCode(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FastCode)
{
    ui->setupUi(this);

    ui->textEdit->setUndoRedoEnabled(true);

    connect(ui->textEdit, &QTextEdit::textChanged, [this]() {isModified = true;});
    connect(ui->actionopen, &QAction::triggered,this, &FastCode::handleOpenAction);
    connect(ui->actionsaveAs, &QAction::triggered,this, &FastCode::handleSaveAsAction);
    connect(ui->actionsave, &QAction::triggered,this, &FastCode::handleSaveAction);
}

FastCode::~FastCode()
{
    delete ui;
}

void FastCode::handleOpenAction() {
    QFile file(QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath()));
    QTextStream in(&file);

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Error", file.errorString());
    }

    ui->textEdit->setText(in.readAll());

    file.close();
}

void FastCode::handleSaveAsAction() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"));
    if (fileName != "")
    {
        QFile file(QFileInfo(fileName).absoluteFilePath());
        if (file.open(QIODevice::WriteOnly))
        {
            QString text = ui->textEdit->toPlainText();
            QTextStream out(&file);
            out << text;
            file.close();
        }
    }
}

void FastCode::saveFile(const QString &filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << ui->textEdit->toPlainText();
        file.close();

        currentFilePath = filePath;
        isModified = false;
        this->setWindowTitle("FastCode - " + filePath);
    } else {
        QMessageBox::critical(this, "Error", "Could not save file");
    }
}

void FastCode::handleSaveAction() {
    if (currentFilePath.isEmpty()) {

        handleSaveAsAction();
    } else {
        saveFile(currentFilePath);
    }
}

void FastCode::closeEvent(QCloseEvent *event)
{
    if (isModified) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Unsaved Changes",
                                      "Save changes before closing?",
                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (reply == QMessageBox::Save) {
            handleSaveAction();
            if (isModified) {

                event->ignore();
            } else {
                event->accept();
            }
        } else if (reply == QMessageBox::Discard) {
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        event->accept();
    }
}

