#include <QApplication>
#include "ui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    app.setApplicationName("FL Studio Editor");
    app.setOrganizationName("MusicEditor");
    app.setStyle("Fusion");
    
    // Set dark palette
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(26, 26, 26));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(34, 34, 34));
    darkPalette.setColor(QPalette::AlternateBase, QColor(42, 42, 42));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(51, 51, 51));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Highlight, QColor(76, 175, 80));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    app.setPalette(darkPalette);
    
    flstudio::ui::MainWindow window;
    window.show();
    
    return app.exec();
}
