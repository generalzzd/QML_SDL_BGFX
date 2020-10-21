#ifndef ZDWINDOW_H
#define ZDWINDOW_H

#include <QObject>

class ZDWindow : public QObject
{
    Q_OBJECT
public:
    explicit ZDWindow(QObject *parent = nullptr);

    Q_INVOKABLE bool openGLWindow();

signals:

};



#endif // ZDWINDOW_H
