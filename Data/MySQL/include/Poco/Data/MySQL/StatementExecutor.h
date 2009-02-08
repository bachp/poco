//
// StatementExecutor.h
//
// $Id: //poco/1.4/Data/MySQL/include/Poco/Data/MySQL/StatementExecutor.h#1 $
//
// Library: Data
// Package: MySQL
// Module:  StatementExecutor
//
// Definition of the StatementExecutor class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
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


#ifndef Data_MySQL_StatementHandle_INCLUDED
#define Data_MySQL_StatementHandle_INCLUDED

#include <mysql.h>
#include "Poco/Data/MySQL/MySQLException.h"

#include <vector>

namespace Poco {
namespace Data {
namespace MySQL {

class StatementExecutor
	/// MySQL statement executor.
{
public:
	enum State
	{
		STMT_INITED,
		STMT_COMPILED,
		STMT_EXECUTED
	};

	explicit StatementExecutor(MYSQL* mysql);
		/// Creates the StatementExecutor.

	~StatementExecutor();
		/// Destroys the StatementExecutor.

	int state() const;
		/// Returns the current state.

	void prepare(const std::string& query);
		/// Prepares the statement for execution.

	void bindParams(MYSQL_BIND* params, size_t count);
		/// Binds the params.

	void bindResult(MYSQL_BIND* result);
		/// Binds result.

	void execute();
		/// Executes the statement.

	bool fetch();
		/// Fetches the data.

	bool fetchColumn(size_t n, MYSQL_BIND *bind);
		/// Fetches the column.
		
	operator MYSQL_STMT* ();
		/// Cast operator to native handle type.

private:

	StatementExecutor(const StatementExecutor&);
	StatementExecutor& operator=(const StatementExecutor&);

private:

	MYSQL_STMT* _pHandle;
	int _state;
	std::string _query;
};


//
// inlines
//

inline StatementExecutor::operator MYSQL_STMT* ()
{
	return _pHandle;
}


}}}


#endif // Data_MySQL_StatementHandle_INCLUDED
