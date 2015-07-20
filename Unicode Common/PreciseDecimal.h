#pragma once

#include <string>
#include "main.h"

enum PrecisionMode {
	StringExact = 0,
	ApproxExact = 1
};

class PreciseDecimal
{
public:
	PreciseDecimal(const PrecisionMode CreationMode = StringExact);
	PreciseDecimal(PreciseDecimal& CopyPrecise);
	virtual ~PreciseDecimal(void);

	void BasicConstruct();

	const long GetInteger() const;
	const char* GetIntegerStr() const;

	const long GetMantissa() const;
	const char* GetMantissaStr() const;

	void SetInteger(const long I);
	void SetInteger(std::string I);
	void SetMantissa(const long M);
	void SetMantissa(std::string M);

	PrecisionMode GetPrecision() {return DecimalState;}
	void SetPrecision(PrecisionMode NewPrecision);

	void SetRealNumber(const double NewRealNumber);
	const double GetRealNumber() const;

	operator const char*();

	//overloaded operators
	const bool operator==(PreciseDecimal& Compare) const;
	const bool operator!=(PreciseDecimal& Compare) const;
	const PreciseDecimal& operator=(PreciseDecimal& Dest);

private:

	union StorageFormat {
		
		struct StrExact {
			char* Integer;
			char* Mantissa;
		} StringPrecision;

		double RealPrecision;
	} NumberData;

	PrecisionMode DecimalState;

	char* StringStorage; //temporary memory buffer
};
