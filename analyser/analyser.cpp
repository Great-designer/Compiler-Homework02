#include "analyser.h"

#include <climits>

namespace miniplc0
{

std::pair<std::vector<Instruction>,std::optional<CompilationError>>Analyser::Analyse()
{
	auto err=analyseProgram();
	if(err.has_value())
	{
		return std::make_pair(std::vector<Instruction>(),err);
	}
	else
	{
		return std::make_pair(_instructions,std::optional<CompilationError>());
	}
}

// ʾ��������ʾ����ε����ӳ���
// <����> ::= 'begin'<������>'end'
std::optional<CompilationError> Analyser::analyseProgram()
{
// 'begin'
	auto bg=nextToken();
	if(!bg.has_value()||bg.value().GetType()!=TokenType::BEGIN)
	{
		return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrNoBegin);
	}
// <������>
	auto err=analyseMain();
	if(err.has_value())
	{
		return err;
	}
// 'end'
	auto ed=nextToken();
	if(!ed.has_value()||ed.value().GetType()!=TokenType::END)
	{
		return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrNoEnd);
	}
	return {};
}

// ��Ҫ��ȫ1
// <������> ::= <��������><��������><�������>
std::optional<CompilationError> Analyser::analyseMain()
{
// ��ȫ���Բ��� <����> ��д
// <��������>
	auto err=analyseConstantDeclaration();
	if(err.has_value())
	{
		return err;
	}
// <��������>
	err=analyseVariableDeclaration();
	if(err.has_value())
	{
		return err;
	}
// <�������>
	err=analyseStatementSequence();
	if(err.has_value())
	{
		return err;
	}
	return {};
}

// ʾ��������ʾ����η�����������
// <��������> ::= {<�����������>}
// <�����������> ::= 'const'<��ʶ��>'='<�����ʽ>';'
std::optional<CompilationError> Analyser::analyseConstantDeclaration()
{
// ���������������� 0 ��������
	while(true)
	{
// Ԥ��һ�� token����Ȼ��֪���Ƿ�Ӧ���� <��������> �Ƶ�
		auto next=nextToken();
		if(!next.has_value())
		{
			return {};
		}
// ����� const ��ô˵��Ӧ���Ƶ� <��������> ����ֱ�ӷ���
		if(next.value().GetType()!=TokenType::CONST)
		{
			unreadToken();
			return {};
		}
// <�����������>
		next=nextToken();
		if(!next.has_value()||next.value().GetType()!=TokenType::IDENTIFIER)
		{
			return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrNeedIdentifier);
		}
		if(isDeclared(next.value().GetValueString()))
		{
			return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrDuplicateDeclaration);
		}
		addConstant(next.value());
// '='
		next=nextToken();
		if(!next.has_value()||next.value().GetType()!=TokenType::EQUAL_SIGN)
		{
			return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrConstantNeedValue);
		}
// <�����ʽ>
		int32_t val;
		auto err=analyseConstantExpression(val);
		if(err.has_value())
		{
			return err;
		}
// ';'
		next=nextToken();
		if(!next.has_value()||next.value().GetType()!=TokenType::SEMICOLON)
		{
			return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrNoSemicolon);
		}
// ����һ�� LIT ָ����س���
		_instructions.emplace_back(Operation::LIT,val);
	}
	return {};
}

// <��������> ::= {<�����������>}
// <�����������> ::= 'var'<��ʶ��>['='<���ʽ>]';'
// ��Ҫ��ȫ2
std::optional<CompilationError> Analyser::analyseVariableDeclaration()
{
// ����������������һ�����߶��
	while(true)
	{
// Ԥ����
		auto next=nextToken();
		if(!next.has_value())
		{
			return {};
		}
// 'var'
		if(next.value().GetType()!=TokenType::VAR)
		{
			unreadToken();
			return {};
		}
// <��ʶ��>
		next=nextToken();
		if(!next.has_value()||next.value().GetType()!=TokenType::IDENTIFIER)
		{
			return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrNeedIdentifier);
		}
		if(isDeclared(next.value().GetValueString()))
		{
			return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrDuplicateDeclaration);
		}
		addVariable(next.value());
// ��������û�г�ʼ������Ȼ��Ҫһ��Ԥ��
		next=nextToken();
// '='
		if(next.has_value()&&next.value().GetType()==TokenType::EQUAL_SIGN)
		{
// '<���ʽ>'
			auto err=analyseExpression();
			if(err.has_value())
			{
				return err;
			}
// ';'
			next=nextToken();
		}
		if(!next.has_value()||next.value().GetType()!=TokenType::SEMICOLON)
		{
			return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrNoSemicolon);
		}
	}
	return {};
}

// <�������> ::= {<���>}
// <���> :: = <��ֵ���> | <������> | <�����>
// <��ֵ���> :: = <��ʶ��>'='<���ʽ>';'
// <������> :: = 'print' '(' <���ʽ> ')' ';'
// <�����> :: = ';'
// ��Ҫ��ȫ3
std::optional<CompilationError> Analyser::analyseStatementSequence()
{
	while(true)
	{
// Ԥ��
		auto next=nextToken();
		if(!next.has_value())
		{
			return {};
		}
		unreadToken();
		if(next.value().GetType()!=TokenType::IDENTIFIER&&next.value().GetType()!=TokenType::PRINT&&next.value().GetType()!=TokenType::SEMICOLON)
		{
			return {};
		}
		std::optional<CompilationError> err;
		switch(next.value().GetType())
		{
// ������Ҫ����Բ�ͬ��Ԥ����������ò�ͬ���ӳ���
			case IDENTIFIER:
				unreadToken();
				err=analyseAssignmentStatement();
				if(err.has_value())
				{
					return err;
				}
			break;
			case PRINT:
				unreadToken();
				err=analyseOutputStatement();
				if(err.has_value())
				{
					return err;
				}
			break;
// ע������û����Կ���䵥������һ����������˿���ֱ�������ﷵ��
			case SEMICOLON:
				return {};
			break;
			default:
			break;
		}
	}
	return {};
}

// <�����ʽ> ::= [<����>]<�޷�������>
// ��Ҫ��ȫ4
std::optional<CompilationError> Analyser::analyseConstantExpression(int32_t &out)
{
// out �ǳ����ʽ�Ľ��
// ������Ҫ���������ʽ���Ҽ�����
// ע�����¾�Ϊ�����ʽ
// +1 -1 1
// ͬʱҪע���Ƿ����
	auto next=nextToken();
	int temp=1;
	if(next.has_value()&&next.value().GetType()==TokenType::PLUS_SIGN)
	{
		auto err=analyseExpression();
		if(err.has_value())
		{
			return err;
		}
		next=nextToken();
	}
	else if(next.has_value()&&next.value().GetType()==TokenType::MINUS_SIGN)
	{
		auto err=analyseExpression();
		if(err.has_value())
		{
			return err;
		}
		next=nextToken();
		temp=-1;
	}
	if(!next.has_value()||next.value().GetType()!=TokenType::UNSIGNED_INTEGER)
	{
		return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrNeedIdentifier);
	}
	out=std::any_cast<int32_t>(next.value().GetValue());
	out*=temp; 
	if(out==-2147483648&&temp=1)
	{
		return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrNeedIdentifier);
	}
	return {};
}

// <���ʽ> ::= <��>{<�ӷ��������><��>}
std::optional<CompilationError> Analyser::analyseExpression()
{
// <��>
	auto err = analyseItem();
	if (err.has_value())
	{
		return err;
	}
// {<�ӷ��������><��>}
	while (true)
	{
// Ԥ��
		auto next = nextToken();
		if (!next.has_value())
		{
			return {};
		}
		auto type = next.value().GetType();
		if (type != TokenType::PLUS_SIGN && type != TokenType::MINUS_SIGN)
		{
			unreadToken();
			return {};
		}
// <��>
		err = analyseItem();
		if (err.has_value())
		{
			return err;
		}
// ���ݽ������ָ��
		if (type == TokenType::PLUS_SIGN)
		{
			_instructions.emplace_back(Operation::ADD,0);
		}
		else if (type == TokenType::MINUS_SIGN)
		{
			_instructions.emplace_back(Operation::SUB,0);
		}
	}
	return {};
}

// <��ֵ���> ::= <��ʶ��>'='<���ʽ>';'
// ��Ҫ��ȫ5
std::optional<CompilationError> Analyser::analyseAssignmentStatement()
{
	auto next=nextToken();
// ��������﷨�������⻹Ҫ����
// ��ʶ����������
// ��ʶ���ǳ�����
	next=nextToken();
	if(!next.has_value()||next.value().GetType()!=TokenType::IDENTIFIER)
	{
		return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrNeedIdentifier);
	}
	if(!isDeclared(next.value().GetValueString()))
	{
		return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrDuplicateDeclaration);
	}
	next=nextToken();
	if(!next.has_value() || next.value().GetType() != TokenType::EQUAL_SIGN)
	{
		return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrInvalidPrint);
	}
	auto err=analyseExpression();
	if(err.has_value())
	{
		return err;
	}
// ��Ҫ����ָ����
	return {};
}

// <������> ::= 'print' '(' <���ʽ> ')' ';'
std::optional<CompilationError> Analyser::analyseOutputStatement()
{
// ���֮ǰ <�������> ��ʵ����ȷ�������һ�� next һ���� TokenType::PRINT
	auto next=nextToken();
// '('
	next=nextToken();
	if (!next.has_value() || next.value().GetType() != TokenType::LEFT_BRACKET)
	{
		return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrInvalidPrint);
	}
// <���ʽ>
	auto err = analyseExpression();
	if (err.has_value())
	{
		return err;
	}
// ')'
	next=nextToken();
	if(!next.has_value() || next.value().GetType() != TokenType::RIGHT_BRACKET)
	{
		return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrInvalidPrint);
	}
// ';'
	next = nextToken();
	if (!next.has_value() || next.value().GetType() != TokenType::SEMICOLON)
	{
		return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrNoSemicolon);
	}
// ������Ӧ��ָ�� WRT
	_instructions.emplace_back(Operation::WRT, 0);
	return {};
}

// <��> :: = <����>{ <�˷��������><����> }
// ��Ҫ��ȫ6
std::optional<CompilationError> Analyser::analyseItem()
{
	// ���Բο� <���ʽ> ʵ��
	auto err=analyseFactor();
	if(err.has_value())
	{
		return err;
	}
	while(true)
	{
		auto next=nextToken();
		if(!next.has_value())
		{
			return {};
		}
		auto type=next.value().GetType();
		if(type!=TokenType::MULTIPLICATION_SIGN&&type!=TokenType::DIVISION_SIGN)
		{
			unreadToken();
			return {};
		}
		err=analyseFactor();
		if(err.has_value())
		{
			return err;
		}
		if(type==TokenType::MULTIPLICATION_SIGN)
		{
			_instructions.emplace_back(Operation::MUL,0);
		}
		else if(type==TokenType::DIVISION_SIGN)
		{
			_instructions.emplace_back(Operation::DIV,0);
		}
	}
	return {};
}

// <����> ::= [<����>]( <��ʶ��> | <�޷�������> | '('<���ʽ>')' )
// ��Ҫ��ȫ7
std::optional<CompilationError> Analyser::analyseFactor()
{
// [<����>]
	auto next=nextToken();
	auto prefix=1;
	if(!next.has_value())
	{
		return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrIncompleteExpression);
	}
	if(next.value().GetType()==TokenType::PLUS_SIGN)
	{
		prefix=1;
	}
	else if(next.value().GetType()==TokenType::MINUS_SIGN)
	{
		prefix=-1;
		_instructions.emplace_back(Operation::LIT,0);
	}
	else
	{
		unreadToken();
	}
// Ԥ��
	next=nextToken();
	if(!next.has_value())
	{
		return std::make_optional<CompilationError>(_current_pos, ErrorCode::ErrIncompleteExpression);
	}
	std::optional<CompilationError> err;
	switch(next.value().GetType())
	{
// ����� <�������> ���ƣ���Ҫ����Ԥ��������ò�ͬ���ӳ���
// ����Ҫע�� default ���ص���һ���������
		case IDENTIFIER:
			if(!next.has_value()||next.value().GetType()!=TokenType::IDENTIFIER)
			{
				return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrNeedIdentifier);
			}
			if(!isDeclared(next.value().GetValueString()))
			{
				return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrDuplicateDeclaration);
			}
		break;
		case UNSIGNED_INTEGER:
			if(!next.has_value()||next.value().GetType()!=TokenType::UNSIGNED_INTEGER)
			{
				return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrNeedIdentifier);
			}
		break;
		case LEFT_BRACKET:
			err=analyseExpression();
			if(err.has_value())
			{
				return err;
			}
			next=nextToken();
			if(!next.has_value()||next.value().GetType()!=TokenType::RIGHT_BRACKET)
			{
				return std::make_optional<CompilationError>(_current_pos,ErrorCode::ErrNeedIdentifier);
			}
		break; 
		default:
		return std::make_optional<CompilationError>(_current_pos, ErrorCode::ErrIncompleteExpression);
	}
// ȡ��
	if(prefix == -1)
	{
		_instructions.emplace_back(Operation::SUB, 0);
	}
	return {};
}

std::optional<Token> Analyser::nextToken()
{
	if (_offset == _tokens.size())
	{
		return {};
	}
// ���ǵ� _tokens[0..._offset-1] �Ѿ�����������
// ��������ѡ�� _tokens[0..._offset-1] �� EndPos ��Ϊ��ǰλ��
	_current_pos = _tokens[_offset].GetEndPos();
	return _tokens[_offset++];
}

void Analyser::unreadToken()
{
	if (_offset == 0)
	{
		DieAndPrint("analyser unreads token from the begining.");
	}
	_current_pos = _tokens[_offset - 1].GetEndPos();
	_offset--;
}

void Analyser::_add(const Token &tk, std::map<std::string, int32_t> &mp)
{
	if (tk.GetType() != TokenType::IDENTIFIER)
	{
		DieAndPrint("only identifier can be added to the table.");
	}
	mp[tk.GetValueString()] = _nextTokenIndex;
	_nextTokenIndex++;
}

void Analyser::addVariable(const Token &tk)
{
	_add(tk, _vars);
}

void Analyser::addConstant(const Token &tk)
{
	_add(tk, _consts);
}

void Analyser::addUninitializedVariable(const Token &tk)
{
	_add(tk, _uninitialized_vars);
}

int32_t Analyser::getIndex(const std::string &s)
{
	if (_uninitialized_vars.find(s) != _uninitialized_vars.end())
	{
		return _uninitialized_vars[s];
	}
	else if (_vars.find(s) != _vars.end())
	{
		return _vars[s];
	}
	else
	{
		return _consts[s];
	}
}

bool Analyser::isDeclared(const std::string &s)
{
	return isConstant(s) || isUninitializedVariable(s) ||isInitializedVariable(s);
}

bool Analyser::isUninitializedVariable(const std::string &s)
{
	return _uninitialized_vars.find(s) != _uninitialized_vars.end();
}

bool Analyser::isInitializedVariable(const std::string &s)
{
	return _vars.find(s) != _vars.end();
}

bool Analyser::isConstant(const std::string &s)
{
	return _consts.find(s) != _consts.end();
}

}  // namespace miniplc0
