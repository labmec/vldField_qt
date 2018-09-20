#ifndef VLDFUNITINPUT_H
#define VLDFUNITINPUT_H

#include <QWidget>
#include <QTemporaryFile>
#include <QValidator>

class QLineEdit;
class QLabel;
class QComboBox;
class QString;
class QValidator;
class VLDFDoubleLineEdit;

class VLDFUnitInput : public QWidget
{
    Q_OBJECT

    Q_ENUMS (EUnitType)

    Q_PROPERTY(double value READ value NOTIFY valueChanged)
    Q_PROPERTY(double SIValue READ SIValue NOTIFY siValueChanged)
    Q_PROPERTY(EUnitType unitType READ unitType WRITE setUnitType )
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)

public:

    enum EUnitType {EPressure, EPPG, EDimension, EAngle, EDimensionless, EInvertedPressure};
	
	QComboBox *fDisplayUnit;

    explicit VLDFUnitInput(QWidget *parent = 0);
    virtual ~VLDFUnitInput();

    double value() const;
    void SetSIValue(const double value);
    double SIValue() const {
        return fSIValue;
    }

    QString currentUnitText() const;
    EUnitType unitType() const;
    void setUnitType(const EUnitType & unittp);

    bool isEnabled() const;
    void setEnabled (bool status);
    QLineEdit *lineEdit() const;
    void setUnitChangeEnabled (bool status);

    void setRange(const double min, const double max);

    // Units fix: create a temporary file with contents of :/definition.units (resource file)
    static QString fTmpDefsUnitsFileName;
    static void setupUnitsFile();

signals:
    void valueChanged(double newValue);
    void siValueChanged(const double newSiValue);
    
public slots:
    void reload(double);
    void changeUnit(int idx);

private:
    VLDFDoubleLineEdit *fDisplayValue;

    double fSIValue;
    EUnitType fUnitType;

    double fTVD;

    int fPrecision;

    static QTemporaryFile fTmpDefsUnitsFile;

private slots:
    void convert(int idx);
    void set();
    
};

#endif // VLDFUNITINPUT_H
