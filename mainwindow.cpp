#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QScreen>
#include <QGuiApplication>
#include <QKeyEvent>

#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
// Include Windows header for hotkey functionality
#ifdef Q_OS_WIN
#include <windows.h>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set fixed size to match wireframe
    setFixedSize(200, 600);

    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    setupSystemTray();
    connectButtons();
    registerGlobalHotkey();  // Call during initialization
    positionOnRightSide();
}

MainWindow::~MainWindow()
{
    // Unregister hotkey when closing
#ifdef Q_OS_WIN
    UnregisterHotKey((HWND)winId(), 1);
#endif

    delete trayMenu;
    delete trayIcon;
    delete ui;
}
#include <QKeyEvent>

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Escape:
        hide();
        break;
    case Qt::Key_Up:
        // Focus previous button
        focusPreviousChild();
        break;
    case Qt::Key_Down:
        // Focus next button
        focusNextChild();
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        // Click focused button
        if (QWidget *focused = focusWidget()) {
            if (QPushButton *btn = qobject_cast<QPushButton*>(focused)) {
                btn->click();
            }
        }
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}
void MainWindow::connectButtons()
{
    // Connect icon buttons to actions
    connect(ui->iconButton1, &QPushButton::clicked, this, &MainWindow::onIconButton1Clicked);
    connect(ui->iconButton2, &QPushButton::clicked, this, &MainWindow::onIconButton2Clicked);
    connect(ui->iconButton3, &QPushButton::clicked, this, &MainWindow::onIconButton3Clicked);
    connect(ui->iconButton4, &QPushButton::clicked, this, &MainWindow::onIconButton4Clicked);
    connect(ui->iconButton5, &QPushButton::clicked, this, &MainWindow::onIconButton5Clicked);

    // Connect close button
    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::hide);
}

// Action handlers
void MainWindow::onIconButton1Clicked()
{
    qDebug() << "Icon 1 clicked!";
    QDesktopServices::openUrl(QUrl("https://search.brave.com/search?q="));
    hide();
}

void MainWindow::onIconButton2Clicked()
{
    qDebug() << "Icon 2 clicked!";
    QDesktopServices::openUrl(QUrl("https://www.youtube.com"));
    hide();
}

void MainWindow::onIconButton3Clicked()
{
    qDebug() << "Icon 3 clicked!";
    QProcess::startDetached("C:/Users/guzke/AppData/Local/Programs/Microsoft VS Code/``Code.exe");
    hide();
}

void MainWindow::onIconButton4Clicked()
{
    qDebug() << "Icon 4 clicked!";
    QProcess::startDetached("C:/Program Files/CodeBlocks/codeblocks.exe");
    hide();
}

void MainWindow::onIconButton5Clicked()
{
    qDebug() << "Icon 5 clicked!";//"C:\Program Files\CodeBlocks\codeblocks.exe"
    QDesktopServices::openUrl(QUrl("https://app.diagrams.net/index.html"));
    hide();
}

void MainWindow::setupSystemTray()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icon.png"));

    trayMenu = new QMenu();
    trayMenu->addAction("Show Menu", this, &MainWindow::toggleMenu);
    trayMenu->addSeparator();
    trayMenu->addAction("Exit", qApp, &QApplication::quit);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated,
            this, &MainWindow::trayIconActivated);
}

void MainWindow::toggleMenu()
{
    if (isVisible()) {
        hide();
    } else {
        positionOnRightSide();
        show();
        activateWindow();
        raise();
    }
}

void MainWindow::positionOnRightSide()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    int x = screenGeometry.right() - width() - 20;
    int y = screenGeometry.top() + 100;
    move(x, y);
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        toggleMenu();
    }
}

// Windows-specific hotkey registration
#ifdef Q_OS_WIN
void MainWindow::registerGlobalHotkey()
{
    // Register Super+` (Windows key + backtick)
    // MOD_WIN = Windows key, VK_OEM_3 = backtick key
    RegisterHotKey((HWND)winId(), 1, MOD_WIN, VK_OEM_3);
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
{
    if (eventType == "windows_generic_MSG") {
        MSG *msg = static_cast<MSG*>(message);
        if (msg->message == WM_HOTKEY) {
            toggleMenu();
            return true;
        }
    }
    return QMainWindow::nativeEvent(eventType, message, result);
}
#else
// Stub for non-Windows platforms
void MainWindow::registerGlobalHotkey()
{
    // Could use different approach on Linux/Mac
    qWarning("Global hotkeys not implemented for this platform");
}
#endif
