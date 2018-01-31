#pragma once
#ifndef  EXCEPTION_H
#define	 EXCEPTION_H
#include <iostream>
#include <string>

class ExceptionBase {
public:
	virtual void Handle() = 0;
	inline void SetInfo(std::string content) { info = content; };
	inline std::string GetInfo() { return info; };
private:
	std::string info;
};

// Calculation Error
class DetermintZeroError : public ExceptionBase {
public:
	DetermintZeroError() {
		std::string error_message = "Error! Calculate Inverse Matrix, Determint is Zero.";
		SetInfo(error_message);
	}
	virtual void Handle() { std::cout << GetInfo() << std::endl; };
};


// File Error
class FileOpenError:public ExceptionBase {
public :
	FileOpenError() {
		std::string error_message = "Fail to Open File";
		SetInfo(error_message);
	};
	virtual void Handle() { std::cout << GetInfo() << std::endl; };
};

#endif // ! EXCEPTION_H
