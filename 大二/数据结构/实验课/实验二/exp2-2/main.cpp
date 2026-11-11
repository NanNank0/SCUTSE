/*
* SID: 202530551453
* Author: 徐克楠
* Date: 2026.11.11
*/
#include "tree_window.h"
int main(int argc,char** argv){QApplication app(argc,argv);styleApp(app);TreeWindow w(true);w.show();setupCapture(app,w);return app.exec();}
