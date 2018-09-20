#include "VLDFdoublelineedit.h"

#include <QDoubleValidator>
#include <QLocale>
#include <limits>
#include <QKeyEvent>
#include <QStyle>
#include <QDebug>
#include <QWidget>
#include <QToolTip>

VLDFDoubleLineEdit::VLDFDoubleLineEdit(QWidget *parent) :
    QLineEdit(parent),
    mValidator(this)
{
    const double maxDouble = std::numeric_limits<double>::max();
    mValidator.setRange(-maxDouble, maxDouble);
    mValidator.setDecimals(5);
    mValidator.setNotation(QDoubleValidator::ScientificNotation);
    mValidator.setLocale(QLocale::C); // This locale accepts only point as decimal separator (US locale)
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    setValidator(&mValidator);

    connect(this, &VLDFDoubleLineEdit::textChanged, [this] {
        emit valueChanged(value());
    });
}

double VLDFDoubleLineEdit::value() const {
    return text().toDouble();
}

void VLDFDoubleLineEdit::setRange(const double min, const double max) {
    mValidator.setRange(min, max);
}

void VLDFDoubleLineEdit::setDecimals(const int decimals) {
    mValidator.setDecimals(decimals);
}

void VLDFDoubleLineEdit::setValue(const double value) {
    setText(QString::number(value, 'f', mValidator.decimals()));
}

void VLDFDoubleLineEdit::keyPressEvent(QKeyEvent *event) {
    // This prevents the user from using comma, allowing only point as decimal separator
    // avoiding confusion from asking what is thousand an what is decimal
    if(event->key() == Qt::Key_Comma) {
        // do nothing
        // TODO Maybe show a tooltip informing the user to use point and not comma
    } else {
        QLineEdit::keyPressEvent(event);
    }
}
