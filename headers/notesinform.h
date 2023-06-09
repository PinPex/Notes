#ifndef NOTESINFORM_H
#define NOTESINFORM_H


class NotesInform
{
public:
    int NoteId;
    QString Name;
    QString Date;
    QString Text;
    QString DateRemind;
    QString fontFamily;
    QString textSize;
    QString expan;
    QPoint coord;
    int height;
    int width;
    NotesInform(int NoteId){
        this->NoteId = NoteId;
    }
    void operator =(NotesInform inf){
        NoteId = inf.NoteId;
        Name = inf.Name;
        Text = inf.Text;
        Date = inf.Date;
        DateRemind = inf.DateRemind;
        fontFamily = inf.fontFamily;
        textSize = inf.textSize;
        expan = inf.expan;
        coord = inf.coord;
        height = inf.height;
        width = inf.width;
    }

    NotesInform(){
    }
};

#endif // NOTESINFORM_H
