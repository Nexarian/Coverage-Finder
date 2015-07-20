#include "stdafx.h"
#include "PreciseDecimal.h"
#include "PersistentString.h"

PreciseDecimal::PreciseDecimal(const PrecisionMode CreationMode) {
	DecimalState = CreationMode;
	BasicConstruct();
}

PreciseDecimal::PreciseDecimal(PreciseDecimal &CopyPrecise) {
	DecimalState = StringExact;
	BasicConstruct();
	*this = CopyPrecise;
}

void PreciseDecimal::BasicConstruct() {
	if (DecimalState == StringExact) {
		NumberData.StringPrecision.Integer = new char[1];
		NumberData.StringPrecision.Integer[0] = '\0';

		NumberData.StringPrecision.Mantissa = new char[1];
		NumberData.StringPrecision.Mantissa[0] = '\0';
	}
	else {
		NumberData.RealPrecision = 0;
	}
	StringStorage = new char[1];
	StringStorage[0] = '\0';
}

PreciseDecimal::~PreciseDecimal(void) {
	if (DecimalState == StringExact) {
		delete [] NumberData.StringPrecision.Integer;
		delete [] NumberData.StringPrecision.Mantissa;
	}
	delete [] StringStorage;
}

void PreciseDecimal::SetPrecision(PrecisionMode NewPrecision) {

	if (DecimalState == NewPrecision) return; //nothing to do

	if (NewPrecision == ApproxExact) {
		delete [] NumberData.StringPrecision.Integer;
		delete [] NumberData.StringPrecision.Mantissa;
	}
	else {
		NumberData.StringPrecision.Integer = new char[1];
		NumberData.StringPrecision.Integer[0] = '\0';

		NumberData.StringPrecision.Mantissa = new char[1];
		NumberData.StringPrecision.Mantissa[0] = '\0';
	}

	DecimalState = NewPrecision;
}

void PreciseDecimal::SetInteger(long I) {
	if (DecimalState == StringExact) {
		delete [] NumberData.StringPrecision.Integer;

		//convert the number to a string and then add it
		long Length = NumberDigits(I);
		NumberData.StringPrecision.Integer = new char[Length + 1];
		NumberData.StringPrecision.Integer = '\0';
		_ltoa(I, NumberData.StringPrecision.Integer, 10);
	}
	else {
		NumberData.RealPrecision = I + DecimalPart(NumberData.RealPrecision);
	}
}
void PreciseDecimal::SetInteger(std::string I) {
	if (DecimalState == StringExact) {
		delete [] NumberData.StringPrecision.Integer;

		unsigned long BufferLength = I.length();
		NumberData.StringPrecision.Integer = new char[BufferLength + 1];
		NumberData.StringPrecision.Integer[BufferLength] = '\0';
		strncpy_s(NumberData.StringPrecision.Integer, I.size(), I.c_str(), I.size());
	}
	else { //DecimalState == ApproxExact
		NumberData.RealPrecision = atol(I.c_str()) + DecimalPart(NumberData.RealPrecision);
	}
}
const long PreciseDecimal::GetInteger() const {
	if (DecimalState == StringExact) {
		return atoi(NumberData.StringPrecision.Integer);
	}
	else if (DecimalState == ApproxExact) {
		return IntegerPartUnRound(NumberData.RealPrecision);
	}
	return 0; //something went wrong
}
const char* PreciseDecimal::GetIntegerStr() const {
	if (DecimalState == StringExact) {
		return NumberData.StringPrecision.Integer;
	}
	return 0;
}
const long PreciseDecimal::GetMantissa() const {
	if (DecimalState == StringExact) {
		return atoi(NumberData.StringPrecision.Mantissa);
	}
	else if (DecimalState == ApproxExact) {
		return long(DecimalPart(NumberData.RealPrecision) * pow(10.0, 10.0)); //default precision of 10 decimal places
	}
	return 0; //something went wrong
}
const char* PreciseDecimal::GetMantissaStr() const {
	if (DecimalState == StringExact) { 
		return NumberData.StringPrecision.Mantissa;
	}
	return 0;
}
void PreciseDecimal::SetMantissa(long M) {
	if (DecimalState == StringExact) {
		delete [] NumberData.StringPrecision.Mantissa;

		//convert the number to a string and then add it
		long Length = NumberDigits(M);
		NumberData.StringPrecision.Mantissa = new char[Length + 1];
		NumberData.StringPrecision.Mantissa = '\0';
		_ltoa(M, NumberData.StringPrecision.Mantissa, 10);
	}
	else {
		NumberData.RealPrecision = ((double)M / (double)NumberDigits(M)) + IntegerPartUnRound(NumberData.RealPrecision);
	}
}
void PreciseDecimal::SetMantissa(std::string M) {
	if (DecimalState == StringExact) {
		delete [] NumberData.StringPrecision.Mantissa;

		unsigned long BufferLength = M.length();
		NumberData.StringPrecision.Mantissa = new char[BufferLength + 1];
		NumberData.StringPrecision.Mantissa[BufferLength] = '\0';
		strcpy_s(NumberData.StringPrecision.Mantissa, BufferLength, M.c_str());
	}
	else { //DecimalState == ApproxExact
		long MantissaBuffer = atol(M.c_str());

		NumberData.RealPrecision = ((double)MantissaBuffer / (double)NumberDigits(MantissaBuffer)) + IntegerPartUnRound(NumberData.RealPrecision);
	}
}
void PreciseDecimal::SetRealNumber(const double NewRealNumber) {
	if (DecimalState == StringExact) {
		long IntPart = IntegerPartUnRound(NewRealNumber);
		long IntLen = NumberDigits(IntPart);
		
		//now get the mantissa
		long MantPart = DecimalPart(NewRealNumber) * pow(10.0, 10.0); //lose some precision
		long MantLen = NumberDigits(MantPart);

		//convert both to strings and store
		delete [] NumberData.StringPrecision.Integer;
		NumberData.StringPrecision.Integer = new char[IntLen + 1];
		NumberData.StringPrecision.Integer[IntLen] = 0;
		_ltoa(IntPart, NumberData.StringPrecision.Integer, 10);

		delete [] NumberData.StringPrecision.Mantissa;
		NumberData.StringPrecision.Mantissa = new char[MantLen + 1];
		NumberData.StringPrecision.Mantissa[MantLen] = 0;
		_ltoa(IntPart, NumberData.StringPrecision.Mantissa, 10);
	}
	else {
		NumberData.RealPrecision = NewRealNumber;
	}
}
const double PreciseDecimal::GetRealNumber() const {
	if (DecimalState == StringExact) {
		double ReturnBuffer = (double)GetInteger();

		long MantLen = strlen(NumberData.StringPrecision.Mantissa);
		double MantBuffer = (double)GetMantissa() / pow(10.0, MantLen);

		return ReturnBuffer + MantBuffer;
	}
	else if (DecimalState == ApproxExact) {
		return NumberData.RealPrecision;
	}
	return 0; //something went wrong
}
//overloaded operators
const bool PreciseDecimal::operator==(PreciseDecimal& Compare) const {
	bool ReturnBuffer = (DecimalState == Compare.DecimalState);
	if (ReturnBuffer) {
		if (DecimalState == StringExact) {
			ReturnBuffer = (strcmp(NumberData.StringPrecision.Integer, Compare.NumberData.StringPrecision.Integer) == 0) ||
				(strcmp(NumberData.StringPrecision.Mantissa, Compare.NumberData.StringPrecision.Mantissa) == 0);
		}
		else {
			ReturnBuffer = (NumberData.RealPrecision == Compare.NumberData.RealPrecision);
		}
		return ReturnBuffer;
	}
	return false;
}
const bool PreciseDecimal::operator!=(PreciseDecimal& Compare) const {
	return !(*this == Compare);
}
const PreciseDecimal& PreciseDecimal::operator=(PreciseDecimal& Dest) {
	SetPrecision(Dest.DecimalState);

	if (DecimalState == StringExact) {
		char** StringBuffer = &NumberData.StringPrecision.Integer;
		long BufferLength = strlen(Dest.NumberData.StringPrecision.Integer) + 1;
		delete [] *StringBuffer;
		(*StringBuffer) = new char[BufferLength];
		(*StringBuffer)[BufferLength - 1] = '\0';
		strncpy_s(*StringBuffer, BufferLength, Dest.NumberData.StringPrecision.Integer, BufferLength);


		StringBuffer = &NumberData.StringPrecision.Mantissa;
		BufferLength = strlen(Dest.NumberData.StringPrecision.Mantissa) + 1;
		delete [] *StringBuffer;
		(*StringBuffer) = new char[BufferLength];
		(*StringBuffer)[BufferLength - 1] = '\0';
		strcpy_s(*StringBuffer, BufferLength, Dest.NumberData.StringPrecision.Mantissa);
	}
	else {
		NumberData.RealPrecision = Dest.NumberData.RealPrecision;
	}
	return *this;
}
PreciseDecimal::operator const char*() {
	std::string StringBuffer;
	if (DecimalState == StringExact) {
		StringBuffer = NumberData.StringPrecision.Integer;
		StringBuffer += ".";
		StringBuffer += NumberData.StringPrecision.Mantissa;
	}
	else {
		StringBuffer = (const char*)NumberToString(NumberData.RealPrecision);
	}

	delete [] StringStorage;
	StringStorage = new char[StringBuffer.length() + 1];
	StringStorage[StringBuffer.length()] = '\0';
	strcpy_s(StringStorage, StringBuffer.length(), StringBuffer.c_str());

	return StringStorage;
}