#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Declaración de las funciones toggle
    void toggleBold();
    void toggleItalic();


private slots:
    void on_actionAbrir_triggered();

    void on_actionGuardar_como_triggered();

    void on_actionGuardar_triggered();

    void on_actionImprimir_triggered();

    void on_actionEliminar_triggered();

    void on_actionCopiar_triggered();

    void on_actionPegar_triggered();

    void on_actionCortar_triggered();

    void on_actionNegrilla_triggered();

    void on_actionCursiva_triggered();

    void on_actionFuente_triggered();

    void on_actionResaltado_triggered();

    void on_actionSubrayado_triggered();

    void on_actionColor_triggered();

    void on_actionTama_o_triggered();

    void on_actionAlineado_izquierda_triggered();

    void on_actionAlineado_derecha_triggered();

    void on_actionAlineado_centro_triggered();

    void on_actionAlineado_justificado_triggered();

private:
    Ui::MainWindow *ui;
    QString nombreArchivo;
    void applyBlockAlignment(Qt::Alignment alignment);




};
#endif // MAINWINDOW_
