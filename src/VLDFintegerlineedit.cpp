#include "VLDFintegerlineedit.h"

#include <QIntValidator>
#include <limits.h>

VLDFIntegerLineEdit::VLDFIntegerLineEdit(QWidget *parent) :
    QLineEdit(parent),
    mValidator(this)
{
    const int maxInt = std::numeric_limits<int>::max();
    mValidator.setRange(0, maxInt);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    setValidator(&mValidator);

    connect(this, &QLineEdit::textChanged, [this] {
        emit valueChanged(value());
    });
}

void VLDFIntegerLineEdit::setRange(const int min, const int max) {
    mValidator.setRange(min, max);
}

void VLDFIntegerLineEdit::setValue(const int value) {
    setText(QString::number(value));
}

int VLDFIntegerLineEdit::value() const {
    return text().toInt();
}
