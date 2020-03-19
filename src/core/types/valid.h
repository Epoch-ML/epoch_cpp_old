#ifndef VALID_H_
#define VALID_H_

// epoch types: valid
// epoch/src/core/types/valid.h

class valid {
private:
	bool m_fValidated;
	bool m_fValid;

public:
	valid() :
		m_fValid(false),
		m_fValidated(false)
	{
		/* empty stub */
	}

	~valid() {
		/* empty stub */
	}

public:
	void Validate() {
		if (m_fValidated)
			return;

		m_fValidated = true;
		m_fValid = true;
	}

	void Invalidate() {
		if (m_fValidated)
			return;

		m_fValidated = true;
		m_fValid = false;
	}

	void InvalidateOverride() {
		m_fValidated = true;
		m_fValid = false;
	}

	void ValidateOverride() {
		m_fValidated = true;
		m_fValid = true;
	}

public:
	bool IsValid() {
		return m_fValid;
	}

};

#endif // ! VALID_H_