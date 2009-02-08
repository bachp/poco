//
// SessionImpl.h
//
// $Id: //poco/Main/Data/ODBC/include/Poco/Data/ODBC/SessionImpl.h#4 $
//
// Library: ODBC
// Package: ODBC
// Module:  SessionImpl
//
// Definition of the SessionImpl class.
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


#ifndef Data_ODBC_SessionImpl_INCLUDED
#define Data_ODBC_SessionImpl_INCLUDED


#include "Poco/Data/ODBC/ODBC.h"
#include "Poco/Data/ODBC/Connector.h"
#include "Poco/Data/ODBC/TypeInfo.h"
#include "Poco/Data/ODBC/Binder.h"
#include "Poco/Data/ODBC/Handle.h"
#include "Poco/Data/ODBC/ODBCException.h"
#include "Poco/Data/AbstractSessionImpl.h"
#include "Poco/SharedPtr.h"
#include "Poco/Mutex.h"
#ifdef POCO_OS_FAMILY_WINDOWS
#include <windows.h>
#endif
#include <sqltypes.h>


namespace Poco {
namespace Data {
namespace ODBC {


class ODBC_API SessionImpl: public Poco::Data::AbstractSessionImpl<SessionImpl>
	/// Implements SessionImpl interface
{
public:
	enum TransactionCapability
	{
		ODBC_TXN_CAPABILITY_UNKNOWN = -1,
		ODBC_TXN_CAPABILITY_FALSE = 0,
		ODBC_TXN_CAPABILITY_TRUE = 1
	};

	SessionImpl(const std::string& connect, 
		Poco::Any maxFieldSize = std::size_t(1024), 
		bool enforceCapability=false,
		bool autoBind = true,
		bool autoExtract = true);
		/// Creates the SessionImpl. Opens a connection to the database

	~SessionImpl();
		/// Destroys the SessionImpl.

	Poco::Data::StatementImpl* createStatementImpl();
		/// Returns an ODBC StatementImpl

	void begin();
		/// Starts a transaction

	void commit();
		/// Commits and ends a transaction

	void rollback();
		/// Aborts a transaction

	void close();
		/// Closes the connection

	bool isConnected();
		/// Returns true if session is connected

	bool isTransaction();
		/// Returns true iff a transaction is in progress.

	const std::string& connectorName();
		/// Returns the name of the connector.

	bool canTransact();
		/// Returns true if connection is transaction-capable.

	void setTransactionIsolation(Poco::UInt32 ti);
		/// Sets the transaction isolation level.

	Poco::UInt32 getTransactionIsolation();
		/// Returns the transaction isolation level.

	bool hasTransactionIsolation(Poco::UInt32);
		/// Returns true iff the transaction isolation level corresponding
		/// to the supplied bitmask is supported.

	bool isTransactionIsolation(Poco::UInt32);
		/// Returns true iff the transaction isolation level corresponds
		/// to the supplied bitmask.

	void autoCommit(const std::string&, bool val);
		/// Sets autocommit property for the session.

	bool isAutoCommit(const std::string& name="");
		/// Returns autocommit property value.

	void autoBind(const std::string&, bool val);
		/// Sets automatic binding for the session.

	bool isAutoBind(const std::string& name="");
		/// Returns true if binding is automatic for this session.

	void autoExtract(const std::string&, bool val);
		/// Sets automatic extraction for the session.

	bool isAutoExtract(const std::string& name="");
		/// Returns true if extraction is automatic for this session.

	void setMaxFieldSize(const std::string& rName, const Poco::Any& rValue);
		/// Sets the max field size (the default used when column size is unknown).
		
	Poco::Any getMaxFieldSize(const std::string& rName="");
		/// Returns the max field size (the default used when column size is unknown).

	int maxStatementLength();
		/// Returns maximum length of SQL statement allowed by driver.

	const ConnectionHandle& dbc() const;
		/// Returns the connection handle.

	Poco::Any dataTypeInfo(const std::string& rName="");
		/// Returns the data types information.

private:
	void setDataTypeInfo(const std::string& rName, const Poco::Any& rValue);
		/// No-op. Throws InvalidAccessException.

	static const int FUNCTIONS = SQL_API_ODBC3_ALL_FUNCTIONS_SIZE;

	void open();
		/// Opens a connection to the Database

	void checkError(SQLRETURN rc, const std::string& msg="");

	Poco::UInt32 getDefaultTransactionIsolation();

	Poco::UInt32 transactionIsolation(SQLUINTEGER isolation);

	std::string _connector;
	const ConnectionHandle _db;
	Poco::Any _maxFieldSize;
	bool _autoBind;
	bool _autoExtract;
	TypeInfo _dataTypes;
	char _canTransact;
	bool _inTransaction;
	Poco::FastMutex _mutex;
};


///
/// inlines
///
inline void SessionImpl::checkError(SQLRETURN rc, const std::string& msg)
{
	if (Utility::isError(rc))
		throw ConnectionException(_db, msg);
}


inline const ConnectionHandle& SessionImpl::dbc() const
{
	return _db;
}


inline void SessionImpl::setMaxFieldSize(const std::string& rName, const Poco::Any& rValue)
{
	_maxFieldSize = rValue;
}

		
inline Poco::Any SessionImpl::getMaxFieldSize(const std::string& rName)
{
	return _maxFieldSize;
}


inline void SessionImpl::setDataTypeInfo(const std::string& rName, const Poco::Any& rValue)
{
	throw InvalidAccessException();
}

		
inline Poco::Any SessionImpl::dataTypeInfo(const std::string& rName)
{
	return &_dataTypes;
}


inline void SessionImpl::autoBind(const std::string&, bool val)
{
	_autoBind = val;
}


inline bool SessionImpl::isAutoBind(const std::string& name)
{
	return _autoBind;
}


inline void SessionImpl::autoExtract(const std::string&, bool val)
{
	_autoExtract = val;
}


inline bool SessionImpl::isAutoExtract(const std::string& name)
{
	return _autoExtract;
}


inline const std::string& SessionImpl::connectorName()
{
	return _connector;
}


inline bool SessionImpl::isTransactionIsolation(Poco::UInt32 ti)
{
	return 0 != (ti & getTransactionIsolation());
}


} } } // namespace Poco::Data::ODBC


#endif // Data_ODBC_SessionImpl_INCLUDED
