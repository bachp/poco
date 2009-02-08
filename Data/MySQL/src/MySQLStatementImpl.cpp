//
// MySQLException.cpp
//
// $Id: //poco/1.4/Data/MySQL/src/MySQLStatementImpl.cpp#1 $
//
// Library: Data
// Package: MySQL
// Module:  MySQLStatementImpl
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


#include "Poco/Data/MySQL/MySQLStatementImpl.h"

namespace Poco {
namespace Data {
namespace MySQL {


MySQLStatementImpl::MySQLStatementImpl(SessionImpl& h) 
        : Poco::Data::StatementImpl(h), 
          _stmt(h.handle()), 
          _extractor(_stmt, _metadata), 
	_hasNext(NEXT_DONTKNOW)
{
}


MySQLStatementImpl::~MySQLStatementImpl()
{
}


Poco::UInt32 MySQLStatementImpl::columnsReturned() const
{
	return _metadata.columnsReturned();
}


Poco::UInt32 MySQLStatementImpl::affectedRowCount() const
{
    return 0;
}

	
const MetaColumn& MySQLStatementImpl::metaColumn(Poco::UInt32 pos) const
{
	return _metadata.metaColumn(pos);
}

	
bool MySQLStatementImpl::hasNext()
{
	if (_hasNext == NEXT_DONTKNOW)
	{
		if (_metadata.columnsReturned() == 0)
		{
			return false;
		}

		if (_stmt.fetch())
		{
			_hasNext = NEXT_TRUE;
			return true;
		}

		_hasNext = NEXT_FALSE;
		return false;
	}
	else if (_hasNext == NEXT_TRUE)
	{
		return true;
	}

	return false;
}

	
Poco::UInt32 MySQLStatementImpl::next()
{
	if (!hasNext())
		throw StatementException("No data received");	

	Poco::Data::AbstractExtractionVec::iterator it = extractions().begin();
	Poco::Data::AbstractExtractionVec::iterator itEnd = extractions().end();
	std::size_t pos = 0;

	for (; it != itEnd; ++it)
	{
		(*it)->extract(pos);
		pos += (*it)->numOfColumnsHandled();
	}

	_hasNext = NEXT_DONTKNOW;
    return 1;
}


bool MySQLStatementImpl::canBind() const
{
	bool ret = false;

	if ((_stmt.state() >= StatementExecutor::STMT_COMPILED) && !bindings().empty())
		ret = (*bindings().begin())->canBind();

	return ret;
}


bool MySQLStatementImpl::canCompile() const
{
	return (_stmt.state() < StatementExecutor::STMT_COMPILED);
}

	
void MySQLStatementImpl::compileImpl()
{
	_metadata.reset();
	_stmt.prepare(toString());
	_metadata.init(_stmt);

	if (_metadata.columnsReturned() > 0)
		_stmt.bindResult(_metadata.row());
}

	
void MySQLStatementImpl::bindImpl()
{
	Poco::Data::AbstractBindingVec& binds = bindings();
	size_t pos = 0;
	Poco::Data::AbstractBindingVec::iterator it = binds.begin();
	Poco::Data::AbstractBindingVec::iterator itEnd = binds.end();
	for (; it != itEnd && (*it)->canBind(); ++it)
	{
		(*it)->bind(pos);
		pos += (*it)->numOfColumnsHandled();
	}

	_stmt.bindParams(_binder.getBindArray(), _binder.size());
	_stmt.execute();
	_hasNext = NEXT_DONTKNOW;
}

	
AbstractExtractor& MySQLStatementImpl::extractor()
{
	return _extractor;
}

	
AbstractBinder& MySQLStatementImpl::binder()
{
	return _binder;
}


} } } // namespace Poco::Data::MySQL
