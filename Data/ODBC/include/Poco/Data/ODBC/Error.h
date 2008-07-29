//
// Error.h
//
// $Id: //poco/Main/Data/ODBC/include/Poco/Data/ODBC/Error.h#3 $
//
// Library: ODBC
// Package: ODBC
// Module:  Error
//
// Definition of Error.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Data_ODBC_Error_INCLUDED
#define Data_ODBC_Error_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "Poco/Data/ODBC/Utility.h"
#include "Poco/Data/ODBC/Diagnostics.h"
#include "Poco/Format.h"
#include <vector>
#ifdef POCO_OS_FAMILY_WINDOWS
#include <windows.h>
#endif
#include <sqlext.h>


namespace Poco {
namespace Data {
namespace ODBC {


template <typename H, SQLSMALLINT handleType>
class Error
	/// Class encapsulating ODBC diagnostic record collection. Collection is generated
	/// during construction. Class provides access and string generation for the collection
	/// as well as individual diagnostic records.
{
public:
	explicit Error(const H& handle) : _diagnostics(handle)
		/// Creates the Error.
	{
	}

	~Error()
		/// Destroys the Error.
	{
	}

	const Diagnostics<H, handleType>& diagnostics()
		/// Returns the associated diagnostics.
	{
		return _diagnostics;
	}

	int count() const
		/// Returns the count of diagnostic records.
	{
		return (int) _diagnostics.count();
	}

	std::string& toString(int index, std::string& str) const
		/// Generates the string for the diagnostic record.
	{
		if ((index < 0) || (index > (count() - 1))) 
			return str;

		std::string s;
		Poco::format(s, 
			"===========================\n"
			"ODBC Diagnostic record #%d:\n"
			"===========================\n"
			"SQLSTATE = %s\nNative Error Code = %ld\n%s\n\n",
			index + 1,
			_diagnostics.sqlState(index),
			_diagnostics.nativeError(index),
			_diagnostics.message(index));

		str.append(s);

		return str;
	}

	std::string toString() const
		/// Generates the string for the diagnostic record collection.
	{
		std::string str;

		Poco::format(str, 
			"Connection:%s\nServer:%s\n",
			_diagnostics.connectionName(),
			_diagnostics.serverName());

		std::string s;
		for (int i = 0; i < count(); ++i)
		{
			s.clear();
			str.append(toString(i, s));
		}

		return str;
	}

private:
	Error();

	Diagnostics<H, handleType> _diagnostics;
};


typedef Error<SQLHENV, SQL_HANDLE_ENV> EnvironmentError;
typedef Error<SQLHDBC, SQL_HANDLE_DBC> ConnectionError;
typedef Error<SQLHSTMT, SQL_HANDLE_STMT> StatementError;
typedef Error<SQLHSTMT, SQL_HANDLE_DESC> DescriptorError;


} } } // namespace Poco::Data::ODBC


#endif
