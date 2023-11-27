#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPrintDialog>
#include <QPrinter>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QFontDialog>
#include <QInputDialog>
#include <QColorDialog>
#include <QTextCharFormat>
#include <QBrush>
#include <QTextBlockFormat>
#include <QTextDocument>
#include <QTextDocumentWriter>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->textEdit);
    nombreArchivo = "";

    // Conectar acciones a funciones
    connect(ui->actionNegrilla, &QAction::triggered, this, &MainWindow::on_actionNegrilla_triggered);
    connect(ui->actionCursiva, &QAction::triggered, this, &MainWindow::on_actionCursiva_triggered);
    connect(ui->actionFuente, &QAction::triggered, this, &MainWindow::on_actionFuente_triggered);
    connect(ui->actionResaltado, &QAction::triggered, this, &MainWindow::on_actionResaltado_triggered);
    connect(ui->actionSubrayado, &QAction::triggered, this, &MainWindow::on_actionSubrayado_triggered);
    connect(ui->actionColor, &QAction::triggered, this, &MainWindow::on_actionColor_triggered);
    connect(ui->actionTama_o, &QAction::triggered, this, &MainWindow::on_actionTama_o_triggered);

}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionAbrir_triggered()
{

    QString nombreArchivo = QFileDialog::getOpenFileName(this, "Abrir");
    if (nombreArchivo.isEmpty())
        return;

    QFile arch(nombreArchivo);
    if (arch.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream io(&arch);
        ui->textEdit->setPlainText(io.readAll());
        arch.close();

        // Actualizar nombreArchivo para mantener un seguimiento del archivo abierto
        this->nombreArchivo = nombreArchivo;
    }
    else
    {
        QMessageBox::critical(this, "Error", arch.errorString());
    }

}


void MainWindow::on_actionGuardar_como_triggered()
{
    QString filtro = "Archivos de texto (*.txt);;Todos los archivos (*.*)";
    QString nombreArchivo = QFileDialog::getSaveFileName(this, "Guardar", "", filtro);

    if (nombreArchivo.isEmpty())
        return;

    QFile archivo(nombreArchivo);
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", archivo.errorString());
        return;
    }

    QTextStream io(&archivo);
    io << ui->textEdit->toPlainText();
    archivo.flush();
    archivo.close();

    // Actualizar nombreArchivo para mantener un seguimiento del archivo guardado
    this->nombreArchivo = nombreArchivo;
}

void MainWindow::on_actionGuardar_triggered()
{
    QString filtro = "Archivos de texto (*.txt);;Archivos ODT (*.odt);;Archivos HTML (*.html);;Todos los archivos (*.*)";
    QString selectedFilter;
    QString nombreArchivo = QFileDialog::getSaveFileName(this, "Guardar", "", filtro, &selectedFilter);

    if (nombreArchivo.isEmpty())
        return;

    if (selectedFilter == "Archivos ODT (*.odt)" || selectedFilter == "Archivos HTML (*.html)") {
        // Guardar como ODT o HTML
        QTextDocument document;
        document.setPlainText(ui->textEdit->toPlainText());

        QTextDocumentWriter writer(nombreArchivo);
        writer.setFormat(selectedFilter == "Archivos ODT (*.odt)" ? "odt" : "html");
        writer.write(&document);
    } else {
        // Guardar como texto plano por defecto
        QFile archivo(nombreArchivo);
        if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::critical(this, "Error", archivo.errorString());
            return;
        }

        QTextStream io(&archivo);
        io << ui->textEdit->toPlainText();
        archivo.flush();
        archivo.close();
    }

    // Actualizar nombreArchivo para mantener un seguimiento del archivo guardado
    this->nombreArchivo = nombreArchivo;
}



void MainWindow::on_actionImprimir_triggered()
{
    QPrinter *Impresora;
    QPrintDialog pd (this);
    if (pd.exec() == QDialog::Rejected)
        return;
    Impresora = pd.printer();
    ui->textEdit->print(Impresora);
}


void MainWindow::on_actionEliminar_triggered()
{
    if (nombreArchivo.isEmpty())
    {
        QMessageBox::warning(this, "Eliminar", "No hay un archivo abierto para eliminar.");
        return;
    }

    QFile archivo(nombreArchivo);

    if (archivo.exists())
    {
        if (QFile::remove(nombreArchivo))
        {
            QMessageBox::information(this, "Eliminar", "El archivo se ha eliminado correctamente.");
            nombreArchivo.clear(); // Borra el nombre del archivo actual
            ui->textEdit->clear(); // Borra el contenido del QPlainTextEdit
        }
        else
        {
            QMessageBox::critical(this, "Error", "No se pudo eliminar el archivo.");
        }
    }
    else
    {
        QMessageBox::warning(this, "Eliminar", "El archivo no existe en la ubicación especificada.");
    }
}


void MainWindow::on_actionCopiar_triggered()
{
    ui->textEdit->copy();

}


void MainWindow::on_actionPegar_triggered()
{
    ui->textEdit->paste();

}


void MainWindow::on_actionCortar_triggered()
{
    ui->textEdit->cut();

}

void MainWindow::on_actionNegrilla_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat formato = cursor.charFormat();

    // Mantener otros formatos y cambiar la negrilla
    formato.setFontWeight(formato.fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold);

    // Mantener la cursiva
    formato.setFontItalic(cursor.charFormat().fontItalic());

    // Aplicar el nuevo formato al texto seleccionado
    cursor.mergeCharFormat(formato);
    ui->textEdit->setTextCursor(cursor);
}

void MainWindow::on_actionCursiva_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat formato = cursor.charFormat();

    // Mantener otros formatos y cambiar la cursiva
    formato.setFontItalic(!formato.fontItalic());

    // Mantener la negrilla
    formato.setFontWeight(cursor.charFormat().fontWeight());

    // Aplicar el nuevo formato al texto seleccionado
    cursor.mergeCharFormat(formato);
    ui->textEdit->setTextCursor(cursor);
}





void MainWindow::on_actionFuente_triggered()
{
    bool ok;
    QFont fuenteActual = ui->textEdit->textCursor().charFormat().font();
    QFont nuevaFuente = QFontDialog::getFont(&ok, fuenteActual, this, "Selecciona una fuente");
    if (ok)
    {
        QTextCharFormat formato = ui->textEdit->textCursor().charFormat();
        formato.setFont(nuevaFuente);
        ui->textEdit->mergeCurrentCharFormat(formato);
    }
}

void MainWindow::on_actionResaltado_triggered()
{
    QColor color = QColorDialog::getColor();
    if (color.isValid())
    {
        QTextCharFormat formato = ui->textEdit->textCursor().charFormat();
        formato.setBackground(QBrush(color));
        ui->textEdit->mergeCurrentCharFormat(formato);
    }
}


void MainWindow::on_actionSubrayado_triggered()
{
    // Obtener el formato actual del texto
    QTextCharFormat formato = ui->textEdit->textCursor().charFormat();

    // Preguntar al usuario por el color del subrayado
    QColorDialog colorDialog(this);
    QColor colorSubrayado = colorDialog.getColor(formato.foreground().color(), this, "Selecciona el color del subrayado");

    if (colorSubrayado.isValid())
    {
        // Crear un nuevo formato solo para el subrayado
        QTextCharFormat subrayadoFormato;

        // Configurar el color del subrayado en el nuevo formato
        subrayadoFormato.setForeground(QBrush(colorSubrayado));

        // Alternar el estado de subrayado
        subrayadoFormato.setFontUnderline(!formato.fontUnderline());

        // Aplicar el nuevo formato solo para el subrayado al texto
        ui->textEdit->mergeCurrentCharFormat(subrayadoFormato);
    }
}

void MainWindow::on_actionColor_triggered()
{
    QColor color = QColorDialog::getColor();
    if (color.isValid())
    {
        QTextCharFormat formato = ui->textEdit->textCursor().charFormat();
        formato.setForeground(color);
        ui->textEdit->mergeCurrentCharFormat(formato);
    }
}

void MainWindow::on_actionTama_o_triggered()
{
    bool ok;
    QTextCharFormat formato = ui->textEdit->currentCharFormat();
    int nuevoTamaño = QInputDialog::getInt(this, "Selecciona el tamaño de fuente", "Tamaño:", formato.fontPointSize(), 1, 100, 1, &ok);
    if (ok)
    {
        formato.setFontPointSize(nuevoTamaño);
        ui->textEdit->mergeCurrentCharFormat(formato);
    }
}

void MainWindow::on_actionAlineado_izquierda_triggered()
{
    ui->textEdit->setAlignment(Qt::AlignLeft);
}

void MainWindow::on_actionAlineado_derecha_triggered()
{
    ui->textEdit->setAlignment(Qt::AlignRight);
}

void MainWindow::on_actionAlineado_centro_triggered()
{
    ui->textEdit->setAlignment(Qt::AlignCenter);
}

void MainWindow::on_actionAlineado_justificado_triggered()
{
    ui->textEdit->setAlignment(Qt::AlignJustify);
}


