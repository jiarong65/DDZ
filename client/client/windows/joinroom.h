#ifndef JOINROOM_H
#define JOINROOM_H

#include <QDialog>
#include "codec.h"

namespace Ui {
class JoinRoom;
}

enum class DialogType:char{Manual,Search};

class JoinRoom : public QDialog
{
    Q_OBJECT

public:
    explicit JoinRoom(DialogType type,QWidget *parent = nullptr);
    ~JoinRoom();
    void joinRoom();
    void searchRoom();

private:
    void encodeMessage(RequestCode reqCode);

private:
    Ui::JoinRoom *ui;
};

#endif // JOINROOM_H
