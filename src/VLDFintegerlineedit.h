#ifndef VLDFINTEGERLINEEDIT_H
#define VLDFINTEGERLINEEDIT_H

#include <QLineEdit>
#include <QIntValidator>

class VLDFIntegerLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    VLDFIntegerLineEdit(QWidget *parent = Q_NULLPTR);

    int value() const;
    void setValue(const int value);

    void setRange(const int min, const int max);

signals:
    void valueChanged(int);

private:
    QIntValidator mValidator;

};

#endif // VLDFINTEGERLINEEDIT_H
