#ifndef VLDFDOUBLELINEEDIT_H
#define VLDFDOUBLELINEEDIT_H

#include <QLineEdit>
#include <QTimer>
#include <QAction>
#include <QValidator>
#include <iostream>

class QDoubleValidator;

class VLDFDoubleLineEdit : public QLineEdit
{
    Q_OBJECT
public:
	VLDFDoubleLineEdit(QWidget *parent = Q_NULLPTR);
    double value() const;
    void setRange(const double min, const double max);
    void setDecimals(const int decimals);
    void setValue(const double value);

signals:
    void valueChanged(double);

private:
    QDoubleValidator mValidator;
    QDoubleValidator::State mCurrentValidationState;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // VLDFDOUBLELINEEDIT_H
