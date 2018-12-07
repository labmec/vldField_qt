#include "VLDFunitinput.h"

#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVariant>
#include <QTextStream>
#include <cmath>

#include "VLDFdoublelineedit.h"

//#define DEBUG
#ifdef DEBUG
#include <QDebug>
#endif

extern "C" {
    #include "externallibs/units-2.11/units.h"
}

// Units fix: create a temporary file with contents of :/definition.units (resource file)
QString VLDFUnitInput::fTmpDefsUnitsFileName;
QTemporaryFile VLDFUnitInput::fTmpDefsUnitsFile;

VLDFUnitInput::VLDFUnitInput(QWidget *parent) :
    QWidget(parent),
    fDisplayValue(new VLDFDoubleLineEdit(this)),
    fDisplayUnit(new QComboBox(this)),
    fSIValue(0.),
    fTVD(0.),
    fPrecision(5),
    fUnitType(EDimensionless)
{
    fDisplayValue->setDecimals(fPrecision);
    setUnitType(fUnitType);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(fDisplayValue);
    mainLayout->addWidget(fDisplayUnit);
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    connect(fDisplayValue, SIGNAL(editingFinished()), this, SLOT(set()));
    connect(fDisplayValue, SIGNAL(valueChanged(double)), this, SIGNAL(valueChanged(double)));
    connect(fDisplayUnit, SIGNAL(currentIndexChanged(int)), this, SLOT(convert(int)));

}

VLDFUnitInput::~VLDFUnitInput () {
    delete this->layout();
}

QLineEdit *VLDFUnitInput::lineEdit() const {
    return fDisplayValue;
}


/**
 * @brief Creating a temporary file with resource file definition.units contents.
 * Temporary file will be removed when program ends!
 */
void VLDFUnitInput::setupUnitsFile() {
    QFile unitsFile (":/units/definitions.units");
    if (unitsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (fTmpDefsUnitsFile.open()) {
            QTextStream in(&unitsFile);
            QString line = in.readLine();
            QTextStream out(&fTmpDefsUnitsFile);

            while (!line.isNull()) {
                out << line << endl;
                line = in.readLine();
            }
            fTmpDefsUnitsFile.close();
        }
        unitsFile.close();
        // saving it's name
        fTmpDefsUnitsFileName = fTmpDefsUnitsFile.fileName();
    }
}

void VLDFUnitInput::reload(double newTVD) {
    fTVD = newTVD;
    if(fUnitType == EPPG && fDisplayUnit->itemText(fDisplayUnit->currentIndex()) == "ppg")
        set();
}

void VLDFUnitInput::changeUnit(int idx) {
    if (fUnitType == EPressure && idx > 2) {
        return;
    }
    fDisplayUnit->setCurrentIndex(idx);
}

// Display converted internal value according to combobox option
void VLDFUnitInput::convert(int idx) {
    if (fUnitType != EDimensionless) {
        QString fromStr(fDisplayUnit->itemData(idx).toString());
        QString toStr(fDisplayUnit->itemText(idx));

#ifdef DEBUG
        //qDebug()  << "Fromstring " << fromStr.toStdString().c_str();
        //qDebug()  << "Tostring " << toStr.toStdString().c_str();
#endif

        double multiplier = 0.;
        double divider = 0.;

        // All exceptions
        if (toStr == "ppg") {
            multiplier = 850.994463723 / fTVD;
            fDisplayValue->setValue(fSIValue * multiplier);
            return;
        }

        int rett = convertGNUUnits(fTmpDefsUnitsFileName.toStdString().c_str(), fromStr.toStdString().c_str(), toStr.toStdString().c_str(), &multiplier, &divider);

        if (!rett) {
				fDisplayValue->setValue(fSIValue * multiplier);
        }
    } else {
        fDisplayValue->setValue(fSIValue);
    }
}

/**
 * @brief Change internal value of the SIValue, converting it according to combobox option
 */
void VLDFUnitInput::set() {
    if (fUnitType != EDimensionless) {
        QString fromStr(fDisplayUnit->itemText(fDisplayUnit->currentIndex()));
        QString toStr(fDisplayUnit->itemData(fDisplayUnit->currentIndex()).toString());

        double multiplier = 0.;
        double divider = 0.;
        bool invert = 0;

        // All exceptions
        if (fromStr == "ppg") {
            divider = 850.994463723 / fTVD ;
            fSIValue = fDisplayValue->value() / divider;
            emit siValueChanged(fSIValue);
            return;
        }

#ifdef DEBUG
        qDebug()  << "set Fromstring " << fromStr.toStdString().c_str();
        qDebug()  << "Tostring " << toStr.toStdString().c_str();
#endif

        int rett = convertGNUUnits(fTmpDefsUnitsFileName.toStdString().c_str(), fromStr.toStdString().c_str(), toStr.toStdString().c_str(), &multiplier, &divider);
#ifdef DEBUG
        qDebug()  << "multiplier " << multiplier;
        qDebug()  << "divider " << divider;
#endif

        if (!rett) {
            if (invert) {
                fSIValue =  1. / (fDisplayValue->value() * multiplier);
#ifdef DEBUG
                qDebug() << "***set**************************************>>>>> " << fSIValue;
#endif
            } else {
                fSIValue = fDisplayValue->value() * multiplier;
            }
        }
    } else {
        fSIValue = fDisplayValue->value();
    }

    emit siValueChanged(fSIValue);
}

double VLDFUnitInput::value() const {
    return fDisplayValue->value();
}

/**
 * @brief Sets the value from the SIValue in this class and updates the qlineedit with this value
 * @param value - the new value for the VLDFunitinput (the value is coverted to the selected unit in the ui)
 */
void VLDFUnitInput::SetSIValue(const double value) {
    fSIValue = value;
    convert(fDisplayUnit->currentIndex());
    emit siValueChanged(fSIValue);
}

QString VLDFUnitInput::currentUnitText() const {
    return fDisplayUnit->itemData(fDisplayUnit->currentIndex()).toString();
}

VLDFUnitInput::EUnitType VLDFUnitInput::unitType() const {
    return fUnitType;
}

/**
 * @brief Set the items in from the VLDFUnitInput's combo boxes according
 * to it's unit type and also sets the conversion configuration (from -> to)
 * @param unittp - EUnitType representing this VLDFUnitInput's type
 */
void VLDFUnitInput::setUnitType(const EUnitType & unittp) {
    fUnitType = unittp;
    fDisplayUnit->clear();

    // The items are structed to easy the conversion in the convert function
    // read: fDisplayUnit->addItem("FROM UNIT", QVariant("CONVERT TO 'SI' UNIT"))
    if (fUnitType == EPressure) {
        fDisplayUnit->addItem("MPa", QVariant("MPa"));
        fDisplayUnit->addItem("Pa", QVariant("MPa"));
        fDisplayUnit->addItem("psi", QVariant("MPa"));
    } else if (fUnitType == EPPG) {
        fDisplayUnit->addItem("MPa", QVariant("MPa"));
        fDisplayUnit->addItem("Pa", QVariant("MPa"));
        fDisplayUnit->addItem("psi", QVariant("MPa"));
        fDisplayUnit->addItem("ppg", QVariant("MPa"));
    } else if (fUnitType == EInvertedPressure) {
        fDisplayUnit->addItem("1/MPa", QVariant("1/MPa"));
        fDisplayUnit->addItem("1/Pa", QVariant("1/MPa"));
        fDisplayUnit->addItem("1/psi", QVariant("1/MPa"));
      //fDisplayUnit->addItem("1/ppg", QVariant("MPa"));
    } else if (fUnitType == EDimension) {
        fDisplayUnit->addItem("m", QVariant("m"));
        fDisplayUnit->addItem("ft", QVariant("m"));
        fDisplayUnit->addItem("in", QVariant("m"));
    } else if (fUnitType == EAngle) {
        fDisplayUnit->addItem("radian", QVariant("radian"));
        fDisplayUnit->addItem("degree", QVariant("radian"));
    } 
    else if (fUnitType == EPermeability) {
        fDisplayUnit->addItem("darcy", QVariant("m^2"));
        fDisplayUnit->addItem("milidarcy", QVariant("m^2"));
        fDisplayUnit->addItem("m^2", QVariant("m^2"));
    }
    else if (fUnitType == EDimensionless) {
        fDisplayUnit->hide();
        return;
    }
	fDisplayUnit->show();
}

bool VLDFUnitInput::isEnabled() const {
    return fDisplayValue->isEnabled();
}

void VLDFUnitInput::setEnabled ( bool status ) {
    fDisplayValue->setEnabled(status);
    fDisplayUnit->setEnabled(status);
}

void VLDFUnitInput::setUnitChangeEnabled( bool status ) {
    fDisplayUnit->setEnabled(status);
}

void VLDFUnitInput::setRange(const double min, const double max) {
    fDisplayValue->setRange(min, max);
}

