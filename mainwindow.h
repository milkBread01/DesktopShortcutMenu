#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleMenu();           // Show/hide popup
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

    // NEW: Button click handlers
    void onIconButton1Clicked();
    void onIconButton2Clicked();
    void onIconButton3Clicked();
    void onIconButton4Clicked();
    void onIconButton5Clicked();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;   // System tray icon
    QMenu *trayMenu;             // Tray right-click menu

    void setupSystemTray();      // Initialize tray
    void connectButtons();  // NEW
    void registerGlobalHotkey(); // Setup Super+` hotkey
    void positionOnRightSide();  // Position window on monitor 1

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
    void keyPressEvent(QKeyEvent *event) override;  // NEW
};
#endif // MAINWINDOW_H
