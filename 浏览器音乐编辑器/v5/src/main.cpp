#include <QApplication>
#include "ui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    app.setApplicationName("FL Studio Editor");
    app.setOrganizationName("MusicEditor");
    
    flstudio::ui::MainWindow window;
    window.show();
    
    return app.exec();
}
