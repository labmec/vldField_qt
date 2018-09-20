#include "VLDFvalidationcontroller.h"

#include <QAbstractButton>
#include <QDebug>
#include "VLDFvalidationinput.h"

/**
 * @brief class constructor
 * @param parent - the parent widget relative to this, must be the top widget in the context,
 * for example a wizard, a dialog or a window.
 */
VLDFValidationController::VLDFValidationController(QObject *parent) :
    QObject(parent),
    mInputs(),
    isCurrentStateValid(false),
    mPopup(static_cast<QWidget*>(parent->parent())),
    mIsPopupEnabled(true)
{

}

/**
 * @brief class Destructor
 */
VLDFValidationController::~VLDFValidationController() {
    unregisterAllLineEdits();
}

/**
 * @brief Appends a lineedit to the list of obrigatory fields that must be valid in order to enable
 * the control buttons.
 * @param lineEdit - the lineedit that must contain a valid input.
 * @param aditionalValidationRule - An std::function that returns a boolean value according to user specifiend rules,
 * use this argument in order to provide more complex validation rules that wasn't provided by qt validators.
 */
bool VLDFValidationController::registerLineEdit(QLineEdit *lineEdit, std::function<bool(void)> aditionalValidationRule, const QString additionalErrorMessage) {
    if(!lineEdit) {
        return false;
    }
    VLDFValidationInput *input = new VLDFValidationInput(lineEdit, this, aditionalValidationRule, additionalErrorMessage);
    addInput(input);
	return true;
}

/**
* @brief Some validations of the list of obrigatory fields may be hidden from validation. This is useful in pages where
* optional lineedit's may or may not be present (e.g., material wizard). 
* @param lineEdit - the lineedit that will not be hidden from validation.
*/
bool VLDFValidationController::softRegisterLineEdit(QLineEdit *lineEdit)
{
	if (mInputs.contains(lineEdit)) {
		VLDFValidationInput* input = mInputs.value(lineEdit);
		input->mMustValidate = true;
		return input->isValid();
	}
}

/**
* @brief removes the lineedit from the obrigatory fields
* @param lineEdit - the lineedit to be removed
*/
bool VLDFValidationController::unregisterLineEdit(QLineEdit *lineEdit) {

	if (mInputs.contains(lineEdit)) {
		delete mInputs.take(lineEdit);
		return true;
	}
	else
		return false;
}

/**
* @brief Removes all the registered inputs from the obrigatory fields that must be validated
*/
void VLDFValidationController::unregisterAllLineEdits()
{
	qDeleteAll(mInputs.values());
	mInputs.clear();
}

/**
* @brief "Soft removes" (mark as hidden from validation) all inputs from the obrigatory fields that must be validated
*/
void VLDFValidationController::softUnregisterAllLineEdits() 
{
	for (VLDFValidationInput *input : mInputs.values()) {
		input->mMustValidate = false; // for inputs of pages that have variable list of validation inputs (e.g., materialWizardPage) 
	}
}

/**
 * @brief When a depency is added every time that the fields validity changes, the validator also
 * checks the validity of the other depency field. The fields must have already been registered.
 * @param dependantField
 * @param dependencyField
 */
bool VLDFValidationController::appendDependency(QLineEdit *dependantField, QLineEdit *dependencyField) {
    if(mInputs.contains(dependantField) && mInputs.contains(dependencyField)) {
        mInputs[dependantField]->appendDependency(mInputs[dependencyField]);
        mInputs[dependencyField]->appendDependency(mInputs[dependantField]);
		return true;
    } else 
        return false;
}

/**
 * @brief Appends a lineedit to the list of obrigatory fields that must be valid in order to enable
 * the control buttons.
 * @param lineEdit - the lineedit that must contain a valid input.
 * @param aditionalValidationRule - An std::function that returns a boolean value according to user specifiend rules,
 * use this argument in order to provide more complex validation rules that wasn't provided by qt validators.
 */
bool VLDFValidationController::addInput(VLDFValidationInput *input) {
    if(!input) {
        return false;
    }
    
    connect(input, SIGNAL(validityChanged(bool)), this, SLOT(checkInputsValidity(bool)));
    input->lineEdit()->installEventFilter(this);
    connect(input, &VLDFValidationInput::validityChanged, [this] {
        mPopup.hide();
    });
    
    mInputs[input->lineEdit()] = input;
	return true;
}


/**
 * @brief method that disables or enables all the control buttons acording to the boolean value passed.
 * @param enable - a boolean value telling if the buttons should be enabled, true, or disabled, false.
 */
void VLDFValidationController::setControlButtonsEnabled(bool enable) {
    for(QAbstractButton *button : mControlButtons)
        button->setEnabled(enable);
    isCurrentStateValid = enable;
}

/**
 * @brief Iterate over the inputs vector and checks if any is invalid, if so deactivate the control buttons,
 * else enables then.
 */
void VLDFValidationController::checkInputsValidity(const bool inputBecameValid) {

	if (!inputBecameValid && isCurrentStateValid) {
        setControlButtonsEnabled(false);
    } else if(inputBecameValid && !isCurrentStateValid) {
        for(VLDFValidationInput *input : mInputs.values()) {
            if(!input->isValid()) {
                setControlButtonsEnabled(false);
                showErrorMessage(input);
                return;
            }
        }
		setControlButtonsEnabled(true);
    }
}

void VLDFValidationController::setPopupMessageEnabled(bool enable) {
    mIsPopupEnabled = enable;
}

void VLDFValidationController::showErrorMessage(VLDFValidationInput *input) {
    if(mIsPopupEnabled) {
        const QPoint globalPos = input->lineEdit()->mapTo(mPopup.parentWidget(), QPoint(0, 0));
        mPopup.move(globalPos.x(), globalPos.y() - mPopup.height());
        mPopup.raise();

        if(input->lineEdit()->text().isEmpty()) {
            mPopup.setMessage("All fields must be filled");
        } else {
            mPopup.setMessage(input->additionalErrorMessage());
        }

		mPopup.show();
    }
}

/**
 * @brief Appends a button to the list of buttons that must be disabled if the inputs aren't valid.
 * You can add multiple buttons to be controlled and disabled.
 * @param button - the button that must be disabled
 */
bool VLDFValidationController::addControlButton(QAbstractButton *button) {
    if(!button)
        return false;
    mControlButtons.append(button);
	return true;
}

/**
 * @brief Removes the validation icons from the inputs
 */
void VLDFValidationController::clearValidationInfo() {
    for(VLDFValidationInput *input : mInputs.values()) {
        input->clearValidationInfo();
    }
}

/**
 * @brief last Minute Validation, check if any of the inputs are invalid. Useful in the cases
 * where you must validate all the fields at the same time, for example when the user confirms/sends
 * a form.
 */
bool VLDFValidationController::validateInputs() {
    bool isStateValid = true;
    for(VLDFValidationInput *input : mInputs.values()) {
        emit input->lineEdit()->editingFinished();
        isStateValid = isStateValid && input->isValid();
    }
	return isStateValid;
}


bool VLDFValidationController::eventFilter(QObject *watched, QEvent *event)
{
    QLineEdit *field = qobject_cast<QLineEdit*>(watched);
    if(field && mInputs.contains(field) && event->type() == QEvent::FocusIn && !mInputs[field]->isValid()) {
        showErrorMessage(mInputs[field]);
    }
    return QObject::eventFilter(watched, event);
}

void VLDFValidationController::updateButtonsState(bool newValidState)
{
	setControlButtonsEnabled(newValidState);
}