/***************************************************************************
 *            PicoPico.cc
 *
 *  May 03 12:20:00 2008
 *  Copyright  2008  Sebastian Mach
 *  seb@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

// -lboost_regex

// TODO URGENT check each parsing function (and each other) for correct and complete deletes

/*
// left for fun: i just tried to statically initialize a std::map ... but it failed to compile
static const std::pair<std::string,int> binopPrecPairs[] = {
     std::pair<std::string,int>("*",20)
    ,std::pair<std::string,int>("/",20)
    ,std::pair<std::string,int>("+",40)
    ,std::pair<std::string,int>("-",40)
};
static const int numBinOps = sizeof(binopPrecPairs)/sizeof(binopPrecPairs[0]);
static std::map<std::string,int> binOpPrec(
    std::list<std::pair<std::string,int>(binopPrecPairs,binopPrecPairs+numBinOps).begin(),
    std::list<std::pair<std::string,int>(binopPrecPairs,binopPrecPairs+numBinOps).end()
);
*/

#include <vector>
#include <string>
#include <iostream>
#include <boost/regex.hpp>

#include <picogen/picogen.h>
#include <picogen/experimental/PicoPico.h>
#include <picogen/misc/scripting/AST.h>

using namespace std;
using namespace boost;






typedef enum TokenType{
    int_token,
    float_token,
    id_token,
    keyword_token,
    other_token,
    omitted_token
};

typedef struct TokenDescriptor{
    const TokenType tokenType;
    const std::string name;
    const boost::regex regex;
    const bool omit;
    TokenDescriptor( TokenType tokenType, std::string name, boost::regex regex, bool omit = false )
    : tokenType( tokenType ), name(name), regex(regex), omit(omit)
    {
    }
};

// \d is any digit (as with [[:digit:]])
static const TokenDescriptor tokenDescriptors[] = {
     TokenDescriptor( float_token,    "float",      regex( "\\d+\\.\\d+" ) )
    ,TokenDescriptor( int_token,      "int",        regex( "\\d+" ) )
    ,TokenDescriptor( keyword_token,  "keyword",    regex( "if|else|do|while|for" ) )
    ,TokenDescriptor( id_token,       "identifier", regex( "[[:alpha:]]([[:alpha:]]|[[:digit:]])*" ) )
    ,TokenDescriptor( other_token,    "other",      regex( "\\+|-|\\*|/|\\(|\\)|=|;|\\{|\\}" ) )
    ,TokenDescriptor( omitted_token,  "",           regex( "[[:space:]]+" ), true )
};
static const int tokenDescriptorCount = sizeof(tokenDescriptors) / sizeof(tokenDescriptors[0]);



struct Token{
    const TokenDescriptor * tokenDescriptor;
    std::string value;
    // TODO remember start/end of token (for good dumping of error messages)
    Token( const TokenDescriptor *tokenDescriptor, std::string value )
    : tokenDescriptor(tokenDescriptor), value(value)
    {
    }
};


//==-------------------------------------------------------------==//
// outline of PicoPico grammar
//==-------------------------------------------------------------==//
//
// *   >> zero or more
// +   >> one or more
// ?   >> zero or one
// |   >> or
// ::= >> is defined by
// 'x' >>
//
// atom        ::= float | integer | id
// parameters  ::= expression? ( expression ',' )*
// call        ::= id '(' parameters ')'
// expression  ::=   call
//                 | atom
//                 | '(' expression ')'
//                 | expression '*' expression
//                 | expression '/' expression
//                 | expression '+' expression
//                 | expression '-' expression
// statement   ::= expression ';'
//
//==-------------------------------------------------------------==//




//==-------------------------------------------------------------==//
// parsing
//==-------------------------------------------------------------==//

static int getTokenPrecedence( const std::vector<Token>::const_iterator &curr, const std::vector<Token>::const_iterator &end ){
    /*
    static std::map<int, char> binopPrecedence; // will be initialized once in PicoPico
    //--------------------------------------------------------
    static bool binopPrecedenceInitialized = false;
    if( !binopPrecedenceInitialized ){
        binopPrecedenceInitialized = true;
        binopPrecedence['+'] = 20;
        binopPrecedence['-'] = 20;
        binopPrecedence['*'] = 40;
        binopPrecedence['/'] = 40;
    }
    //--------------------------------------------------------
    if( binopPrecedence.end() == binopPrecedence.find(ch) )
        return -1;
    return binopPrecedence[ch];
    */    
    if( curr == end )
        return -1;
    if( curr->value == "+" || curr->value == "-"  )
        return 20;
    if( curr->value == "*" || curr->value == "/"  )
        return 40;
    return -1;
}

static bool tokenEquals( string checkee, const std::vector<Token>::const_iterator &curr, const std::vector<Token>::const_iterator &end ){
    // hehe, "checkee"...like it :)
    if( curr == end )
        return false;
    return curr->value == checkee;
}



static ExprAST *parsePrimary( std::vector<Token>::const_iterator &curr, const std::vector<Token>::const_iterator &end );
static ExprAST *parseStatement( std::vector<Token>::const_iterator &curr, const std::vector<Token>::const_iterator &end );
static ExprAST *parseBlock( std::vector<Token>::const_iterator &curr, const std::vector<Token>::const_iterator &end );




static ExprAST *parseIntExpr(
    std::vector<Token>::const_iterator &curr,
    const std::vector<Token>::const_iterator &end
){
    // convert token value-string to integer
    int result = 0;
    int mag = 1;

    std::string::const_iterator d = curr->value.end();
    if( curr->value.size() > 0 ) do{
        --d;
        switch( *d ){
            case '0': result += 0*mag; break;
            case '1': result += 1*mag; break;
            case '2': result += 2*mag; break;
            case '3': result += 3*mag; break;
            case '4': result += 4*mag; break;
            case '5': result += 5*mag; break;
            case '6': result += 6*mag; break;
            case '7': result += 7*mag; break;
            case '8': result += 8*mag; break;
            case '9': result += 9*mag; break;
        };
        mag *= 10;
    }while( d != curr->value.begin() );

    ++curr;
    return new IntExprAST( result );
}


static ExprAST *parseFloatExpr(
    std::vector<Token>::const_iterator &curr,
    const std::vector<Token>::const_iterator &end
){
    // convert token value-string to float
    float result = 0;
    // TODO write overflow save str-to-float converter
    sscanf( curr->value.c_str(), "%f", &result );
    ++curr;
    return new FloatExprAST( result );
}


static ExprAST *parseIdExpr(
    std::vector<Token>::const_iterator &curr,
    const std::vector<Token>::const_iterator &end
){
    return new IdExprAST( (curr++)->value );
}




static ExprAST *parseBinOpRhs(
    // standard parameters
    std::vector<Token>::const_iterator &curr,
    const std::vector<Token>::const_iterator &end,
    // additional parameters
    int exprPrec,
    ExprAST *lhs
    // --
){
    if( curr == end )
        return lhs;

    while( 1 ){	
        const int tokPrec = getTokenPrecedence(curr,end);

        if( tokPrec < exprPrec ){
            return lhs;
        }

        const char binOp = curr->value[0];
        ++curr;

        ExprAST *rhs = parsePrimary( curr, end );
        if( NULL == rhs )
            return NULL;

        // look ahead next token precedence
        const int nextTokPrec = getTokenPrecedence(curr,end);
        if( tokPrec < nextTokPrec ){
            rhs = parseBinOpRhs( curr, end, tokPrec+1, rhs );
            if( NULL == rhs )
                return NULL;
        }

        // construct binary expression ast
        lhs = new BinaryExprAST( binOp, lhs, rhs );
    }

    /*cout << "looking good" << endl;
    cout << "}}}" << endl;
    return lhs;*/
    return lhs;
}


static ExprAST *parseExpr(
    std::vector<Token>::const_iterator &curr,
    const std::vector<Token>::const_iterator &end
){
    ExprAST *lhs = parsePrimary( curr, end );
    if( lhs == NULL )
        return NULL;
    return parseBinOpRhs( curr, end, 0, lhs );
}


static ExprAST *parseParenExpr(
    std::vector<Token>::const_iterator &curr,
    const std::vector<Token>::const_iterator &end
){
    if( !tokenEquals( "(", curr, end ) ) // assert "(" to be the start token of this parenthesized expression
        return NULL;
    ++curr; // eat '('
    ExprAST *ast = parseExpr( curr, end );
    if( curr != end && ast != NULL ){
        if( curr->value != ")" )
            return NULL;
        ++curr;
        return ast;
    }
    return NULL;
}

static ExprAST *parsePrimary(
    std::vector<Token>::const_iterator &curr,
    const std::vector<Token>::const_iterator &end
){
    if( curr != end ){
        switch( curr->tokenDescriptor->tokenType ){
            case int_token:
                return parseIntExpr( curr, end );
            case float_token:
                return parseFloatExpr( curr, end );
            case id_token:
                return parseIdExpr( curr, end );
            case other_token:
                if( curr->value == "(" )
                    return parseParenExpr( curr, end );
                break;
            default:
                ;
        }
    }
    return NULL;
}

/*
static ExprAST *parseBracketedBlock( std::vector<Token>::const_iterator &curr, const std::vector<Token>::const_iterator &end ){
    if( !tokenEquals( "{", curr, end ) ){ // assert the body of the if be enclosed in "{"
        cout << "error: missing '{' in statement" << endl;
        return NULL;
    }
    ++curr;
    ExprAST *body = parseBlock( curr, end );
    if( NULL == body ){
        cout << "error: parsing a bracketed statement failed" << endl;
        return NULL;
    }
    if( !tokenEquals( "}", curr, end ) ){ // assert the body of the if be enclosed in "}"
        cout << "error: missing '}' in statement" << endl;
        return NULL;
    }
    ++curr;
    return body;
}
*/

static ExprAST *parseStatement( std::vector<Token>::const_iterator &curr, const std::vector<Token>::const_iterator &end ){
    if( curr != end ){        
        ExprAST *tmpAST;
        
        // empty-statement ?
        if( tokenEquals( ";", curr, end ) ){
            ++curr;
            return NULL;
        }

        // block-statement ?
        if( tokenEquals( "{", curr, end ) ){
            ++curr;
            tmpAST = parseBlock( curr, end );
            if( NULL!=tmpAST && tokenEquals( "}", curr, end ) ){
                ++curr;
            }else{
                if( NULL != tmpAST )
                    delete tmpAST;
                tmpAST = NULL;
            }
            return tmpAST;
        }
        
        // if-then[-else] ?
        if( tokenEquals( "if", curr, end ) ){
            ++curr;
            // scan if-clause
            const ExprAST *if_clause = parseParenExpr( curr, end );
            if( NULL == if_clause ){
                cout << "error: malformed if-clause" << endl;
                return NULL;
            }
            // scan body
            const ExprAST *if_body = parseStatement( curr, end );
            // scan else
            const ExprAST *else_body = tokenEquals( "else", curr, end ) ? parseStatement( ++curr, end ) : NULL;
            return new IfBlockAST( if_clause, if_body, else_body );
        }

        // while-loop ?
        if( tokenEquals( "while", curr, end ) ){
            ++curr;
            // scan condition
            const ExprAST *cond = parseParenExpr( curr, end );
            if( NULL == cond ){
                cout << "error: malformed while-condition" << endl;
                return NULL;
            }
            // scan body
            const ExprAST *body = parseStatement( curr, end );
            return new WhileLoopAST( cond, body );
        }
        
        // do-while-loop ?
        if( tokenEquals( "do", curr, end ) ){
            ++curr;
            // scan body
            const ExprAST *body = parseStatement( curr, end );
            // scan condition
            if( !tokenEquals( "while", curr, end ) ){
                cout << "error: missing 'while' in do-while-statement" << endl;
                return NULL;
            }
            ++curr;
            const ExprAST *cond = parseParenExpr( curr, end );
            if( NULL == cond ){
                cout << "error: malformed do-while-condition" << endl;
                return NULL;
            }
            // close with ";"
            if( !tokenEquals( ";", curr, end ) ){
                cout << "error: missing ';' after do-while-statement" << endl;
                return NULL;
            }
            ++curr;
            return new DoWhileLoopAST( cond, body );
        }
        
        // for-loop ?
        if( tokenEquals( "for", curr, end ) ){
            ++curr;
            
            if( !tokenEquals( "(", curr, end ) ){
                cout << "error: wrong for-loop syntax (must be like 'for( init; condition; iterative ) body')" << endl;
                return NULL;
            }
            ++curr;

            const ExprAST *init = parseExpr(curr,end);
            if( !tokenEquals( ";", curr, end ) ){
                cout << "error: wrong for-loop syntax (must be like 'for( init; condition; iterative ) body')" << endl;
                return NULL;
            }
            ++curr;
            
            const ExprAST *cond = parseExpr(curr,end);
            if( !tokenEquals( ";", curr, end ) ){
                cout << "error: wrong for-loop syntax (must be like 'for( init; condition; iterative ) body')" << endl;
                return NULL;
            }
            ++curr;
            
            const ExprAST *iter = parseExpr(curr,end);
            if( !tokenEquals( ")", curr, end ) ){
                cout << "error: wrong for-loop syntax (must be like 'for( init; condition; iterative ) body')" << endl;
                return NULL;
            }
            ++curr;
            
            const ExprAST *body = parseStatement( curr, end );

            // let's now build a while-loop
            BlockAST *whileBody = new BlockAST();
            whileBody->addTail( body );
            whileBody->addTail( iter );
            const WhileLoopAST *while_ = new WhileLoopAST( cond, whileBody );
            // not ready yet, implement init
            BlockAST *fullCorpse = new BlockAST();
            fullCorpse->addTail( init );
            fullCorpse->addTail( while_ );
            return fullCorpse;
        }
        
        
        // try to parse a simple statement
        tmpAST = parseExpr( curr, end );
        if( tmpAST != NULL ){
            if( tokenEquals( ";", curr, end ) ){ // simple statement must end on ";"
                ++curr;
                return tmpAST;
            }
            return NULL;
        }
        return NULL;
    }
    return NULL;
}

static ExprAST *parseBlock( std::vector<Token>::const_iterator &curr, const std::vector<Token>::const_iterator &end ){
    BlockAST *block = NULL;
    if( curr != end ){
        while( curr != end ){
            if( tokenEquals( "}", curr, end ) ){ // this is more an soon-"}"-catch-optimisation. parseStatement() below should also return NULL when detecting a closing bracket
                return block;
            }
            
            ExprAST *tmp = NULL;

            tmp = parseStatement( curr, end );
            if( tmp == NULL ){ // skip empty // TODO what to do on error? >> try catch throw
                continue;
            }
            if( NULL == block ){ // first block element? then do not build a BlockAST yet
                block = new BlockAST();
            }
            block->addTail( tmp );
        }
    }
    return block;
}



namespace picogen{ // TODO find proper namespace for this

PicoPico::PicoPico(){

    std::string code( 
        "if(alpha) for( init; cond; iter ){ body;}"
    );

    match_results<std::string::const_iterator> results;
    string::const_iterator begin = code.begin();
    string::const_iterator end   = code.end();

    vector<Token> tokens;

    // tokenize
    while( begin != end ){
        bool found = false;
        for( int i=0; i<tokenDescriptorCount; ++i ){
            bool matches = regex_search(
                begin,
                end,
                results,
                tokenDescriptors[i].regex,
                match_continuous | match_nosubs | format_first_only
            );
            if( matches ){
                //cout << "match for '" << tokenDescriptors[i].name << "'! (" << string( results[0].first, results[0].second ) << ")" << endl;
                begin = results[0].second;
                found = true;
                if( !tokenDescriptors[i].omit )
                    tokens.push_back( Token( &tokenDescriptors[i], string(results[0].first,results[0].second) ) );
                break;
            }
        }
        if( !found ){
            cerr << "Error while tokenizing at \"" <<
                ( (string(begin,end).length() > 25) // print maximum n chars
                    ? ( string(begin,begin+25) + string("......") ) // (if rest of string.length > n, then cat '......' into output)
                    : string( begin, end )
                )
                << "\"" << endl;
            // TODO throw or something
            break;
        }
    }

    // done
    vector<Token>::const_iterator it;
    #if 1
    cout << endl;
    for( it=tokens.begin(); it != tokens.end(); ++it ){
        cout << "[" << it->tokenDescriptor->name << "(" << it->value << ")" << "]";
    }
    cout << endl;
    #endif // #if 0

    // parse
    vector<Token>::const_iterator curr = tokens.begin();
    const ExprAST *ast = parseBlock( curr, tokens.end() ); //parsePrimary( curr, tokens.end() );
    if( ast != NULL ){
        cout << endl << "program's AST:\n";
        ast->print(1);
    }else{
        cout << "parse error or no program" << endl;
    }
}


PicoPico::~PicoPico(){
}


} /* namespace PicoPico */